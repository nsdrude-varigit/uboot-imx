// SPDX-License-Identifier: GPL-2.0+
/**
 *  Driver for Analog Devices Industrial Ethernet PHYs
 *
 * Copyright 2019 Analog Devices Inc.
 * Copyright 2022 Variscite Ltd.
 */
#include <common.h>
#include <phy.h>
#include <linux/bitops.h>

#define PHY_ID_ADIN1300			0x0283bc30
#define ADIN1300_EXT_REG_PTR		0x10
#define ADIN1300_EXT_REG_DATA		0x11
#define ADIN1300_GE_RGMII_CFG		0xff23
#define ADIN1300_GE_RGMII_RXID_EN	BIT(2)
#define ADIN1300_GE_RGMII_TXID_EN	BIT(1)
#define ADIN1300_GE_RGMII_EN		BIT(0)

static u16 adin_ext_read(struct phy_device *phydev, const u32 regnum)
{
	u16 val;

	phy_write(phydev, MDIO_DEVAD_NONE, ADIN1300_EXT_REG_PTR, regnum);
	val = phy_read(phydev, MDIO_DEVAD_NONE, ADIN1300_EXT_REG_DATA);

	debug("%s: adin@0x%x 0x%x=0x%x\n", __func__, phydev->addr, regnum, val);

	return val;
}

static int adin_ext_write(struct phy_device *phydev, const u32 regnum, const u16 val)
{
	debug("%s: adin@0x%x 0x%x=0x%x\n", __func__, phydev->addr, regnum, val);

	phy_write(phydev, MDIO_DEVAD_NONE, ADIN1300_EXT_REG_PTR, regnum);

	return phy_write(phydev, MDIO_DEVAD_NONE, ADIN1300_EXT_REG_DATA, val);
}

static int adin_config_rgmii_mode(struct phy_device *phydev)
{
	u16 val;

	val = adin_ext_read(phydev, ADIN1300_GE_RGMII_CFG);

	if (!phy_interface_is_rgmii(phydev)) {
		/* Disable RGMII */
		val &= ~ADIN1300_GE_RGMII_EN;
		return adin_ext_write(phydev, ADIN1300_GE_RGMII_CFG, val);
	}

	/* Enable RGMII */
	val |= ADIN1300_GE_RGMII_EN;

	/* Enable / Disable RGMII RX Delay */
	if (phydev->interface == PHY_INTERFACE_MODE_RGMII_ID ||
	    phydev->interface == PHY_INTERFACE_MODE_RGMII_RXID) {
		val |= ADIN1300_GE_RGMII_RXID_EN;
	} else {
		val &= ~ADIN1300_GE_RGMII_RXID_EN;
	}

	/* Enable / Disable RGMII RX Delay */
	if (phydev->interface == PHY_INTERFACE_MODE_RGMII_ID ||
	    phydev->interface == PHY_INTERFACE_MODE_RGMII_TXID) {
		val |= ADIN1300_GE_RGMII_TXID_EN;
	} else {
		val &= ~ADIN1300_GE_RGMII_TXID_EN;
	}

	return adin_ext_write(phydev, ADIN1300_GE_RGMII_CFG, val);
}

static int adin1300_config(struct phy_device *phydev)
{
	int ret;

	printf("ADIN1300 PHY detected at addr %d\n", phydev->addr);

	ret = adin_config_rgmii_mode(phydev);

	if (ret < 0)
		return ret;

	return genphy_config(phydev);
}

static struct phy_driver ADIN1300_driver =  {
	.name = "ADIN1300",
	.uid = PHY_ID_ADIN1300,
	.mask = 0xffffffff,
	.features = PHY_GBIT_FEATURES,
	.config = adin1300_config,
	.startup = genphy_startup,
	.shutdown = genphy_shutdown,
};

int phy_adin_init(void)
{
	phy_register(&ADIN1300_driver);

	return 0;
}
