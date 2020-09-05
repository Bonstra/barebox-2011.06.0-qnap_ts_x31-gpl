#define A9_TIMER_LOAD		0x0
#define A9_TIMER_COUNTER	0x4
#define A9_TIMER_CNTRL		0x8
#define A9_TIMER_INT_STAT	0xC

#define	A9_WD_LOAD		0x20
#define	A9_WD_COUNTER		0x24
#define	A9_WD_CNTRL		0x28
#define	A9_WD_INT_STAT		0x2C
#define	A9_WD_RST_STAT		0x30
#define	A9_WD_DISABLE		0x34

#define A9_TIMER_ENABLE		(1<<0)
#define A9_TIMER_RELOAD		(1<<1)
#define A9_TIMER_IRQ_EN		(1<<2)

#define A9_WD_ENABLE		(1<<0)
#define A9_WD_RELOAD		(1<<1)
#define A9_WD_MODE_WD		(1<<3)

