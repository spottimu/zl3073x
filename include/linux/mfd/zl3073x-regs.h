/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LINUX_MFD_ZL3073X_REGS_H
#define __LINUX_MFD_ZL3073X_REGS_H

#include <linux/bitfield.h>
#include <linux/bits.h>
#include <linux/mfd/zl3073x.h>

/*
 * Register address structure:
 * ===========================
 *  25        19 18   16 15     7 6           0
 * +-------------------------------------------+
 * | max_offset | width |  page  | page_offset |
 * +-------------------------------------------+
 *
 * page_offset ... <0x00..0x7F>
 * page .......... HW page number
 * size .......... register byte size (1, 2, 4 or 6)
 * max_offset .... maximal offset for indexed registers
 *                 (for non-indexed regs max_offset == page_offset)
 */

#define ZL_REG_OFFSET_MASK	GENMASK(6, 0)
#define ZL_REG_PAGE_MASK	GENMASK(15, 7)
#define ZL_REG_SIZE_MASK	GENMASK(18, 16)
#define ZL_REG_MAX_OFFSET_MASK	GENMASK(25, 19)
#define ZL_REG_ADDR_MASK	GENMASK(15, 0)

#define ZL_REG_OFFSET(_reg)	FIELD_GET(ZL_REG_OFFSET_MASK, _reg)
#define ZL_REG_MAX_OFFSET(_reg)	FIELD_GET(ZL_REG_MAX_OFFSET_MASK, _reg)
#define ZL_REG_SIZE(_reg)	FIELD_GET(ZL_REG_SIZE_MASK, _reg)
#define ZL_REG_ADDR(_reg)	FIELD_GET(ZL_REG_ADDR_MASK, _reg)

/**
 * ZL_REG_IDX - define indexed register
 * @_idx: index of register to access
 * @_page: register page
 * @_offset: register offset in page
 * @_size: register byte size (1, 2, 4 or 6)
 * @_items: number of register indices
 * @_stride: stride between items in bytes
 *
 * All parameters except @_idx should be constant.
 */
#define ZL_REG_IDX(_idx, _page, _offset, _size, _items, _stride)	\
	(FIELD_PREP(ZL_REG_OFFSET_MASK,					\
		    (_offset) + (_idx) * (_stride))		|	\
	 FIELD_PREP_CONST(ZL_REG_PAGE_MASK, _page)		|	\
	 FIELD_PREP_CONST(ZL_REG_SIZE_MASK, _size)		|	\
	 FIELD_PREP_CONST(ZL_REG_MAX_OFFSET_MASK,			\
			  (_offset) + ((_items) - 1) * (_stride)))

/**
 * ZL_REG - define simple (non-indexed) register
 * @_page: register page
 * @_offset: register offset in page
 * @_size: register byte size (1, 2, 4 or 6)
 *
 * All parameters should be constant.
 */
#define ZL_REG(_page, _offset, _size)					\
	ZL_REG_IDX(0, _page, _offset, _size, 1, 0)

/*************************
 * Register Page 2, Status
 *************************/

#define ZL_REG_REF_MON_STATUS(_idx)					\
	ZL_REG_IDX(_idx, 2, 0x02, 1, ZL3073X_NUM_INPUT_PINS, 1)
#define ZL_REF_MON_STATUS_OK			0 /* all bits zeroed */

#define ZL_REG_DPLL_MON_STATUS(_idx)					\
	ZL_REG_IDX(_idx, 2, 0x10, 1, ZL3073X_MAX_CHANNELS, 1)
#define ZL_DPLL_MON_STATUS_LOCK			BIT(0)
#define ZL_DPLL_MON_STATUS_HO			BIT(1)
#define ZL_DPLL_MON_STATUS_HO_READY		BIT(2)

#define ZL_REG_DPLL_REFSEL_STATUS(_idx)					\
	ZL_REG_IDX(_idx, 2, 0x30, 1, ZL3073X_MAX_CHANNELS, 1)
#define ZL_DPLL_REFSEL_STATUS_REFSEL		GENMASK(3, 0)
#define ZL_DPLL_REFSEL_STATUS_STATE		GENMASK(6, 4)
#define ZL_DPLL_REFSEL_STATUS_STATE_FREERUN	0
#define ZL_DPLL_REFSEL_STATUS_STATE_HOLDOVER	1
#define ZL_DPLL_REFSEL_STATUS_STATE_FASTLOCK	2
#define ZL_DPLL_REFSEL_STATUS_STATE_ACQUIRING	3
#define ZL_DPLL_REFSEL_STATUS_STATE_LOCK	4

/***********************
 * Register Page 5, DPLL
 ***********************/

#define ZL_REG_DPLL_MODE_REFSEL(_idx)					\
	ZL_REG_IDX(_idx, 5, 0x04, 1, ZL3073X_MAX_CHANNELS, 4)
#define ZL_DPLL_MODE_REFSEL_MODE		GENMASK(2, 0)
#define ZL_DPLL_MODE_REFSEL_MODE_FREERUN	0
#define ZL_DPLL_MODE_REFSEL_MODE_HOLDOVER	1
#define ZL_DPLL_MODE_REFSEL_MODE_REFLOCK	2
#define ZL_DPLL_MODE_REFSEL_MODE_AUTO		3
#define ZL_DPLL_MODE_REFSEL_MODE_NCO		4
#define ZL_DPLL_MODE_REFSEL_REF			GENMASK(7, 4)

/***********************************
 * Register Page 9, Synth and Output
 ***********************************/

#define ZL_REG_SYNTH_PHASE_SHIFT_CTRL		ZL_REG(9, 0x1e, 1)
#define ZL_REG_SYNTH_PHASE_SHIFT_MASK		ZL_REG(9, 0x1f, 1)
#define ZL_REG_SYNTH_PHASE_SHIFT_INTVL		ZL_REG(9, 0x20, 1)
#define ZL_REG_SYNTH_PHASE_SHIFT_DATA		ZL_REG(9, 0x21, 2)

#endif /* __LINUX_MFD_ZL3073X_REGS_H */
