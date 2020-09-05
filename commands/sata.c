#include <common.h>
#include <command.h>
#include <part.h>
#include <sata.h>

#define CONFIG_SYS_SATA_MAX_DEVICE      1

static int sata_curr_device = -1;
block_dev_desc_t sata_dev_desc[CONFIG_SYS_SATA_MAX_DEVICE];

int sata_initialize(void)
{
        int rc;
        int i;

        for (i = 0; i < CONFIG_SYS_SATA_MAX_DEVICE; i++) {
                memset(&sata_dev_desc[i], 0, sizeof(struct block_dev_desc));
                sata_dev_desc[i].if_type = IF_TYPE_SATA;
                sata_dev_desc[i].dev = i;
                sata_dev_desc[i].part_type = PART_TYPE_UNKNOWN;
                sata_dev_desc[i].type = DEV_TYPE_HARDDISK;
                sata_dev_desc[i].lba = 0;
                sata_dev_desc[i].blksz = 512;
                sata_dev_desc[i].block_read = sata_read;
                sata_dev_desc[i].block_write = sata_write;

                rc = init_sata(i);
                if (!rc) {
                        rc = scan_sata(i);
                        if (!rc && (sata_dev_desc[i].lba > 0) &&
                                (sata_dev_desc[i].blksz > 0))
                                init_part(&sata_dev_desc[i]);
                }
        }
        sata_curr_device = 0;
        return rc;
}


static int do_sata(struct command *cmdtp, int argc, char *argv[])
{
        int rc = 0;

        if (argc == 2 && strcmp(argv[1], "init") == 0)
                return sata_initialize();

        if (argc == 2 && strcmp(argv[1], "stop") == 0)
                return stop_sata();

        /* If the user has not yet run `sata init`, do it now */
        if (sata_curr_device == -1)
                if (sata_initialize())
                        return 1;

        switch (argc) {
        case 0:
        case 1:
                return -1;
        case 2:
                if (strncmp(argv[1],"inf", 3) == 0) {
                        int i;
                        printf('\n');
                        for (i = 0; i < CONFIG_SYS_SATA_MAX_DEVICE; ++i) {
                                if (sata_dev_desc[i].type == DEV_TYPE_UNKNOWN)
                                        continue;
                                printf ("SATA device %d: ", i);
                                dev_print(&sata_dev_desc[i]);
                        }
                        return 0;
                } else if (strncmp(argv[1],"dev", 3) == 0) {
                        if ((sata_curr_device < 0) || (sata_curr_device >= CONFIG_SYS_SATA_MAX_DEVICE)) {
                                printf("\nno SATA devices available\n");
                                return 1;
                        }
                        printf("\nSATA device %d: ", sata_curr_device);
                        dev_print(&sata_dev_desc[sata_curr_device]);
                        return 0;
                } else if (strncmp(argv[1],"part",4) == 0) {
                        int dev, ok;

                        for (ok = 0, dev = 0; dev < CONFIG_SYS_SATA_MAX_DEVICE; ++dev) {
                                if (sata_dev_desc[dev].part_type != PART_TYPE_UNKNOWN) {
                                        ++ok;
                                        if (dev)
                                                printf ('\n');
                                        print_part(&sata_dev_desc[dev]);
                                }
                        }
                        if (!ok) {
                                printf("\nno SATA devices available\n");
                                rc ++;
                        }
                        return rc;
                }
                return -1;
        case 3:
                if (strncmp(argv[1], "dev", 3) == 0) {
                        int dev = (int)simple_strtoul(argv[2], NULL, 10);

                        printf("\nSATA device %d: ", dev);
                        if (dev >= CONFIG_SYS_SATA_MAX_DEVICE) {
                                printf ("unknown device\n");
                                return 1;
                        }
                        dev_print(&sata_dev_desc[dev]);

                        if (sata_dev_desc[dev].type == DEV_TYPE_UNKNOWN)
                                return 1;

                        sata_curr_device = dev;

                        printf("... is now current device\n");

                        return 0;
                } else if (strncmp(argv[1], "part", 4) == 0) {
                        int dev = (int)simple_strtoul(argv[2], NULL, 10);

                        if (sata_dev_desc[dev].part_type != PART_TYPE_UNKNOWN) {
                                print_part(&sata_dev_desc[dev]);
                        } else {
                                printf("\nSATA device %d not available\n", dev);
                                rc = 1;
                        }
                        return rc;
                }
                return -1;

        default: /* at least 4 args */
                if (strcmp(argv[1], "read") == 0) {
                        ulong addr = simple_strtoul(argv[2], NULL, 16);
                        ulong cnt = simple_strtoul(argv[4], NULL, 16);
                        ulong n;
                        lbaint_t blk = simple_strtoul(argv[3], NULL, 16);

                        printf("\nSATA read: device %d block # %ld, count %ld ... ",
                                sata_curr_device, blk, cnt);

                        n = sata_read(sata_curr_device, blk, cnt, (u32 *)addr);

                        /* flush cache after read */
			//RAJIV
                        //flush_cache(addr, cnt * sata_dev_desc[sata_curr_device].blksz);

                        printf("%ld blocks read: %s\n",
                                n, (n==cnt) ? "OK" : "ERROR");
                        return (n == cnt) ? 0 : 1;
                } else if (strcmp(argv[1], "write") == 0) {
                        ulong addr = simple_strtoul(argv[2], NULL, 16);
                        ulong cnt = simple_strtoul(argv[4], NULL, 16);
                        ulong n;

                        lbaint_t blk = simple_strtoul(argv[3], NULL, 16);

                        printf("\nSATA write: device %d block # %ld, count %ld ... ",
                                sata_curr_device, blk, cnt);

                        n = sata_write(sata_curr_device, blk, cnt, (u32 *)addr);

                        printf("%ld blocks written: %s\n",
                                n, (n == cnt) ? "OK" : "ERROR");
                        return (n == cnt) ? 0 : 1;
                } else {
                        return -1;
                }

                return rc;
        }

}

BAREBOX_CMD_START(sata)
        .cmd            = do_sata,
        .usage          = "SATA sub system",
BAREBOX_CMD_END

