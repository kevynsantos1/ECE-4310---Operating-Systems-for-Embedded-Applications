#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kern_levels.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
MODULE_LICENSE("GPL");

#define ECE_BUF_SIZE 256 

#define ECE_RESET_W _IO('k', 1) //_IO means no data passed, it encodes our arguments into a unique value, first argument is some char to differentiate in the case we have other drivers, second argument is command number just so _IO can encode a different command to a different value
#define ECE_RESET_R _IO('k', 2)

static char ece_buffer[ECE_BUF_SIZE]; 
int isReg; //flag for if register_chrdev worked.
int major; //used to hold our major number
int ece_offset_w; //offset to keep track of our index when writing. for checks that we dont go over our buffer size and so we dont overwrite anything
int ece_offset_r; //
int ece_size;

int ece_init(void); //prototype for our module init function
void ece_end(void); //prototype for our module exit function
static ssize_t ece_write(struct file*, const char*, size_t, loff_t*); 
static ssize_t ece_read(struct file*, char*, size_t, loff_t*);

static long int my_ioctl(struct file*f, unsigned int cmd, unsigned long args);

static struct file_operations ece_fops = //our operations table
{
    .read = ece_read,
    .write = ece_write,
    .unlocked_ioctl = my_ioctl,
};

int ece_init(void)
{
    major = register_chrdev(0, "test_mod02", &ece_fops); //registers a character device (our code). allows for us to provide our file operations structure. using a 0 for our major number argument tells the kernel to assign us one. the function will return our assigned major number or if its a negative number it failed.
    ece_offset_w = 0;
    ece_offset_r = 0;
    ece_size = 0;

    if(major < 0) //if register_chrdev failed
    {
        isReg = 0; //isRegistered is not true
        printk(KERN_INFO " ECE4310: Start FAIL \n");
    }
    else //if register_chrdev succeeded
    {
        isReg = 1; //isRegistered is true
        printk(KERN_INFO " ECE4310: Start here \n");
    }

    return 0;
}

void ece_end(void)
{
    if(isReg) //if we actually started (registered our major number) then we can unregister our major number.
    {
        unregister_chrdev(major, "test_mod02"); //unregisters our character device (our code) from kernel list by making sure the major number and name of the device match.
    }
    printk(KERN_INFO " ECE4310: End here \n");
}

static ssize_t ece_write(struct file *fp, const char * buf, size_t count, loff_t * op) //function for writing. *fp is a pointer to our file /dev/test_mod02. *buf is a pointer to the user-space buffer that contains our char bytes that we want to write into the device. count is where we saves the number of bytes the user writes + 1 for newline. *op is a pointer to the current position in the file like an offset
{
    int ret = 0; //we use this as a flag for when we copy user input
    if(ece_offset_w + count >= ECE_BUF_SIZE)//checks to make sure we dont exceed the buffer size. It checks because we update our ece_offset_w after every entry. 
    {
        printk(KERN_INFO " ECE4310: Write overflow. Abort. \n");
        
        return -1;//failed due to overflow.
    }

    printk(KERN_INFO " ECE4310: Copy from user. \n"); //means we are able to continue because we did not exceed the buffer size
    ret = copy_from_user(&ece_buffer[ece_offset_w], buf, count); //first argument is where we are writing to (in this case an array with a max buffer size. in specific we are writing at the index of the write offset), second argument is were we are writing from (in our case we are writing from the user-space buffer), third argument is how many bytes we are copying (we get this from count)
    if (ret != 0) //copy_from_user returns 0 when it succeeds so this is in the case it failed.
    {
        printk(KERN_INFO "ECE4310: ERR copy from user. \n");
        return -1;
    }
    ece_offset_w = ece_offset_w + count; //updates the offset so that we continue writing at the correct index

    return count; //returns the count to show we successfully transferred that many bytes
}

static ssize_t ece_read(struct file *fp, char *buf, size_t count, loff_t *offset) //function for reading. *fp is a pointer to our file /dev/test_mod02. *buf is a pointer to the user-space buffer that contains our char bytes that we want to copy from. count is the number of bytes we want to read. *offset is a pointer to the current position in the file (we dont end up using it and use our own offset)
{
    int ret; //we use this as a flag for when we copy to user
    if(ece_offset_r + count >= ECE_BUF_SIZE) //checks to make sure we dont exceed the buffer size. It checks because we update our ece_offset_r after every copy to user.
    {
        printk(KERN_INFO "ECE4310: Read overflow. Abort. \n");
        
        return -1;//the read failed because we exceeded buf size
    }
    printk(KERN_INFO "ECE4310: Copy to user. \n"); //means we did not overflow when reading
    ret = copy_to_user(buf, &ece_buffer[ece_offset_r], count); //first argument is where we are reading to (in this case it is the user-space buffer), second argument is where we are reading from (our array that we wrote to earlier), third argument is the count of how many bytes we are reading
    if(ret != 0) //copy to user returns 0 when successful but this is when it fails
    {
        printk(KERN_INFO "ECE4310: ERR copy to user. \n");
        return -1;
    }

    ece_offset_r = ece_offset_r + count; //updates the read offset so that next time we read we start at the correct spot

    return count; //returns the number of bytes we counted to show we successfully read that many bytes
}

static long int my_ioctl(struct file*f, unsigned int cmd, unsigned long args) //first argument is pointer to the currently opened device file, second argument is the command which we are passing to ioctl, third argument is a pointer to receive or pass some data.
{
    printk(KERN_INFO "Hello_Cdev - ioctl called with cmd: %d and args: %p\n", cmd, (void*) args);

    switch (cmd)
    {
        case(ECE_RESET_W):
        ece_offset_w = 0;
        printk(KERN_INFO "Write offset reset");
        break;
        case(ECE_RESET_R):
        ece_offset_r = 0;
        printk(KERN_INFO "Read offset reset\n");
        break;
        default:
        printk(KERN_INFO "UNKNOWN COMMAND BYE");
        break;
    }

    return 0;
}



module_init(ece_init);
module_exit(ece_end);

//current task: start on IOcontrol
//looks like we need to define _IOW, _IOR. seems like standard practice but can we implement our own read and write instead 