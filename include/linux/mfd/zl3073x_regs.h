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

/* Register polling sleep & timeout */
#define ZL_POLL_SLEEP_US   10
#define ZL_POLL_TIMEOUT_US 2000000

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

/***********************************
 * Register Page 9, Synth and Output
 ***********************************/

/*
 * Register array 'synth_ctrl'
 * Page: 9, Offset: 0x00, Size: 8 bits, Items: 5, Stride: 1
 */
#define ZL_REG_SYNTH_CTRL	 ZL_REG_ADDR(9, 0x00)
#define ZL_REG_SYNTH_CTRL_ITEMS	 5
#define ZL_REG_SYNTH_CTRL_STRIDE 1
#define ZL_SYNTH_CTRL_EN	 BIT(0)
#define ZL_SYNTH_CTRL_DPLL_SEL	 GENMASK(6, 4)

static inline __maybe_unused int
zl3073x_read_synth_ctrl(struct zl3073x_dev *zldev, unsigned int idx, u8 *value)
{
	unsigned int addr, v;
	int rc;

	if (idx >= ZL_REG_SYNTH_CTRL_ITEMS)
		return -EINVAL;

	addr = ZL_REG_SYNTH_CTRL + idx * ZL_REG_SYNTH_CTRL_STRIDE;
	rc = regmap_read(zldev->regmap, addr, &v);
	*value = v;
	return rc;
}

/*
 * Register array 'output_ctrl'
 * Page: 9, Offset: 0x28, Size: 8 bits, Items: 10, Stride: 1
 */
#define ZL_REG_OUTPUT_CTRL	  ZL_REG_ADDR(9, 0x28)
#define ZL_REG_OUTPUT_CTRL_ITEMS  10
#define ZL_REG_OUTPUT_CTRL_STRIDE 1
#define ZL_OUTPUT_CTRL_EN	  BIT(0)
#define ZL_OUTPUT_CTRL_STOP	  BIT(1)
#define ZL_OUTPUT_CTRL_STOP_HIGH  BIT(2)
#define ZL_OUTPUT_CTRL_STOP_HZ	  BIT(3)
#define ZL_OUTPUT_CTRL_SYNTH_SEL  GENMASK(6, 4)

static inline __maybe_unused int
zl3073x_read_output_ctrl(struct zl3073x_dev *zldev, unsigned int idx, u8 *value)
{
	unsigned int addr, v;
	int rc;

	if (idx >= ZL_REG_OUTPUT_CTRL_ITEMS)
		return -EINVAL;

	addr = ZL_REG_OUTPUT_CTRL + idx * ZL_REG_OUTPUT_CTRL_STRIDE;
	rc = regmap_read(zldev->regmap, addr, &v);
	*value = v;
	return rc;
}

/*******************************
 * Register Page 10, Ref Mailbox
 *******************************/

/*
 * Register 'ref_mb_mask'
 * Page: 10, Offset: 0x02, Size: 16 bits
 */
#define ZL_REG_REF_MB_MASK ZL_REG_ADDR(10, 0x02)

static inline __maybe_unused int
zl3073x_mb_read_ref_mb_mask(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_REF_MB_MASK, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_ref_mb_mask(struct zl3073x_dev *zldev, u16 value)
{
	__be16 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be16(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_REF_MB_MASK, &temp,
				 sizeof(temp));
}

/*
 * Register 'ref_mb_sem'
 * Page: 10, Offset: 0x04, Size: 8 bits
 */
#define ZL_REG_REF_MB_SEM ZL_REG_ADDR(10, 0x04)
#define ZL_REF_MB_SEM_WR  BIT(0)
#define ZL_REF_MB_SEM_RD  BIT(1)

static inline __maybe_unused int
zl3073x_mb_read_ref_mb_sem(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_read(zldev->regmap, ZL_REG_REF_MB_SEM, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_ref_mb_sem(struct zl3073x_dev *zldev, u8 value)
{
	lockdep_assert_held(&zldev->mailbox_lock);
	return regmap_write(zldev->regmap, ZL_REG_REF_MB_SEM, value);
}

static inline __maybe_unused int
zl3073x_mb_poll_ref_mb_sem(struct zl3073x_dev *zldev, u8 bitmask)
{
	unsigned int v;

	lockdep_assert_held(&zldev->mailbox_lock);
	return regmap_read_poll_timeout(zldev->regmap, ZL_REG_REF_MB_SEM, v,
					!(v & bitmask), ZL_POLL_SLEEP_US,
					ZL_POLL_TIMEOUT_US);
}

/*
 * Register 'ref_config'
 * Page: 10, Offset: 0x0d, Size: 8 bits
 */
#define ZL_REG_REF_CONFIG     ZL_REG_ADDR(10, 0x0d)
#define ZL_REF_CONFIG_ENABLE  BIT(0)
#define ZL_REF_CONFIG_DIFF_EN BIT(2)

static inline __maybe_unused int
zl3073x_mb_read_ref_config(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_read(zldev->regmap, ZL_REG_REF_CONFIG, &v);
	*value = v;
	return rc;
}

/********************************
 * Register Page 12, DPLL Mailbox
 ********************************/

/*
 * Register 'dpll_mb_mask'
 * Page: 12, Offset: 0x02, Size: 16 bits
 */
#define ZL_REG_DPLL_MB_MASK ZL_REG_ADDR(12, 0x02)

static inline __maybe_unused int
zl3073x_mb_read_dpll_mb_mask(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_DPLL_MB_MASK, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_dpll_mb_mask(struct zl3073x_dev *zldev, u16 value)
{
	__be16 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be16(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_DPLL_MB_MASK, &temp,
				 sizeof(temp));
}

/*
 * Register 'dpll_mb_sem'
 * Page: 12, Offset: 0x04, Size: 8 bits
 */
#define ZL_REG_DPLL_MB_SEM ZL_REG_ADDR(12, 0x04)
#define ZL_DPLL_MB_SEM_WR  BIT(0)
#define ZL_DPLL_MB_SEM_RD  BIT(1)

static inline __maybe_unused int
zl3073x_mb_read_dpll_mb_sem(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_read(zldev->regmap, ZL_REG_DPLL_MB_SEM, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_dpll_mb_sem(struct zl3073x_dev *zldev, u8 value)
{
	lockdep_assert_held(&zldev->mailbox_lock);
	return regmap_write(zldev->regmap, ZL_REG_DPLL_MB_SEM, value);
}

static inline __maybe_unused int
zl3073x_mb_poll_dpll_mb_sem(struct zl3073x_dev *zldev, u8 bitmask)
{
	unsigned int v;

	lockdep_assert_held(&zldev->mailbox_lock);
	return regmap_read_poll_timeout(zldev->regmap, ZL_REG_DPLL_MB_SEM, v,
					!(v & bitmask), ZL_POLL_SLEEP_US,
					ZL_POLL_TIMEOUT_US);
}

/*********************************
 * Register Page 13, Synth Mailbox
 *********************************/

/*
 * Register 'synth_mb_mask'
 * Page: 13, Offset: 0x02, Size: 16 bits
 */
#define ZL_REG_SYNTH_MB_MASK ZL_REG_ADDR(13, 0x02)

static inline __maybe_unused int
zl3073x_mb_read_synth_mb_mask(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_SYNTH_MB_MASK, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_synth_mb_mask(struct zl3073x_dev *zldev, u16 value)
{
	__be16 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be16(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_SYNTH_MB_MASK, &temp,
				 sizeof(temp));
}

/*
 * Register 'synth_mb_sem'
 * Page: 13, Offset: 0x04, Size: 8 bits
 */
#define ZL_REG_SYNTH_MB_SEM ZL_REG_ADDR(13, 0x04)
#define ZL_SYNTH_MB_SEM_WR  BIT(0)
#define ZL_SYNTH_MB_SEM_RD  BIT(1)

static inline __maybe_unused int
zl3073x_mb_read_synth_mb_sem(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_read(zldev->regmap, ZL_REG_SYNTH_MB_SEM, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_synth_mb_sem(struct zl3073x_dev *zldev, u8 value)
{
	lockdep_assert_held(&zldev->mailbox_lock);
	return regmap_write(zldev->regmap, ZL_REG_SYNTH_MB_SEM, value);
}

static inline __maybe_unused int
zl3073x_mb_poll_synth_mb_sem(struct zl3073x_dev *zldev, u8 bitmask)
{
	unsigned int v;

	lockdep_assert_held(&zldev->mailbox_lock);
	return regmap_read_poll_timeout(zldev->regmap, ZL_REG_SYNTH_MB_SEM, v,
					!(v & bitmask), ZL_POLL_SLEEP_US,
					ZL_POLL_TIMEOUT_US);
}

/*
 * Register 'synth_freq_base'
 * Page: 13, Offset: 0x06, Size: 16 bits
 */
#define ZL_REG_SYNTH_FREQ_BASE ZL_REG_ADDR(13, 0x06)

static inline __maybe_unused int
zl3073x_mb_read_synth_freq_base(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_SYNTH_FREQ_BASE, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

/*
 * Register 'synth_freq_mult'
 * Page: 13, Offset: 0x08, Size: 32 bits
 */
#define ZL_REG_SYNTH_FREQ_MULT ZL_REG_ADDR(13, 0x08)

static inline __maybe_unused int
zl3073x_mb_read_synth_freq_mult(struct zl3073x_dev *zldev, u32 *value)
{
	__be32 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_SYNTH_FREQ_MULT, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be32_to_cpu(temp);
	return rc;
}

/*
 * Register 'synth_freq_m'
 * Page: 13, Offset: 0x0c, Size: 16 bits
 */
#define ZL_REG_SYNTH_FREQ_M ZL_REG_ADDR(13, 0x0c)

static inline __maybe_unused int
zl3073x_mb_read_synth_freq_m(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_SYNTH_FREQ_M, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

/*
 * Register 'synth_freq_n'
 * Page: 13, Offset: 0x0e, Size: 16 bits
 */
#define ZL_REG_SYNTH_FREQ_N ZL_REG_ADDR(13, 0x0e)

static inline __maybe_unused int
zl3073x_mb_read_synth_freq_n(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_SYNTH_FREQ_N, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

/**********************************
 * Register Page 14, Output Mailbox
 **********************************/

/*
 * Register 'output_mb_mask'
 * Page: 14, Offset: 0x02, Size: 16 bits
 */
#define ZL_REG_OUTPUT_MB_MASK ZL_REG_ADDR(14, 0x02)

static inline __maybe_unused int
zl3073x_mb_read_output_mb_mask(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_OUTPUT_MB_MASK, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_output_mb_mask(struct zl3073x_dev *zldev, u16 value)
{
	__be16 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be16(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_OUTPUT_MB_MASK, &temp,
				 sizeof(temp));
}

/*
 * Register 'output_mb_sem'
 * Page: 14, Offset: 0x04, Size: 8 bits
 */
#define ZL_REG_OUTPUT_MB_SEM ZL_REG_ADDR(14, 0x04)
#define ZL_OUTPUT_MB_SEM_WR  BIT(0)
#define ZL_OUTPUT_MB_SEM_RD  BIT(1)

static inline __maybe_unused int
zl3073x_mb_read_output_mb_sem(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_read(zldev->regmap, ZL_REG_OUTPUT_MB_SEM, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_output_mb_sem(struct zl3073x_dev *zldev, u8 value)
{
	lockdep_assert_held(&zldev->mailbox_lock);
	return regmap_write(zldev->regmap, ZL_REG_OUTPUT_MB_SEM, value);
}

static inline __maybe_unused int
zl3073x_mb_poll_output_mb_sem(struct zl3073x_dev *zldev, u8 bitmask)
{
	unsigned int v;

	lockdep_assert_held(&zldev->mailbox_lock);
	return regmap_read_poll_timeout(zldev->regmap, ZL_REG_OUTPUT_MB_SEM, v,
					!(v & bitmask), ZL_POLL_SLEEP_US,
					ZL_POLL_TIMEOUT_US);
}

/*
 * Register 'output_mode'
 * Page: 14, Offset: 0x05, Size: 8 bits
 */
#define ZL_REG_OUTPUT_MODE	     ZL_REG_ADDR(14, 0x05)
#define ZL_OUTPUT_MODE_SIGNAL_FORMAT GENMASK(7, 4)

static inline __maybe_unused int
zl3073x_mb_read_output_mode(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_read(zldev->regmap, ZL_REG_OUTPUT_MODE, &v);
	*value = v;
	return rc;
}

#endif /* __LINUX_MFD_ZL3073X_REGS_H */
