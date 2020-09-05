/*
 *  (C) Copyright 2011
 *  Author : Mindspeed Technologes
 *  
 *  See file CREDITS for list of people who contributed to this
 *  project.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA 02111-1307 USA
 * */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>



static struct proc_dir_entry *pdir = NULL;
static struct proc_dir_entry *unit = NULL;
static struct proc_dir_entry *pkt = NULL;
static struct proc_dir_entry *dump = NULL;


extern void pfe_class_read(int id);

ssize_t pfe_unit_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	printk(KERN_INFO "%s:\n", __func__);
	return 0;
}

static unsigned char lbuf[20];
ssize_t pfe_unit_write(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	int id = -1;

	printk(KERN_INFO "%s:\n", __func__);

	if (copy_from_user(lbuf, buf,count))
		return -ENOMEM;
	lbuf[count] = 0;

	id = simple_strtoul(lbuf, NULL, 0);
	printk(KERN_INFO "id: %d\n", id);

	if ((id >= 0) || (id < 4))
		pfe_class_read(id);
	
	return count;
}

extern void pfe_print_tx_status(void);
ssize_t pfe_pkt_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	printk(KERN_INFO "%s:\n", __func__);
	pfe_print_tx_status();
	return 0;
}

extern void pfe_send_pkt(int len);

ssize_t pfe_pkt_write(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	int len = -1;

	printk(KERN_INFO "%s:\n", __func__);

	if (copy_from_user(lbuf, buf,count))
		return -ENOMEM;
	lbuf[count] = 0;

	len = simple_strtoul(lbuf, NULL, 0);
	printk(KERN_INFO "len: %d\n", len);

	pfe_send_pkt(len);

	return count;
}
//============================================================
extern void	hif_tx_desc_dump(void);
extern void	hif_rx_desc_dump(void);
extern void class_pe_dump(void);

ssize_t pfe_dump_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	printk(KERN_INFO "%s:\n", __func__);
	hif_tx_desc_dump();
	hif_rx_desc_dump();
   class_pe_dump();
	return 0;
}

extern void class_dmem_dump(int id);
ssize_t pfe_dump_write(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	int id;
	printk(KERN_INFO "%s:\n", __func__);

	if (copy_from_user(lbuf, buf,count))
		return -ENOMEM;
	lbuf[count] = 0;

	id = simple_strtoul(lbuf, NULL, 0);
	printk(KERN_INFO "id: %d\n", id);

	class_dmem_dump(id);

	return count;
}
//============================================================

static struct file_operations pfe_unit_fops = {
	.owner = THIS_MODULE,
	.read = pfe_unit_read,
	.write = pfe_unit_write,
};

static struct file_operations pfe_pkt_fops = {
	.owner = THIS_MODULE,
	.read = pfe_pkt_read,
	.write = pfe_pkt_write,
};

static struct file_operations pfe_dump_fops = {
	.owner = THIS_MODULE,
	.read = pfe_dump_read,
	.write = pfe_dump_write,
};

void pfe_proc_init(void)
{
	pdir = proc_mkdir("pfe", NULL);	
	if (pdir == NULL) {
		printk(KERN_ERR "%s: Unable to create proc dir entry\n", __func__);
		return;
	}

	unit = create_proc_entry("unit", S_IFREG, pdir);
	if (unit == NULL) {
		printk(KERN_ERR "%s: Unable to create proc entry: unit\n", __func__);
		remove_proc_entry("pfe", NULL);
		return;
	}
	unit->proc_fops = &pfe_unit_fops;

	pkt = create_proc_entry("pkt", S_IFREG, pdir);
	if (unit == NULL) {
		printk(KERN_ERR "%s: Unable to create proc entry: pkt\n", __func__);
		remove_proc_entry("unit", pdir);
		remove_proc_entry("pfe", NULL);
		return;
	}
	pkt->proc_fops = &pfe_pkt_fops;

	dump = create_proc_entry("dump", S_IFREG, pdir);
	if (dump == NULL) {
		printk(KERN_ERR "%s: Unable to create proc entry: pkt\n", __func__);
		remove_proc_entry("unit", pdir);
		remove_proc_entry("pkt", pdir);
		remove_proc_entry("pfe", NULL);
		return;
	}
	dump->proc_fops = &pfe_dump_fops;


	
}

void pfe_proc_exit(void)
{
	if (unit)
		remove_proc_entry("unit", pdir);
	if (pkt)
		remove_proc_entry("pkt", pdir);
	if (dump)
		remove_proc_entry("dump", pdir);
	if (pdir) 
		remove_proc_entry("pfe", NULL);

	return;
}

