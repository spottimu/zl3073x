// SPDX-License-Identifier: GPL-2.0-or-later

#include <linux/bitfield.h>
#include <linux/mfd/zl3073x.h>
#include <linux/types.h>
#include <net/devlink.h>
#include "zl3073x.h"
#include "zl3073x-flash.h"

/**
 * enum zl3073x_flash_image_id - Identifiers for possible flash image types
 */
enum zl3073x_flash_image_id {
	ZL3073X_FLASH_IMAGE_INVALID = -1,
	ZL3073X_FLASH_IMAGE_UTIL = 0,
	ZL3073X_FLASH_IMAGE_FW1,
	ZL3073X_FLASH_IMAGE_FW2,
	ZL3073X_FLASH_IMAGE_FW3,
	ZL3073X_FLASH_IMAGE_CFG0,
	ZL3073X_FLASH_IMAGE_CFG1,
	ZL3073X_FLASH_IMAGE_CFG2,
	ZL3073X_FLASH_IMAGE_CFG3,
	ZL3073X_FLASH_IMAGE_CFG4,
	ZL3073X_FLASH_IMAGE_CFG5,
	ZL3073X_FLASH_IMAGE_CFG6,
	ZL3073X_NUM_FLASH_IMAGES,
};

/*
 * Array that specifies all possible flash image types
 */
static const struct zl3073x_flash_image_type zl3073x_flash_image_types[] = {
	[ZL3073X_FLASH_IMAGE_UTIL] = {
		.name		= "utility",
		.max_words	= 0x08c0,
		.load_addr	= 0x20000000,
	},
	[ZL3073X_FLASH_IMAGE_FW1] = {
		.name		= "firmware1",
		.max_words	= 0xd400,
		.load_addr	= 0x20002000,
	},
	[ZL3073X_FLASH_IMAGE_FW2] = {
		.name		= "firmware2",
		.max_words	= 0x0010,
		.load_addr	= 0x20000000,
	},
	[ZL3073X_FLASH_IMAGE_FW3] = {
		.name		= "firmware3",
		.max_words	= 0x0092,
		.load_addr	= 0x20000400,
	},
	[ZL3073X_FLASH_IMAGE_CFG0] = {
		.name		= "config0",
		.max_words	= 0x0400,
		.load_addr	= 0x20000000,
	},
	[ZL3073X_FLASH_IMAGE_CFG1] = {
		.name		= "config1",
		.max_words	= 0x0400,
		.load_addr	= 0x20000000,
	},
	[ZL3073X_FLASH_IMAGE_CFG2] = {
		.name		= "config2",
		.max_words	= 0x0400,
		.load_addr	= 0x20000000,
	},
	[ZL3073X_FLASH_IMAGE_CFG3] = {
		.name		= "config3",
		.max_words	= 0x0400,
		.load_addr	= 0x20000000,
	},
	[ZL3073X_FLASH_IMAGE_CFG4] = {
		.name		= "config4",
		.max_words	= 0x0400,
		.load_addr	= 0x20000000,
	},
	[ZL3073X_FLASH_IMAGE_CFG5] = {
		.name		= "config5",
		.max_words	= 0x0400,
		.load_addr	= 0x20000000,
	},
	[ZL3073X_FLASH_IMAGE_CFG6] = {
		.name		= "config6",
		.max_words	= 0x0400,
		.load_addr	= 0x20000000,
	},
};

/* Santity check */
static_assert(ZL3073X_NUM_FLASH_IMAGES ==
	      ARRAY_SIZE(zl3073x_flash_image_types));

/**
 * zl3073x_flash_image_id - Get ID for flash image name
 * @name: input flash image name
 *
 * Returns appropriate ZL3073X_FLASH_IMAGE_* ID for known image name
 * or ZL3073X_FLASH_IMAGE_INVALID if the name is unknown.
 */
static enum zl3073x_flash_image_id zl3073x_flash_image_get_id(const char *name)
{
	size_t i;

	for (i = 0; i < ZL3073X_NUM_FLASH_IMAGES; i++)
		if (!strcasecmp(name, zl3073x_flash_image_types[i].name))
			return i;

	return ZL3073X_FLASH_IMAGE_INVALID;
}

/**
 * zl3073x_flash_image_alloc - Alloc structure to hold image
 * @nwords: size of buffer in 32-bit words to store data
 *
 * Returns pointer to allocated image structure in case of success or
 * NULL if allocation fails.
 */
static struct zl3073x_flash_image *zl3073x_flash_image_alloc(u32 nwords)
{
	struct zl3073x_flash_image *image;

	image = kzalloc(sizeof(struct zl3073x_flash_image), GFP_KERNEL);
	if (!image)
		return NULL;

	image->words = kcalloc(nwords, sizeof(u32), GFP_KERNEL);
	if (!image->words) {
		kfree(image);
		return NULL;
	}

	image->nwords = nwords;

	return image;
}

/**
 * zl3073x_flash_image_free - Free allocated image structure
 * @image: pointer to allocated structure
 */
static void zl3073x_flash_image_free(struct zl3073x_flash_image *image)
{
	if (image)
		kfree(image->words);

	kfree(image);
}

/**
 * zl3073x_flash_image_readline - Read next line from image
 * @dst: destination buffer
 * @dst_sz: destination buffer size
 * @src: source buffer
 * @src_sz: source buffer size
 *
 * Returns number of characters read in case of success or -EINVAL if
 * the line to be read is too long for destination buffer.
 */
static ssize_t zl3073x_flash_image_readline(char *dst, size_t dst_sz,
					    const char *src, size_t src_sz)
{
	size_t skip, len;
	const char *ptr;

	/* Skip any existing new-lines at the beginning */
	ptr = memchr_inv(src, '\n', src_sz);
	if (ptr) {
		skip = ptr - src;
		src_sz -= skip;
		src = ptr;
	} else {
		skip = 0;
	}

	/* Now look for the next new-line in the source */
	ptr = memscan((void *)src, '\n', src_sz);
	len = ptr - src;

	/* Return if the source line is too long for destination */
	if (len >= dst_sz)
		return -EINVAL;

	/* Copy the line from source and append NUL char  */
	memcpy(dst, src, len);
	*(dst+len) = '\0';

	/* Return number of read chars */
	return len + skip;
}

#define FLASH_ERR_PREFIX "FW update failed: "
#define FLASH_ERR_MSG(_zldev, _extack, _msg, ...) do {			\
	dev_err((_zldev)->dev, FLASH_ERR_PREFIX _msg "\n",		\
		## __VA_ARGS__);					\
	NL_SET_ERR_MSG_FMT_MOD((_extack), FLASH_ERR_PREFIX _msg,	\
			       ## __VA_ARGS__);				\
} while (0)

/**
 * zl3073x_flash_image_load - Load image from source
 * @zldev: pointer to device structure
 * @imagep: pointer to image structure pointer
 * @src: source buffer pointer
 * @size: size of source buffer
 * @extack: netlink extack pointer to report errors
 *
 * Loads single image from source and stores its data into allocated
 * structure. Pointer to this structure is stored in @imagep.
 *
 * Returns number of characters read from source or negative value otherwise.
 */
static ssize_t zl3073x_flash_image_load(struct zl3073x_dev *zldev,
					struct zl3073x_flash_image **imagep,
					const char *src, size_t size,
					struct netlink_ext_ack *extack)
{
	struct zl3073x_flash_image *image = NULL;
	struct device *dev = zldev->dev;
	enum zl3073x_flash_image_id id;
	const char *ptr = src;
	u32 nwords, count;
	char line[32];
	ssize_t len;
	int rc;

	/* Fetch image name from input */
	len = zl3073x_flash_image_readline(line, sizeof(line), src, size);
	if (len < 0)
		goto err_too_long;
	else if (!len)
		return 0; /* No more data */

	size -= len;
	ptr += len;

	dev_dbg(dev, "Hex-image '%s' found\n", line);

	id = zl3073x_flash_image_get_id(line);
	if (id == ZL3073X_FLASH_IMAGE_INVALID) {
		FLASH_ERR_MSG(zldev, extack,
			      "FW parse error - unknown image type '%s'", line);
		return -EINVAL;
	}

	/* Fetch image size from input */
	len = zl3073x_flash_image_readline(line, sizeof(line), ptr, size);
	if (len < 0)
		goto err_too_long;
	else if (!len) {
		FLASH_ERR_MSG(zldev, extack, "FW parse error - missing size");
		return -EINVAL;
	}

	size -= len;
	ptr += len;

	rc = kstrtou32(line, 10, &nwords);
	if (rc) {
		FLASH_ERR_MSG(zldev, extack,
			      "FW parse error - invalid size: '%s'", line);
		return rc;
	}

	/* Check image size validity */
	if (nwords > zl3073x_flash_image_types[id].max_words) {
		FLASH_ERR_MSG(zldev, extack,
			      "FW parse error - image too big: %u", nwords);
		return -EINVAL;
	}

	dev_dbg(dev, "Expected image size: %u 32bit words\n", nwords);

	/* Alloc image */
	image = zl3073x_flash_image_alloc(nwords);
	if (!image) {
		FLASH_ERR_MSG(zldev, extack, "Failed to alloc memory");
		return -ENOMEM;
	}

	/* Set image type */
	image->type = &zl3073x_flash_image_types[id];

	/* Load image data */
	for (count = 0; count < nwords; count++) {
		len = zl3073x_flash_image_readline(line, sizeof(line), ptr,
						   size);
		if (len < 0) {
			goto err_too_long;
		} else if (!len) {
			FLASH_ERR_MSG(zldev, extack,
				      "FW parse error - missing data");
			goto err_common;
		}

		size -= len;
		ptr += len;

		rc = kstrtou32(line, 16, &image->words[count]);
		if (rc) {
			FLASH_ERR_MSG(zldev, extack,
				      "FW parse error - invalid data: '%s'",
				      line);
			goto err_common;
		}
	}

	*imagep = image;

	return ptr - src;

err_too_long:
	FLASH_ERR_MSG(zldev, extack, "FW parse error - line too long");
	rc = -EINVAL;

err_common:
	zl3073x_flash_image_free(image);

	return rc;
}

/**
 * zl3073x_flash_image_load_all - Load all images from source
 * @zldev: pointer to device structure
 * @images: pointer to images array
 * @data: source buffer pointer
 * @size: size of source buffer
 * @extack: netlink extack pointer to report errors
 *
 * Loads all images from source and stores them into array provided
 * by caller.
 *
 * Returns 0 in case of success or negative value otherwise.
 */
static int zl3073x_flash_image_load_all(struct zl3073x_dev *zldev,
					struct zl3073x_flash_image **images,
					const char *data, size_t size,
					struct netlink_ext_ack *extack)
{
	struct zl3073x_flash_image *image;
	enum zl3073x_flash_image_id id;
	ssize_t rc;

	do {
		rc = zl3073x_flash_image_load(zldev, &image, data, size,
					      extack);
		if (rc > 0) {
			size -= rc;
			data += rc;

			id = zl3073x_flash_image_get_id(image->type->name);
			if (images[id]) {
				FLASH_ERR_MSG(zldev, extack,
					      "Duplicate flash image '%s'",
					      image->type->name);
				rc = -EINVAL;
				break;
			}
			images[id] = image;
		}
	} while (rc > 0);

	if (rc) {
		for (id = 0; id < ZL3073X_NUM_FLASH_IMAGES; id++)
			zl3073x_flash_image_free(images[id]);
	}

	return rc;
}

/**
 * zl3073x_hwreg_do_op - Perform HW register read/write operation
 * @regmap: regmap to access HW
 * @op: operation to perform
 *
 * Returns -ETIMEDOUT in case of failing to finish requested operation
 * or 0 in case of success.
 */
static int
zl3073x_hwreg_do_op(struct zl3073x_dev *zldev, unsigned char op)
{
	int rc;

	/* Set requested operation and set pending bit */
	rc = zl3073x_write_u8(zldev, ZL_REG_HWREG_OP, op | ZL_HWREG_OP_PENDING);
	if (rc)
		return rc;

	/* Poll for completion - pending bit cleared */
	return zl3073x_poll_zero_u8(zldev, ZL_REG_HWREG_OP,
				    ZL_HWREG_OP_PENDING);
}

/**
 * zl3073x_hwreg_read - Read HW register
 * @regmap: regmap to access HW
 * @addr: HW register address
 * @value: Value of the HW register
 *
 * Reads HW register value and stores it into value and returns 0 in case of
 * success. Otherwise returns negative value.
 */
static int
zl3073x_hwreg_read(struct zl3073x_dev *zldev, u32 addr, u32 *value)
{
	int rc;

	/* Set address to read data from */
	rc = zl3073x_write_u32(zldev, ZL_REG_HWREG_ADDR, addr);
	if (rc)
		return rc;

	/* Perform the read operation */
	rc = zl3073x_hwreg_do_op(zldev, ZL_HWREG_OP_READ);
	if (rc)
		return rc;

	/* Read the received data */
	return zl3073x_read_u32(zldev, ZL_REG_HWREG_READ_DATA, value);
}

/**
 * zl3073x_hwreg_write - Write value to HW register
 * @regmap: regmap to access HW
 * @addr: HW registers address
 * @value: Value to be written to HW register
 *
 * Stores the requested value into HW register and returns 0 in case of
 * success. Otherwise returns negative value.
 */
static int
zl3073x_hwreg_write(struct zl3073x_dev *zldev, u32 addr, u32 value)
{
	int rc;

	/* Set address to write data to */
	rc = zl3073x_write_u32(zldev, ZL_REG_HWREG_ADDR, addr);
	if (rc)
		return rc;

	/* Set data to be written */
	rc = zl3073x_write_u32(zldev, ZL_REG_HWREG_WRITE_DATA, value);
	if (rc)
		return rc;

	/* Perform the write operation */
	return zl3073x_hwreg_do_op(zldev, ZL_HWREG_OP_WRITE);
}

/**
 * zl3073x_hwreg_update - Update certain bits in HW register
 * @regmap: regmap to access HW
 * @addr: HW register address
 * @value: Value to be written into HW register
 * @mask: Bitmask indicating bits to be updated
 *
 * Reads HW register, updates requested bits specified by value&mask and
 * writes result back to HW register. Returns 0 in case of success or
 * negative value otherwise.
 */
static int
zl3073x_hwreg_update(struct zl3073x_dev *zldev, u32 addr, u32 value, u32 mask)
{
	u32 tmp;
	int rc;

	rc = zl3073x_hwreg_read(zldev, addr, &tmp);
	if (rc)
		return rc;

	tmp &= ~mask;
	tmp |= value & mask;

	return zl3073x_hwreg_write(zldev, addr, tmp);
}

/**
 * struct zl3073x_hwreg_seq_item
 * @addr: HW register to be written
 * @value: value to be written to HW register
 * @mask: bitmask indicating bits to be updated
 * @wait: number of ms to wait after register write
 */
struct zl3073x_hwreg_seq_item {
	u32	addr;
	u32	value;
	u32	mask;
	u32	wait;
};

#define HWREG_SEQ_ITEM(_addr, _value, _mask, _wait)	\
{							\
	.addr	= _addr,				\
	.value	= FIELD_PREP_CONST(_mask, _value),	\
	.mask	= _mask,				\
	.wait	= _wait,				\
}

/**
 * zl3073x_hwreg_write_seq - Write HW registers sequence
 * @zldev: pointer to device structure
 * @seq: pointer to first sequence item
 * @num_items: number of items in sequence
 */
static int __maybe_unused
zl3073x_hwreg_write_seq(struct zl3073x_dev *zldev,
			const struct zl3073x_hwreg_seq_item *seq,
			size_t num_items)
{
	int i, rc = 0;

	for (i = 0; i < num_items; i++) {
		if (seq[i].mask == U32_MAX)
			/* Write value directly */
			rc = zl3073x_hwreg_write(zldev, seq[i].addr,
						 seq[i].value);
		else
			/* Update only bits specified by the mask */
			rc = zl3073x_hwreg_update(zldev, seq[i].addr,
						  seq[i].value, seq[i].mask);
		if (rc)
			return rc;

		if (seq->wait)
			msleep(seq->wait);
	}

	return rc;
}

static void zl3073x_flash_notify(struct zl3073x_dev *zldev, const char *msg,
				 const char *component, u32 done, u32 total)
{
	struct devlink *devlink = priv_to_devlink(zldev);

	devlink_flash_update_status_notify(devlink, msg, component, done,
					   total);
}

/**
 * zl3073x_flash_download_image - Download image to device memory
 * @zldev - zl3073x device structure
 * @image - image to be downloaded
 * @extack: netlink extack pointer to report errors
 *
 * Returns 0 in case of success or negative value otherwise.
 */
static __maybe_unused
int zl3073x_flash_download_image(struct zl3073x_dev *zldev,
				 struct zl3073x_flash_image *image,
				 struct netlink_ext_ack *extack)
{
#define CHECK_DELAY	5000 /* Check for interrupt each 5 seconds */
	struct device *dev = zldev->dev;
	unsigned long timeout;
	u32 idx, dest_addr;
	int rc;

	dev_info(zldev->dev, "Loading %u words to device memory at 0x%0x\n",
		 image->nwords, image->type->load_addr);

	/* Send devlink flash notification */
	zl3073x_flash_notify(zldev, "Downloading image started",
			     image->type->name, 0, 0);

	timeout = jiffies + msecs_to_jiffies(CHECK_DELAY);

	dest_addr = image->type->load_addr;
	for (idx = 0; idx < image->nwords; idx++, dest_addr += 4) {
		/* Write current word to HW memory */
		rc = zl3073x_hwreg_write(zldev, dest_addr, image->words[idx]);
		if (rc) {
			FLASH_ERR_MSG(zldev, extack,
				      "Failed to write to memory at 0x%0x",
				      dest_addr);
			goto error;
		}

		/* Check for pending interrupt each 5 seconds */
		if (time_after(jiffies, timeout)) {
			if (signal_pending(current)) {
				FLASH_ERR_MSG(zldev, extack,
					      "Flashing interrupted by signal");
				rc = -EINTR;
				goto error;
			}

			timeout = jiffies + msecs_to_jiffies(CHECK_DELAY);
		}

		/* Report status each 1 kB block */
		if (!((idx + 1) & U8_MAX)) {
			zl3073x_flash_notify(zldev, "Downloading image",
					     image->type->name, idx,
					     image->nwords);
		}
	}

	dev_info(dev, "%u words written to device memory\n", image->nwords);

error:
	/* Send final notification - success or failure */
	zl3073x_flash_notify(zldev,
			     rc ? "Downloading failed" : "Downloading done",
			     image->type->name, 0, 0);

	return rc;
}

/**
 * zl3073x_flash_image_flash_all - Flash all images
 * @zldev: pointer to device structure
 * @images: pointer to images array
 * @extack: netlink extack pointer to report errors
 *
 * Returns 0 in case of success or negative number otherwise.
 */
static int zl3073x_flash_image_flash_all(struct zl3073x_dev *zldev,
					 struct zl3073x_flash_image **images,
					 struct netlink_ext_ack *extack)
{
	enum zl3073x_flash_image_id id;
	int rc = 0;

	for (id = 0; id < ZL3073X_NUM_FLASH_IMAGES; id++) {
		if (!images[id] || !images[id]->type->flash)
			continue;

		rc = images[id]->type->flash(zldev, images[id], extack);
		if (rc) {
			FLASH_ERR_MSG(zldev, extack,
				      "Failed to flash image '%s'",
				      images[id]->type->name);
			break;
		}
	}

	return rc;
}

/**
 * zl3073x_flash_update - Devlink flash update callback
 * @devlink: devlink structure pointer
 * @params: flashing parameters pointer
 * @extack: netlink extack pointer to report errors
 *
 * Returns 0 in case of success or negative value otherwise
 */
int zl3073x_flash_update(struct devlink *devlink,
			 struct devlink_flash_update_params *params,
			 struct netlink_ext_ack *extack)
{
	struct zl3073x_flash_image *images[ZL3073X_NUM_FLASH_IMAGES] = { };
	struct zl3073x_dev *zldev = devlink_priv(devlink);
	enum zl3073x_flash_image_id id;
	int rc;

	zl3073x_flash_notify(zldev, "Preparing to flash", params->component,
			     0, 0);

	/* Load all images from firmware bundle */
	rc = zl3073x_flash_image_load_all(zldev, &images[0], params->fw->data,
					  params->fw->size, extack);
	if (rc)
		goto err_load;

	if (!images[ZL3073X_FLASH_IMAGE_UTIL]) {
		zl3073x_flash_notify(zldev,
				     "Flash utility is missing in firmware",
				     params->component, 0, 0);
		rc = -EINVAL;
		goto err_load;
	}

	/* Flash all loaded images */
	rc = zl3073x_flash_image_flash_all(zldev, images, extack);

	/* Free allocated images */
	for (id = 0; id < ZL3073X_NUM_FLASH_IMAGES; id++)
		zl3073x_flash_image_free(images[id]);

err_load:
	zl3073x_flash_notify(zldev, rc ? "Flashing failed" : "Flashing done",
			     params->component, 0, 0);

	return rc;
}
