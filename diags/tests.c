#include <diags.h>
#include "tests.h"

extern int Comcerto_ddr_test(struct diags_test_param *p);
extern int Comcerto_pfe_test(struct diags_test_param *p);
extern int Comcerto_tz_test(struct diags_test_param *p);
extern int Comcerto_dect_test(struct diags_test_param *p);
extern int comcerto_pcie_ep_test(struct diags_test_param *p);
extern int comcerto_pcie_rc_test(struct diags_test_param *p);
extern int comcerto_pcie_perf_test(struct diags_test_param *p);
extern int Comcerto_fast_uart_test(struct diags_test_param *p);
extern int Comcerto_nor_test(struct diags_test_param *p);
extern int Comcerto_zds_tdm_test(struct diags_test_param *p);
extern int Comcerto_zds_slic_test(struct diags_test_param *p);
extern int zds_init(void);
extern int c2k_zds_init(void);
extern int Comcerto_spi_test(struct diags_test_param *p);
extern int Comcerto_msif_tdm_test(struct diags_test_param *p);
extern int Comcerto_msif_slic_test(struct diags_test_param *p);
extern int Comcerto_i2c_test(struct diags_test_param *p);
extern int Comcerto_otp_test(struct diags_test_param *p);
extern int Comcerto_usb3_test(struct diags_test_param *p);
extern int Comcerto_usb3_phy_ber_test(struct diags_test_param *p);
extern int Comcerto_util_ipsec_test(struct diags_test_param *p);
extern int Comcerto_nand_ecc_test(struct diags_test_param *p);
extern int Comcerto_sata_test(struct diags_test_param *p);

/* Diagnostic commands */
struct diags_tests test_list[] = {
#ifdef CONFIG_COMCERTO_TEST_UTILPE_IPSEC
{ CMDID_UTILPE_IPSEC,   CMD_UTILPE_IPSEC,       &Comcerto_util_ipsec_test,    NULL,    DESCR_CM_UTILPE_IPSEC,  USAGE_CM_UTILPE_IPSEC, 0},
#endif
#ifdef CONFIG_COMCERTO_TEST_DDR
{ CMDID_DDR,	CMD_DDR,	&Comcerto_ddr_test,	NULL,	DESCR_CM_DDR,	USAGE_CM_DDR},
#endif
#ifdef CONFIG_COMCERTO_TEST_PFE
{ CMDID_PFE,	CMD_PFE,	&Comcerto_pfe_test,	NULL,	DESCR_CM_PFE,	USAGE_CM_PFE},
#endif
#ifdef CONFIG_COMCERTO_TEST_TZ
{ CMDID_TZ,	CMD_TZ,		&Comcerto_tz_test,	NULL,	DESCR_CM_TZ,	USAGE_CM_TZ},
#endif
#ifdef CONFIG_COMCERTO_TEST_L2CC
{ CMDID_L2CC,   CMD_L2CC,       &Comcerto_l2cc_test,    &Comcerto_l2cc_init,    DESCR_CM_L2CC,  USAGE_CM_L2CC, 0},
#endif
#ifdef CONFIG_COMCERTO_TEST_PFE
{ CMDID_DECT,	CMD_DECT,	&Comcerto_dect_test,	NULL,	DESCR_CM_DECT,	USAGE_CM_DECT},
#endif
#ifdef CONFIG_COMCERTO_TEST_DUALCORE
{ CMDID_DUALCORE,   CMD_DUALCORE,       &Comcerto_dualcore_test,    NULL,    DESCR_CM_DUALCORE,  USAGE_CM_DUALCORE, 0},
#endif
#ifdef CONFIG_COMCERTO_TEST_PCIE
{ CMDID_PCIE_RC,   CMD_PCIE_RC,       &comcerto_pcie_rc_test,    NULL, DESCR_CM_PCIE_RC,  USAGE_CM_PCIE_RC, 0},
{ CMDID_PCIE_EP,   CMD_PCIE_EP,       &comcerto_pcie_ep_test,    NULL, DESCR_CM_PCIE_EP,  USAGE_CM_PCIE_EP, 0},
{ CMDID_PCIE_PERF,   CMD_PCIE_PERF,       &comcerto_pcie_perf_test,    NULL, DESCR_CM_PCIE_PERF,  USAGE_CM_PCIE_PERF, 0},
#endif
#ifdef CONFIG_COMCERTO_TEST_FUART
{ CMDID_FUART,   CMD_FUART,       &Comcerto_fast_uart_test,    NULL,   DESCR_CM_FUART,  USAGE_CM_FUART},
#endif
#ifdef CONFIG_COMCERTO_TEST_NOR
{ CMDID_NOR,    CMD_NOR,        &Comcerto_nor_test,     NULL,   DESCR_CM_NOR, USAGE_CM_NOR},
#endif
#ifdef CONFIG_COMCERTO_TEST_ZDSTDM
{ CMDID_ZDSTDM,    CMD_ZDSTDM,        &Comcerto_zds_tdm_test,     &zds_init, DESCR_CM_ZDSTDM,   USAGE_CM_ZDSTDM,   0},
#endif
#ifdef CONFIG_COMCERTO_TEST_ZDS_SLIC
{ CMDID_ZDS_SLIC,    CMD_ZDS_SLIC,        &Comcerto_zds_slic_test,	&c2k_zds_init,	DESCR_CM_ZDS_SLIC, USAGE_CM_ZDS_SLIC},
#endif
#ifdef CONFIG_COMCERTO_TEST_SPI
{ CMDID_SPI,    CMD_SPI,        &Comcerto_spi_test,     NULL,   DESCR_CM_SPI,   USAGE_CM_SPI},
#endif
#ifdef CONFIG_COMCERTO_TEST_MSIF_TDM
{ CMDID_MSIF_TDM,   CMD_MSIF_TDM,       &Comcerto_msif_tdm_test,    NULL, DESCR_CM_MSIF_TDM, USAGE_CM_MSIF_TDM, 0},
#endif
#ifdef CONFIG_COMCERTO_TEST_MSIF_SLIC
{ CMDID_MSIF_SLIC,    CMD_MSIF_SLIC,        &Comcerto_msif_slic_test,     NULL, DESCR_CM_MSIF_SLIC, USAGE_CM_MSIF_SLIC},
#endif
#ifdef CONFIG_COMCERTO_TEST_I2C
{ CMDID_I2C, CMD_I2C, &Comcerto_i2c_test, NULL, DESCR_CM_I2C, USAGE_CM_I2C, 0},
#endif
#ifdef CONFIG_COMCERTO_TEST_OTP
{ CMDID_OTP, CMD_OTP, &Comcerto_otp_test, NULL, DESCR_CM_OTP, USAGE_CM_OTP, 0},
#endif
#ifdef CONFIG_COMCERTO_TEST_USB3
{ CMDID_USB3, CMD_USB3, &Comcerto_usb3_test, NULL, DESCR_CM_USB3, USAGE_CM_USB3, 0},
{ CMDID_USB3_PHY_BER, CMD_USB3_PHY_BER, &Comcerto_usb3_phy_ber_test, NULL, DESCR_CM_USB3_PHY_BER, USAGE_CM_USB3_PHY_BER, 0},
#endif
#ifdef CONFIG_COMCERTO_TEST_NAND_ECC
{ CMDID_NAND_ECC, CMD_NAND_ECC, &Comcerto_nand_ecc_test, NULL, DESCR_CM_NAND_ECC, USAGE_CM_NAND_ECC, 0},
#endif
#ifdef CONFIG_COMCERTO_TEST_SATA
{ CMDID_SATA, CMD_SATA, &Comcerto_sata_test, NULL, DESCR_CM_SATA, USAGE_CM_SATA, 0},
#endif
};

struct diags_test_param	test_param_list[] = {
#ifdef CONFIG_COMCERTO_TEST_UTILPE_IPSEC
{ CMDID_UTILPE_IPSEC, 1, {UTIL_IPSEC_OUT_POLL |UTIL_IPSEC_IN_POLL | UTIL_IPSEC_OUT_INTR|UTIL_IPSEC_IN_INTR }},
#endif
#ifdef CONFIG_COMCERTO_TEST_DDR
{ CMDID_DDR,  3,  { 0, 128*1024*1024, MEMTEST_ADDRBUS|MEMTEST_DATABUS|MEMTEST_PATTERN_AA|MEMTEST_PATTERN_55}} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_PFE
{ CMDID_PFE,   0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_TZ
{ CMDID_TZ,   0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_L2CC
{ CMDID_L2CC,   1,  { L2CCTEST_INVALIDATE_BY_WAY|L2CCTEST_INVALIDATE_BY_LINE|L2CCTEST_CLEAN_BY_WAY| L2CCTEST_CLEAN_BY_LINE|L2CCTEST_LOCK_BY_LINE }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_DECT
{ CMDID_DECT,  0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_DUALCORE
{ CMDID_DUALCORE,   0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_FUART
{ CMDID_FUART,  0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_NOR
{ CMDID_NOR,   0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_ZDSTDM
{ CMDID_ZDSTDM,   0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_ZDS_SLIC
{ CMDID_ZDS_SLIC,   0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_SPI
{ CMDID_SPI,  0,  { SPI_WRITE_TEST | SPI_WRITE_READ_TEST | SPI_READ_TEST | SPI_EEPROM_READ_TEST, SPI_DEVICE_RW_ADDR, SPI_DEVICE_RW_LEN}} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_MSIF_TDM
{ CMDID_MSIF_TDM,  0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_MSIF_SLIC
{ CMDID_MSIF_SLIC,   0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_I2C
{ CMDID_I2C, 0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_OTP
{ CMDID_OTP, 0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_USB3
{ CMDID_USB3, 0,  { 0 }} ,
{ CMDID_USB3_PHY_BER, 0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_NAND_ECC
{ CMDID_NAND_ECC, 0,  { 0 }} ,
#endif
#ifdef CONFIG_COMCERTO_TEST_SATA
{ CMDID_SATA, 0,  { 0 }} ,
#endif
};

unsigned int test_list_size = sizeof (test_list) / sizeof (struct diags_tests);
