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


#include <linux/delay.h>
#include "pfe_driver.h"


void dump_phy_reg();
#if 0
#define DMEM_TEST_BASE_ADDR	0x0	/* !!! For class overlaps with packets in dmem, for tmu overwrites exception vectors !!! */

#define BUF_SIZE 6

static u32 w[BUF_SIZE] = {0x01234567, 0x89abcdef, 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff};
static u32 zero[BUF_SIZE] = {0, };
static u32 r[BUF_SIZE];

static void dmem_read4_write4(unsigned int id)
{
	pe_dmem_write(id, w[0], DMEM_TEST_BASE_ADDR, 4);

	r[0] = pe_dmem_read(id, DMEM_TEST_BASE_ADDR, 4);

	if (r[0] != w[0])
		printk(KERN_ERR "PE %d: %s failed: %#x %#x\n", id, __func__, w[0], r[0]);
	else
		printk(KERN_INFO "PE %d: %s wr-pass: %#x %#x\n", id, __func__, w[0], r[0]);
}
#endif

#if 0
void pfe_unit_test(void)
{
	u32 i,val,j;

#if 0
	dmem_read4_write4(0);
	dmem_read4_write4(1);
	dmem_read4_write4(2);
	dmem_read4_write4(3);

	//dmem_read4_write4(4);
	for(i=0; i < 4; i++) {
		val = pe_dmem_read(0, 0x830 + (i * 4), 4);
		printk(KERN_ERR "PE %d: loc: %d, regMask: %08x\n", 0, i, val);
	}

	val = pe_dmem_read(0, 0x840, 4);
	printk(KERN_ERR "PE %d: %08x\n", 0, val);

	val = pe_dmem_read(0, 0x844, 4);
	printk(KERN_ERR "PE %d: %08x\n", 0, val);

	val = pe_dmem_read(0, 0x840, 4);
	printk(KERN_ERR "PE %d: %08x\n", 0, val);

	val = pe_dmem_read(0, 0x844, 4);
	printk(KERN_ERR "PE %d: %08x\n", 0, val);


	val = pe_dmem_read(0, 0x844, 4);
	printk(KERN_ERR "PE %d: %08x\n", 0, val);

#endif

	for(i=0; i < 4; i++)  {
		//printk(KERN_ERR "%s: %#lx %#lx\n", __func__, readl(CLASS_STATE), readl(CLASS_PE0_DEBUG));	
		
		for(j=0; j < 10; j++)  {
			printk(KERN_ERR "%s: %#lx\n", __func__, readl(CLASS_PE0_DEBUG));	
		}
		val = pe_dmem_read(i, 0x840, 4);
		printk(KERN_ERR "PE %d: %08x\n", i, val);
		val = pe_dmem_read(i, 0x844, 4);
		printk(KERN_ERR "PE %d: %08x\n", i, val);
	}

}
#endif
static char stat_strings[][50] = {
	"tx- octets  (Lower 32-bits)",
	"tx- octets  (Upper 16-bits)",
	"tx- packets",
	"tx- broadcast",
	"tx- multicast",
	"tx- pause",
	"tx- 64 bytes packets",
	"tx- 64 - 127 bytes packets",
	"tx- 128 - 255 bytes packets",
	"tx- 256 - 511 bytes packets",
	"tx- 512 - 1023 bytes packets",
	"tx- 1024 - 1518 bytes packets",
	"tx- > 1518 bytes packets",
	"tx- underruns  - errors",
	"tx- single collision",
	"tx- multi collision",
	"tx- exces. collision  - errors",
	"tx- late collision  - errors",
	"tx- deferred",
	"tx- carrier sense - errors",
	"rx- octets (Lower 32-bits)",
	"rx- octets (Upper 16-bits)",
	"rx- packets",
	"rx- broadcast",
	"rx- multicast",
	"rx- pause",
	"rx- 64 bytes packets",
	"rx- 64 - 127 bytes packets",
	"rx- 128 - 255 bytes packets",
	"rx- 256 - 511 bytes packets",
	"rx- 512 - 1023 bytes packets",
	"rx- 1024 - 1518 bytes packets",
	"rx- > 1518 bytes packets",
	"rx- undersize -errors",
	"rx- oversize  - errors ",
	"rx- jabbers - errors",
	"rx- fcs - errors",
	"rx- length - errors",
	"rx- symbol - errors",
	"rx- align - errors",
	"rx- ressource - errors",
	"rx- overrun - errors",
	"rx- IP cksum - errors",
	"rx- TCP cksum - errors",
	"rx- UDP cksum - errors" 
};
static void gem_print_stats(void *base)
{
	u32 *pstat = (u32*)gemac_get_stats(base);
	int i;

	for (i=0; i<(sizeof(GEM_STATS)/4); i++)
		printk("%s : %d\n", stat_strings[i], *pstat++);
}


void pfe_class_read (int id)
{
	u32 val,j;

	for(j=0; j < 20; j++)  {
		printk(KERN_ERR "%s: %#lx\n", __func__, readl(CLASS_PE0_DEBUG));	
	}

	val = pe_dmem_read(id, 0x840, 4);
	printk(KERN_ERR "PE %d: %08x\n", id, val);
	val = pe_dmem_read(id, 0x844, 4);
	printk(KERN_ERR "PE %d: %08x\n", id, val);

	return;

}

void pfe_send_pkt(int len)
{
	unsigned char *pbuf;
	int i;
	int rcount = 0;
	int rc,phyport=0;

	printk(KERN_INFO "%s:\n", __func__);

	pbuf = (unsigned char *)kmalloc(1024*2, GFP_ATOMIC | GFP_DMA);

	if ((len >= 0) || (len < 1000))  {

		for(i=0; i < len; i++)
			pbuf[i] = i;
#if 0
		if(len == 100) phyport=0;
		if(len == 200) phyport=1;
		if(len == 300) phyport=2;
		if(len == 400) phyport=3;
#endif

		rc = pfe_send(phyport, (void *)(pbuf), len);

		if (rc < 0)
			goto done;
			
		/* wait for tx done/completion indication */
		rcount = 0;
		while (1) {
			rc = pfe_tx_done(); 
			if (rc == 0)
				break;
			mdelay(100);
			if (rcount == 20) {
				printk(KERN_ERR "Tx send fail\n");
				break;
			}
			rcount++;
		}
	}

done:
	kfree(pbuf);
}

void pfe_print_tx_status(void)
{
	u32 val;

	gem_print_stats(EMAC1_BASE_ADDR);
	//gem_print_stats(EMAC2_BASE_ADDR);

	val = readl(HIF_TX_CTRL);
	printk(KERN_INFO "TX_CTRL: %08x\n", val);

	val = readl(HIF_TX_CURR_BD_ADDR);
	printk(KERN_INFO "TX_CURR_BD_ADDR: %08x\n", val);

	val = readl(HIF_TX_BDP_ADDR);
	printk(KERN_INFO "TX_BDP_ADDR: %08x\n", val);

	val = readl(HIF_TX_STATUS);
	printk(KERN_INFO "TX_STATUS: %08x\n", val);

	val = readl(HIF_TX_DMA_STATUS);
	printk(KERN_INFO "TX_DMA_STATUS: %08x\n", val);

	val = readl(BMU2_BASE_ADDR+BMU_REM_BUF_CNT);
	printk(KERN_INFO "BMU2_REM_BUF_CNT: %08x\n", val);

	val = readl(BMU1_BASE_ADDR+BMU_REM_BUF_CNT);
	printk(KERN_INFO "BMU1_REM_BUF_CNT: %08x\n", val);

}

void class_dmem_readn(int id, u32 addr, u32 len)
{
	u32 val, i;


	for(i=0; i < len; i += 4)  {
		val = pe_dmem_read(id, addr+i, 4);
		printk(KERN_INFO "%08x: %08x\n",addr+i, val);
	}
}

void class_pmem_readn(int id, u32 addr, u32 len)
{
	u32 val, i;

	printk(KERN_INFO "Class pe: %d PMEM\n", id);
	for(i=0; i < len; i += 4)  {
		val = pe_pmem_read(id, addr+i, 4);
		printk(KERN_INFO "%08x: %08x\n",addr+i, val);
		mdelay(40);
	}
}

void class_pe_dump(void)
{
	u32 val,j,id;

#if 0
	printk(KERN_INFO "Class-PE 0: ===========>\n");
	for(j=0; j < 20; j++)  {
		printk(KERN_ERR "%s: PC: %#lx\n", __func__, readl(CLASS_PE0_DEBUG)&0xffff);	
		mdelay(40);
	}
	printk(KERN_INFO "Class-PE 0: DMEM /n");
	class_dmem_readn(0,0x814,20);
	//class_pmem_readn(0,0x10000, 256);
#endif
	printk(KERN_INFO "Class-PE 1: ===========>\n");
	for(j=0; j < 10; j++)  {
		printk(KERN_ERR "%s: PC: %#lx\n", __func__, readl(CLASS_PE1_DEBUG)&0xffff);	
		mdelay(40);
	}
	class_dmem_readn(1,0x858,4);
	//printk(KERN_INFO "Class-PE 1: DMEM /n");
	//class_dmem_readn(1,0x814,20);
	//class_pmem_readn(1,0x10000, 256);

#if 0
	printk(KERN_INFO "Class-PE 2: ===========>\n");
	for(j=0; j < 20; j++)  {
		printk(KERN_ERR "%s: PC: %#lx\n", __func__, readl(CLASS_PE2_DEBUG)&0xffff);	
		mdelay(40);
	}
	printk(KERN_INFO "Class-PE 2: DMEM /n");
	class_dmem_readn(2,0x814,20);
	//class_pmem_readn(2,0x10000, 256);


	printk(KERN_INFO "Class-PE 3: ===========>\n");
	for(j=0; j < 20; j++)  {
		printk(KERN_ERR "%s: PC: %#lx\n", __func__, readl(CLASS_PE3_DEBUG)&0xffff);	
		mdelay(40);
	}
	printk(KERN_INFO "Class-PE 3: DMEM /n");
	class_dmem_readn(3,0x814,20);
	//class_pmem_readn(3,0x10000, 256);
#endif
	
}

void class_dmem_dump(int id)
{
	u32 val, i;

	if (id > 3)
		id = 0;

	printk(KERN_ERR "PE %d:\n", id);

	for(i=0; i < 256; i += 4)  {
		val = pe_dmem_read(id, i, 4);
		printk("%08x: %08x\n",i, val);
	}
}
