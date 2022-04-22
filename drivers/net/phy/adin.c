#include <common.h>
#include <phy.h>
#include <adin.h>

/**
 * phy_interface_is_rgmii - Convenience function for testing if a PHY interface
 * is RGMII (all variants)
 * @phydev: the phy_device struct
 */
static inline bool phy_interface_is_rgmii(struct phy_device *phydev)
{
        return phydev->interface >= PHY_INTERFACE_MODE_RGMII &&
                phydev->interface <= PHY_INTERFACE_MODE_RGMII_TXID;
}

u16 adin_ext_read(struct phy_device *phydev, const u32 regnum)
{
	u16 val;

	phy_write(phydev, MDIO_DEVAD_NONE, ADIN1300_EXT_REG_PTR, regnum);
	val = phy_read(phydev, MDIO_DEVAD_NONE, ADIN1300_EXT_REG_DATA);

	debug("%s: adin@0x%x 0x%x=0x%x\n", __func__, phydev->addr, regnum, val);

	return val;
}

int adin_ext_write(struct phy_device *phydev, const u32 regnum, const u16 val)
{
	debug("%s: adin@0x%x 0x%x=0x%x\n", __func__, phydev->addr, regnum, val);

	phy_write(phydev, MDIO_DEVAD_NONE, ADIN1300_EXT_REG_PTR, regnum);

	return phy_write(phydev, MDIO_DEVAD_NONE, ADIN1300_EXT_REG_DATA, val);
}

int adin_set_clk_rcvr_125_en(struct phy_device *phydev, const int clk_rcvr_125_en)
{
	u16 reg = 0;

	reg = adin_ext_read(phydev, ADIN1300_GE_CLK_CFG);

	if (clk_rcvr_125_en)
		reg |= ADIN1300_GE_CLK_RCVR_125_EN;
	else
		reg &= ~ADIN1300_GE_CLK_RCVR_125_EN;

	return adin_ext_write(phydev, ADIN1300_GE_CLK_CFG, reg);
}

int adin_config_rgmii_mode(struct phy_device *phydev)
{
	u16 reg;

	reg = adin_ext_read(phydev, ADIN1300_GE_RGMII_CFG_REG);

	if (!phy_interface_is_rgmii(phydev)) {
		reg &= ~ADIN1300_GE_RGMII_EN;
		return adin_ext_write(phydev, ADIN1300_GE_RGMII_CFG_REG, reg);
	}

	reg |= ADIN1300_GE_RGMII_EN;

	if (phydev->interface == PHY_INTERFACE_MODE_RGMII_ID ||
	    phydev->interface == PHY_INTERFACE_MODE_RGMII_RXID)
		reg |= ADIN1300_GE_RGMII_RXID_EN;
	else
		reg &= ~ADIN1300_GE_RGMII_RXID_EN;


	if (phydev->interface == PHY_INTERFACE_MODE_RGMII_ID ||
	    phydev->interface == PHY_INTERFACE_MODE_RGMII_TXID)
		reg |= ADIN1300_GE_RGMII_TXID_EN;
	else
		reg &= ~ADIN1300_GE_RGMII_TXID_EN;

	return adin_ext_write(phydev, ADIN1300_GE_RGMII_CFG_REG, reg);
}

static int adin1300_config(struct phy_device *phydev)
{
	int ret;

	ret = adin_config_rgmii_mode(phydev);

	if (ret < 0)
		return ret;

	return genphy_config(phydev);
}

static struct phy_driver adin1300_driver = {
	.name = "ADIN1300",
	.uid  = ADIN1300_PHY_ID,
	.mask = 0xffffff,
	.features = PHY_GBIT_FEATURES,
	.config   = &adin1300_config,
	.startup  = &genphy_startup,
	.shutdown = &genphy_shutdown,
};

int phy_adin_init(void)
{
	phy_register(&adin1300_driver);
	return 0;
}
