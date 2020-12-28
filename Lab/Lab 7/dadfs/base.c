/*
 * Dad Filesystem for the Linux Kernel.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/buffer_head.h>
#include <linux/slab.h> 
#include <linux/random.h>
#include <linux/version.h>
#include <linux/jbd2.h>
#include <linux/parser.h>
#include <linux/blkdev.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
#include <linux/uio.h>
#endif

#include "sblock.h"

#ifndef f_dentry
#define f_dentry f_path.dentry
#endif

/* A super block lock that must be used for any critical section operation on the sb,
 * such as: updating the free_blocks, inodes_count etc. */
static DEFINE_MUTEX(dadfs_sb_lock);
static DEFINE_MUTEX(dadfs_inodes_mgmt_lock);

/* FIXME: This can be moved to an in-memory structure of the dadfs_inode.
 * Because of the global nature of this lock, we cannot create
 * new children (without locking) in two different dirs at a time.
 * They will get sequentially created. If we move the lock
 * to a directory-specific way (by moving it inside inode), the
 * insertion of two children in two different directories can be
 * done in parallel */
static DEFINE_MUTEX(dadfs_directory_children_update_lock);

static struct kmem_cache *sfs_inode_cachep;

void dadfs_sb_sync(struct super_block *vsb)
{
	struct buffer_head *bh;
	struct dadfs_super_block *sb = DADFS_SB(vsb);

	bh = sb_bread(vsb, DADFS_SUPERBLOCK_BLOCK_NUMBER);
	BUG_ON(!bh);

	bh->b_data = (char *)sb;
	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);
	brelse(bh);
}

struct dadfs_inode *dadfs_inode_search(struct super_block *sb,
		struct dadfs_inode *start,
		struct dadfs_inode *search)
{
	uint64_t count = 0;
	while (start->inode_no != search->inode_no
			&& count < DADFS_SB(sb)->inodes_count) {
		count++;
		start++;
	}

	if (start->inode_no == search->inode_no) {
		return start;
	}

	return NULL;
}

void dadfs_inode_add(struct super_block *vsb, struct dadfs_inode *inode)
{
	struct dadfs_super_block *sb = DADFS_SB(vsb);
	struct buffer_head *bh;
	struct dadfs_inode *inode_iterator;

	if (mutex_lock_interruptible(&dadfs_inodes_mgmt_lock)) {
		sfs_trace("Failed to acquire mutex lock\n");
		return;
	}

	bh = sb_bread(vsb, DADFS_INODESTORE_BLOCK_NUMBER);
	BUG_ON(!bh);

	inode_iterator = (struct dadfs_inode *)bh->b_data;

	if (mutex_lock_interruptible(&dadfs_sb_lock)) {
		sfs_trace("Failed to acquire mutex lock\n");
		return;
	}

	/* Append the new inode in the end in the inode store */
	inode_iterator += sb->inodes_count;

	memcpy(inode_iterator, inode, sizeof(struct dadfs_inode));
	sb->inodes_count++;

	mark_buffer_dirty(bh);
	dadfs_sb_sync(vsb);
	brelse(bh);

	mutex_unlock(&dadfs_sb_lock);
	mutex_unlock(&dadfs_inodes_mgmt_lock);
}

/* This function returns a blocknumber which is free.
 * The block will be removed from the freeblock list.
 *
 * In an ideal, production-ready filesystem, we will not be dealing with blocks,
 * and instead we will be using extents
 *
 * If for some reason, the file creation/deletion failed, the block number
 * will still be marked as non-free. You need fsck to fix this.*/
int dadfs_sb_get_a_freeblock(struct super_block *vsb, uint64_t * out)
{
	struct dadfs_super_block *sb = DADFS_SB(vsb);
	int i;
	int ret = 0;

	if (mutex_lock_interruptible(&dadfs_sb_lock)) {
		sfs_trace("Failed to acquire mutex lock\n");
		ret = -EINTR;
		goto end;
	}

	/* Loop until we find a free block. We start the loop from 3,
	 * as all prior blocks will always be in use */
	for (i = 3; i < DADFS_MAX_FILESYSTEM_OBJECTS_SUPPORTED; i++) {
		if (sb->free_blocks & (1 << i)) {
			break;
		}
	}

	if (unlikely(i == DADFS_MAX_FILESYSTEM_OBJECTS_SUPPORTED)) {
		printk(KERN_ERR "No more free blocks available");
		ret = -ENOSPC;
		goto end;
	}

	*out = i;

	/* Remove the identified block from the free list */
	sb->free_blocks &= ~(1 << i);

	dadfs_sb_sync(vsb);

end:
	mutex_unlock(&dadfs_sb_lock);
	return ret;
}

static int dadfs_sb_get_objects_count(struct super_block *vsb,
					 uint64_t * out)
{
	struct dadfs_super_block *sb = DADFS_SB(vsb);

	if (mutex_lock_interruptible(&dadfs_inodes_mgmt_lock)) {
		sfs_trace("Failed to acquire mutex lock\n");
		return -EINTR;
	}
	*out = sb->inodes_count;
	mutex_unlock(&dadfs_inodes_mgmt_lock);

	return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
static int dadfs_iterate(struct file *filp, struct dir_context *ctx)
#else
static int dadfs_readdir(struct file *filp, void *dirent, filldir_t filldir)
#endif
{
	loff_t pos;
	struct inode *inode;
	struct super_block *sb;
	struct buffer_head *bh;
	struct dadfs_inode *sfs_inode;
	struct dadfs_dir_record *record;
	int i;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
	pos = ctx->pos;
#else
	pos = filp->f_pos;
#endif
	inode = filp->f_dentry->d_inode;
	sb = inode->i_sb;

	if (pos) {
		/* FIXME: We use a hack of reading pos to figure if we have filled in all data.
		 * We should probably fix this to work in a cursor based model and
		 * use the tokens correctly to not fill too many data in each cursor based call */
		return 0;
	}

	sfs_inode = DADFS_INODE(inode);

	if (unlikely(!S_ISDIR(sfs_inode->mode))) {
		printk(KERN_ERR
		       "inode [%llu][%lu] for fs object [%s] not a directory\n",
		       sfs_inode->inode_no, inode->i_ino,
		       filp->f_dentry->d_name.name);
		return -ENOTDIR;
	}

	bh = sb_bread(sb, sfs_inode->data_block_number);
	BUG_ON(!bh);

	record = (struct dadfs_dir_record *)bh->b_data;
	for (i = 0; i < sfs_inode->dir_children_count; i++) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
		dir_emit(ctx, record->filename, DADFS_FILENAME_MAXLEN,
			record->inode_no, DT_UNKNOWN);
		ctx->pos += sizeof(struct dadfs_dir_record);
#else
		filldir(dirent, record->filename, DADFS_FILENAME_MAXLEN, pos,
			record->inode_no, DT_UNKNOWN);
		filp->f_pos += sizeof(struct dadfs_dir_record);
#endif
		pos += sizeof(struct dadfs_dir_record);
		record++;
	}
	brelse(bh);

	return 0;
}

/* This functions returns a dadfs_inode with the given inode_no
 * from the inode store, if it exists. */
struct dadfs_inode *dadfs_get_inode(struct super_block *sb,
					  uint64_t inode_no)
{
	struct dadfs_super_block *sfs_sb = DADFS_SB(sb);
	struct dadfs_inode *sfs_inode = NULL;
	struct dadfs_inode *inode_buffer = NULL;

	int i;
	struct buffer_head *bh;

	/* The inode store can be read once and kept in memory permanently while mounting.
	 * But such a model will not be scalable in a filesystem with
	 * millions or billions of files (inodes) */
	bh = sb_bread(sb, DADFS_INODESTORE_BLOCK_NUMBER);
	BUG_ON(!bh);

	sfs_inode = (struct dadfs_inode *)bh->b_data;

#if 0
	if (mutex_lock_interruptible(&dadfs_inodes_mgmt_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
		       __FILE__, __LINE__);
		return NULL;
	}
#endif
	for (i = 0; i < sfs_sb->inodes_count; i++) {
		if (sfs_inode->inode_no == inode_no) {
			inode_buffer = kmem_cache_alloc(sfs_inode_cachep, GFP_KERNEL);
			memcpy(inode_buffer, sfs_inode, sizeof(*inode_buffer));

			break;
		}
		sfs_inode++;
	}
//      mutex_unlock(&dadfs_inodes_mgmt_lock);

	brelse(bh);
	return inode_buffer;
}

ssize_t dadfs_read(struct file * filp, char __user * buf, size_t len,
		      loff_t * ppos)
{
	/* After the commit dd37978c5 in the upstream linux kernel,
	 * we can use just filp->f_inode instead of the
	 * f->f_path.dentry->d_inode redirection */
	struct dadfs_inode *inode =
	    DADFS_INODE(filp->f_path.dentry->d_inode);
	struct buffer_head *bh;

	char *buffer;
	int nbytes;

	if (*ppos >= inode->file_size) {
		/* Read request with offset beyond the filesize */
		return 0;
	}

	bh = sb_bread(filp->f_path.dentry->d_inode->i_sb,
					    inode->data_block_number);

	if (!bh) {
		printk(KERN_ERR "Reading the block number [%llu] failed.",
		       inode->data_block_number);
		return 0;
	}

	buffer = (char *)bh->b_data;
	nbytes = min((size_t) inode->file_size, len);

	if (copy_to_user(buf, buffer, nbytes)) {
		brelse(bh);
		printk(KERN_ERR
		       "Error copying file contents to the userspace buffer\n");
		return -EFAULT;
	}

	brelse(bh);

	*ppos += nbytes;

	return nbytes;
}

/* Save the modified inode */
int dadfs_inode_save(struct super_block *sb, struct dadfs_inode *sfs_inode)
{
	struct dadfs_inode *inode_iterator;
	struct buffer_head *bh;

	bh = sb_bread(sb, DADFS_INODESTORE_BLOCK_NUMBER);
	BUG_ON(!bh);

	if (mutex_lock_interruptible(&dadfs_sb_lock)) {
		sfs_trace("Failed to acquire mutex lock\n");
		return -EINTR;
	}

	inode_iterator = dadfs_inode_search(sb,
		(struct dadfs_inode *)bh->b_data,
		sfs_inode);

	if (likely(inode_iterator)) {
		memcpy(inode_iterator, sfs_inode, sizeof(*inode_iterator));
		printk(KERN_INFO "The inode updated\n");

		mark_buffer_dirty(bh);
		sync_dirty_buffer(bh);
	} else {
		mutex_unlock(&dadfs_sb_lock);
		printk(KERN_ERR
		       "The new filesize could not be stored to the inode.");
		return -EIO;
	}

	brelse(bh);

	mutex_unlock(&dadfs_sb_lock);

	return 0;
}

/* FIXME: The write support is rudimentary. I have not figured out a way to do writes
 * from particular offsets (even though I have written some untested code for this below) efficiently. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)		
ssize_t dadfs_write(struct file * filp, const char __user * buf, size_t len,
		       loff_t * ppos)
#else
ssize_t dadfs_write(struct kiocb * kiocb, struct iov_iter * iov_iter)
#endif
{
	/* After the commit dd37978c5 in the upstream linux kernel,
	 * we can use just filp->f_inode instead of the
	 * f->f_path.dentry->d_inode redirection */
	struct inode *inode;
	struct dadfs_inode *sfs_inode;
	struct buffer_head *bh;
	struct super_block *sb;
	struct dadfs_super_block *sfs_sb;
	// struct *i_mapping;
	handle_t *handle;

	char *buffer;

	int retval;
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)	
	sb = filp->f_path.dentry->d_inode->i_sb;
#else
	sb = kiocb->ki_filp->f_inode->i_sb;
#endif	
	sfs_sb = DADFS_SB(sb);

	handle = jbd2_journal_start(sfs_sb->journal, 1);
	if (IS_ERR(handle))
		return PTR_ERR(handle);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)		
	retval = generic_write_checks(filp, ppos, &len, 0);
	// int generic_write_checks(struct file *file, loff_t *pos, size_t *count, int isblk);
#else
	retval = generic_write_checks(kiocb, iov_iter);
	// extern ssize_t generic_write_checks(struct kiocb *, struct iov_iter *);
#endif	
	if (retval)
		return retval;
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)	
	inode = filp->f_path.dentry->d_inode;
#else
	inode = kiocb->ki_filp->f_inode;
#endif	
	
	sfs_inode = DADFS_INODE(inode);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
	bh = sb_bread(filp->f_path.dentry->d_inode->i_sb,
					    sfs_inode->data_block_number);
#else
	bh = sb_bread(kiocb->ki_filp->f_inode->i_sb,
					    sfs_inode->data_block_number);
#endif

	if (!bh) {
		printk(KERN_ERR "Reading the block number [%llu] failed.",
		       sfs_inode->data_block_number);
		return 0;
	}
	buffer = (char *)bh->b_data;

	/* Move the pointer until the required byte offset */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)	
	buffer += *ppos;
#else
	buffer += kiocb->ki_pos;
#endif

	retval = jbd2_journal_get_write_access(handle, bh);
	if (WARN_ON(retval)) {
		brelse(bh);
		sfs_trace("Can't get write access for bh\n");
		return retval;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
	if (copy_from_user(buffer, buf, len)) { // (void *to, const void __user *from, __kernel_size_t n);
#else
	if (copy_from_iter(buffer, iov_iter->count, iov_iter) == 0) { // FIXME
		// size_t copy_from_iter(void *addr, size_t bytes, struct iov_iter *i)
#endif
		brelse(bh); 
		printk(KERN_ERR
		       "Error copying file contents from the userspace buffer to the kernel space\n");
		return -EFAULT;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)	
	*ppos += len;
#else
	kiocb->ki_pos += iov_iter->count; // FIXME: len
#endif

	retval = jbd2_journal_dirty_metadata(handle, bh);
	if (WARN_ON(retval)) {
		brelse(bh);
		return retval;
	}
	handle->h_sync = 1;
	retval = jbd2_journal_stop(handle);
	if (WARN_ON(retval)) {
		brelse(bh);
		return retval;
	}

	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);
	brelse(bh);

	/* Set new size
	 * sfs_inode->file_size = max(sfs_inode->file_size, *ppos);
	 *
	 * FIXME: What to do if someone writes only some parts in between ?
	 * The above code will also fail in case a file is overwritten with
	 * a shorter buffer */
	if (mutex_lock_interruptible(&dadfs_inodes_mgmt_lock)) {
		sfs_trace("Failed to acquire mutex lock\n");
		return -EINTR;
	}
	
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)	
	sfs_inode->file_size = *ppos;
#else
	sfs_inode->file_size = (kiocb->ki_pos);
#endif		
	retval = dadfs_inode_save(sb, sfs_inode);
	if (retval) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)	
		len = retval;
	}
	mutex_unlock(&dadfs_inodes_mgmt_lock);
	return len;
#else
		;
		mutex_unlock(&dadfs_inodes_mgmt_lock);
		return retval;
	}
	mutex_unlock(&dadfs_inodes_mgmt_lock);
	return iov_iter->count; // TODO: change len
#endif
}

const struct file_operations dadfs_file_operations = {
	.read = dadfs_read,
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)	
	.write = dadfs_write,
#else
	.write_iter = dadfs_write,
#endif
};

const struct file_operations dadfs_dir_operations = {
	.owner = THIS_MODULE,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
	.iterate = dadfs_iterate,
#else
	.readdir = dadfs_readdir,
#endif
};

struct dentry *dadfs_lookup(struct inode *parent_inode,
			       struct dentry *child_dentry, unsigned int flags);

static int dadfs_create(struct inode *dir, struct dentry *dentry,
			   umode_t mode, bool excl);

static int dadfs_mkdir(struct inode *dir, struct dentry *dentry,
			  umode_t mode);

static struct inode_operations dadfs_inode_ops = {
	.create = dadfs_create,
	.lookup = dadfs_lookup,
	.mkdir = dadfs_mkdir,
};

static int dadfs_create_fs_object(struct inode *dir, struct dentry *dentry,
				     umode_t mode)
{
	struct inode *inode;
	struct dadfs_inode *sfs_inode;
	struct super_block *sb;
	struct dadfs_inode *parent_dir_inode;
	struct buffer_head *bh;
	struct dadfs_dir_record *dir_contents_datablock;
	uint64_t count;
	int ret;

	if (mutex_lock_interruptible(&dadfs_directory_children_update_lock)) {
		sfs_trace("Failed to acquire mutex lock\n");
		return -EINTR;
	}
	sb = dir->i_sb;

	ret = dadfs_sb_get_objects_count(sb, &count);
	if (ret < 0) {
		mutex_unlock(&dadfs_directory_children_update_lock);
		return ret;
	}

	if (unlikely(count >= DADFS_MAX_FILESYSTEM_OBJECTS_SUPPORTED)) {
		/* The above condition can be just == insted of the >= */
		printk(KERN_ERR
		       "Maximum number of objects supported by dadfs is already reached");
		mutex_unlock(&dadfs_directory_children_update_lock);
		return -ENOSPC;
	}

	if (!S_ISDIR(mode) && !S_ISREG(mode)) {
		printk(KERN_ERR
		       "Creation request but for neither a file nor a directory");
		mutex_unlock(&dadfs_directory_children_update_lock);
		return -EINVAL;
	}

	inode = new_inode(sb);
	if (!inode) {
		mutex_unlock(&dadfs_directory_children_update_lock);
		return -ENOMEM;
	}

	inode->i_sb = sb;
	inode->i_op = &dadfs_inode_ops;
	inode->i_atime = inode->i_mtime = inode->i_ctime = current_time(inode);
	inode->i_ino = (count + DADFS_START_INO - DADFS_RESERVED_INODES + 1);

	sfs_inode = kmem_cache_alloc(sfs_inode_cachep, GFP_KERNEL);
	sfs_inode->inode_no = inode->i_ino;
	inode->i_private = sfs_inode;
	sfs_inode->mode = mode;

	if (S_ISDIR(mode)) {
		printk(KERN_INFO "New directory creation request\n");
		sfs_inode->dir_children_count = 0;
		inode->i_fop = &dadfs_dir_operations;
	} else if (S_ISREG(mode)) {
		printk(KERN_INFO "New file creation request\n");
		sfs_inode->file_size = 0;
		inode->i_fop = &dadfs_file_operations;
	}

	/* First get a free block and update the free map,
	 * Then add inode to the inode store and update the sb inodes_count,
	 * Then update the parent directory's inode with the new child.
	 *
	 * The above ordering helps us to maintain fs consistency
	 * even in most crashes
	 */
	ret = dadfs_sb_get_a_freeblock(sb, &sfs_inode->data_block_number);
	if (ret < 0) {
		printk(KERN_ERR "dadfs could not get a freeblock");
		mutex_unlock(&dadfs_directory_children_update_lock);
		return ret;
	}

	dadfs_inode_add(sb, sfs_inode);

	parent_dir_inode = DADFS_INODE(dir);
	bh = sb_bread(sb, parent_dir_inode->data_block_number);
	BUG_ON(!bh);

	dir_contents_datablock = (struct dadfs_dir_record *)bh->b_data;

	/* Navigate to the last record in the directory contents */
	dir_contents_datablock += parent_dir_inode->dir_children_count;

	dir_contents_datablock->inode_no = sfs_inode->inode_no;
	strcpy(dir_contents_datablock->filename, dentry->d_name.name);

	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);
	brelse(bh);

	if (mutex_lock_interruptible(&dadfs_inodes_mgmt_lock)) {
		mutex_unlock(&dadfs_directory_children_update_lock);
		sfs_trace("Failed to acquire mutex lock\n");
		return -EINTR;
	}

	parent_dir_inode->dir_children_count++;
	ret = dadfs_inode_save(sb, parent_dir_inode);
	if (ret) {
		mutex_unlock(&dadfs_inodes_mgmt_lock);
		mutex_unlock(&dadfs_directory_children_update_lock);

		/* TODO: Remove the newly created inode from the disk and in-memory inode store
		 * and also update the superblock, freemaps etc. to reflect the same.
		 * Basically, Undo all actions done during this create call */
		return ret;
	}

	mutex_unlock(&dadfs_inodes_mgmt_lock);
	mutex_unlock(&dadfs_directory_children_update_lock);

	inode_init_owner(inode, dir, mode);
	d_add(dentry, inode);

	return 0;
}

static int dadfs_mkdir(struct inode *dir, struct dentry *dentry,
			  umode_t mode)
{
	/* I believe this is a bug in the kernel, for some reason, the mkdir callback
	 * does not get the S_IFDIR flag set. Even ext2 sets is explicitly */
	return dadfs_create_fs_object(dir, dentry, S_IFDIR | mode);
}

// dadfs_bmap
// TODO: address_space_operations dadfs_a_ops = {}

static int dadfs_create(struct inode *dir, struct dentry *dentry,
			   umode_t mode, bool excl)
{
	return dadfs_create_fs_object(dir, dentry, mode);
}

static struct inode *dadfs_iget(struct super_block *sb, int ino)
{
	struct inode *inode;
	struct dadfs_inode *sfs_inode;

	sfs_inode = dadfs_get_inode(sb, ino);

	inode = new_inode(sb);
	inode->i_ino = ino;
	inode->i_sb = sb;
	inode->i_op = &dadfs_inode_ops;

	if (S_ISDIR(sfs_inode->mode))
		inode->i_fop = &dadfs_dir_operations;
	else if (S_ISREG(sfs_inode->mode) || ino == DADFS_JOURNAL_INODE_NUMBER)
		inode->i_fop = &dadfs_file_operations;
	else
		printk(KERN_ERR
					 "Unknown inode type. Neither a directory nor a file");

	/* FIXME: We should store these times to disk and retrieve them */
	inode->i_atime = inode->i_mtime = inode->i_ctime =
			current_time(inode);

	inode->i_private = sfs_inode;

	return inode;
}

struct dentry *dadfs_lookup(struct inode *parent_inode,
			       struct dentry *child_dentry, unsigned int flags)
{
	struct dadfs_inode *parent = DADFS_INODE(parent_inode);
	struct super_block *sb = parent_inode->i_sb;
	struct buffer_head *bh;
	struct dadfs_dir_record *record;
	int i;

	bh = sb_bread(sb, parent->data_block_number);
	BUG_ON(!bh);
	sfs_trace("Lookup in: ino=%llu, b=%llu\n",
				parent->inode_no, parent->data_block_number);

	record = (struct dadfs_dir_record *)bh->b_data;
	for (i = 0; i < parent->dir_children_count; i++) {
		sfs_trace("Have file: '%s' (ino=%llu)\n",
					record->filename, record->inode_no);

		if (!strcmp(record->filename, child_dentry->d_name.name)) {
			/* FIXME: There is a corner case where if an allocated inode,
			 * is not written to the inode store, but the inodes_count is
			 * incremented. Then if the random string on the disk matches
			 * with the filename that we are comparing above, then we
			 * will use an invalid uninitialized inode */

			struct inode *inode = dadfs_iget(sb, record->inode_no);
			inode_init_owner(inode, parent_inode, DADFS_INODE(inode)->mode);
			d_add(child_dentry, inode);
			return NULL;
		}
		record++;
	}

	printk(KERN_ERR
	       "No inode found for the filename [%s]\n",
	       child_dentry->d_name.name);

	return NULL;
}


/**
 * Simplest
 */
void dadfs_destory_inode(struct inode *inode)
{
	struct dadfs_inode *sfs_inode = DADFS_INODE(inode);

	printk(KERN_INFO "Freeing private data of inode %p (%lu)\n",
	       sfs_inode, inode->i_ino);
	kmem_cache_free(sfs_inode_cachep, sfs_inode);
}

static void dadfs_put_super(struct super_block *sb)
{
	struct dadfs_super_block *sfs_sb = DADFS_SB(sb);
	if (sfs_sb->journal)
		WARN_ON(jbd2_journal_destroy(sfs_sb->journal) < 0);
	sfs_sb->journal = NULL;
}

static const struct super_operations dadfs_sops = {
	.destroy_inode = dadfs_destory_inode,
	.put_super = dadfs_put_super,
};

static int dadfs_load_journal(struct super_block *sb, int devnum)
{
	struct journal_s *journal;
	char b[BDEVNAME_SIZE];
	dev_t dev;
	struct block_device *bdev;
	int hblock, blocksize, len;
	struct dadfs_super_block *sfs_sb = DADFS_SB(sb);

	dev = new_decode_dev(devnum);
	printk(KERN_INFO "Journal device is: %s\n", __bdevname(dev, b));

	bdev = blkdev_get_by_dev(dev, FMODE_READ|FMODE_WRITE|FMODE_EXCL, sb);
	if (IS_ERR(bdev))
		return 1;
	blocksize = sb->s_blocksize;
	hblock = bdev_logical_block_size(bdev);
	len = DADFS_MAX_FILESYSTEM_OBJECTS_SUPPORTED;

	journal = jbd2_journal_init_dev(bdev, sb->s_bdev, 1, -1, blocksize);
	if (!journal) {
		printk(KERN_ERR "Can't load journal\n");
		return 1;
	}
	journal->j_private = sb;

	sfs_sb->journal = journal;

	return 0;
}
static int dadfs_sb_load_journal(struct super_block *sb, struct inode *inode)
{
	struct journal_s *journal;
	struct dadfs_super_block *sfs_sb = DADFS_SB(sb);

	journal = jbd2_journal_init_inode(inode);
	if (!journal) {
		printk(KERN_ERR "Can't load journal\n");
		return 1;
	}
	journal->j_private = sb;

	sfs_sb->journal = journal;

	return 0;
}

#define DADFS_OPT_JOURNAL_DEV 1
#define DADFS_OPT_JOURNAL_PATH 2
static const match_table_t tokens = {
	{DADFS_OPT_JOURNAL_DEV, "journal_dev=%u"},
	{DADFS_OPT_JOURNAL_PATH, "journal_path=%s"},
};
static int dadfs_parse_options(struct super_block *sb, char *options)
{
	substring_t args[MAX_OPT_ARGS];
	int token, ret, arg;
	char *p;

	while ((p = strsep(&options, ",")) != NULL) {
		if (!*p)
			continue;

		args[0].to = args[0].from = NULL;
		token = match_token(p, tokens, args);

		switch (token) {
			case DADFS_OPT_JOURNAL_DEV:
				if (args->from && match_int(args, &arg))
					return 1;
				printk(KERN_INFO "Loading journal devnum: %i\n", arg);
				if ((ret = dadfs_load_journal(sb, arg)))
					return ret;
				break;

			case DADFS_OPT_JOURNAL_PATH:
			{
				char *journal_path;
				struct inode *journal_inode;
				struct path path;

				BUG_ON(!(journal_path = match_strdup(&args[0])));
				ret = kern_path(journal_path, LOOKUP_FOLLOW, &path);
				if (ret) {
					printk(KERN_ERR "could not find journal device path: error %d\n", ret);
					kfree(journal_path);
				}

				journal_inode = path.dentry->d_inode;

				path_put(&path);
				kfree(journal_path);

				if (S_ISBLK(journal_inode->i_mode)) {
					unsigned long journal_devnum = new_encode_dev(journal_inode->i_rdev);
					if ((ret = dadfs_load_journal(sb, journal_devnum)))
						return ret;
				} else {
					/** Seems didn't work properly */
					if ((ret = dadfs_sb_load_journal(sb, journal_inode)))
						return ret;
				}

				break;
			}
		}
	}

	return 0;
}

/* This function, as the name implies, Makes the super_block valid and
 * fills filesystem specific information in the super block */
int dadfs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct inode *root_inode;
	struct buffer_head *bh;
	struct dadfs_super_block *sb_disk;
	int ret = -EPERM;

	bh = sb_bread(sb, DADFS_SUPERBLOCK_BLOCK_NUMBER);
	BUG_ON(!bh);

	sb_disk = (struct dadfs_super_block *)bh->b_data;

	printk(KERN_INFO "The magic number obtained in disk is: [%llu]\n",
	       sb_disk->magic);

	if (unlikely(sb_disk->magic != DADFS_MAGIC)) {
		printk(KERN_ERR
		       "The filesystem that you try to mount is not of type dadfs. Magicnumber mismatch.");
		goto release;
	}

	if (unlikely(sb_disk->block_size != DADFS_DEFAULT_BLOCK_SIZE)) {
		printk(KERN_ERR
		       "dadfs seem to be formatted using a non-standard block size.");
		goto release;
	}
	/** XXX: Avoid this hack, by adding one more sb wrapper, but non-disk */
	sb_disk->journal = NULL;

	printk(KERN_INFO
	       "dadfs filesystem of version [%llu] formatted with a block size of [%llu] detected in the device.\n",
	       sb_disk->version, sb_disk->block_size);

	/* A magic number that uniquely identifies our filesystem type */
	sb->s_magic = DADFS_MAGIC;

	/* For all practical purposes, we will be using this s_fs_info as the super block */
	sb->s_fs_info = sb_disk;

	sb->s_maxbytes = DADFS_DEFAULT_BLOCK_SIZE;
	sb->s_op = &dadfs_sops;

	root_inode = new_inode(sb);
	root_inode->i_ino = DADFS_ROOTDIR_INODE_NUMBER;
	inode_init_owner(root_inode, NULL, S_IFDIR);
	root_inode->i_sb = sb;
	root_inode->i_op = &dadfs_inode_ops;
	root_inode->i_fop = &dadfs_dir_operations;
	root_inode->i_atime = root_inode->i_mtime = root_inode->i_ctime =
	    current_time(root_inode);

	root_inode->i_private =
	    dadfs_get_inode(sb, DADFS_ROOTDIR_INODE_NUMBER);

	/* TODO: move such stuff into separate header. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 3, 0)
	sb->s_root = d_make_root(root_inode);
#else
	sb->s_root = d_alloc_root(root_inode);
	if (!sb->s_root)
		iput(root_inode);
#endif

	if (!sb->s_root) {
		ret = -ENOMEM;
		goto release;
	}

	if ((ret = dadfs_parse_options(sb, data)))
		goto release;

	if (!sb_disk->journal) {
		struct inode *journal_inode;
		journal_inode = dadfs_iget(sb, DADFS_JOURNAL_INODE_NUMBER);

		ret = dadfs_sb_load_journal(sb, journal_inode);
		goto release;
	}
	ret = jbd2_journal_load(sb_disk->journal);

release:
	brelse(bh);

	return ret;
}

static struct dentry *dadfs_mount(struct file_system_type *fs_type,
				     int flags, const char *dev_name,
				     void *data)
{
	struct dentry *ret;

	ret = mount_bdev(fs_type, flags, dev_name, data, dadfs_fill_super);

	if (unlikely(IS_ERR(ret)))
		printk(KERN_ERR "Error mounting dadfs");
	else
		printk(KERN_INFO "dadfs is succesfully mounted on [%s]\n",
		       dev_name);

	return ret;
}

static void dadfs_kill_superblock(struct super_block *sb)
{
	printk(KERN_INFO
	       "dadfs superblock is destroyed. Unmount succesful.\n");
	/* This is just a dummy function as of now. As our filesystem gets matured,
	 * we will do more meaningful operations here */

	kill_block_super(sb);
	return;
}

struct file_system_type dadfs_fs_type = {
	.owner = THIS_MODULE,
	.name = "dadfs",
	.mount = dadfs_mount,
	.kill_sb = dadfs_kill_superblock,
	.fs_flags = FS_REQUIRES_DEV,
};

static int dadfs_init(void)
{
	int ret;

	sfs_inode_cachep = kmem_cache_create("sfs_inode_cache",
	                                     sizeof(struct dadfs_inode),
	                                     0,
	                                     (SLAB_RECLAIM_ACCOUNT| SLAB_MEM_SPREAD),
	                                     NULL);
	if (!sfs_inode_cachep) {
		return -ENOMEM;
	}

	ret = register_filesystem(&dadfs_fs_type);
	if (likely(ret == 0))
		printk(KERN_INFO "Sucessfully registered dadfs\n");
	else
		printk(KERN_ERR "Failed to register dadfs. Error:[%d]", ret);

	return ret;
}

static void dadfs_exit(void)
{
	int ret;

	ret = unregister_filesystem(&dadfs_fs_type);
	kmem_cache_destroy(sfs_inode_cachep);

	if (likely(ret == 0))
		printk(KERN_INFO "Sucessfully unregistered dadfs\n");
	else
		printk(KERN_ERR "Failed to unregister dadfs. Error:[%d]",
		       ret);
}

module_init(dadfs_init);
module_exit(dadfs_exit);

MODULE_LICENSE("CC0");
MODULE_AUTHOR("Dad");
