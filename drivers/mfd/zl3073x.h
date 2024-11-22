/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef __ZL3073X_CORE_H
#define __ZL3073X_CORE_H

#include <linux/mfd/zl3073x.h>
#include <net/devlink.h>

struct zl3073x_dev *zl3073x_dev_alloc(struct device *dev);
int zl3073x_dev_init(struct zl3073x_dev *zldev, u8 dev_id);
void zl3073x_dev_exit(struct zl3073x_dev *zldev);
const struct regmap_config *zl3073x_get_regmap_config(void);

/*
 * Misc functions
 */
int zl3073x_flash_update(struct devlink *devlink,
			 struct devlink_flash_update_params *params,
			 struct netlink_ext_ack *extack);

#endif /* __ZL3073X_CORE_H */
