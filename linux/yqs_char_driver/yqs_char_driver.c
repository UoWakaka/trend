#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>

static dev_t dev;
static int dev_count = 200;
static const char *DEV_NAME = "yqs_char_driver";
static struct cdev *yqs_cdev = NULL;
static struct class *dev_class = NULL;
static struct device *dev_object = NULL;

static int yqs_open(struct inode *inode, struct file *file)
{
    int major = MAJOR(inode->i_rdev);
    int minor = MINOR(inode->i_rdev);
    printk("%s: major=%d, minor=%d\n", __func__, major, minor);
    return 0;
}

static ssize_t yqs_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    if (copy_to_user(buf, "YQS\n", 4))
    {
        return -EFAULT;
    }
    return 4;
}

static ssize_t yqs_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
    char d[64];
    len = (len > 63 ? 63 : len);
    if (copy_from_user(d, buf, len))
    {
        return -EFAULT;
    }
    d[len] = 0;

    printk("yqs_write: len=%ld, data=%s\n", len, d);
    return len;
}

static const struct file_operations yqs_fops = {
    .owner = THIS_MODULE,
    .open = yqs_open,
    .read = yqs_read,
    .write = yqs_write,
};

static int __init yqs_init(void)
{
    int ret = 0;

    if ((ret = alloc_chrdev_region(&dev, 0, dev_count, DEV_NAME)))
    {
        printk("failed to allocate char device region\n");
        return ret;
    }

    if (!(yqs_cdev = cdev_alloc()))
    {
        printk("failed to cdev alloc\n");
        goto unregister_chrdev;
    }

    yqs_cdev->owner = THIS_MODULE;
    cdev_init(yqs_cdev, &yqs_fops);
    if ((ret = cdev_add(yqs_cdev, dev, dev_count)))
    {
        printk("failed to cdev add\n");
        goto cdev_fail;
    }

    // auto mknod
    dev_class = class_create(THIS_MODULE, DEV_NAME);
    if (IS_ERR(dev_class))
    {
        printk("failed to create class\n");
        goto cdev_fail;
    }

    dev_object = device_create(dev_class, NULL, dev, NULL, DEV_NAME);
    if (IS_ERR(dev_object))
    {
        class_destroy(dev_class);
        printk("failed to create device\n");
        goto cdev_fail;
    }

    printk("success to create yqs char driver: %s, major=%d, minor=%d\n", DEV_NAME, MAJOR(dev), MINOR(dev));
    return 0;

cdev_fail:
    cdev_del(yqs_cdev);
unregister_chrdev:
    unregister_chrdev_region(dev, dev_count);

    return ret;
}

static void __exit yqs_exit(void)
{
    printk("yqs_exit: %s, major=%d, minor=%d\n", DEV_NAME, MAJOR(dev), MINOR(dev));
    if (dev_object)
    {
        device_destroy(dev_class, dev);
    }
    if (dev_class)
    {
        class_destroy(dev_class);
    }
    if (yqs_cdev)
    {
        cdev_del(yqs_cdev);
    }
    unregister_chrdev_region(dev, dev_count);
}

module_init(yqs_init);
module_exit(yqs_exit);
MODULE_LICENSE("GPL");
