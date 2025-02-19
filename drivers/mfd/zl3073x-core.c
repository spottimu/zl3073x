// SPDX-License-Identifier: GPL-2.0-only

#include <linux/array_size.h>
#include <linux/bits.h>
#include <linux/dev_printk.h>
#include <linux/device.h>
#include <linux/export.h>
#include <linux/mfd/zl3073x.h>
#include <linux/mfd/zl3073x_regs.h>
#include <linux/module.h>
#include <linux/netlink.h>
#include <linux/regmap.h>
#include <linux/sprintf.h>
#include <net/devlink.h>
#include "zl3073x.h"

/* Chip IDs for zl30731 */
static const u16 zl30731_ids[] = {
	0x0E93,
	0x1E93,
	0x2E93,
};

/* Chip IDs for zl30732 */
static const u16 zl30732_ids[] = {
	0x0E30,
	0x0E94,
	0x1E94,
	0x1F60,
	0x2E94,
	0x3FC4,
};

/* Chip IDs for zl30733 */
static const u16 zl30733_ids[] = {
	0x0E95,
	0x1E95,
	0x2E95,
};

/* Chip IDs for zl30734 */
static const u16 zl30734_ids[] = {
	0x0E96,
	0x1E96,
	0x2E96,
};

/* Chip IDs for zl30735 */
static const u16 zl30735_ids[] = {
	0x0E97,
	0x1E97,
	0x2E97,
};

const struct zl3073x_chip_info zl3073x_chip_info[] = {
	[ZL30731] = {
		.ids = zl30731_ids,
		.num_ids = ARRAY_SIZE(zl30731_ids),
		.num_channels = 1,
	},
	[ZL30732] = {
		.ids = zl30732_ids,
		.num_ids = ARRAY_SIZE(zl30732_ids),
		.num_channels = 2,
	},
	[ZL30733] = {
		.ids = zl30733_ids,
		.num_ids = ARRAY_SIZE(zl30733_ids),
		.num_channels = 3,
	},
	[ZL30734] = {
		.ids = zl30734_ids,
		.num_ids = ARRAY_SIZE(zl30734_ids),
		.num_channels = 4,
	},
	[ZL30735] = {
		.ids = zl30735_ids,
		.num_ids = ARRAY_SIZE(zl30735_ids),
		.num_channels = 5,
	},
};
EXPORT_SYMBOL_NS_GPL(zl3073x_chip_info, "ZL3073X");

#define ZL_NUM_PAGES		15
#define ZL_NUM_SIMPLE_PAGES	10
#define ZL_PAGE_SEL		0x7F
#define ZL_PAGE_SEL_MASK	GENMASK(3, 0)
#define ZL_NUM_REGS		(ZL_NUM_PAGES * ZL_PAGE_SIZE)

/* Regmap range configuration */
static const struct regmap_range_cfg zl3073x_regmap_range = {
	.range_min	= ZL_RANGE_OFF,
	.range_max	= ZL_RANGE_OFF + ZL_NUM_REGS - 1,
	.selector_reg	= ZL_PAGE_SEL,
	.selector_mask	= ZL_PAGE_SEL_MASK,
	.selector_shift	= 0,
	.window_start	= 0,
	.window_len	= ZL_PAGE_SIZE,
};

static bool
zl3073x_is_volatile_reg(struct device *dev, unsigned int reg)
{
	/* Only page selector is non-volatile */
	return (reg != ZL_PAGE_SEL);
}

static const struct regmap_config zl3073x_regmap_config = {
	.reg_bits	= 8,
	.val_bits	= 8,
	.max_register	= ZL_RANGE_OFF + ZL_NUM_REGS - 1,
	.ranges		= &zl3073x_regmap_range,
	.num_ranges	= 1,
	.cache_type	= REGCACHE_RBTREE,
	.volatile_reg	= zl3073x_is_volatile_reg,
};

/**
 * zl3073x_mb_dpll_read - read given DPLL configuration to mailbox
 * @zldev: pointer to device structure
 * @index: DPLL index
 *
 * Reads configuration of given DPLL into DPLL mailbox.
 *
 * Context: Process context. Expects zldev->regmap_lock to be held by caller.
 * Return: 0 on success, <0 on error
 */
int zl3073x_mb_dpll_read(struct zl3073x_dev *zldev, u8 index)
{
	int rc;

	/* Select requested index in mask register */
	rc = zl3073x_mb_write_dpll_mb_mask(zldev, BIT(index));
	if (rc)
		return rc;

	/* Perform read operation */
	rc = zl3073x_mb_write_dpll_mb_sem(zldev, ZL_DPLL_MB_SEM_RD);
	if (rc)
		return rc;

	/* Wait for the command to actually finish */
	return zl3073x_mb_poll_dpll_mb_sem(zldev, ZL_DPLL_MB_SEM_RD);
}
EXPORT_SYMBOL_GPL(zl3073x_mb_dpll_read);

/**
 * zl3073x_mb_dpll_write - write given DPLL configuration from mailbox
 * @zldev: pointer to device structure
 * @index: DPLL index
 *
 * Writes (commits) configuration of given DPLL from DPLL mailbox.
 *
 * Context: Process context. Expects zldev->regmap_lock to be held by caller.
 * Return: 0 on success, <0 on error
 */
int zl3073x_mb_dpll_write(struct zl3073x_dev *zldev, u8 index)
{
	int rc;

	/* Select requested index in mask register */
	rc = zl3073x_mb_write_dpll_mb_mask(zldev, BIT(index));
	if (rc)
		return rc;

	/* Perform read operation */
	rc = zl3073x_mb_write_dpll_mb_sem(zldev, ZL_DPLL_MB_SEM_WR);
	if (rc)
		return rc;

	/* Wait for the command to actually finish */
	return zl3073x_mb_poll_dpll_mb_sem(zldev, ZL_DPLL_MB_SEM_WR);
}
EXPORT_SYMBOL_GPL(zl3073x_mb_dpll_write);

/**
 * zl3073x_mb_output_read - read given output configuration to mailbox
 * @zldev: pointer to device structure
 * @index: output index
 *
 * Reads configuration of given output into output mailbox.
 *
 * Context: Process context. Expects zldev->regmap_lock to be held by caller.
 * Return: 0 on success, <0 on error
 */
int zl3073x_mb_output_read(struct zl3073x_dev *zldev, u8 index)
{
	int rc;

	/* Select requested index in mask register */
	rc = zl3073x_mb_write_output_mb_mask(zldev, BIT(index));
	if (rc)
		return rc;

	/* Perform read operation */
	rc = zl3073x_mb_write_output_mb_sem(zldev, ZL_OUTPUT_MB_SEM_RD);
	if (rc)
		return rc;

	/* Wait for the command to actually finish */
	return zl3073x_mb_poll_output_mb_sem(zldev, ZL_OUTPUT_MB_SEM_RD);
}
EXPORT_SYMBOL_GPL(zl3073x_mb_output_read);

/**
 * zl3073x_mb_output_write - write given output configuration from mailbox
 * @zldev: pointer to device structure
 * @index: output index
 *
 * Writes (commits) configuration of given output from output mailbox.
 *
 * Context: Process context. Expects zldev->regmap_lock to be held by caller.
 * Return: 0 on success, <0 on error
 */
int zl3073x_mb_output_write(struct zl3073x_dev *zldev, u8 index)
{
	int rc;

	/* Select requested index in mask register */
	rc = zl3073x_mb_write_output_mb_mask(zldev, BIT(index));
	if (rc)
		return rc;

	/* Perform read operation */
	rc = zl3073x_mb_write_output_mb_sem(zldev, ZL_OUTPUT_MB_SEM_WR);
	if (rc)
		return rc;

	/* Wait for the command to actually finish */
	return zl3073x_mb_poll_output_mb_sem(zldev, ZL_OUTPUT_MB_SEM_WR);
}
EXPORT_SYMBOL_GPL(zl3073x_mb_output_write);

/**
 * zl3073x_mb_ref_read - read given reference configuration to mailbox
 * @zldev: pointer to device structure
 * @index: reference index
 *
 * Reads configuration of given reference into ref mailbox.
 *
 * Context: Process context. Expects zldev->regmap_lock to be held by caller.
 * Return: 0 on success, <0 on error
 */
int zl3073x_mb_ref_read(struct zl3073x_dev *zldev, u8 index)
{
	int rc;

	/* Select requested index in mask register */
	rc = zl3073x_mb_write_ref_mb_mask(zldev, BIT(index));
	if (rc)
		return rc;

	/* Perform read operation */
	rc = zl3073x_mb_write_ref_mb_sem(zldev, ZL_REF_MB_SEM_RD);
	if (rc)
		return rc;

	/* Wait for the command to actually finish */
	return zl3073x_mb_poll_ref_mb_sem(zldev, ZL_REF_MB_SEM_RD);
}
EXPORT_SYMBOL_GPL(zl3073x_mb_ref_read);

/**
 * zl3073x_mb_ref_write - write given reference configuration from mailbox
 * @zldev: pointer to device structure
 * @index: reference index
 *
 * Writes (commits) configuration of given reference from ref mailbox.
 *
 * Context: Process context. Expects zldev->regmap_lock to be held by caller.
 * Return: 0 on success, <0 on error
 */
int zl3073x_mb_ref_write(struct zl3073x_dev *zldev, u8 index)
{
	int rc;

	/* Select requested index in mask register */
	rc = zl3073x_mb_write_ref_mb_mask(zldev, BIT(index));
	if (rc)
		return rc;

	/* Perform read operation */
	rc = zl3073x_mb_write_ref_mb_sem(zldev, ZL_REF_MB_SEM_WR);
	if (rc)
		return rc;

	/* Wait for the command to actually finish */
	return zl3073x_mb_poll_ref_mb_sem(zldev, ZL_REF_MB_SEM_WR);
}
EXPORT_SYMBOL_GPL(zl3073x_mb_ref_write);

/**
 * zl3073x_mb_synth_read - read given synth configuration to mailbox
 * @zldev: pointer to device structure
 * @index: synth index
 *
 * Reads configuration of given synth into synth mailbox.
 *
 * Context: Process context. Expects zldev->regmap_lock to be held by caller.
 * Return: 0 on success, <0 on error
 */
int zl3073x_mb_synth_read(struct zl3073x_dev *zldev, u8 index)
{
	int rc;

	/* Select requested index in mask register */
	rc = zl3073x_mb_write_synth_mb_mask(zldev, BIT(index));
	if (rc)
		return rc;

	/* Perform read operation */
	rc = zl3073x_mb_write_synth_mb_sem(zldev, ZL_SYNTH_MB_SEM_RD);
	if (rc)
		return rc;

	/* Wait for the command to actually finish */
	return zl3073x_mb_poll_synth_mb_sem(zldev, ZL_SYNTH_MB_SEM_RD);
}
EXPORT_SYMBOL_GPL(zl3073x_mb_synth_read);

/**
 * zl3073x_mb_synth_write - write given synth configuration from mailbox
 * @zldev: pointer to device structure
 * @index: synth index
 *
 * Writes (commits) configuration of given synth from synth mailbox.
 *
 * Context: Process context. Expects zldev->regmap_lock to be held by caller.
 * Return: 0 on success, <0 on error
 */
int zl3073x_mb_synth_write(struct zl3073x_dev *zldev, u8 index)
{
	int rc;

	/* Select requested index in mask register */
	rc = zl3073x_mb_write_synth_mb_mask(zldev, BIT(index));
	if (rc)
		return rc;

	/* Perform read operation */
	rc = zl3073x_mb_write_synth_mb_sem(zldev, ZL_SYNTH_MB_SEM_WR);
	if (rc)
		return rc;

	/* Wait for the command to actually finish */
	return zl3073x_mb_poll_synth_mb_sem(zldev, ZL_SYNTH_MB_SEM_WR);
}
EXPORT_SYMBOL_GPL(zl3073x_mb_synth_write);

/**
 * zl3073x_devlink_info_get - Devlink device info callback
 * @devlink: devlink structure pointer
 * @req: devlink request pointer to store information
 * @extack: netlink extack pointer to report errors
 *
 * Return: 0 on success, <0 on error
 */
static int zl3073x_devlink_info_get(struct devlink *devlink,
				    struct devlink_info_req *req,
				    struct netlink_ext_ack *extack)
{
	struct zl3073x_dev *zldev = devlink_priv(devlink);
	u16 id, revision, fw_ver;
	char buf[16];
	u32 cfg_ver;
	int rc;

	rc = zl3073x_read_id(zldev, &id);
	if (rc)
		return rc;

	snprintf(buf, sizeof(buf), "%X", id);
	rc = devlink_info_version_fixed_put(req,
					    DEVLINK_INFO_VERSION_GENERIC_ASIC_ID,
					    buf);
	if (rc)
		return rc;

	rc = zl3073x_read_revision(zldev, &revision);
	if (rc)
		return rc;

	snprintf(buf, sizeof(buf), "%X", revision);
	rc = devlink_info_version_fixed_put(req,
					    DEVLINK_INFO_VERSION_GENERIC_ASIC_REV,
					    buf);
	if (rc)
		return rc;

	rc = zl3073x_read_fw_ver(zldev, &fw_ver);
	if (rc)
		return rc;

	snprintf(buf, sizeof(buf), "%u", fw_ver);
	rc = devlink_info_version_fixed_put(req,
					    DEVLINK_INFO_VERSION_GENERIC_FW,
					    buf);
	if (rc)
		return rc;

	rc = zl3073x_read_custom_config_ver(zldev, &cfg_ver);
	if (rc)
		return rc;

	/* No custom config version */
	if (cfg_ver == U32_MAX)
		return 0;

	snprintf(buf, sizeof(buf), "%lu.%lu.%lu.%lu",
		 FIELD_GET(GENMASK(31, 24), cfg_ver),
		 FIELD_GET(GENMASK(23, 16), cfg_ver),
		 FIELD_GET(GENMASK(15, 8), cfg_ver),
		 FIELD_GET(GENMASK(7, 0), cfg_ver));

	return devlink_info_version_running_put(req, "cfg.custom_ver", buf);
}

static const struct devlink_ops zl3073x_devlink_ops = {
	.info_get = zl3073x_devlink_info_get,
};

static void zl3073x_devlink_free(void *ptr)
{
	devlink_free(ptr);
}

/**
 * zl3073x_devm_alloc - allocates zl3073x device structure
 * @dev: pointer to device structure
 *
 * Allocates zl3073x device structure as device resource and initializes
 * regmap_mutex.
 *
 * Return: pointer to zl3073x device on success, error pointer on error
 */
struct zl3073x_dev *zl3073x_devm_alloc(struct device *dev)
{
	struct zl3073x_dev *zldev;
	struct devlink *devlink;
	int rc;

	devlink = devlink_alloc(&zl3073x_devlink_ops, sizeof(*zldev), dev);
	if (!devlink)
		return ERR_PTR(-ENOMEM);

	/* Add devres action to free devlink device */
	if (devm_add_action_or_reset(dev, zl3073x_devlink_free, devlink))
		return ERR_PTR(-ENOMEM);

	zldev = devlink_priv(devlink);
	zldev->dev = dev;

	/* We have to initialize regmap mutex here because during
	 * zl3073x_dev_probe() is too late as the regmaps are already
	 * initialized.
	 */
	rc = devm_mutex_init(zldev->dev, &zldev->mailbox_lock);
	if (rc) {
		dev_err_probe(zldev->dev, rc, "Failed to initialize mutex\n");
		return ERR_PTR(rc);
	}

	return zldev;
}
EXPORT_SYMBOL_NS_GPL(zl3073x_devm_alloc, "ZL3073X");

/**
 * zl3073x_dev_init_regmap_config - initialize regmap config
 * @regmap_cfg: regmap_config structure to fill
 *
 * Initializes regmap config common for I2C and SPI.
 */
void zl3073x_dev_init_regmap_config(struct regmap_config *regmap_cfg)
{
	*regmap_cfg = zl3073x_regmap_config;
}
EXPORT_SYMBOL_NS_GPL(zl3073x_dev_init_regmap_config, "ZL3073X");

static void zl3073x_devlink_unregister(void *ptr)
{
	devlink_unregister(ptr);
}

/**
 * zl3073x_dev_probe - initialize zl3073x device
 * @zldev: pointer to zl3073x device
 * @chip_info: chip info based on compatible
 *
 * Common initialization of zl3073x device structure.
 *
 * Returns: 0 on success, <0 on error
 */
int zl3073x_dev_probe(struct zl3073x_dev *zldev,
		      const struct zl3073x_chip_info *chip_info)
{
	u16 id, revision, fw_ver;
	struct devlink *devlink;
	u32 cfg_ver;
	int i, rc;

	/* Read chip ID */
	rc = zl3073x_read_id(zldev, &id);
	if (rc)
		return rc;

	/* Check it matches */
	for (i = 0; i < chip_info->num_ids; i++) {
		if (id == chip_info->ids[i])
			break;
	}

	if (i == chip_info->num_ids) {
		dev_err(zldev->dev, "Unknown or non-match chip ID: 0x%0x\n", id);
		return -ENODEV;
	}

	/* Read revision, firmware version and custom config version */
	rc = zl3073x_read_revision(zldev, &revision);
	if (rc)
		return rc;
	rc = zl3073x_read_fw_ver(zldev, &fw_ver);
	if (rc)
		return rc;
	rc = zl3073x_read_custom_config_ver(zldev, &cfg_ver);
	if (rc)
		return rc;

	dev_dbg(zldev->dev, "ChipID(%X), ChipRev(%X), FwVer(%u)\n", id,
		revision, fw_ver);
	dev_dbg(zldev->dev, "Custom config version: %lu.%lu.%lu.%lu\n",
		FIELD_GET(GENMASK(31, 24), cfg_ver),
		FIELD_GET(GENMASK(23, 16), cfg_ver),
		FIELD_GET(GENMASK(15, 8), cfg_ver),
		FIELD_GET(GENMASK(7, 0), cfg_ver));

	/* Register the device as devlink device */
	devlink = priv_to_devlink(zldev);
	devlink_register(devlink);

	/* Add devres action to unregister devlink device */
	rc = devm_add_action_or_reset(zldev->dev, zl3073x_devlink_unregister,
				      devlink);
	if (rc)
		return rc;

	return 0;
}
EXPORT_SYMBOL_NS_GPL(zl3073x_dev_probe, "ZL3073X");

MODULE_AUTHOR("Ivan Vecera <ivecera@redhat.com>");
MODULE_DESCRIPTION("Microchip ZL3073x core driver");
MODULE_LICENSE("GPL");
