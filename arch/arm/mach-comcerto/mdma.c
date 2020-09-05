#include <common.h>
#include <asm/io.h>
#include <mach/comcerto-2000.h>

#define MDMA_MEM_IO_CTRL	0x0
#define MDMA_MEM_IO_HEAD	0x4
#define MDMA_MEM_IO_BURST_LEN	0x8
#define MDMA_MEM_IO_FRAME_LEN	0xC
#define MDMA_IO_MEM_CTRL	0x80
#define MDMA_IO_MEM_HEAD	0x84
#define MDMA_IO_MEM_BURST_LEN	0x88
#define MDMA_IO_MEM_FRAME_LEN	0x8C

#define MDMA_BCTRL_BLAST	( 1 << 16 )
#define MDMA_BCTRL_BLEN_MSK		0xffff

struct comcerto_xor_inbound_desc {
	u32  next_desc;
	u32  fcontrol;
	u32  fstatus0;
	u32  fstatus1;
	u32  buff_info[12]; // 6 Buffer Descriptors
}__attribute__ ((packed)) __attribute__ ((aligned(16)));

struct comcerto_xor_outbound_desc {
	u32  next_desc;
	u32  fcontrol;
	u32  fstatus0;
	u32  fstatus1;
	u32  buff_info[4]; // 2 Buffer Descriptors
}__attribute__ ((packed)) __attribute__ ((aligned(16)));

struct comcerto_xor_inbound_desc mdma_in_desc ;
struct comcerto_xor_outbound_desc mdma_out_desc ;

void set_inbound_mdma_desc(struct comcerto_xor_inbound_desc *hw_desc, u32 addr, int len)
{
	hw_desc->next_desc = 0;
	hw_desc->fcontrol = 0;
	hw_desc->fstatus0 = 0;
	hw_desc->fstatus1 = 0;
	hw_desc->buff_info[0] = addr;
	hw_desc->buff_info[1] = (MDMA_BCTRL_BLAST | ( len & MDMA_BCTRL_BLEN_MSK));
}

void set_outbound_mdma_desc(struct comcerto_xor_outbound_desc *hw_desc, u32 addr, int len)
{
	hw_desc->next_desc = 0;
	hw_desc->fcontrol = 0;
	hw_desc->fstatus0 = 0;
	hw_desc->fstatus1 = 0;
	hw_desc->buff_info[0] = addr;
	hw_desc->buff_info[1] = (MDMA_BCTRL_BLAST | ( len & MDMA_BCTRL_BLEN_MSK));
}

void init_and_start_mdma(u32 src_frame, u32 dst_frame)
{
	writel(0x8, COMCERTO_MDMA_BASE + MDMA_MEM_IO_CTRL); 
	writel(0xf, COMCERTO_MDMA_BASE + MDMA_MEM_IO_BURST_LEN);

	writel(0x8, COMCERTO_MDMA_BASE + MDMA_IO_MEM_CTRL);
	writel(0xf, COMCERTO_MDMA_BASE + MDMA_IO_MEM_BURST_LEN);

	// Initialize the Outbound Head Pointer
	writel(dst_frame, COMCERTO_MDMA_BASE + MDMA_IO_MEM_HEAD);

	// Initialize the Inbound Head Pointer
	writel(src_frame, COMCERTO_MDMA_BASE + MDMA_MEM_IO_HEAD);

}

void wait_for_completion(struct comcerto_xor_outbound_desc *hw_desc)
{
	u32 status = readl((u32)&hw_desc->fstatus1);
	while(!status)
	{
		status = readl((u32)&hw_desc->fstatus1);
	}
}

void mdma_transfer_single(u32 src_data, u32 dst_data, int size)
{
	set_inbound_mdma_desc(&mdma_in_desc, src_data, size);	
	set_outbound_mdma_desc(&mdma_out_desc, dst_data, size);
	init_and_start_mdma((u32)&mdma_in_desc, (u32)&mdma_out_desc);
	wait_for_completion(&mdma_out_desc);
}

