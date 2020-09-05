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
#include <linux/pci.h>
#include <linux/vmalloc.h>


#include "pfe_mod.h"
#include "pfe_driver.h"

struct pfe *pfe;

#if 0
static struct pci_device_id pfe_pci_tbl[] =
{
	{ 0x0700, 0x1108, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0 },
	{ 0 },
};
#else
static struct pci_device_id pfe_pci_tbl[] =
{
	{ 0x0700, 0x1107, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0 },
	{ 0 },
};
#endif


extern void pfe_proc_init(void);
static int __devinit pfe_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	int rc;
	unsigned long mem_start, mem_end, mem_len;

	printk(KERN_INFO "%s\n", __func__);

	pfe = kzalloc(sizeof(struct pfe), GFP_KERNEL);
	if (!pfe) {
		rc = -ENOMEM;
		goto err_alloc;
	}

	pci_set_drvdata(pdev, pfe);

	rc = pci_enable_device(pdev);
	if (rc < 0)
	{
		printk(KERN_INFO "pci_enable_device() failed\n");
		goto err_pci_enable;
	}

	printk(KERN_INFO "PCI device enabled\n");

	mem_start = pci_resource_start(pdev, 0);
	mem_end = pci_resource_end(pdev, 0);
	mem_len = pci_resource_len(pdev, 0);

	printk(KERN_INFO "PCI resource 0 %#lx:%#lx (%lx)\n", mem_start, mem_end, mem_len);

#if 0
	if (!(pci_resource_flags(pdev, 0) & IORESOURCE_MEM))
	{
		printk("\n cann't read PCI resource flags");
		goto err_out_disable_pdev;
	}

	printk("\n PCI resource flags read as MEM mapped");


	io_start = pci_resource_start(pdev, 1);
	printk("\n PCI resource start address:%x ",io_start);
	io_end = pci_resource_end(pdev, 1);
	printk("\n PCI resource end address:%x ",io_end);
	io_len = pci_resource_len(pdev, 1);
	printk("\n PCI resource end address:%x ",io_len);
	
	if (!(pci_resource_flags(pdev, 1) & IORESOURCE_IO))
	{
		printk("\n cann't read PCI resource flags");
		goto err_out_disable_pdev;
	}

	printk("\n PCI resource flags read as IO mapped");
#endif
	if ((rc = pci_request_regions(pdev, "pfe-pci"))) {
		printk(KERN_INFO "pci_request_regions() failed\n");
		goto err_pci_request;
	}
#if 0
	printk("\n PCI acquired regions");
	if ((rc = pci_set_dma_mask(pdev, DMA_32BIT_MASK))){
		printk("\n No usable DMA configuration");
		goto err_out_free_res;
	}

	printk("using i/o access mode\n");
#endif
	pfe->cbus_baseaddr = ioremap_nocache(mem_start, mem_len);
	if (!pfe->cbus_baseaddr) {
		printk(KERN_INFO "ioremap() cbus failed\n");
		rc = -ENOMEM;
		goto err_cbus;
	}

	pfe->ddr_baseaddr = pfe->cbus_baseaddr + 8 * SZ_1M;
	pfe->ddr_phys_baseaddr = 0x20000; /* FIXME This is the physical address seen by the FPGA ... */

	pfe->apb_baseaddr = vmalloc(16 * SZ_1M);
	if (!pfe->apb_baseaddr) {
		printk(KERN_INFO "vmalloc() apb failed\n");
		rc = -ENOMEM;
		goto err_apb;
	}

	pfe->iram_baseaddr = vmalloc(128 * SZ_1K);
	if (!pfe->iram_baseaddr) {
		printk(KERN_INFO "vmalloc() iram failed\n");
		rc = -ENOMEM;
		goto err_iram;
	}

	pfe->hif_irq = pdev->irq;
	pfe->dev = &pdev->dev;
	pfe->pdev = pdev;

	rc = pfe_probe(pfe);
	if (rc < 0)
		goto err_probe;

	/* GEMAC init */	
	pfe_gemac_init(EMAC1_BASE_ADDR, MII, SPEED_100M, DUPLEX_FULL);
	pfe_gemac_init(EMAC2_BASE_ADDR, MII, SPEED_100M, DUPLEX_FULL);

	gemac_enable_mdio(EMAC1_BASE_ADDR);
	gemac_enable_mdio(EMAC2_BASE_ADDR);

	/*Put in promiscuous mode*/
	gemac_enable_copy_all(EMAC1_BASE_ADDR);
	gemac_enable_copy_all(EMAC2_BASE_ADDR);

	/* set ethernet mac address */
	//c2000_set_ethaddr(edev, priv->einfo->mac_addr);
	pfe_gem_enable_all();
	pfe_proc_init();


	return 0;

err_probe:
	vfree(pfe->iram_baseaddr);

err_iram:
	vfree(pfe->apb_baseaddr);

err_apb:
	iounmap(pfe->cbus_baseaddr);

err_cbus:
	pci_release_regions(pdev);

err_pci_request:
	pci_disable_device(pdev);

err_pci_enable:

	pci_set_drvdata(pdev, NULL);

	kfree(pfe);

err_alloc:
	return rc;
}


extern void pfe_proc_exit(void);
static void __devexit pfe_pci_remove (struct pci_dev *pdev)
{
	struct pfe *pfe = pci_get_drvdata(pdev);

	printk(KERN_INFO "%s\n", __func__);

	pfe_remove(pfe);

	pfe_proc_exit();

	vfree(pfe->iram_baseaddr);
	vfree(pfe->apb_baseaddr);

	iounmap(pfe->cbus_baseaddr);

	pci_release_regions(pdev);
	pci_disable_device(pdev);
	pci_set_drvdata(pdev, NULL);

	kfree(pfe);
}


static struct pci_driver pfe_pci_driver = {
	.name       = "pfe-pci",
	.id_table   = pfe_pci_tbl,
	.probe      = pfe_pci_probe,
	.remove     = __devexit_p(pfe_pci_remove),
};


static int __init pfe_module_init(void)
{
	printk(KERN_INFO "%s\n", __func__);

	return pci_register_driver(&pfe_pci_driver);
}

static void __exit pfe_module_exit(void)
{
	pci_unregister_driver(&pfe_pci_driver);

	printk(KERN_INFO "%s\n", __func__);
}

MODULE_LICENSE("GPL");
module_init(pfe_module_init);
module_exit(pfe_module_exit);
