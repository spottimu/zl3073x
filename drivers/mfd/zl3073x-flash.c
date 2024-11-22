// SPDX-License-Identifier: GPL-2.0-or-later

#include <linux/types.h>
#include <net/devlink.h>
#include "zl3073x.h"

static void zl3073x_flash_notify(struct zl3073x_dev *zldev, const char *msg,
				 const char *component, u32 done, u32 total)
{
	struct devlink *devlink = priv_to_devlink(zldev);

	devlink_flash_update_status_notify(devlink, msg, component, done,
					   total);
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
	struct zl3073x_dev *zldev = devlink_priv(devlink);

	zl3073x_flash_notify(zldev, "Preparing to flash", params->component,
			     0, 0);

	zl3073x_flash_notify(zldev, "Flashing done", params->component, 0, 0);

	return 0;
}
