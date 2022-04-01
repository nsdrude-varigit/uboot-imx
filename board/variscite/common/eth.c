/*
 * Copyright 2020-2021 Variscite Ltd.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <net.h>
#include <miiphy.h>
#include <env.h>

#include "../common/imx8_eeprom.h"

#define AR803x_PHY_ID_1			0x4d
#define ADIN1300_PHY_ID_1		0x283
#define ADIN1300_EXT_REG_PTR		0x10
#define ADIN1300_EXT_REG_DATA		0x11
#define ADIN1300_GE_RGMII_CFG		0xff23
#define ADIN1300_GE_RGMII_RXID_EN	BIT(2)
#define ADIN1300_GE_RGMII_TXID_EN	BIT(1)
#define ADIN1300_GE_RGMII_EN		BIT(0)

static u16 adin_ext_read(struct phy_device *phydev, const u32 regnum) {
	u16 val;

	phy_write(phydev, MDIO_DEVAD_NONE, ADIN1300_EXT_REG_PTR, regnum);
	val = phy_read(phydev, MDIO_DEVAD_NONE, ADIN1300_EXT_REG_DATA);

	printf("adin_ext_read: adin1300@0x%x 0x%x=0x%x\n", phydev->addr, regnum, val);

	return val;
}

static int adin_ext_write(struct phy_device *phydev, const u32 regnum, const u16 val) {

	printf("adin_ext_write: adin1300@0x%x 0x%x=0x%x\n", phydev->addr, regnum, val);

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

int board_phy_config(struct phy_device *phydev)
{
	u32 phy_id_1;

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	phy_id_1 = (phydev->phy_id >> 16);

	/* Use mii register 0x2 to determine if AR8033 or ADIN1300 */
	switch(phy_id_1) {
	case AR803x_PHY_ID_1:
		printf("AR8033 PHY detected at addr %d\n", phydev->addr);
		break;
	case ADIN1300_PHY_ID_1:
		printf("ADIN1300 PHY detected at addr %d\n", phydev->addr);
		adin_config_rgmii_mode(phydev);
	break;
	default:
		printf("%s: unknown phy_id 0x%x at addr %d\n", __func__, phy_id_1, phydev->addr);
		break;
	}

	return 0;
}

#if defined(CONFIG_ARCH_IMX8) || defined(CONFIG_IMX8MP)

#define CHAR_BIT 8

static uint64_t mac2int(const uint8_t hwaddr[])
{
	int8_t i;
	uint64_t ret = 0;
	const uint8_t *p = hwaddr;

	for (i = 5; i >= 0; i--) {
		ret |= (uint64_t)*p++ << (CHAR_BIT * i);
	}

	return ret;
}

static void int2mac(const uint64_t mac, uint8_t *hwaddr)
{
	int8_t i;
	uint8_t *p = hwaddr;

	for (i = 5; i >= 0; i--) {
		*p++ = mac >> (CHAR_BIT * i);
	}
}
#endif

int var_setup_mac(struct var_eeprom *eeprom)
{
	int ret;
	uint8_t enetaddr[6];

#if defined(CONFIG_ARCH_IMX8) || defined(CONFIG_IMX8MP)
	uint64_t addr;
	uint8_t enet1addr[6];
#endif

	ret = eth_env_get_enetaddr("ethaddr", enetaddr);
	if (ret)
		return 0;

	ret = var_eeprom_get_mac(eeprom, enetaddr);
	if (ret)
		return ret;

	if (!is_valid_ethaddr(enetaddr))
		return -1;

	eth_env_set_enetaddr("ethaddr", enetaddr);

#if defined(CONFIG_ARCH_IMX8) || defined(CONFIG_IMX8MP)
	addr = mac2int(enetaddr);
	int2mac(addr + 1, enet1addr);
	eth_env_set_enetaddr("eth1addr", enet1addr);
#endif

	return 0;
}
