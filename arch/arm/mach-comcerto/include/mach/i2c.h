/* I2C */
#define CFG_TCLK        187500000

#define MAX_I2C_RETRYS      10
#define I2C_DELAY           1000        /* Should be at least the # of MHz of Tclk */

#define I2C_ADDR                        0x00
#define I2C_DATA                        0x04
#define I2C_CNTR                        0x08
#define I2C_STAT                        0x0c
#define I2C_CCRFS                       0x0c
#define I2C_XADDR                       0x10
#define I2C_CCRH                        0x14
#define I2C_SOFT_RESET                  0x1c

/* CNTR - Control register bits */
#define I2C_IEN                         (1<<7)
#define I2C_ENAB                        (1<<6)
#define I2C_STA                         (1<<5)
#define I2C_STP                         (1<<4)
#define I2C_IFLG                        (1<<3)
#define I2C_AAK                         (1<<2)

/* STAT - Status codes */
#define I2C_BUS_ERROR                   0x00    /* Bus error in master mode only */
#define I2C_START_TRANSMIT              0x08    /* Start condition transmitted */
#define I2C_REPEAT_START_TRANSMIT       0x10    /* Repeated Start condition transmited */
#define I2C_ADDRESS_W_ACK               0x18    /* Address + Write bit transmitted, ACK received */
#define I2C_ADDRESS_W_NACK              0x20    /* Address + Write bit transmitted, NACK received */
#define I2C_DATA_TRANSMIT_ACK           0x28    /* Data byte transmitted in master mode , ACK received */
#define I2C_DATA_TRANSMIT_NACK          0x30    /* Data byte transmitted in master mode , NACK received */
#define I2C_ARBIT_LOST                  0x38    /* Arbitration lost in address or data byte */
#define I2C_ADDRESS_R_ACK               0x40    /* Address + Read bit transmitted, ACK received  */
#define I2C_ADDRESS_R_NACK              0x48    /* Address + Read bit transmitted, NACK received  */
#define I2C_DATA_RECEIVE_ACK            0x50    /* Data byte received in master mode, ACK transmitted  */
#define I2C_DATA_RECEIVE_NACK           0x58    /* Data byte received in master mode, NACK transmitted*/
#define I2C_ARBIT_LOST_ADDRESS          0x68    /* Arbitration lost in address  */
#define I2C_GENERAL_CALL                0x70    /* General Call, ACK transmitted */
#define I2C_NO_RELEVANT_INFO            0xF8    /* No relevant status information, IFLF=0 */


