#define COMCERTO_AXI_APB_CFG_BASE	0x90400000

#define GPIO_BASEADDR		(COMCERTO_AXI_APB_CFG_BASE + 0x070000)

#define GPIO_OUTPUT_REG                 (GPIO_BASEADDR + 0x00)  // GPIO outputs register
#define GPIO_OE_REG                     (GPIO_BASEADDR + 0x04)  // GPIO Output Enable register
#define GPIO_CONFIG_IRQ_REG             (GPIO_BASEADDR + 0x08)
#define GPIO_UP_ALIGN_ACCESS_LOGIC      (GPIO_BASEADDR + 0x0C)
#define GPIO_INPUT_REG                  (GPIO_BASEADDR + 0x10)  // GPIO input register
#define APB_ACCESS_WS_REG               (GPIO_BASEADDR + 0x14)
#define USB_PHY_CONF_REG                (GPIO_BASEADDR + 0x18)
#define SYSCONF_REG                     (GPIO_BASEADDR + 0x1C)
#define GPIO_ARM_ID_REG                 (GPIO_BASEADDR + 0x30)
#define GPIO_GENERIC_PAD_CONTROL        (GPIO_BASEADDR + 0x34)
#define GPIO_BOOTSTRAP_STATUS_REG       (GPIO_BASEADDR + 0x40)
#define GPIO_BOOTSTRAP_OVERRIDE_REG     (GPIO_BASEADDR + 0x44)
#define GPIO_DEVID_REG                  (GPIO_BASEADDR + 0x50)
#define GPIO_ARM_MEMORY_SENSE_REGS      (GPIO_BASEADDR + 0x54)
#define GPIO_PIN_SELECT_REG             (GPIO_BASEADDR + 0x58)
