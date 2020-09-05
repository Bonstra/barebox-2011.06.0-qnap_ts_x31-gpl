#include "../../test_dect.h"

#if 0
#define APB_BASE	0x90000000

#define EXP_BASE	(APB_BASE + 0x5A0000)
#define EXP_SWRST	(EXP_BASE + 0x0)
#define EXP_CSEN	(EXP_BASE + 0x4)
#define EXP_CS0_BASE 	(EXP_BASE + 0x8)
#define EXP_CS0_SEG 	(EXP_BASE + 0x1c)
#define EXP_CSO_CFG	(EXP_BASE + 0x30)

#define GPIO_BASE	(APB_BASE + 0x470000)
#define DECT_SYS_CFG0	(GPIO_BASE + 0xb0)
#define DECT_SYS_CFG1	(GPIO_BASE + 0xb4)
#define DECT_CTRL	(GPIO_BASE + 0xb8)
#endif


static void expansion_config(void) {

  unsigned int timer_count=0;
  int read_data;

  writel(EXP_SWRST, 0x1);    // Soft reset
  writel(EXP_CSEN, 0x3);    // Enable Chip select-0
  writel(EXP_CS0_BASE, 0x001ff);// Defining lower memory segment for CS-0
  writel(EXP_CS0_SEG, 0x01200);// Defining upper memory segment for CS-0

  read_data = readl(EXP_CSO_CFG);
  read_data = read_data | 0x2; // To configure DUT for 16-bit normal mode
  writel(EXP_CSO_CFG, read_data);


  // just to buy some delay
  timer_count = 0;
  while(timer_count != 0x1F) {
    timer_count++;
  };
}


extern void mainc_mcu_scu_arm_dtcm_with_cortex(void);

int host_scu_arm_dtcm_with_cortex(void) {

  // Dect slave interface remapping
  writel(DECT_CTRL,0x0308);
  expansion_config();

  // Bring A926 out of reset

  writel(DECT_SYS_CFG0, 0x7c00); // keep dect in reset
  writel(DECT_SYS_CFG1, 0x0); // writing to upper bits
  dect_delay();
  writel(DECT_SYS_CFG0, 0x7cf3); // drive the clocks
  writel(DECT_SYS_CFG1, 0x0); // writing to upper bits
  dect_delay();
  writel(DECT_SYS_CFG0, 0x8f3);
  writel(DECT_SYS_CFG1, 0x0); // writing to upper bits
  dect_delay();

  mainc_mcu_scu_arm_dtcm_with_cortex(); // here ARM926 is out of reset & both processor executes

  return 0;
};
