#include <common.h>
#include <driver.h>
#include <init.h>
#include <malloc.h>
#include <asm/io.h>
#include <mach/comcerto-2000.h>
#include <mach/serial.h>
#include <fast_uart.h>

static int fast_uart_setbrg (struct console_device *cdev, int baudrate)
{

	struct fast_uart_plat *plat = (struct fast_uart_plat *) cdev->dev->platform_data;
	u32 clock = plat->clock;

/* 
 * 16-bit Divisor Latch register that contains the baud rate divisor for the UART.
 *
 * baud rate = (serial clock freq) / (16 * divisor).
 */
	u32 div;

	/* round to nearest */
	div = (clock + 8 * baudrate) / (16 * baudrate);

	writel(LCR_DLAB, cdev->dev->map_base + UART_LCR); /* Enable Data latch to write divisor latch */

	writel( (div & 0xFF), cdev->dev->map_base + UART_DLL); 
	writel( (div >> 8 ) & 0xFF, cdev->dev->map_base + UART_DLH);

	writel(0x00, cdev->dev->map_base + UART_LCR); /* Disable date latch */
	writel(LCR_CHAR_LEN_8 , cdev->dev->map_base + UART_LCR); /* Eight bits per character, 1 stop bit */
	writel(FCR_FIFOEN | FCR_RCVRRES | FCR_XMITRES, cdev->dev->map_base + UART_FCR); /* Reset Tx and Rx FIFOs; Enable FIFO mode; Set Rx FIFO threshold */

	return 0;
}

void fast_uart_putc (struct console_device *cdev, const char c)
{
	/* wait for room in the tx FIFO on FFUART */
	while ((readl(cdev->dev->map_base + UART_LSR) & LSR_THRE) == 0) ;
	writel(c, cdev->dev->map_base + UART_THR);
	if (c == '\n')
		fast_uart_putc (cdev, '\r');
}

int fast_uart_getc (struct console_device *cdev)
{
	while ((readl(cdev->dev->map_base + UART_LSR) & LSR_DR) == 0);
	return (char)readl(cdev->dev->map_base + UART_RBR) & 0xff;
}

int fast_uart_tstc (struct console_device *cdev)
{
	return readl(cdev->dev->map_base + UART_LSR) & LSR_DR;
}

static int fast_uart_probe(struct device_d *dev)
{
        struct console_device *cdev;

        cdev = xmalloc(sizeof(struct console_device));
        dev->type_data = cdev;
        cdev->dev = dev;
        cdev->f_caps = CONSOLE_STDIN | CONSOLE_STDOUT | CONSOLE_STDERR;
        cdev->tstc = fast_uart_tstc;
        cdev->putc = fast_uart_putc;
        cdev->getc = fast_uart_getc;
        cdev->setbrg = fast_uart_setbrg;

        console_register(cdev);

        return 0;
}

static struct driver_d fast_uart_driver = {
        .name = "fast_uart",
        .probe = fast_uart_probe,
};

static int fast_uart_init(void)
{
        return register_driver(&fast_uart_driver);
}

console_initcall(fast_uart_init);

