#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/cdev.h>         /* kernel device */
#include <linux/init.h>         /* km init */
#include <linux/module.h>       /* kernel module */
#include <linux/fs.h>           /* file_operations structure */
#include <linux/uaccess.h>      /* copy_to_user */
#include <linux/kdev_t.h>       /* MKDEV */
#include <linux/export.h>       /* THIS_MODULE */
#include <linux/time64.h>
#include <linux/timekeeping.h>
#include <linux/random.h>       /* get_random_bytes */
#include <linux/slab.h>         /* kmalloc */

#include "dice.h"

int dice_major;                 /* major number of the device */
static const char dice_name[] = "Dice";
int dice_devs = DICE_DEVS;    	/* number of devices */
int dice_num = DICE_NUM; 	    /* initial number of dice in a device */
int gen_sides = GEN_SIDES; 	    /* initial number of sides of `arbitrary` dice game */

module_param(gen_sides, int, 0);

// struct dice_dev *dice_devices;
struct dice_dev dice_devices[DICE_DEVS];

static int dice_open(struct inode *inode, struct file *filp)
/*
 * EFFECTS: called when device is opened;
 *          sets up `filp->private_data` as current device
 */
{
    // hints: container_of
    unsigned int minor_num = MINOR(inode->i_rdev);
    printk(KERN_NOTICE "Dice: opening device with minor %d\n",minor_num);
    filp->private_data = &dice_devices[minor_num];
    struct dice_dev *dev = (struct dice_dev *)filp->private_data;
    
    return 0;
}

static int dice_release(struct inode *inode, struct file *filp)
/*
 * EFFECTS: called when device is closed; do nothing here
 */
{
    return 0;
}

static ssize_t dice_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
/*
 * EFFECTS: called when `read` is performed on the device;
 *          output `dice_num` dice, with faces randomly generated
 *          according to `dice_num` and `dice_type` stored in `filp->private_data`
 */
{
    // hints: __put_user
    printk(KERN_NOTICE "Dice: outputing data\n");

    struct dice_dev *dev = (struct dice_dev *)filp->private_data;
    int dice_type = dev->dice_type;
    dice_num = dev->num;

    int strcount = 0;
#define MAX_DICE_STR 10240   
    char str[MAX_DICE_STR];

    int rd[dice_num];
    int i;
    int err = 0;
    
    if (dice_type == TYPE_REGULAR){
        // regular
        printk(KERN_NOTICE "Dice: outputing regular dice | ");
        for(i=0;i<dice_num;i++){
            get_random_bytes(&rd[i], sizeof(int));
            rd[i] = ((rd[i] % 6) + 12) % 6; // 0~5
            // printk("%d ",rd[i]);
        }
        // printk("\n");

        static const char DICE_REG_PATTERN[3][6][12] = {
            {"|     |  ","|     |  ","|  o  |  ","| o o |  ","| o o |  ","| o o |  "},
            {"|  o  |  ","| o o |  ","|     |  ","|     |  ","|  o  |  ","| o o |  "},
            {"|     |  ","|     |  ","| o o |  ","| o o |  ","| o o |  ","| o o |  "}
        };

        for(i=0;i<dice_num;i++) err += snprintf(str+err,MAX_DICE_STR,"-------  ");
        err += snprintf(str+err,MAX_DICE_STR,"\n");
        for(i=0;i<dice_num;i++) err += snprintf(str+err,MAX_DICE_STR,"%s",DICE_REG_PATTERN[0][rd[i]]);
        err += snprintf(str+err,MAX_DICE_STR,"\n");
        for(i=0;i<dice_num;i++) err += snprintf(str+err,MAX_DICE_STR,"%s",DICE_REG_PATTERN[1][rd[i]]);
        err += snprintf(str+err,MAX_DICE_STR,"\n");
        for(i=0;i<dice_num;i++) err += snprintf(str+err,MAX_DICE_STR,"%s",DICE_REG_PATTERN[2][rd[i]]);
        err += snprintf(str+err,MAX_DICE_STR,"\n");
        for(i=0;i<dice_num;i++) err += snprintf(str+err,MAX_DICE_STR,"-------  ");
        err += snprintf(str+err,MAX_DICE_STR,"\n");
    }
    else if (dice_type == TYPE_BACKGAMMON){
        // backgammon
        printk(KERN_NOTICE "Dice: outputing backgammon dice | ");
        for(i=0;i<dice_num;i++){
            get_random_bytes(&rd[i], sizeof(int));
            rd[i] = ((rd[i] % 6) + 12) % 6; // 0~5
            // printk("%d ",rd[i]);
        }
        // printk("\n");

        static const char DICE_BACKGAMMON[6][4] = {
            "2","4","8","16","32","64"
        };

        for(i=0;i<dice_num;i++) err += snprintf(str+err,MAX_DICE_STR,"%s ",DICE_BACKGAMMON[rd[i]]);
        err += snprintf(str+err,MAX_DICE_STR,"\n");
    }
    else if (dice_type == TYPE_GENERIC){
        // arbitrary number of sides
        printk(KERN_NOTICE "Dice: outputing generic dice | ");
        for(i=0;i<dice_num;i++){
            get_random_bytes(&rd[i], sizeof(int));
            rd[i] = ((rd[i] % gen_sides) + 2*gen_sides) % gen_sides + 1; // 0~5
            // printk("%d ",rd[i]);
        }
        // printk("\n");

        for(i=0;i<dice_num;i++) err += snprintf(str+err,MAX_DICE_STR,"%d ",rd[i]);
        err += snprintf(str+err,MAX_DICE_STR,"\n");
    }
    
    /* examing output to user space */
    if (err<0){
        printk(KERN_NOTICE "Dice: error in snprintf\n");
    }
    strcount = err;
    if ( *offp >= strcount ) {
        printk(KERN_NOTICE "Dice: printer reaches ending, aborting\n");
        return 0;
    }
    if ( *offp + count > strcount ){
        count = strcount - *offp;
    }
    if ( copy_to_user(buff, str+*offp, count) != 0 ){
        printk(KERN_NOTICE "Dice: copy_to_user error, aborting\n");
        return -EFAULT;
    }
    *offp += count;

    return count;
}

static ssize_t dice_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
/*
 * EFFECTS: called when `write` is performed on the device;
 *          change `dice_num` according to the number written
 */
{
    // hints: __get_user
    // int retval = 0; // return value
    struct dice_dev *dev = (struct dice_dev *)filp->private_data;
#define MAX_DICE_IN 10
    char input_str[MAX_DICE_IN];
    long int tmp_num = 0;
    // retval = __get_user(dice_num, buff);
    // if(retval != 0){ // error
    //     printk(KERN_NOTICE "Dice: error on getting user's input!\n");
    // }
    // if(dice_num == '\n') return 1;
    // retval = 1; // return number of bytes written
    if (count > MAX_DICE_IN){
        printk(KERN_NOTICE "Dice: too much input");
        return -EINVAL;
    }
    if ( copy_from_user(input_str, buff, count) != 0 ){
        printk(KERN_NOTICE "Dice: copy_from_user error!\n");
        return -EINVAL;
    }
    input_str[count-1] = '\0';
    if (kstrtol(input_str, 10, &tmp_num) != 0){
        printk(KERN_NOTICE "Dice: kstrtol error handling <%s> with count %d!\n", input_str, (int)count);
    }
    dice_num = (int) tmp_num;
    // dice_num = dice_num - '0';
    printk(KERN_NOTICE "Dice: new dice number assigned: %d\n",dice_num);
    // printk("return: %d ", retval);
    dev->num = dice_num;
    // return retval;
    return count;
}

/**
 * @brief file_operations structure
 * 
 */
static struct file_operations fops = {
    .read = dice_read,
    .write = dice_write,
    .open = dice_open,
    .release = dice_release,
};

static void dice_setup_cdev(struct cdev *dev, int i)
/*
 * EFFECTS: registers `dev` with major number `dice_major` and minor number `i`
 */
{
    int err, devno = MKDEV(dice_major, i);

    cdev_init(dev, &fops);
    dev->owner = THIS_MODULE;
    dev->ops = &fops;
    err = cdev_add(dev, devno, 1);
    /* Fail gracefully if need be */
    if (err)
        printk(KERN_NOTICE "Error %d adding dice%d", err, i);
}

static int __init dice_initialization(void)
/*
 * EFFECTS: Initialization; called when insmod
 */
{
    /* Register your major dynamically */
    printk("Initializing Grandpa\'s Dice Module...\n");
    int reg_result = 0;
    dev_t dev = MKDEV(dice_major, 0);
    /* kernel memory allocation */
    // dice_devices = kmalloc(dice_devs * sizeof(struct dice_dev), GFP_KERNEL);

    /* figure out the device number */
    if (dice_major) reg_result = register_chrdev_region(dev, dice_devs, dice_name);
    else{
        // baseminor: 0, count: DICE_DEVS, name: dice_name
        reg_result = alloc_chrdev_region(&dev, 0, dice_devs, dice_name);
        dice_major = MAJOR(dev);
        // reg_result = register_chrdev(0, dice_name, &fops);
    }
    /* err handling */
    if (reg_result < 0){
        printk(KERN_WARNING "Dice: fail to register the device with error %d\n", reg_result);
        return reg_result;
    }
    if (dice_major == 0) dice_major = reg_result;

    /* Char device registration; three devices (three minor numbers) for three types of dice */
    int i;
    for (i=0; i<DICE_DEVS; i++){
        // dice_setup_cdev(&((dice_devices + i)->d_cdev), i);
        dice_setup_cdev(&(dice_devices[i].d_cdev), i);
    }
    dice_devices[0].dice_type = TYPE_REGULAR;
    dice_devices[1].dice_type = TYPE_BACKGAMMON;
    dice_devices[2].dice_type = TYPE_GENERIC;
    dice_devices[0].num = dice_num;
    dice_devices[1].num = dice_num;
    dice_devices[2].num = dice_num;
    printk(KERN_NOTICE "Dice: successfully register the device with major number %d\n",dice_major);
    printk("Hi Grandpa!\n");
    /* Initialize random number; we omit it here */

    return 0;
}

static void __exit dice_cleanup(void)
/*
 * EFFECTS: Cleanup; called when rmmod
 */
{
    /* Free char device */
    int i;
    for (i=0; i<DICE_DEVS; i++){
        // cdev_del(&((dice_devices + i)->d_cdev));
        cdev_del(&(dice_devices[i].d_cdev));
    }

    // kfree(dice_devices);

    /* Unregister */
    printk(KERN_WARNING "Grandpa, professor Horst told us that do not gamble, probably you should listen to him.\n");
    printk(KERN_NOTICE "Dice: unregistering the device...\n");
    unregister_chrdev(dice_major, dice_name);
    unregister_chrdev_region(MKDEV(dice_major, 0), dice_devs);
    printk("Hope Mum is not at home. Enjoy the day, goodbye!\n");
}

module_init(dice_initialization);
module_exit(dice_cleanup);

MODULE_DESCRIPTION("Grandpa's Dice Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("matrixpecker");
