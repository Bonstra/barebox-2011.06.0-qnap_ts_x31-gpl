

/* =--------------------------------------------------------------= */
/*                       M E M O R Y   M A P                        */
/* =--------------------------------------------------------------= */
// PCIE Controller's Configuration Space
#define PCIE0_DBI_AXI_BASE 0x98000000
#define PCIE1_DBI_AXI_BASE 0x99000000

//Base Address of PCIE Controller's AXI Slave Space
#define PCIE0_SLV_AXI_BASE 0xA0000000
#define PCIE1_SLV_AXI_BASE 0xB0000000

//Base Address of DWC Registers
#define DWC_CFG_BASE 0x90460000


/* =--------------------------------------------------------------= */
/*                    PCIE0 Register Addresses                      */
/* =--------------------------------------------------------------= */
/*                     B A S E   A D D R E S S                      */
/* =--------------------------------------------------------------= */

/***********************************************************/
/*   PF PCI Configuration Space Header - Type 0            */
/***********************************************************/
#define PCIE0_CFG_BASE                   (PCIE0_DBI_AXI_BASE + 0x0 )
#define PCIE0_PM_CAP_BASE                (PCIE0_DBI_AXI_BASE + 0x40)
#define PCIE0_MSI_CAP_BASE               (PCIE0_DBI_AXI_BASE + 0x50)
#define PCIE0_PCIE_CAP_BASE              (PCIE0_DBI_AXI_BASE + 0x70)
#define PCIE0_MSIX_CAP_BASE              (PCIE0_DBI_AXI_BASE + 0xB0)
#define PCIE0_VPD_CAP_BASE               (PCIE0_DBI_AXI_BASE + 0xD0) 

/***********************************************************/
/*   PF PCI0 Configuration Space Header - Type 0            */
/***********************************************************/

#define PCIE0_ID_REG                (PCIE0_CFG_BASE + 0x0 ) 
#define PCIE0_CMD_REG               (PCIE0_CFG_BASE + 0x4 )  //9800_0060
#define PCIE0_REV_REG               (PCIE0_CFG_BASE + 0x8 )
#define PCIE0_CACHE_REG             (PCIE0_CFG_BASE + 0xC )
#define PCIE0_BAR0_REG              (PCIE0_CFG_BASE + 0x10)
#define PCIE0_BAR1_REG              (PCIE0_CFG_BASE + 0x14) 
#define PCIE0_BAR2_REG              (PCIE0_CFG_BASE + 0x18)
#define PCIE0_BAR3_REG              (PCIE0_CFG_BASE + 0x1C)
#define PCIE0_BAR4_REG              (PCIE0_CFG_BASE + 0x20)
#define PCIE0_BAR5_REG              (PCIE0_CFG_BASE + 0x24)
#define PCIE0_CARDBUS_REG           (PCIE0_CFG_BASE + 0x28) 
#define PCIE0_SID_REG               (PCIE0_CFG_BASE + 0x2C)
#define PCIE0_ROM_BASE_REG          (PCIE0_CFG_BASE + 0x30)
#define PCIE0_ROM_MASK_REG          (PCIE0_CFG_BASE + 0x30)
#define PCIE0_CAP_REG               (PCIE0_CFG_BASE + 0x34)
#define PCIE0_INT_REG               (PCIE0_CFG_BASE + 0x3C)
			    
/* Port Logic Register Base */
#define PCIE0_PL_REG                (PCIE0_CFG_BASE + 0x700)

/***********************************************************/
/* PF Power Management Capability Structure(CFG_PM_CAP)    */
/***********************************************************/
#define PCIE0_PM_CAP_REG                 (PCIE0_PM_CAP_BASE + 0x0)
#define PCIE0_PMCSR_REG                  (PCIE0_PM_CAP_BASE + 0x4)

/***********************************************************/
/* PF MSI Capability Structure                             */
/***********************************************************/
#define PCIE0_MSI_CAP_REG                (PCIE0_MSI_CAP_BASE + 0x0 ) 
#define PCIE0_MSI_LADDR_REG              (PCIE0_MSI_CAP_BASE + 0x4 )
#define PCIE0_MSI_UADDR_REG              (PCIE0_MSI_CAP_BASE + 0x8 )
#define PCIE0_MSI_DATA_REG               (PCIE0_MSI_CAP_BASE + 0xC )
#define PCIE0_MSI_MASK_REG               (PCIE0_MSI_CAP_BASE + 0x10)
#define PCIE0_MSI_PEND_REG               (PCIE0_MSI_CAP_BASE + 0x14)

/***********************************************************/
/* PF PCI Express Capability Structure                     */
/***********************************************************/
#define PCIE0_CID_REG               (PCIE0_PCIE_CAP_BASE + 0x0 )          
#define PCIE0_DCAP_REG              (PCIE0_PCIE_CAP_BASE + 0x04)         
#define PCIE0_DCNT_REG              (PCIE0_PCIE_CAP_BASE + 0x08)         
#define PCIE0_LCAP_REG              (PCIE0_PCIE_CAP_BASE + 0x0C)         
#define PCIE0_LCNT_REG              (PCIE0_PCIE_CAP_BASE + 0x10)  //9800_0080       
#define PCIE0_SCAP_REG              (PCIE0_PCIE_CAP_BASE + 0x14)         
#define PCIE0_SCNT_REG              (PCIE0_PCIE_CAP_BASE + 0x18)         
#define PCIE0_DCAP2_REG             (PCIE0_PCIE_CAP_BASE + 0x24)         
#define PCIE0_DCNT2_REG             (PCIE0_PCIE_CAP_BASE + 0x28)         
#define PCIE0_LCNT2_REG             (PCIE0_PCIE_CAP_BASE + 0x30)         

/***********************************************************/
/* Port Logic Registers                                    */
/***********************************************************/

#define PCIE0_ALRT_REG              (PCIE0_PL_REG)
#define PCIE0_AFL0L1_REG            (PCIE0_PL_REG + 0xC)
#define PCIE0_SYMNUM_REG            (PCIE0_PL_REG + 0x18)
#define PCIE0_G2CTRL_REG            (PCIE0_PL_REG + 0x10C)

/***********************************************************/
/* MSI Interrupt Controller Registers                      */
/***********************************************************/
#define PCIE0_MSI_CTRL_ADDR_REG         (PCIE0_PL_REG + 0x120)
#define PCIE0_MSI_CTRL_INTR0_EN_REG     (PCIE0_PL_REG + 0x128)
#define PCIE0_MSI_CTRL_INTR0_STATUS_REG     (PCIE0_PL_REG + 0x130)

/***********************************************************/
/* iATU Registers                                          */
/***********************************************************/
#define PCIE0_iATU_VIEW_REG              (PCIE0_PL_REG + 0x200)
#define PCIE0_iATU_CTL1_REG              (PCIE0_PL_REG + 0x204)
#define PCIE0_iATU_CTL2_REG              (PCIE0_PL_REG + 0x208)
#define PCIE0_iATU_RLBA_REG              (PCIE0_PL_REG + 0x20C)
#define PCIE0_iATU_RUBA_REG              (PCIE0_PL_REG + 0x210)
#define PCIE0_iATU_RLIA_REG              (PCIE0_PL_REG + 0x214)
#define PCIE0_iATU_RLTA_REG              (PCIE0_PL_REG + 0x218)
#define PCIE0_iATU_RUTA_REG              (PCIE0_PL_REG + 0x21C)


/*******************************************************/
/* BAR MASK Registers (uses dbi_cs2)                   */
/*******************************************************/
#define PCIE0_BAR0_MASK_REG              (PCIE0_CFG_BASE + 0x100010)  //0x98100010


/* Transmit and receive start address */
#define  TRANS_START_ADDR          PCIE0_SLV_AXI_BASE



/* PCIE1 Register Addresses*/
/* =--------------------------------------------------------------= */
/*                     B A S E   A D D R E S S                      */
/* =--------------------------------------------------------------= */
#define PCIE1_CFG_BASE                   (PCIE1_DBI_AXI_BASE + 0x0 )
#define PCIE1_PM_CAP_BASE                (PCIE1_DBI_AXI_BASE + 0x40)
#define PCIE1_MSI_CAP_BASE               (PCIE1_DBI_AXI_BASE + 0x50)
#define PCIE1_PCIE_CAP_BASE              (PCIE1_DBI_AXI_BASE + 0x70)
#define PCIE1_MSIX_CAP_BASE              (PCIE1_DBI_AXI_BASE + 0xB0)
#define PCIE1_VPD_CAP_BASE               (PCIE1_DBI_AXI_BASE + 0xD0) 

/***********************************************************/
/*   PF PCI0 Configuration Space Header - Type 0            */
/***********************************************************/

#define PCIE1_ID_REG                (PCIE1_CFG_BASE + 0x0 ) 
#define PCIE1_CMD_REG               (PCIE1_CFG_BASE + 0x4 )  //9900_0060
#define PCIE1_REV_REG               (PCIE1_CFG_BASE + 0x8 )
#define PCIE1_CACHE_REG             (PCIE1_CFG_BASE + 0xC )
#define PCIE1_BAR0_REG              (PCIE1_CFG_BASE + 0x10)  //99000010
#define PCIE1_BAR1_REG              (PCIE1_CFG_BASE + 0x14) 
#define PCIE1_BAR2_REG              (PCIE1_CFG_BASE + 0x18)
#define PCIE1_BAR3_REG              (PCIE1_CFG_BASE + 0x1C)
#define PCIE1_BAR4_REG              (PCIE1_CFG_BASE + 0x20)
#define PCIE1_BAR5_REG              (PCIE1_CFG_BASE + 0x24)
#define PCIE1_CARDBUS_REG           (PCIE1_CFG_BASE + 0x28) 
#define PCIE1_SID_REG               (PCIE1_CFG_BASE + 0x2C)
#define PCIE1_ROM_BASE_REG          (PCIE1_CFG_BASE + 0x30)
#define PCIE1_ROM_MASK_REG          (PCIE1_CFG_BASE + 0x30)
#define PCIE1_CAP_REG               (PCIE1_CFG_BASE + 0x34)
#define PCIE1_INT_REG               (PCIE1_CFG_BASE + 0x3C)
			    
/* Port Logic Register Base */
#define PCIE1_PL_REG                (PCIE1_CFG_BASE + 0x700)

/***********************************************************/
/* PF Power Management Capability Structure(CFG_PM_CAP)    */
/***********************************************************/
#define PCIE1_PM_CAP_REG                 (PCIE1_PM_CAP_BASE + 0x0)
#define PCIE1_PMCSR_REG                  (PCIE1_PM_CAP_BASE + 0x4)

/***********************************************************/
/* PF MSI Capability Structure                             */
/***********************************************************/
#define PCIE1_MSI_CAP_REG                (PCIE1_MSI_CAP_BASE + 0x0 ) 
#define PCIE1_MSI_LADDR_REG              (PCIE1_MSI_CAP_BASE + 0x4 )
#define PCIE1_MSI_UADDR_REG              (PCIE1_MSI_CAP_BASE + 0x8 )
#define PCIE1_MSI_DATA_REG               (PCIE1_MSI_CAP_BASE + 0xC )
#define PCIE1_MSI_MASK_REG               (PCIE1_MSI_CAP_BASE + 0x10)
#define PCIE1_MSI_PEND_REG               (PCIE1_MSI_CAP_BASE + 0x14)

/***********************************************************/
/* PF PCI Express Capability Structure                     */
/***********************************************************/
#define PCIE1_CID_REG               (PCIE1_PCIE_CAP_BASE + 0x0 )          
#define PCIE1_DCAP_REG              (PCIE1_PCIE_CAP_BASE + 0x04)         
#define PCIE1_DCNT_REG              (PCIE1_PCIE_CAP_BASE + 0x08)         
#define PCIE1_LCAP_REG              (PCIE1_PCIE_CAP_BASE + 0x0C)         
#define PCIE1_LCNT_REG              (PCIE1_PCIE_CAP_BASE + 0x10)  //9800_0080       
#define PCIE1_DCAP2_REG             (PCIE1_PCIE_CAP_BASE + 0x24)         
#define PCIE1_DCNT2_REG             (PCIE1_PCIE_CAP_BASE + 0x28)         
#define PCIE1_LCNT2_REG             (PCIE1_PCIE_CAP_BASE + 0x30)         

/***********************************************************/
/* Port Logic Registers                                    */
/***********************************************************/

#define PCIE1_ALRT_REG              (PCIE1_PL_REG)
#define PCIE1_AFL0L1_REG            (PCIE1_PL_REG + 0xC)
#define PCIE1_SYMNUM_REG            (PCIE1_PL_REG + 0x18)
#define PCIE1_G2CTRL_REG            (PCIE1_PL_REG + 0x10C)

/***********************************************************/
/* MSI Interrupt Controller Registers                      */
/***********************************************************/
#define PCIE1_MSI_CTRL_ADDR_REG         (PCIE1_PL_REG + 0x120)
#define PCIE1_MSI_CTRL_INTR0_EN_REG     (PCIE1_PL_REG + 0x128)
#define PCIE1_MSI_CTRL_INTR0_STATUS_REG     (PCIE1_PL_REG + 0x130)

/***********************************************************/
/* iATU Registers                                          */
/***********************************************************/
#define PCIE1_iATU_VIEW_REG              (PCIE1_PL_REG + 0x200)
#define PCIE1_iATU_CTL1_REG              (PCIE1_PL_REG + 0x204)
#define PCIE1_iATU_CTL2_REG              (PCIE1_PL_REG + 0x208)
#define PCIE1_iATU_RLBA_REG              (PCIE1_PL_REG + 0x20C)
#define PCIE1_iATU_RUBA_REG              (PCIE1_PL_REG + 0x210)
#define PCIE1_iATU_RLIA_REG              (PCIE1_PL_REG + 0x214)
#define PCIE1_iATU_RLTA_REG              (PCIE1_PL_REG + 0x218)
#define PCIE1_iATU_RUTA_REG              (PCIE1_PL_REG + 0x21C)

/*******************************************************/
/* BAR MASK Registers (uses dbi_cs2)                   */
/*******************************************************/
#define PCIE1_BAR0_MASK_REG              (PCIE1_CFG_BASE + 0x100010)  //0x99100010
#define PCIE1_BAR1_MASK_REG              (PCIE1_CFG_BASE + 0x100014)  //0x99100014
#define PCIE1_BAR2_MASK_REG              (PCIE1_CFG_BASE + 0x100018)  //0x99100018
#define PCIE1_BAR4_MASK_REG              (PCIE1_CFG_BASE + 0x100020)  //0x99100020
#define PCIE1_BAR5_MASK_REG              (PCIE1_CFG_BASE + 0x100024)  //0x99100024

/* Transmit and receive start address */
#define  TRANS_START_ADDR2         PCIE1_SLV_AXI_BASE

/*******************************************************/
/* DWC Configuration Registers                         */
/*******************************************************/
#define DWC_CFG_REG0         (DWC_CFG_BASE + 0x0 )
#define DWC_CFG_REG1         (DWC_CFG_BASE + 0x04)
#define DWC_CFG_REG2         (DWC_CFG_BASE + 0x08)
#define DWC_CFG_REG3         (DWC_CFG_BASE + 0x0C)
#define DWC_CFG_REG4         (DWC_CFG_BASE + 0x10)
#define DWC_CFG_REG5         (DWC_CFG_BASE + 0x14)
#define DWC_CFG_REG6         (DWC_CFG_BASE + 0x18)
#define DWC_CFG_REG7         (DWC_CFG_BASE + 0x1C) 

#define DWC_CFG_REG8         (DWC_CFG_BASE + 0x20)
#define DWC_CFG_REG9         (DWC_CFG_BASE + 0x24)
#define DWC_CFG_REGA         (DWC_CFG_BASE + 0x28)
#define DWC_CFG_REGB         (DWC_CFG_BASE + 0x2C)
#define DWC_CFG_REGC         (DWC_CFG_BASE + 0x30)
#define DWC_CFG_REGD         (DWC_CFG_BASE + 0x34)
#define DWC_CFG_REGE         (DWC_CFG_BASE + 0x38)
#define DWC_CFG_REGF         (DWC_CFG_BASE + 0x3C)

#define DWC_CFG_REG40         (DWC_CFG_BASE + 0x100)     //dwc_intr_status_reg20
#define DWC_CFG_REG41         (DWC_CFG_BASE + 0x104)     //dwc_intr_en_reg20
#define DWC_CFG_REG42         (DWC_CFG_BASE + 0x108)     //dwc_intr_status_reg21
#define DWC_CFG_REG43         (DWC_CFG_BASE + 0x10C)     //dwc_intr_en_reg21
#define DWC_CFG_REG44         (DWC_CFG_BASE + 0x110)     //dwc_intr_status_reg22
#define DWC_CFG_REG45         (DWC_CFG_BASE + 0x114)     //dwc_intr_en_reg22
#define DWC_CFG_REG46         (DWC_CFG_BASE + 0x118)     //dwc_intr_status_reg23
#define DWC_CFG_REG47         (DWC_CFG_BASE + 0x11C)     //dwc_intr_en_reg23
 

#define DWC_STS_REG0         (DWC_CFG_BASE + 0x40)
#define DWC_STS_REG1         (DWC_CFG_BASE + 0x44)
#define DWC_STS_REG2         (DWC_CFG_BASE + 0x48)
#define DWC_STS_REG3         (DWC_CFG_BASE + 0x4C)
#define DWC_STS_REG4         (DWC_CFG_BASE + 0x50)
#define DWC_STS_REG5         (DWC_CFG_BASE + 0x54)
#define DWC_STS_REG6         (DWC_CFG_BASE + 0x58)
#define DWC_STS_REG7         (DWC_CFG_BASE + 0x5C) 

#define DWC_STS_REG10         (DWC_CFG_BASE + 0xC0)
#define DWC_STS_REG11         (DWC_CFG_BASE + 0xC4)
#define DWC_STS_REG12         (DWC_CFG_BASE + 0xC8)
#define DWC_STS_REG13         (DWC_CFG_BASE + 0xCC)
#define DWC_STS_REG20         (DWC_CFG_BASE + 0x100)
#define DWC_STS_REG21         (DWC_CFG_BASE + 0x108)
#define DWC_STS_REG22         (DWC_CFG_BASE + 0x110)
#define DWC_STS_REG23         (DWC_CFG_BASE + 0x118) 


/* =--------------------------------------------------------------= */
/*                     Some Function Macros                         */
/* =--------------------------------------------------------------= */

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

