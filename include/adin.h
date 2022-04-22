/*
 * Copyright 2022 Variscite LTD.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 */
#ifndef _PHY_ADIN_H
#define _PHY_ADIN_H

#ifndef BIT
#define BIT(x)			(1 << (x))
#endif

#define ADIN1300_PHY_ID		0x0283bc30
#define ADIN1300_EXT_REG_PTR	0x10
#define ADIN1300_EXT_REG_DATA	0x11

#define ADIN1300_GE_CLK_CFG			0xff1f
#define   ADIN1300_GE_CLK_RCVR_125_EN		BIT(5)

#define ADIN1300_GE_RGMII_CFG_REG		0xff23
#define   ADIN1300_GE_RGMII_RX_MSK		GENMASK(8, 6)
#define   ADIN1300_GE_RGMII_RX_SEL(x)		\
		FIELD_PREP(ADIN1300_GE_RGMII_RX_MSK, x)
#define   ADIN1300_GE_RGMII_GTX_MSK		GENMASK(5, 3)
#define   ADIN1300_GE_RGMII_GTX_SEL(x)		\
		FIELD_PREP(ADIN1300_GE_RGMII_GTX_MSK, x)
#define   ADIN1300_GE_RGMII_RXID_EN		BIT(2)
#define   ADIN1300_GE_RGMII_TXID_EN		BIT(1)
#define   ADIN1300_GE_RGMII_EN			BIT(0)

u16 adin_ext_read(struct phy_device *phydev, const u32 regnum);
int adin_ext_write(struct phy_device *phydev, const u32 regnum, const u16 val);
int adin_set_clk_rcvr_125_en(struct phy_device *phydev, const int clk_rcvr_125_en);
int adin_config_rgmii_mode(struct phy_device *phydev);

#endif
