/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LINUX_MFD_ZL3073X_REGS_H
#define __LINUX_MFD_ZL3073X_REGS_H

#include <asm/byteorder.h>
#include <linux/lockdep.h>
#include <linux/mfd/zl3073x.h>
#include <linux/regmap.h>
#include <linux/types.h>
#include <linux/unaligned.h>

/* Registers are mapped at offset 0x100 */
#define ZL_RANGE_OFF	       0x100
#define ZL_PAGE_SIZE	       0x80
#define ZL_REG_ADDR(_pg, _off) (ZL_RANGE_OFF + (_pg) * ZL_PAGE_SIZE + (_off))

/**************************
 * Register Page 0, General
 **************************/

/*
 * Register 'id'
 * Page: 0, Offset: 0x01, Size: 16 bits
 */
#define ZL_REG_ID ZL_REG_ADDR(0, 0x01)

static inline __maybe_unused int
zl3073x_read_id(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	rc = regmap_bulk_read(zldev->regmap, ZL_REG_ID, &temp, sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

/*
 * Register 'revision'
 * Page: 0, Offset: 0x03, Size: 16 bits
 */
#define ZL_REG_REVISION ZL_REG_ADDR(0, 0x03)

static inline __maybe_unused int
zl3073x_read_revision(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	rc = regmap_bulk_read(zldev->regmap, ZL_REG_REVISION, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

/*
 * Register 'fw_ver'
 * Page: 0, Offset: 0x05, Size: 16 bits
 */
#define ZL_REG_FW_VER ZL_REG_ADDR(0, 0x05)

static inline __maybe_unused int
zl3073x_read_fw_ver(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	rc = regmap_bulk_read(zldev->regmap, ZL_REG_FW_VER, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

/*
 * Register 'custom_config_ver'
 * Page: 0, Offset: 0x07, Size: 32 bits
 */
#define ZL_REG_CUSTOM_CONFIG_VER ZL_REG_ADDR(0, 0x07)

static inline __maybe_unused int
zl3073x_read_custom_config_ver(struct zl3073x_dev *zldev, u32 *value)
{
	__be32 temp;
	int rc;

	rc = regmap_bulk_read(zldev->regmap, ZL_REG_CUSTOM_CONFIG_VER, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be32_to_cpu(temp);
	return rc;
}

#endif /* __LINUX_MFD_ZL3073X_REGS_H */
