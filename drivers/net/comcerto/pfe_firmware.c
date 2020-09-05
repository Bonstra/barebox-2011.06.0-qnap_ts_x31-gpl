

/** @file
 *  Contains all the functions to handle parsing and loading of PE firmware files.
 */

#include "hal.h"
#include "pfe_firmware.h"
#include "pfe/pfe.h"


/* CLASS-PE ELF file content */
unsigned char class_fw_data[] __attribute__((aligned(sizeof(int)))) = {
#include CLASS_FIRMWARE_FILENAME
};

/* TMU-PE ELF file content */
unsigned char tmu_fw_data[] __attribute__((aligned(sizeof(int)))) = {
#include TMU_FIRMWARE_FILENAME
};

#if !defined(CONFIG_UTIL_PE_DISABLED)
unsigned char util_fw_data[] = {
#include UTIL_FIRMWARE_FILENAME
};
#endif

/** PFE elf firmware loader.
* Loads an elf firmware image into a list of PE's (specified using a bitmask)
*
* @param pe_mask	Mask of PE id's to load firmware to
* @param fw		Pointer to the firmware image
*
* @return		0 on sucess, a negative value on error
*
*/
int pfe_load_elf(int pe_mask, const struct firmware *fw)
{
	Elf32_Ehdr *elf_hdr = (Elf32_Ehdr *)fw->data;
	Elf32_Half sections = be16_to_cpu(elf_hdr->e_shnum);
	Elf32_Shdr *shdr = (Elf32_Shdr *) (fw->data + be32_to_cpu(elf_hdr->e_shoff));
	int id, section;
	int rc;

	printk(KERN_INFO "%s\n", __func__);

	printk(KERN_INFO "%s no of sections: %d\n", __func__, sections);

	/* Some sanity checks */
	if (strncmp(&elf_hdr->e_ident[EI_MAG0], ELFMAG, SELFMAG))
	{
		printk(KERN_ERR "%s: incorrect elf magic number\n", __func__);
		return -EINVAL;
	}

	if (elf_hdr->e_ident[EI_CLASS] != ELFCLASS32)
	{
		printk(KERN_ERR "%s: incorrect elf class(%x)\n", __func__, elf_hdr->e_ident[EI_CLASS]);
		return -EINVAL;
	}

	if (elf_hdr->e_ident[EI_DATA] != ELFDATA2MSB)
	{
		printk(KERN_ERR "%s: incorrect elf data(%x)\n", __func__, elf_hdr->e_ident[EI_DATA]);
		return -EINVAL;
	}

	if (be16_to_cpu(elf_hdr->e_type) != ET_EXEC)
	{
		printk(KERN_ERR "%s: incorrect elf file type(%x)\n", __func__, be16_to_cpu(elf_hdr->e_type));
		return -EINVAL;
	}	

	for (section = 0; section < sections; section++, shdr++)
	{
		if (!(be32_to_cpu(shdr->sh_flags) & (SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR)))
                        continue;
		
		for (id = 0; id < MAX_PE; id++)
			if (pe_mask & (1 << id))
			{
				rc = pe_load_elf_section(id, fw->data, shdr);
				if (rc < 0)
					goto err;
			}
	}

	return 0;

err:
	return rc;
}

/** PFE firmware initialization.
* Loads different firmware files from filesystem.
* Initializes PE IMEM/DMEM and UTIL-PE DDR
* Initializes control path symbol addresses (by looking them up in the elf firmware files
* Takes PE's out of reset
*
* @return	0 on sucess, a negative value on error
*
*/
int pfe_firmware_init(void)
{
	struct firmware class_fw, tmu_fw;
#if !defined(CONFIG_UTIL_PE_DISABLED)
 	struct firmware util_fw;
#endif
	int rc = 0;

	printk(KERN_INFO "%s\n", __func__);

	class_fw.data = class_fw_data;
	tmu_fw.data = tmu_fw_data;
#if !defined(CONFIG_UTIL_PE_DISABLED)
	util_fw.data = util_fw_data;
#endif
	
	rc = pfe_load_elf(CLASS_MASK, &class_fw);
	if (rc < 0) {
		printk(KERN_ERR "%s: class firmware load failed\n", __func__);
		goto err3;
	}

	printk(KERN_INFO "%s: class firmware loaded\n", __func__);

	rc = pfe_load_elf(TMU_MASK, &tmu_fw);
	if (rc < 0) {
		printk(KERN_ERR "%s: tmu firmware load failed\n", __func__);
		goto err3;
	}

	printk(KERN_INFO "%s: tmu firmware loaded\n", __func__);

#if !defined(CONFIG_UTIL_PE_DISABLED)
	rc = pfe_load_elf(UTIL_MASK, &util_fw);
	if (rc < 0) {
		printk(KERN_ERR "%s: util firmware load failed\n", __func__);
		goto err3;
	}

	printk(KERN_INFO "%s: util firmware loaded\n", __func__);

	util_enable();
#endif

	tmu_enable(0xf);
	class_enable();

	gpi_enable(HGPI_BASE_ADDR);


err3:
	return rc;
}

/** PFE firmware cleanup
* Puts PE's in reset
*
*
*/
void pfe_firmware_exit(void)
{
	printk(KERN_INFO "%s\n", __func__);

	class_disable();
	tmu_disable(0xf);
#if !defined(CONFIG_UTIL_PE_DISABLED)
	util_disable();
#endif
	hif_tx_disable();
	hif_rx_disable();
}
