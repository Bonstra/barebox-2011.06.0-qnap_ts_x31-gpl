#ifndef _COMCERTO_TESTS_H_
#define _COMCERTO_TESTS_H_

#define CMDID_DDR	0x1
#define CMDID_PFE	0x2
#define CMDID_TZ	0x3
#define CMDID_L2CC      0x4
#define CMDID_DECT	0x5
#define CMDID_DUALCORE  0x6
#define CMDID_PCIE_RC   0x7
#define CMDID_PCIE_EP   0x8
#define CMDID_PCIE_PERF	0x9
#define CMDID_FUART     0xA
#define CMDID_NOR       0xB
#define CMDID_UTILPE_IPSEC  0xC
#define CMDID_ZDSTDM    0xD
#define CMDID_ZDS_SLIC  0xE
#define CMDID_SPI	0xF
#define CMDID_MSIF_TDM	0x10
#define CMDID_MSIF_SLIC	0x11
#define CMDID_I2C	0x12
#define CMDID_OTP	0x13
#define CMDID_USB3	0x14
#define CMDID_USB3_PHY_BER      0x15
#define CMDID_NAND_ECC	0x16
#define	CMDID_SATA	0x17

#define CMD_DDR		"ddr"
#define CMD_PFE		"pfe"
#define CMD_TZ		"tz"
#define CMD_L2CC        "l2cc"
#define CMD_DECT	"dect"
#define CMD_DUALCORE    "dualcore"
#define CMD_PCIE_RC     "pcierc"
#define CMD_PCIE_EP     "pcieep"
#define CMD_PCIE_PERF     "pcieperf"
#define CMD_FUART       "fastuart"
#define CMD_NOR         "nor"
#define CMD_UTILPE_IPSEC        "ipsec"
#define CMD_ZDSTDM      "zdstdm"
#define CMD_ZDS_SLIC   "zdsslic"
#define CMD_SPI		"spi"
#define CMD_MSIF_TDM	"msiftdm"
#define CMD_MSIF_SLIC        "msifslic"
#define CMD_I2C		"i2c"
#define CMD_OTP		"otp"
#define CMD_USB3	"usb3"
#define CMD_USB3_PHY_BER	"usb3phyber"
#define CMD_NAND_ECC   "nandecc"
#define CMD_SATA	"sata"

#define DESCR_CM_DDR	"DDR test"
#define DESCR_CM_PFE	"PFE Test"
#define DESCR_CM_TZ	"TZ Test"
#define DESCR_CM_L2CC   "L2CC Test"
#define DESCR_CM_DECT	"DECT Test"
#define DESCR_CM_DUALCORE   "DualCore Test"
#define DESCR_CM_PCIE_RC   "PCIe RootComplex Test"
#define DESCR_CM_PCIE_EP   "PCIe Enpoint Test"
#define	DESCR_CM_PCIE_PERF   "PCIe Performance Test"
#define DESCR_CM_FUART  "Fast UART Test"
#define DESCR_CM_NOR    "NOR Test"
#define DESCR_CM_UTILPE_IPSEC   "IPSEC Test"
#define DESCR_CM_ZDSTDM "ZDS/TDM Test"
#define DESCR_CM_ZDS_SLIC      "ZDS/SLIC Test"
#define DESCR_CM_SPI	"SPI/Fast SPI Test"
#define DESCR_CM_MSIF_TDM	"MSIF/TDM Test"
#define DESCR_CM_MSIF_SLIC   "MSIF/SLIC Test"
#define DESCR_CM_I2C		"I2C Test"
#define DESCR_CM_OTP		"OTP Test"
#define DESCR_CM_USB3		"USB3.0 Test"
#define DESCR_CM_USB3_PHY_BER	"USB3 PHY BER Test"
#define DESCR_CM_NAND_ECC      "NAND ECC Test"
#define	DESCR_CM_SATA		"SATA Test"

#define USAGE_CM_DDR	"Usage: "CMD_DDR" :<BaseAddress> <Size> <TestFlags>"
#define USAGE_CM_PFE	"Usage: "CMD_PFE" :<>"
#define USAGE_CM_TZ	"Usage: "CMD_TZ" :<>"
#define USAGE_CM_L2CC   "Usage: "CMD_L2CC" :<>"
#define USAGE_CM_DECT	"Usage: "CMD_DECT" :<>"
#define USAGE_CM_DUALCORE   "Usage: "CMD_DUALCORE" :<>"
#define USAGE_CM_PCIE_RC   "Usage: "CMD_PCIE_RC" :<>"
#define USAGE_CM_PCIE_EP   "Usage: "CMD_PCIE_EP" :<>"
#define USAGE_CM_PCIE_PERF   "Usage: "CMD_PCIE_PERF" :<>"
#define USAGE_CM_FUART   "Usage: "CMD_FUART" :<>"
#define USAGE_CM_NOR    "Usage: "CMD_NOR" :<>"
#define USAGE_CM_UTILPE_IPSEC   "Usage: "CMD_UTILPE_IPSEC" :<>"
#define USAGE_CM_ZDSTDM "Usage: "CMD_ZDSTDM" :<>"
#define USAGE_CM_ZDS_SLIC "Usage: "CMD_ZDS_SLIC" :<>"
#define USAGE_CM_SPI    "Usage: "CMD_SPI" :<>"
#define USAGE_CM_MSIF_TDM	"Usage: "CMD_MSIF_TDM" :<>"
#define USAGE_CM_MSIF_SLIC   "Usage: "CMD_MSIF_SLIC" :<>"
#define USAGE_CM_I2C	"Usage: "CMD_I2C" :<>"
#define USAGE_CM_OTP	"Usage: "CMD_OTP" :<>"
#define USAGE_CM_USB3	"Usage: "CMD_USB3" :<>"
#define USAGE_CM_USB3_PHY_BER	"Usage: "CMD_USB3_PHY_BER" :<>"
#define USAGE_CM_NAND_ECC      "Usage: "CMD_NAND_ECC" :<>"
#define USAGE_CM_SATA		"Usage: "CMD_SATA" :<>"

#define MEMTEST_ADDRBUS             0x1
#define MEMTEST_DATABUS		    0x2
#define MEMTEST_PATTERN_AA         0x4
#define MEMTEST_PATTERN_55         0x8

#define L2CCTEST_INVALIDATE_BY_WAY      0x1
#define L2CCTEST_INVALIDATE_BY_LINE     0x2
#define L2CCTEST_CLEAN_BY_WAY           0x3
#define L2CCTEST_CLEAN_BY_LINE          0x4
#define L2CCTEST_LOCK_BY_WAY            0x5
#define L2CCTEST_LOCK_BY_LINE           0x6

#define UTIL_IPSEC_OUT_POLL     0x1
#define UTIL_IPSEC_IN_POLL      0x2
#define UTIL_IPSEC_OUT_INTR     0x4
#define UTIL_IPSEC_IN_INTR      0x8

#define SPI_WRITE_TEST  1
#define SPI_WRITE_READ_TEST   3
#define SPI_READ_TEST  2
#define SPI_EEPROM_READ_TEST    4

#define SPI_DEVICE_RW_ADDR      0x0
#define SPI_DEVICE_RW_LEN       0x8 /* byte(s) */

#endif
