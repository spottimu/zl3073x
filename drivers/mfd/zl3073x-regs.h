/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __ZL3073X_REGS_H
#define __ZL3073X_REGS_H

#include <linux/mfd/zl3073x-regs.h>

/**************************
 * Register Page 0, General
 **************************/

#define ZL_REG_ID				ZL_REG(0, 0x01, 2)
#define ZL_REG_REVISION				ZL_REG(0, 0x03, 2)
#define ZL_REG_FW_VER				ZL_REG(0, 0x05, 2)
#define ZL_REG_CUSTOM_CONFIG_VER		ZL_REG(0, 0x07, 4)

/**********************
 * Register Page 4, Ref
 **********************/

#define ZL_REG_REF_PHASE_ERR_READ_RQST		ZL_REG(4, 0x0f, 1)
#define ZL_REF_PHASE_ERR_READ_RQST_RD		BIT(0)

#define ZL_REG_REF_PHASE(_idx)						\
	ZL_REG_IDX(_idx, 4, 0x20, 6, ZL3073X_NUM_INPUTS, 6)

/***********************
 * Register Page 5, DPLL
 * *********************/

#define ZL_REG_DPLL_MEAS_CTRL			ZL_REG(5, 0x50, 1)
#define ZL_DPLL_MEAS_CTRL_EN			BIT(0)

#define ZL_REG_DPLL_MEAS_IDX			ZL_REG(5, 0x51, 1)
#define ZL_DPLL_MEAS_IDX			GENMASK(2, 0)

/***********************************
 * Register Page 9, Synth and Output
 ***********************************/

#define ZL_REG_SYNTH_CTRL(_idx)						\
	ZL_REG_IDX(_idx, 9, 0x00, 1, ZL3073X_NUM_SYNTHS, 1)
#define ZL_SYNTH_CTRL_EN			BIT(0)
#define ZL_SYNTH_CTRL_DPLL_SEL			GENMASK(6, 4)

#define ZL_REG_OUTPUT_CTRL(_idx)					\
	ZL_REG_IDX(_idx, 9, 0x28, 1, ZL3073X_NUM_OUTPUTS, 1)
#define ZL_OUTPUT_CTRL_EN			BIT(0)
#define ZL_OUTPUT_CTRL_STOP			BIT(1)
#define ZL_OUTPUT_CTRL_STOP_HIGH		BIT(2)
#define ZL_OUTPUT_CTRL_STOP_HZ			BIT(3)
#define ZL_OUTPUT_CTRL_SYNTH_SEL		GENMASK(6, 4)

/*******************************
 * Register Page 10, Ref Mailbox
 *******************************/

#define ZL_REG_REF_MB_MASK			ZL_REG(10, 0x02, 2)

#define ZL_REG_REF_MB_SEM			ZL_REG(10, 0x04, 1)
#define ZL_REF_MB_SEM_WR			BIT(0)
#define ZL_REF_MB_SEM_RD			BIT(1)

#define ZL_REG_REF_FREQ_BASE			ZL_REG(10, 0x05, 2)
#define ZL_REG_REF_FREQ_MULT			ZL_REG(10, 0x07, 2)
#define ZL_REG_REF_RATIO_M			ZL_REG(10, 0x09, 2)
#define ZL_REG_REF_RATIO_N			ZL_REG(10, 0x0b, 2)
#define ZL_REG_REF_CONFIG			ZL_REG(10, 0x0d, 1)
#define ZL_REG_REF_PHASE_OFFSET_COMP		ZL_REG(10, 0x28, 6)
#define ZL_REG_REF_SYNC_CTRL			ZL_REG(10, 0x2e, 1)
#define ZL_REG_REF_ESYNC_DIV			ZL_REG(10, 0x30, 4)

/********************************
 * Register Page 12, DPLL Mailbox
 ********************************/

#define ZL_REG_DPLL_MB_MASK			ZL_REG(12, 0x02, 2)

#define ZL_REG_DPLL_MB_SEM			ZL_REG(12, 0x04, 1)
#define ZL_DPLL_MB_SEM_WR			BIT(0)
#define ZL_DPLL_MB_SEM_RD			BIT(1)

#define ZL_REG_DPLL_REF_PRIO(_idx)					\
	ZL_REG_IDX(_idx, 12, 0x52, 1, ZL3073X_NUM_INPUTS, 1)

/*********************************
 * Register Page 13, Synth Mailbox
 *********************************/

#define ZL_REG_SYNTH_MB_MASK			ZL_REG(13, 0x02, 2)

#define ZL_REG_SYNTH_MB_SEM			ZL_REG(13, 0x04, 1)
#define ZL_SYNTH_MB_SEM_WR			BIT(0)
#define ZL_SYNTH_MB_SEM_RD			BIT(1)

#define ZL_REG_SYNTH_FREQ_BASE			ZL_REG(13, 0x06, 2)
#define ZL_REG_SYNTH_FREQ_MULT			ZL_REG(13, 0x08, 4)
#define ZL_REG_SYNTH_FREQ_M			ZL_REG(13, 0x0c, 2)
#define ZL_REG_SYNTH_FREQ_N			ZL_REG(13, 0x0e, 2)

/**********************************
 * Register Page 14, Output Mailbox
 **********************************/

#define ZL_REG_OUTPUT_MB_MASK			ZL_REG(14, 0x02, 2)

#define ZL_REG_OUTPUT_MB_SEM			ZL_REG(14, 0x04, 1)
#define ZL_OUTPUT_MB_SEM_WR			BIT(0)
#define ZL_OUTPUT_MB_SEM_RD			BIT(1)

#define ZL_REG_OUTPUT_MODE			ZL_REG(14, 0x05, 1)
#define ZL_REG_OUTPUT_DIV			ZL_REG(14, 0x0c, 4)
#define ZL_REG_OUTPUT_WIDTH			ZL_REG(14, 0x10, 4)
#define ZL_REG_OUTPUT_ESYNC_PERIOD		ZL_REG(14, 0x14, 4)
#define ZL_REG_OUTPUT_ESYNC_WIDTH		ZL_REG(14, 0x18, 4)
#define ZL_REG_OUTPUT_PHASE_COMP		ZL_REG(14, 0x20, 4)

#endif /* __ZL3073X_REGS_H */
