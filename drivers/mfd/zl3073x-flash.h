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
 */
struct zl3073x_flash_image_type {
	const char	*name;
	size_t		max_words;
	int		(*flash)(struct zl3073x_dev *zldev,
				 struct zl3073x_flash_image *image,
				 struct netlink_ext_ack *extack);
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

#endif /* __ZL3073X_FLASH_H */
