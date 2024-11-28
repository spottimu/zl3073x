/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef __ZL3073X_FLASH_H
#define __ZL3073X_FLASH_H

#include <linux/mfd/zl3073x-regs.h>
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
#define ZL_REG_FLASH_HASH			ZL_REG(0, 0x78, 4)
#define ZL_REG_FLASH_FAMILY			ZL_REG(0, 0x7c, 1)
#define ZL_REG_FLASH_RELEASE			ZL_REG(0, 0x7d, 1)

#define ZL_REG_HOST_CONTROL			ZL_REG(1, 0x02, 1)
#define ZL_HOST_CONTROL_ENABLE			BIT(0)

#define ZL_REG_IMAGE_START_ADDR			ZL_REG(1, 0x04, 4)
#define ZL_REG_IMAGE_SIZE			ZL_REG(1, 0x08, 4)
#define ZL_REG_FLASH_INDEX_READ			ZL_REG(1, 0x0c, 4)
#define ZL_REG_FLASH_INDEX_WRITE		ZL_REG(1, 0x10, 4)
#define ZL_REG_FILL_PATTERN			ZL_REG(1, 0x14, 4)

#define ZL_REG_WRITE_FLASH			ZL_REG(1, 0x18, 1)
#define ZL_WRITE_FLASH_OP			GENMASK(2, 0)
#define ZL_WRITE_FLASH_OP_DONE			0x0
#define ZL_WRITE_FLASH_OP_SECTORS		0x2
#define ZL_WRITE_FLASH_OP_PAGE			0x3
#define ZL_WRITE_FLASH_OP_COPY_PAGE		0x4

#define ZL_REG_FLASH_INFO			ZL_REG(2, 0x00, 1)
#define ZL_FLASH_INFO_SECTOR_SIZE		GENMASK(3, 0)
#define ZL_FLASH_INFO_SECTOR_4K			0
#define ZL_FLASH_INFO_SECTOR_64K		1

#define ZL_REG_ERROR_COUNT			ZL_REG(2, 0x04, 4)
#define ZL_REG_ERROR_CAUSE			ZL_REG(2, 0x08, 4)

#define ZL_REG_OP_STATE				ZL_REG(2, 0x14, 1)
#define ZL_OP_STATE_NO_COMMAND			0
#define ZL_OP_STATE_PENDING			1
#define ZL_OP_STATE_DONE			2

/*
 * Host registers to access indirectly HW registers
 */
#define ZL_REG_HWREG_OP				ZL_REG(0xff, 0x00, 1)
#define ZL_HWREG_OP_WRITE			0x28
#define ZL_HWREG_OP_READ			0x29
#define ZL_HWREG_OP_PENDING			BIT(1)

#define ZL_REG_HWREG_ADDR			ZL_REG(0xff, 0x04, 4)
#define ZL_REG_HWREG_WRITE_DATA			ZL_REG(0xff, 0x08, 4)
#define ZL_REG_HWREG_READ_DATA			ZL_REG(0xff, 0x0c, 4)

#endif /* __ZL3073X_FLASH_H */
