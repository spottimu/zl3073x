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

/*************************
 * Register Page 2, Status
 *************************/

/*
 * Register array 'ref_mon_status'
 * Page: 2, Offset: 0x02, Size: 8 bits, Items: 10, Stride: 1
 */
#define ZL_REG_REF_MON_STATUS	     ZL_REG_ADDR(2, 0x02)
#define ZL_REG_REF_MON_STATUS_ITEMS  ZL3073X_NUM_INPUT_PINS
#define ZL_REG_REF_MON_STATUS_STRIDE 1
#define ZL_REF_MON_STATUS_OK	     0 /* all bits zeroed */

static inline __maybe_unused int
zl3073x_read_ref_mon_status(struct zl3073x_dev *zldev, unsigned int idx,
			    u8 *value)
{
	unsigned int addr, v;
	int rc;

	if (idx >= ZL_REG_REF_MON_STATUS_ITEMS)
		return -EINVAL;

	addr = ZL_REG_REF_MON_STATUS + idx * ZL_REG_REF_MON_STATUS_STRIDE;
	rc = regmap_read(zldev->regmap, addr, &v);
	*value = v;
	return rc;
}

/*
 * Register array 'dpll_mon_status'
 * Page: 2, Offset: 0x10, Size: 8 bits, Items: 5, Stride: 1
 */
#define ZL_REG_DPLL_MON_STATUS	      ZL_REG_ADDR(2, 0x10)
#define ZL_REG_DPLL_MON_STATUS_ITEMS  ZL3073X_MAX_CHANNELS
#define ZL_REG_DPLL_MON_STATUS_STRIDE 1
#define ZL_DPLL_MON_STATUS_LOCK	      BIT(0)
#define ZL_DPLL_MON_STATUS_HO	      BIT(1)
#define ZL_DPLL_MON_STATUS_HO_READY   BIT(2)

static inline __maybe_unused int
zl3073x_read_dpll_mon_status(struct zl3073x_dev *zldev, unsigned int idx,
			     u8 *value)
{
	unsigned int addr, v;
	int rc;

	if (idx >= ZL_REG_DPLL_MON_STATUS_ITEMS)
		return -EINVAL;

	addr = ZL_REG_DPLL_MON_STATUS + idx * ZL_REG_DPLL_MON_STATUS_STRIDE;
	rc = regmap_read(zldev->regmap, addr, &v);
	*value = v;
	return rc;
}

/*
 * Register array 'dpll_refsel_status'
 * Page: 2, Offset: 0x30, Size: 8 bits, Items: 5, Stride: 1
 */
#define ZL_REG_DPLL_REFSEL_STATUS	      ZL_REG_ADDR(2, 0x30)
#define ZL_REG_DPLL_REFSEL_STATUS_ITEMS	      ZL3073X_MAX_CHANNELS
#define ZL_REG_DPLL_REFSEL_STATUS_STRIDE      1
#define ZL_DPLL_REFSEL_STATUS_REFSEL	      GENMASK(3, 0)
#define ZL_DPLL_REFSEL_STATUS_STATE	      GENMASK(6, 4)
#define ZL_DPLL_REFSEL_STATUS_STATE_FREERUN   0
#define ZL_DPLL_REFSEL_STATUS_STATE_HOLDOVER  1
#define ZL_DPLL_REFSEL_STATUS_STATE_FASTLOCK  2
#define ZL_DPLL_REFSEL_STATUS_STATE_ACQUIRING 3
#define ZL_DPLL_REFSEL_STATUS_STATE_LOCK      4

static inline __maybe_unused int
zl3073x_read_dpll_refsel_status(struct zl3073x_dev *zldev, unsigned int idx,
				u8 *value)
{
	unsigned int addr, v;
	int rc;

	if (idx >= ZL_REG_DPLL_REFSEL_STATUS_ITEMS)
		return -EINVAL;

	addr = ZL_REG_DPLL_REFSEL_STATUS +
	       idx * ZL_REG_DPLL_REFSEL_STATUS_STRIDE;
	rc = regmap_read(zldev->regmap, addr, &v);
	*value = v;
	return rc;
}

/**********************
 * Register Page 4, Ref
 **********************/

/*
 * Register 'ref_phase_err_read_rqst'
 * Page: 4, Offset: 0x0f, Size: 8 bits
 */
#define ZL_REG_REF_PHASE_ERR_READ_RQST ZL_REG_ADDR(4, 0x0f)
#define ZL_REF_PHASE_ERR_READ_RQST_RD  BIT(0)

static inline __maybe_unused int
zl3073x_read_ref_phase_err_read_rqst(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	rc = regmap_read(zldev->regmap, ZL_REG_REF_PHASE_ERR_READ_RQST, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_write_ref_phase_err_read_rqst(struct zl3073x_dev *zldev, u8 value)
{
	return regmap_write(zldev->regmap, ZL_REG_REF_PHASE_ERR_READ_RQST,
			    value);
}

static inline __maybe_unused int
zl3073x_poll_ref_phase_err_read_rqst(struct zl3073x_dev *zldev, u8 bitmask)
{
	unsigned int v;

	return regmap_read_poll_timeout(zldev->regmap,
					ZL_REG_REF_PHASE_ERR_READ_RQST, v,
					!(v & bitmask), ZL_POLL_SLEEP_US,
					ZL_POLL_TIMEOUT_US);
}

/*
 * Register array 'ref_phase'
 * Page: 4, Offset: 0x20, Size: 48 bits, Items: 10, Stride: 6
 */
#define ZL_REG_REF_PHASE	ZL_REG_ADDR(4, 0x20)
#define ZL_REG_REF_PHASE_LEN	6
#define ZL_REG_REF_PHASE_ITEMS	ZL3073X_NUM_INPUT_PINS
#define ZL_REG_REF_PHASE_STRIDE 6

static inline __maybe_unused int
zl3073x_read_ref_phase(struct zl3073x_dev *zldev, unsigned int idx, u64 *value)
{
	u8 buf[ZL_REG_REF_PHASE_LEN];
	unsigned int addr;
	int rc;

	if (idx >= ZL_REG_REF_PHASE_ITEMS)
		return -EINVAL;

	addr = ZL_REG_REF_PHASE + idx * ZL_REG_REF_PHASE_STRIDE;
	rc = regmap_bulk_read(zldev->regmap, addr, buf, sizeof(buf));
	if (rc)
		return rc;

	*value = get_unaligned_be64(buf);
	return rc;
}

/***********************
 * Register Page 5, DPLL
 ***********************/

/*
 * Register array 'dpll_mode_refsel'
 * Page: 5, Offset: 0x04, Size: 8 bits, Items: 5, Stride: 4
 */
#define ZL_REG_DPLL_MODE_REFSEL		  ZL_REG_ADDR(5, 0x04)
#define ZL_REG_DPLL_MODE_REFSEL_ITEMS	  ZL3073X_MAX_CHANNELS
#define ZL_REG_DPLL_MODE_REFSEL_STRIDE	  4
#define ZL_DPLL_MODE_REFSEL_MODE	  GENMASK(2, 0)
#define ZL_DPLL_MODE_REFSEL_MODE_FREERUN  0
#define ZL_DPLL_MODE_REFSEL_MODE_HOLDOVER 1
#define ZL_DPLL_MODE_REFSEL_MODE_REFLOCK  2
#define ZL_DPLL_MODE_REFSEL_MODE_AUTO	  3
#define ZL_DPLL_MODE_REFSEL_MODE_NCO	  4
#define ZL_DPLL_MODE_REFSEL_REF		  GENMASK(7, 4)

static inline __maybe_unused int
zl3073x_read_dpll_mode_refsel(struct zl3073x_dev *zldev, unsigned int idx,
			      u8 *value)
{
	unsigned int addr, v;
	int rc;

	if (idx >= ZL_REG_DPLL_MODE_REFSEL_ITEMS)
		return -EINVAL;

	addr = ZL_REG_DPLL_MODE_REFSEL + idx * ZL_REG_DPLL_MODE_REFSEL_STRIDE;
	rc = regmap_read(zldev->regmap, addr, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_write_dpll_mode_refsel(struct zl3073x_dev *zldev, unsigned int idx,
			       u8 value)
{
	unsigned int addr;

	if (idx >= ZL_REG_DPLL_MODE_REFSEL_ITEMS)
		return -EINVAL;

	addr = ZL_REG_DPLL_MODE_REFSEL + idx * ZL_REG_DPLL_MODE_REFSEL_STRIDE;
	return regmap_write(zldev->regmap, addr, value);
}

/*
 * Register 'dpll_meas_ctrl'
 * Page: 5, Offset: 0x50, Size: 8 bits
 */
#define ZL_REG_DPLL_MEAS_CTRL	     ZL_REG_ADDR(5, 0x50)
#define ZL_DPLL_MEAS_CTRL_EN	     BIT(0)
#define ZL_DPLL_MEAS_CTRL_AVG_FACTOR GENMASK(7, 4)

static inline __maybe_unused int
zl3073x_read_dpll_meas_ctrl(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	rc = regmap_read(zldev->regmap, ZL_REG_DPLL_MEAS_CTRL, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_write_dpll_meas_ctrl(struct zl3073x_dev *zldev, u8 value)
{
	return regmap_write(zldev->regmap, ZL_REG_DPLL_MEAS_CTRL, value);
}

/*
 * Register 'dpll_meas_idx'
 * Page: 5, Offset: 0x51, Size: 8 bits
 */
#define ZL_REG_DPLL_MEAS_IDX ZL_REG_ADDR(5, 0x51)
#define ZL_DPLL_MEAS_IDX     GENMASK(2, 0)

static inline __maybe_unused int
zl3073x_read_dpll_meas_idx(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	rc = regmap_read(zldev->regmap, ZL_REG_DPLL_MEAS_IDX, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_write_dpll_meas_idx(struct zl3073x_dev *zldev, u8 value)
{
	return regmap_write(zldev->regmap, ZL_REG_DPLL_MEAS_IDX, value);
}

/***********************************
 * Register Page 9, Synth and Output
 ***********************************/

/*
 * Register array 'synth_ctrl'
 * Page: 9, Offset: 0x00, Size: 8 bits, Items: 5, Stride: 1
 */
#define ZL_REG_SYNTH_CTRL	 ZL_REG_ADDR(9, 0x00)
#define ZL_REG_SYNTH_CTRL_ITEMS	 ZL3073X_NUM_SYNTHS
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
#define ZL_REG_OUTPUT_CTRL_ITEMS  ZL3073X_NUM_OUTPUTS
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

/*
 * Register 'synth_phase_shift_ctrl'
 * Page: 9, Offset: 0x1e, Size: 8 bits
 */
#define ZL_REG_SYNTH_PHASE_SHIFT_CTRL ZL_REG_ADDR(9, 0x1e)

static inline __maybe_unused int
zl3073x_read_synth_phase_shift_ctrl(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	rc = regmap_read(zldev->regmap, ZL_REG_SYNTH_PHASE_SHIFT_CTRL, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_write_synth_phase_shift_ctrl(struct zl3073x_dev *zldev, u8 value)
{
	return regmap_write(zldev->regmap, ZL_REG_SYNTH_PHASE_SHIFT_CTRL,
			    value);
}

/*
 * Register 'synth_phase_shift_mask'
 * Page: 9, Offset: 0x1f, Size: 8 bits
 */
#define ZL_REG_SYNTH_PHASE_SHIFT_MASK ZL_REG_ADDR(9, 0x1f)

static inline __maybe_unused int
zl3073x_read_synth_phase_shift_mask(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	rc = regmap_read(zldev->regmap, ZL_REG_SYNTH_PHASE_SHIFT_MASK, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_write_synth_phase_shift_mask(struct zl3073x_dev *zldev, u8 value)
{
	return regmap_write(zldev->regmap, ZL_REG_SYNTH_PHASE_SHIFT_MASK,
			    value);
}

/*
 * Register 'synth_phase_shift_intvl'
 * Page: 9, Offset: 0x20, Size: 8 bits
 */
#define ZL_REG_SYNTH_PHASE_SHIFT_INTVL ZL_REG_ADDR(9, 0x20)

static inline __maybe_unused int
zl3073x_read_synth_phase_shift_intvl(struct zl3073x_dev *zldev, u8 *value)
{
	unsigned int v;
	int rc;

	rc = regmap_read(zldev->regmap, ZL_REG_SYNTH_PHASE_SHIFT_INTVL, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_write_synth_phase_shift_intvl(struct zl3073x_dev *zldev, u8 value)
{
	return regmap_write(zldev->regmap, ZL_REG_SYNTH_PHASE_SHIFT_INTVL,
			    value);
}

/*
 * Register 'synth_phase_shift_data'
 * Page: 9, Offset: 0x21, Size: 16 bits
 */
#define ZL_REG_SYNTH_PHASE_SHIFT_DATA ZL_REG_ADDR(9, 0x21)

static inline __maybe_unused int
zl3073x_read_synth_phase_shift_data(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	rc = regmap_bulk_read(zldev->regmap, ZL_REG_SYNTH_PHASE_SHIFT_DATA,
			      &temp, sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_write_synth_phase_shift_data(struct zl3073x_dev *zldev, u16 value)
{
	__be16 temp;
	temp = cpu_to_be16(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_SYNTH_PHASE_SHIFT_DATA,
				 &temp, sizeof(temp));
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
 * Register 'ref_freq_base'
 * Page: 10, Offset: 0x05, Size: 16 bits
 */
#define ZL_REG_REF_FREQ_BASE ZL_REG_ADDR(10, 0x05)

static inline __maybe_unused int
zl3073x_mb_read_ref_freq_base(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_REF_FREQ_BASE, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_ref_freq_base(struct zl3073x_dev *zldev, u16 value)
{
	__be16 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be16(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_REF_FREQ_BASE, &temp,
				 sizeof(temp));
}

/*
 * Register 'ref_freq_mult'
 * Page: 10, Offset: 0x07, Size: 16 bits
 */
#define ZL_REG_REF_FREQ_MULT ZL_REG_ADDR(10, 0x07)

static inline __maybe_unused int
zl3073x_mb_read_ref_freq_mult(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_REF_FREQ_MULT, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_ref_freq_mult(struct zl3073x_dev *zldev, u16 value)
{
	__be16 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be16(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_REF_FREQ_MULT, &temp,
				 sizeof(temp));
}

/*
 * Register 'ref_ratio_m'
 * Page: 10, Offset: 0x09, Size: 16 bits
 */
#define ZL_REG_REF_RATIO_M ZL_REG_ADDR(10, 0x09)

static inline __maybe_unused int
zl3073x_mb_read_ref_ratio_m(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_REF_RATIO_M, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_ref_ratio_m(struct zl3073x_dev *zldev, u16 value)
{
	__be16 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be16(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_REF_RATIO_M, &temp,
				 sizeof(temp));
}

/*
 * Register 'ref_ratio_n'
 * Page: 10, Offset: 0x0b, Size: 16 bits
 */
#define ZL_REG_REF_RATIO_N ZL_REG_ADDR(10, 0x0b)

static inline __maybe_unused int
zl3073x_mb_read_ref_ratio_n(struct zl3073x_dev *zldev, u16 *value)
{
	__be16 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_REF_RATIO_N, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be16_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_ref_ratio_n(struct zl3073x_dev *zldev, u16 value)
{
	__be16 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be16(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_REF_RATIO_N, &temp,
				 sizeof(temp));
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

/*
 * Register array 'dpll_ref_prio'
 * Page: 12, Offset: 0x52, Size: 8 bits, Items: 5, Stride: 1
 */
#define ZL_REG_DPLL_REF_PRIO	    ZL_REG_ADDR(12, 0x52)
#define ZL_REG_DPLL_REF_PRIO_ITEMS  ZL3073X_NUM_INPUT_PINS / 2
#define ZL_REG_DPLL_REF_PRIO_STRIDE 1
#define ZL_DPLL_REF_PRIO_REF_P	    GENMASK(3, 0)
#define ZL_DPLL_REF_PRIO_REF_N	    GENMASK(7, 4)
#define ZL_DPLL_REF_PRIO_MAX	    14
#define ZL_DPLL_REF_PRIO_NONE	    15 /* non-selectable */

static inline __maybe_unused int
zl3073x_mb_read_dpll_ref_prio(struct zl3073x_dev *zldev, unsigned int idx,
			      u8 *value)
{
	unsigned int addr, v;
	int rc;

	if (idx >= ZL_REG_DPLL_REF_PRIO_ITEMS)
		return -EINVAL;

	addr = ZL_REG_DPLL_REF_PRIO + idx * ZL_REG_DPLL_REF_PRIO_STRIDE;
	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_read(zldev->regmap, addr, &v);
	*value = v;
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_dpll_ref_prio(struct zl3073x_dev *zldev, unsigned int idx,
			       u8 value)
{
	unsigned int addr;

	if (idx >= ZL_REG_DPLL_REF_PRIO_ITEMS)
		return -EINVAL;

	addr = ZL_REG_DPLL_REF_PRIO + idx * ZL_REG_DPLL_REF_PRIO_STRIDE;
	lockdep_assert_held(&zldev->mailbox_lock);
	return regmap_write(zldev->regmap, addr, value);
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

/*
 * Register 'output_div'
 * Page: 14, Offset: 0x0c, Size: 32 bits
 */
#define ZL_REG_OUTPUT_DIV ZL_REG_ADDR(14, 0x0c)

static inline __maybe_unused int
zl3073x_mb_read_output_div(struct zl3073x_dev *zldev, u32 *value)
{
	__be32 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_OUTPUT_DIV, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be32_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_output_div(struct zl3073x_dev *zldev, u32 value)
{
	__be32 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be32(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_OUTPUT_DIV, &temp,
				 sizeof(temp));
}

/*
 * Register 'output_width'
 * Page: 14, Offset: 0x10, Size: 32 bits
 */
#define ZL_REG_OUTPUT_WIDTH ZL_REG_ADDR(14, 0x10)

static inline __maybe_unused int
zl3073x_mb_read_output_width(struct zl3073x_dev *zldev, u32 *value)
{
	__be32 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_OUTPUT_WIDTH, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be32_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_output_width(struct zl3073x_dev *zldev, u32 value)
{
	__be32 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be32(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_OUTPUT_WIDTH, &temp,
				 sizeof(temp));
}

/*
 * Register 'output_ndiv_period'
 * Page: 14, Offset: 0x14, Size: 32 bits
 */
#define ZL_REG_OUTPUT_NDIV_PERIOD ZL_REG_ADDR(14, 0x14)

static inline __maybe_unused int
zl3073x_mb_read_output_ndiv_period(struct zl3073x_dev *zldev, u32 *value)
{
	__be32 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_OUTPUT_NDIV_PERIOD, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be32_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_output_ndiv_period(struct zl3073x_dev *zldev, u32 value)
{
	__be32 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be32(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_OUTPUT_NDIV_PERIOD,
				 &temp, sizeof(temp));
}

/*
 * Register 'output_ndiv_width'
 * Page: 14, Offset: 0x18, Size: 32 bits
 */
#define ZL_REG_OUTPUT_NDIV_WIDTH ZL_REG_ADDR(14, 0x18)

static inline __maybe_unused int
zl3073x_mb_read_output_ndiv_width(struct zl3073x_dev *zldev, u32 *value)
{
	__be32 temp;
	int rc;

	lockdep_assert_held(&zldev->mailbox_lock);
	rc = regmap_bulk_read(zldev->regmap, ZL_REG_OUTPUT_NDIV_WIDTH, &temp,
			      sizeof(temp));
	if (rc)
		return rc;

	*value = be32_to_cpu(temp);
	return rc;
}

static inline __maybe_unused int
zl3073x_mb_write_output_ndiv_width(struct zl3073x_dev *zldev, u32 value)
{
	__be32 temp;

	lockdep_assert_held(&zldev->mailbox_lock);
	temp = cpu_to_be32(value);
	return regmap_bulk_write(zldev->regmap, ZL_REG_OUTPUT_NDIV_WIDTH, &temp,
				 sizeof(temp));
}

#endif /* __LINUX_MFD_ZL3073X_REGS_H */
