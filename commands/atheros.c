#include <common.h>
#include <command.h>
#include <errno.h>
#include <miidev.h>
#include "../drivers/net/comcerto/ar8328.h"

#define PHY_DEVICE      "phy0"

static int do_phy(struct command *cmdtp, int argc, char *argv[])
{
	uint16_t val;
	int phyaddr;
	int phyreg;
	int phydata;
	struct device_d *dev;
	struct mii_device *mdev;
        struct cdev *cdev;
        cdev = cdev_by_name(PHY_DEVICE);
        mdev = cdev->priv;

	if(argc < 2)
		return COMMAND_ERROR_USAGE;

	if(!strcmp(argv[1],"stat"))
	{
		athrs17_phy_stat(mdev);
		
	}
	else if(!strcmp(argv[1],"init"))
	{
		phyaddr = simple_strtoul(argv[2], NULL, 0);
		athrs17_phy_setup(mdev, phyaddr);		
	}
	else if(!strcmp(argv[1],"read"))
	{
		if(argc != 4)
			return COMMAND_ERROR_USAGE;

		phyaddr = simple_strtoul(argv[2], NULL, 0);
		phyreg = simple_strtoul(argv[3], NULL, 0);

		val = mdev->read(mdev, phyaddr, phyreg);	
		printf("PHY %d , Addr 0x%x, Val 0x%x\n", phyaddr, phyreg, val);

	}else if(!strcmp(argv[1],"write"))
	{
		if(argc != 5)
			return COMMAND_ERROR_USAGE;
		
		phyaddr = simple_strtoul(argv[2], NULL, 0);
		phyreg = simple_strtoul(argv[3], NULL, 0);
		phydata = simple_strtoul(argv[4], NULL, 0);

		mdev->write(mdev, phyaddr, phyreg, phydata);
	}

	return 0;
}

static const __maybe_unused char cmd_phy_help[] =
"Usage:\n"
"\n"
" phy stat			show all the phy status\n"
" phy init phyno		init the PHYs. For WAN, the PHY no is 4\n"
"				for LAN, the PHY no to be 0 to set PHY0-3\n"				
" phy read phyno reg		read the PHY register\n"
" phy write phyno reg data  		write the PHY register\n"
;


BAREBOX_CMD_START(phy)
        .cmd            = do_phy,
        .usage          = "PHY status, read and write operation",
	BAREBOX_CMD_HELP(cmd_phy_help)
BAREBOX_CMD_END

static int do_switch(struct command *cmdtp, int argc, char *argv[])
{
        uint16_t val;
	int reg;
	int data;
        struct device_d *dev;
        struct mii_device *mdev;
	struct cdev *cdev;
	cdev = cdev_by_name(PHY_DEVICE);
	mdev = cdev->priv;

        if(argc < 2)
                return COMMAND_ERROR_USAGE;



        if(!strcmp(argv[1],"init"))
        {
		athrs17_init(mdev);
	}
        else if(!strcmp(argv[1],"read"))
        {

		reg = simple_strtoul(argv[2], NULL, 0);
		val = athrs17_reg_read(mdev, reg);

		printf("Addr 0x%x Val 0x%x\n", reg, val);
	} 
	else if(!strcmp(argv[1],"write"))
	{
                if(argc != 4)
                        return COMMAND_ERROR_USAGE;

		reg = simple_strtoul(argv[2], NULL, 0);
		data = simple_strtoul(argv[3], NULL, 0);
		athrs17_reg_write(mdev, reg, data);
	}
	else
		return COMMAND_ERROR_USAGE;

	return 0;
}

static const __maybe_unused char cmd_switch_help[] =
"Usage:\n"
"\n"
" switch init			   initialize the AR8328 switch\n"
" switch read phyno reg            read the switch register\n"
" switch write phyno reg data      write the switch register\n"
;


BAREBOX_CMD_START(switch)
        .cmd            = do_switch,
        .usage          = "Atheros AR8328 switch read and write operation",
        BAREBOX_CMD_HELP(cmd_switch_help)
BAREBOX_CMD_END

