#include<linux/init.h>
#include<linux/module.h>
#include<linux/types.h>	//dev_t
#include<linux/kdev_t.h> //Holds MACROs for MAJOR and MINOR
#include<linux/fs.h>	//file operations structure and file structure
#include<linux/cdev.h>	//Char Device Registration- cdev structure
#include<asm/uaccess.h>	//copy to and copy from functions
#include<linux/errno.h>
MODULE_LICENSE("GPL");
#define MAX 1024


		int major=255;
		int minor=0;
		dev_t device_number;
		char Kbuff[MAX],K1buff[MAX]="Hello I am Kernel";
		int my_open(struct inode *inode,struct file *filp);
		int my_release(struct inode *inode,struct file *filp);
		ssize_t my_read(struct file *,char __user *,size_t ,loff_t *);
		ssize_t my_write(struct file *,const char __user *,size_t ,loff_t *);

		struct file_operations fops={		//file operations to be done on driver

		.owner=THIS_MODULE,
		.open=my_open,
		.release=my_release,
		.read=my_read,
		.write=my_write,
		};

		struct cdev *my_cdev;
		static int __init dev_init(void)
		{
			//Allocation of device numbers for drivers use.
			int alloc_devnumber,charreg;
			device_number=MKDEV(major,minor);
			alloc_devnumber=register_chrdev_region(device_number,1,"Surya_Device");
			if(alloc_devnumber < 0)
				{
					printk("Device Number is not  allocated\n");
					//exit(EXIT_FAILURE);
				}
			//Char device registration
			my_cdev=cdev_alloc();
			my_cdev->ops=&fops;

			charreg=cdev_add(my_cdev,device_number,1);
			if(charreg < 0)
				{
					printk("Device is not added to the system\n");
					//exit(EXIT_FAILURE);
				}
			return 0;
		}

		static void __exit dev_clean(void)
		{
			unregister_chrdev_region(device_number,1);
			cdev_del(my_cdev);
			printk("Bye Bye Driver\n");
		}

		int my_open(struct inode *inode,struct file *filp)
		{
			printk("\nU are in Open function\n");
			return 0;
		}

		int my_release(struct inode *inode,struct file *filp)
		{
			printk("\nU are in Release function\n");
			return 0;
		}

		ssize_t my_read(struct file *filp,char __user *Ubuff,size_t count,loff_t *offp)
		{
			int retval,copyto;
			copyto=copy_to_user((char *)Ubuff,(char *)K1buff,count);
			return count;
			if(copyto < 0)
			{
				printk("\nError in copy to user\n");
				retval=-EFAULT;
				return retval;
			}

		}

		ssize_t my_write(struct file *filp,const char __user *Ubuff,size_t count,loff_t *offp)
		{
			int retval,copyfrom;
			copyfrom=copy_from_user((char *)Kbuff,(char *)Ubuff,count);

			if(copyfrom >=0)
			{
				printk("\nFrom KBUFF: %s\n",Kbuff);
				return count;
			}
			else
			{
				printk("\nError in copy from user\n");
				retval=-EFAULT;
				return retval;
			}
		}

		module_init(dev_init);
		module_exit(dev_clean);
