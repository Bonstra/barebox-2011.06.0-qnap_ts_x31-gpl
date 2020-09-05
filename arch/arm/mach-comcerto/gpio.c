/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>
#include <mach/comcerto-2000.h>
#include <generated/mach-types.h>
#include <mach/gpio.h>

void gpio_set_value(unsigned gpio, int value)
{
  	unsigned bit;
 
    if (gpio == 44 ||  gpio ==45 || gpio == 46 || gpio == 47)
    {
        bit = 1 << (gpio - 32);
        
        if (value)
    	    writel(readl(COMCERTO_GPIO_EXT_OUTPUT_REG) | bit , COMCERTO_GPIO_EXT_OUTPUT_REG);
        else
    	    writel(readl(COMCERTO_GPIO_EXT_OUTPUT_REG) & ~ bit , COMCERTO_GPIO_EXT_OUTPUT_REG);
    }
    else if (gpio == 7 || gpio == 15)
    {
        bit = 1 << gpio;
        
        if (value)
    	    writel(readl(COMCERTO_GPIO_OUTPUT_REG) | bit , COMCERTO_GPIO_OUTPUT_REG);
        else
    	    writel(readl(COMCERTO_GPIO_OUTPUT_REG) & ~bit , COMCERTO_GPIO_OUTPUT_REG);
    }
}

int gpio_direction_input(unsigned gpio)
{
	return 0;
}


int gpio_direction_output(unsigned gpio, int value)
{
	return 0;
}

int gpio_get_value(unsigned gpio)
{
 
    if (gpio != 1 && gpio !=2)
        return 1;

    return !!(readl(COMCERTO_GPIO_INPUT_REG) & (1 << gpio));
}

