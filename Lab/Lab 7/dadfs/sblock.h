#ifndef DADFS_SBLOCK
#define DADFS_SBLOCK

#include "base.h"

static inline struct dadfs_super_block *DADFS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}

static inline struct dadfs_inode *DADFS_INODE(struct inode *inode)
{
	return inode->i_private;
}

#endif // #ifndef DADFS_SBLOCK
