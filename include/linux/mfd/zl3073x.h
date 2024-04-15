/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LINUX_MFD_ZL3073X_H
#define __LINUX_MFD_ZL3073X_H

#include <linux/mutex.h>

struct device;
struct regmap;

/**
 * struct zl3073x_dev - zl3073x device
 * @dev: pointer to device
 * @regmap: regmap to access device registers
 * @mailbox_lock: mutex protecting an access to mailbox registers
 */
struct zl3073x_dev {
	struct device		*dev;
	struct regmap		*regmap;
	struct mutex		mailbox_lock;
};

/**
 * zl3073x_mailbox_lock - Lock the device mailbox registers
 * @zldev: zl3073x device pointer
 *
 * Caller has to held this lock when it needs to access device mailbox
 * registers.
 */
static inline void zl3073x_mailbox_lock(struct zl3073x_dev *zldev)
{
	mutex_lock(&zldev->mailbox_lock);
}

/**
 * zl3073x_mailbox_unlock - Unlock the device mailbox registers
 * @zldev: zl3073x device pointer
 *
 * Caller has to unlock this lock when it finishes accessing device mailbox
 * registers.
 */
static inline void zl3073x_mailbox_unlock(struct zl3073x_dev *zldev)
{
	mutex_unlock(&zldev->mailbox_lock);
}

DEFINE_GUARD(zl3073x_mailbox, struct zl3073x_dev *, zl3073x_mailbox_lock(_T),
	     zl3073x_mailbox_unlock(_T));

#endif /* __LINUX_MFD_ZL3073X_H */
