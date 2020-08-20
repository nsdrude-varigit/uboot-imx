/*
 * Copyright 2018 NXP
 * Copyright 2019-2020 Variscite Ltd.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>
#include <asm/arch/imx8mn_pins.h>
#include <asm/arch/clock.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <i2c.h>
#include <asm/io.h>
#include <usb.h>

#include "../common/imx8_eeprom.h"
#include "imx8mn_var_som.h"

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL1)
#define WDOG_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_ODE | PAD_CTL_PUE | PAD_CTL_PE)

static iomux_v3_cfg_t const uart4_pads[] = {
	IMX8MN_PAD_UART4_RXD__UART4_DCE_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MN_PAD_UART4_TXD__UART4_DCE_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
};

static iomux_v3_cfg_t const wdog_pads[] = {
	IMX8MN_PAD_GPIO1_IO02__WDOG1_WDOG_B  | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

int board_early_init_f(void)
{
	struct wdog_regs *wdog = (struct wdog_regs *)WDOG1_BASE_ADDR;

	imx_iomux_v3_setup_multiple_pads(wdog_pads, ARRAY_SIZE(wdog_pads));

	set_wdog_reset(wdog);

	imx_iomux_v3_setup_multiple_pads(uart4_pads, ARRAY_SIZE(uart4_pads));

	init_uart_clk(3);

	return 0;
}

#ifdef CONFIG_FEC_MXC
static void setup_fec(void)
{
	struct iomuxc_gpr_base_regs *const iomuxc_gpr_regs
		= (struct iomuxc_gpr_base_regs *) IOMUXC_GPR_BASE_ADDR;

	/* Use 125M anatop REF_CLK1 for ENET1, not from external */
	clrsetbits_le32(&iomuxc_gpr_regs->gpr[1],
			IOMUXC_GPR_GPR1_GPR_ENET1_TX_CLK_SEL_SHIFT, 0);
}
#endif

#ifdef CONFIG_CI_UDC
int board_usb_init(int index, enum usb_init_type init)
{
	imx8m_usb_power(index, true);

	return 0;
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
	imx8m_usb_power(index, false);

	return 0;
}

#define USB_OTG1_ID_GPIO	IMX_GPIO_NR(1, 10)

static iomux_v3_cfg_t const usb_pads[] = {
	IMX8MN_PAD_GPIO1_IO10__GPIO1_IO10 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

/* Used only on VAR-SOM-MX8M-NANO Rev1.0 (with extcon) */
int board_ehci_usb_phy_mode(struct udevice *dev)
{
	int id = gpio_get_value(USB_OTG1_ID_GPIO);

	return id ? USB_INIT_DEVICE : USB_INIT_HOST;
}

static void setup_usb_rev10(void)
{
	imx_iomux_v3_setup_multiple_pads(usb_pads, ARRAY_SIZE(usb_pads));
	gpio_request(USB_OTG1_ID_GPIO, "usb_otg1_id");
	gpio_direction_input(USB_OTG1_ID_GPIO);
}
#endif

int board_init(void)
{
#ifdef CONFIG_FEC_MXC
	setup_fec();
#endif

	return 0;
}

int var_get_som_rev(struct var_eeprom *ep)
{
	if (ep->somrev == 0)
		return SOM_REV_10;
	else
		return SOM_REV_11;
}

#define SDRAM_SIZE_STR_LEN 5

int board_late_init(void)
{
	int som_rev;
	struct var_eeprom *ep = VAR_EEPROM_DATA;
	char sdram_size_str[SDRAM_SIZE_STR_LEN];

#ifdef CONFIG_FEC_MXC
	var_setup_mac(ep);
#endif
	var_eeprom_print_prod_info(ep);

	som_rev = var_get_som_rev(ep);

#ifdef CONFIG_CI_UDC
	if ((som_rev == SOM_REV_10))
		setup_usb_rev10();
#endif

	snprintf(sdram_size_str, SDRAM_SIZE_STR_LEN, "%d",
		(int) (gd->ram_size / 1024 / 1024));
	env_set("sdram_size", sdram_size_str);

	env_set("board_name", "VAR-SOM-MX8M-NANO");

	if (som_rev == SOM_REV_10)
		env_set("som_rev", "som_rev10");
	else
		env_set("som_rev", "som_rev11");

#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init();
#endif

	return 0;
}
