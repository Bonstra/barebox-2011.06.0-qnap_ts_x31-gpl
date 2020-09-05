/*
Copyright (c) 2008 Silicon Laboratories, Inc.
2008-03-18 16:06:28 */
/*ProSLIC API Tool Rev0.31 Beta*/


#include "proslic_api/proslic.h"
#include "proslic_api/si3226.h"
Si3226_General_Cfg Si3226_General_Configuration  = {
0x8720000L, /*vbatr_expect = 132.000*/
0x1300000L, /*vref_min = 17.000*/
0x200000L, /*dcdc_fsw_onh */
0x200000L, /*dcdc_fsw_offh */
0x200000L, /*dcdc_fsw_ring */
0x100000L, /*dcdc_fsw_ring_lo */
0xD980000L,
0xC00000L,
0x500000L, /*uvhyst = 2.000*/
0x600000L, /*uvthresh = 7.200*/
0xD00000L, /*ovthresh = 148.000*/
0x1F00000L, /*oithresh = 4.000*/
0x7FD710CL, /*coeff_p_hvic = 1.000*/
0x48D000L, /*p_th_hvic = 2000.000*/
0x3D70A20L, /*vbath_expect = 60.000*/
0x0, /*cm_clamp = 0.000*/
0x3F, /*auto = 0.000*/
0x640000L, /*cm_dbi = 100.000*/
0xff,
0xff,
0xff,
0xff
};

Si3226_DTMFDec_Cfg Si3226_DTMFDec_Presets[] = {
	{0x2d40000L,0x1a660000L,0x2d40000L,0x6ba0000L,0x1dcc0000L,0x33f0000L,0xbd30000L,0x19d20000L,0x4150000L,0x188F0000L,0x4150000L,0xd970000L,0x18620000L,0xf1c0000L}
};
Si3226_GPIO_Cfg Si3226_GPIO_Configuration = {
	0,0,0,0,0,0,0
};
Si3226_CI_Cfg Si3226_CI_Presets [] = {
	{0}
};
Si3226_audioGain_Cfg Si3226_audioGain_Presets [] = {
	{0x1377080L,0},
	{0x80C3180L,0}
};

Si3226_Ring_Cfg Si3226_Ring_Presets[] ={


    /* inputs:  ringtrip ac thresh = 450.000, dc_thresh = 10.000, rt debounce = 0.075, vov_ring_gnd = 7.000, vov_ring_bat = 10.000*/
    { 0x7EFE000L, 0x1BA000L, 0x1EB3154L, 0x0L, 0x50000L, 0x80, 0x3E, 0x0, 0x7D, 0x58, 0x800000L, 0xC84B5BL, 0xF5E5200EL, 0x0L, 0x0L, 0x51EB8L, 0x6000L, 0x32EDF9EL, 0x6000L, 0x1C71ECL, 0xA3D70AL, 0x72B020L, 0x4CCCCCCL }
};

Si3226_DCfeed_Cfg Si3226_DCfeed_Presets[] = {

    /* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=10.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1DDF41C9L, 0x1EF68D5EL, 0x40A0E0L, 0x18AAD168L, 0x1CA39FFAL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xFDFAB5L, 0x5D0FA6L, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  }
};

Si3226_Impedance_Cfg Si3226_Impedance_Presets[] ={

{ {0x7F95B80L, 0x24CA80L, 0x1FEF3D00L, 0x15A300L, 0x513F900L, 0x98D80L, 0x1FEDEB80L, 0x35F00L}, {0xA0000L, 0x280000L, 0x1310000L, 0x16D0000L, 0xDD0000L, 0x1FF20000L, 0x180000L, 0x1FE90000L, 0xA0000L, 0x1FF00000L, 0x2480000L, 0x5A80000L}, {0x13F4480L, 0x1E062A00L, 0xBB1480L, 0x26D500L, 0x7D89900L, 0x64}, {0x8000000L, 0}, {0x2000000L, 0}, 0x7B43980L, 0x184BC680L, 0x7687280L }
};

Si3226_FSK_Cfg Si3226_FSK_Presets[] ={

    /* inputs: mark freq=1200.000, space freq2200.000, amp=0.220, baud=1200.000, startStopDis=0, interrupt depth = 0 */
    { 0x2232000L, 0x77C2000L, 0x3C0000L, 0x200000L, 0x6B60000L, 0x79C0000L,0, 0 }
};

Si3226_Tone_Cfg Si3226_Tone_Presets[] ={

    /* inputs:  freq1 = 350.000, amp1 = -18.000, freq2 = 440.000, amp2 = -18.000, ta1 = 0.000, ti1 = 0.000, ta2 = 0.000, ti2 = 0.000*/
    { {0x7B30000L, 0x3A000L, 0x0L, 0x0, 0x0, 0x0, 0x0}, {0x7870000L, 0x4A000L, 0x0L, 0x0, 0x0, 0x0, 0x0}, 0x66 },
    /* inputs:  freq1 = 480.000, amp1 = -18.000, freq2 = 620.000, amp2 = -18.000, ta1 = 0.500, ti1 = 0.500, ta2 = 0.500, ti2 = 0.500*/
    { {0x7700000L, 0x52000L, 0x0L, 0xA0, 0xF, 0xA0, 0xF}, {0x7120000L, 0x6A000L, 0x0L, 0xA0, 0xF, 0xA0, 0xF}, 0x66 },
    /* inputs:  freq1 = 480.000, amp1 = -18.000, freq2 = 440.000, amp2 = -18.000, ta1 = 2.000, ti1 = 4.000, ta2 = 2.000, ti2 = 4.000*/
    { {0x7700000L, 0x52000L, 0x0L, 0x80, 0x3E, 0x0, 0x7D}, {0x7870000L, 0x4A000L, 0x0L, 0x80, 0x3E, 0x0, 0x7D}, 0x66 },
    /* inputs:  freq1 = 480.000, amp1 = -18.000, freq2 = 620.000, amp2 = -18.000, ta1 = 0.300, ti1 = 0.200, ta2 = 0.300, ti2 = 0.200*/
    { {0x7700000L, 0x52000L, 0x0L, 0x60, 0x9, 0x40, 0x6}, {0x7120000L, 0x6A000L, 0x0L, 0x60, 0x9, 0x40, 0x6}, 0x66 },
    /* inputs:  freq1 = 480.000, amp1 = -18.000, freq2 = 620.000, amp2 = -18.000, ta1 = 0.200, ti1 = 0.200, ta2 = 0.200, ti2 = 0.200*/
    { {0x7700000L, 0x52000L, 0x0L, 0x40, 0x6, 0x40, 0x6}, {0x7120000L, 0x6A000L, 0x0L, 0x40, 0x6, 0x40, 0x6}, 0x66 }
};

Si3226_PCM_Cfg Si3226_PCM_Presets[] ={

    /* inputs:  u-law narrowband positive edge, dtx positive edge, both disabled, tx hwy = A, rx hwy = A */
    { 0x1, 0x0, 0x0, 0x0 }
};

