/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef __ZL3073X_FLASH_H
#define __ZL3073X_FLASH_H

#include "zl3073x.h"

struct zl3073x_flash_image;

/**
 * struct zl3073x_flash_image_type - Flash hex-image type info
 *
 * @name: Flash image name
 * @max_words: Maximal image size in 32-bit words
 * @flash_op: Operation specific to flash image type
 * @load_addr: Device memory address where should be the image loaded
 */
struct zl3073x_flash_image_type {
	const char	*name;
	size_t		max_words;
	int		(*flash)(struct zl3073x_dev *zldev,
				 struct zl3073x_flash_image *image,
				 struct netlink_ext_ack *extack);
	u32		load_addr;
	u32		flash_page;
	u32		copy_page;
};

/**
 * struct zl3073x_flash_image - Flash hex-image structure
 * @info: Hex-image type info
 * @words: Buffer with data to be flashed
 * @nwords: Size of the buffer in 32-bit words
 */
struct zl3073x_flash_image {
	const struct zl3073x_flash_image_type	*type;
	u32					*words;
	u32					nwords;
};

/*
 * Host registers for checking flash utility status
 */
ZL3073X_REG32_DEF(flash_hash,			0x0078);
ZL3073X_REG8_DEF(flash_fam,			0x007c);
ZL3073X_REG8_DEF(flash_rel,			0x007d);
ZL3073X_REG8_DEF(host_control,			0x0082);
#define ZL3073X_REG_HOST_CONTROL_ENABLE		BIT(0)

ZL3073X_REG32_DEF(image_start_addr,		0x0084);
ZL3073X_REG32_DEF(image_size,			0x0088);
ZL3073X_REG32_DEF(flash_index_read,		0x008c);
ZL3073X_REG32_DEF(flash_index_write,		0x0090);
ZL3073X_REG32_DEF(fill_pattern,			0x0094);
ZL3073X_REG32_DEF(write_flash,			0x0098);
#define ZL3073X_REG_WRITE_FLASH_OP		GENMASK(2, 0)
#define ZL3073X_REG_WRITE_FLASH_OP_DONE		0x0
#define ZL3073X_REG_WRITE_FLASH_OP_SECTORS	0x2
#define ZL3073X_REG_WRITE_FLASH_OP_PAGE		0x3
#define ZL3073X_REG_WRITE_FLASH_OP_COPY_PAGE	0x4

ZL3073X_REG8_DEF(flash_info,			0x0100);
#define ZL3073X_REG_FLASH_INFO_SECTOR_SIZE	GENMASK(3, 0)
#define ZL3073X_REG_FLASH_INFO_SECTOR_4K	0
#define ZL3073X_REG_FLASH_INFO_SECTOR_64K	1

ZL3073X_REG32_DEF(error_count,			0x0104);
ZL3073X_REG32_DEF(error_cause,			0x0108);

ZL3073X_REG8_DEF(op_state,			0x0114);
#define ZL3073X_REG_OP_STATE_NO_COMMAND		0
#define ZL3073X_REG_OP_STATE_PENDING		1
#define ZL3073X_REG_OP_STATE_DONE		2

/*
 * Host registers to access indirectly HW registers
 */
ZL3073X_REG8_DEF(hwreg_op, 0x7f80);
#define ZL3073X_REG_HWREG_OP_WRITE             0x28
#define ZL3073X_REG_HWREG_OP_READ              0x29
#define ZL3073X_REG_HWREG_OP_PENDING           BIT(1)

ZL3073X_REG32_DEF(hwreg_addr, 0x7f84);
ZL3073X_REG32_DEF(hwreg_write_data, 0x7f88);
ZL3073X_REG32_DEF(hwreg_read_data, 0x7f8c);

#endif /* __ZL3073X_FLASH_H */
