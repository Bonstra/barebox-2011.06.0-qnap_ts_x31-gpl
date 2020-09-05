// Non-legacy
typedef struct _ssboot_img_hdr
{
	unsigned int magic;		/* Magic No to identify Header == 0x4D535044 */
	unsigned int timestamp;		/* image creation timestamp in second starting from 01/01/76 */
	unsigned int ssboot_header_crc;	/* 32 bit CRC of header */
        unsigned int key_length;	/* Length of Public Key 0/1K,2K/4K. */
        unsigned int hash_type;		/* Hash Type 0/Sha-256/Rsa-Sha-256. */
        unsigned int image_len;		/* Length of Current Image (excluding header). */
} ssboot_img_hdr;

#define SSBOOT_HEADER_LEN       sizeof(ssboot_img_hdr)

#define CRC_INIT                0x0
#define SSBOOT_HASH_LENGTH      32
#define C2K_KEY_LENGTH_4K       512
#define MAX_HEADER_LENGTH       (SSBOOT_HEADER_LEN + 2 * C2K_KEY_LENGTH_4K)

#define	C2K_MAGIC		0x4D535044

// Legacy
typedef struct _legacy_img_hdr
{
	unsigned int base_offset;// this is where we download the code in IRAM, offset of the load address from the base of ARAM
	unsigned int len;// program length in bytes
	unsigned int start_offset;// offset from the top of the dowloaded program, this is where pc jumps to
} legacy_img_hdr;

#define LEGACY_MAX_HEADER_LEN	sizeof(legacy_img_hdr)

