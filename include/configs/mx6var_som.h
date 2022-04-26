/*
 * Copyright (C) 2012-2015 Freescale Semiconductor, Inc.
 * Copyright (C) 2016 Variscite Ltd. All Rights Reserved.
 *
 * Author: Eran Matityahu <eran.m@variscite.com>
 *
 * Configuration settings for Variscite VAR-SOM-MX6 board family.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */
#ifndef __MX6VAR_SOM_CONFIG_H
#define __MX6VAR_SOM_CONFIG_H

#include <asm/arch/imx-regs.h>
#include <asm/imx-common/gpio.h>

#ifdef CONFIG_SPL
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#ifdef CONFIG_SYS_BOOT_NAND
#define CONFIG_SPL_NAND_SUPPORT
#else
#define CONFIG_SPL_MMC_SUPPORT
#endif
#include "imx6_spl.h"
#endif

/* Reserve 4Bytes in OCRAM for sending RAM size from SPL to U-Boot */
#undef CONFIG_SPL_MAX_SIZE
#define CONFIG_SPL_MAX_SIZE	0xFFFC  /* ==0x10000-0x4 */
#define RAM_SIZE_ADDR	((CONFIG_SPL_TEXT_BASE) + (CONFIG_SPL_MAX_SIZE))

#define CONFIG_MX6

#ifdef CONFIG_MX6SOLO
#define CONFIG_MX6DL
#endif

#include "mx6_common.h"
#include <linux/sizes.h>

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

#define CONFIG_SYS_GENERIC_BOARD

#define CONFIG_MACH_VAR_SOM_MX6		4419
#define CONFIG_MACH_TYPE		CONFIG_MACH_VAR_SOM_MX6

#define CONFIG_MXC_UART_BASE		UART1_BASE
#define CONFIG_CONSOLE_DEV		"ttymxc0"

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO
#define CONFIG_CMD_GPIO

#define CONFIG_SILENT_CONSOLE

#define CONFIG_MXC_UART

#define CONFIG_CMD_FUSE
#ifdef CONFIG_CMD_FUSE
#define CONFIG_MXC_OCOTP
#endif

#define LOW_POWER_MODE_ENABLE

/* MMC Configs */
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	0
#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_SYS_MMC_ENV_PART		0
#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_BOUNCE_BUFFER
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FAT
#define CONFIG_FAT_WRITE
#define CONFIG_CMD_FS_GENERIC
#define CONFIG_DOS_PARTITION

/*#define CONFIG_SUPPORT_EMMC_BOOT*/ /* eMMC specific */

/* Ethernet Configs */
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_XCV_TYPE		RGMII
#define CONFIG_ETHPRIME			"FEC"
#define CONFIG_FEC_MXC_PHYADDR		7
#define CONFIG_PHYLIB
#define CONFIG_PHY_ADIN
#define CONFIG_PHY_MICREL

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200

/* Command definition */
#include <config_cmd_default.h>

/*#define CONFIG_CMD_BMODE*/
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_SETEXPR
#undef CONFIG_CMD_IMLS
#define CONFIG_CMD_TIME

#define CONFIG_BOOTDELAY		1

#define CONFIG_LOADADDR			0x12000000
#define CONFIG_SYS_TEXT_BASE		0x17800000

#ifdef CONFIG_SYS_BOOT_NAND
#define MFG_NAND_PARTITION "mtdparts=gpmi-nand:16m(boot),16m(kernel),16m(dtb),-(rootfs) "
#else
#define MFG_NAND_PARTITION ""
#endif

#define MFG_ENV_SETTINGS \
	"mfgtool_args=setenv bootargs console=${console},${baudrate} " \
		"rdinit=/linuxrc " \
		"g_mass_storage.stall=0 g_mass_storage.removable=1 " \
		"g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "\
		"g_mass_storage.iSerialNumber=\"\" "\
		"enable_wait_mode=off "\
		MFG_NAND_PARTITION \
		"\0" \
	"initrd_addr=0x12C00000\0" \
	"initrd_high=0xffffffff\0" \
	"bootcmd_mfg=run mfgtool_args;bootm ${loadaddr} ${initrd_addr} ${fdt_addr};\0"

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#ifdef CONFIG_SYS_BOOT_NAND
#define CONFIG_SYS_MMC_ROOT_PART	1
#else
#define CONFIG_SYS_MMC_ROOT_PART	2
#endif

#define MMC_BOOT_ENV_SETTINGS \
	"bootenv=uEnv.txt\0" \
	"script=boot.scr\0" \
	"uimage=uImage\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"mmcdev=" __stringify(CONFIG_SYS_MMC_ENV_DEV) "\0" \
	"mmcblk=0\0" \
	"mmcautodetect=yes\0" \
	"mmcbootpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcrootpart=" __stringify(CONFIG_SYS_MMC_ROOT_PART) "\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
		"root=/dev/mmcblk${mmcblk}p${mmcrootpart} rootwait rw\0 " \
	"loadbootenv=" \
		"load mmc ${mmcdev}:${mmcbootpart} ${loadaddr} ${bootdir}/${bootenv};\0" \
	"importbootenv=echo Importing bootenv from mmc ...; " \
		"env import -t ${loadaddr} ${filesize}\0" \
	"loadbootscript=" \
		"load mmc ${mmcdev}:${mmcbootpart} ${loadaddr} ${bootdir}/${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loaduimage=load mmc ${mmcdev}:${mmcbootpart} ${loadaddr} ${bootdir}/${uimage}\0" \
	"loadfdt=run findfdt; " \
		"echo fdt_file=${fdt_file}; " \
		"load mmc ${mmcdev}:${mmcbootpart} ${fdt_addr} ${bootdir}/${fdt_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"run videoargs; " \
		"run optargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootm ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootm; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootm; " \
		"fi;\0"


#define MMC_BOOTCMD \
	"mmc dev ${mmcdev};" \
	"if mmc rescan; then " \
		"if run loadbootenv; then " \
			"run importbootenv; " \
		"fi; " \
		"if run loadbootscript; then " \
			"run bootscript; " \
		"else " \
			"if run loaduimage; then " \
				"run mmcboot; " \
			"else " \
				"run netboot; " \
			"fi; " \
		"fi; " \
	"else run netboot; fi;"

#ifdef CONFIG_SYS_BOOT_NAND
#define NAND_BOOT_ENV_SETTINGS \
	"nandargs=setenv bootargs console=${console},${baudrate} ubi.mtd=3 " \
		"root=ubi0:rootfs rootfstype=ubifs\0" \
	"rootfs_device=nand\0" \
	"boot_device=nand\0" \
	"nandboot=nand read ${loadaddr} 0x400000 0x800000; " \
		"nand read ${fdt_addr} 0x3e0000 0x20000; " \
		"bootm ${loadaddr} - ${fdt_addr};\0" \
	"bootcmd=" \
		"if test ${rootfs_device} != emmc; then " \
			"run nandargs; " \
			"run videoargs; " \
			"run optargs; " \
			"echo booting from nand ...; " \
			"run nandboot; " \
		"else " \
			"if test ${boot_device} != emmc; then " \
				"run mmcargs; " \
				"run videoargs; " \
				"run optargs; " \
				"echo booting from nand (rootfs on emmc)...; " \
				"run nandboot; " \
			"else " \
				"setenv mmcdev 1; " \
				MMC_BOOTCMD \
			"fi; " \
		"fi;\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0"
#else
#define NAND_BOOT_ENV_SETTINGS ""
#define CONFIG_BOOTCOMMAND	MMC_BOOTCMD
#endif

#define OPT_ENV_SETTINGS \
	"optargs=setenv bootargs ${bootargs} ${kernelargs};\0"

#define VIDEO_ENV_SETTINGS \
	"videoargs=" \
		"if hdmidet; then " \
			"setenv bootargs ${bootargs} " \
				"video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24; " \
		"else " \
			"setenv bootargs ${bootargs} " \
				"video=mxcfb0:dev=ldb; " \
		"fi; " \
		"setenv bootargs ${bootargs} " \
			"video=mxcfb1:off video=mxcfb2:off video=mxcfb3:off;\0"


#define CONFIG_EXTRA_ENV_SETTINGS \
	MFG_ENV_SETTINGS \
	MMC_BOOT_ENV_SETTINGS \
	NAND_BOOT_ENV_SETTINGS \
	VIDEO_ENV_SETTINGS \
	OPT_ENV_SETTINGS \
	"fdt_file=undefined\0" \
	"fdt_addr=0x18000000\0" \
	"fdt_high=0xffffffff\0" \
	"splashsourceauto=yes\0" \
	"splashfile=/boot/splash.bmp\0" \
	"splashimage=0x18100000\0" \
	"splashenable=setenv splashfile /boot/splash.bmp; " \
		"setenv splashimage 0x18100000\0" \
	"splashdisable=setenv splashfile; setenv splashimage\0" \
	"console=" CONFIG_CONSOLE_DEV "\0" \
	"netargs=setenv bootargs console=${console},${baudrate} " \
		"root=/dev/nfs rw " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp; " \
		"run videoargs\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"run optargs; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${uimage}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"run findfdt; " \
			"echo fdt_file=${fdt_file}; " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"bootm ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootm; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootm; " \
		"fi;\0" \
	"findfdt="\
		"if test $fdt_file = undefined; then " \
			"if test $board_name = DT6CUSTOM && test $board_rev = MX6Q; then " \
				"setenv fdt_file imx6q-var-dart.dtb; " \
			"fi; " \
			"if test $board_name = SOLOCUSTOM && test $board_rev = MX6QP; then " \
				"setenv fdt_file imx6qp-var-som-vsc.dtb; " \
			"fi; " \
			"if test $board_name = SOLOCUSTOM && test $board_rev = MX6Q; then " \
				"setenv fdt_file imx6q-var-som-vsc.dtb; " \
			"fi; " \
			"if test $board_name = SOLOCUSTOM && test $board_rev = MX6DL && test $board_som = SOM-SOLO; then " \
				"setenv fdt_file imx6dl-var-som-solo-vsc.dtb; " \
			"fi; " \
			"if test $board_name = SOLOCUSTOM && test $board_rev = MX6DL && test $board_som = SOM-MX6; then " \
				"setenv fdt_file imx6dl-var-som-vsc.dtb; " \
			"fi; " \
			"if test $board_name = MX6CUSTOM && test $board_rev = MX6QP; then " \
				"i2c dev 2; " \
				"if i2c probe 0x38; then " \
					"setenv fdt_file imx6qp-var-som-cap.dtb; " \
				"else " \
					"setenv fdt_file imx6qp-var-som-res.dtb; " \
				"fi; " \
			"fi; " \
			"if test $board_name = MX6CUSTOM && test $board_rev = MX6Q; then " \
				"i2c dev 2; " \
				"if i2c probe 0x38; then " \
					"setenv fdt_file imx6q-var-som-cap.dtb; " \
				"else " \
					"setenv fdt_file imx6q-var-som-res.dtb; " \
				"fi; " \
			"fi; " \
			"if test $board_name = MX6CUSTOM && test $board_rev = MX6DL && test $board_som = SOM-SOLO; then " \
				"i2c dev 2; " \
				"if i2c probe 0x38; then " \
					"setenv fdt_file imx6dl-var-som-solo-cap.dtb; " \
				"else " \
					"setenv fdt_file imx6dl-var-som-solo-res.dtb; " \
				"fi; " \
			"fi; " \
			"if test $board_name = MX6CUSTOM && test $board_rev = MX6DL && test $board_som = SOM-MX6; then " \
				"i2c dev 2; " \
				"if i2c probe 0x38; then " \
					"setenv fdt_file imx6dl-var-som-cap.dtb; " \
				"else " \
					"setenv fdt_file imx6dl-var-som-res.dtb; " \
				"fi; " \
			"fi; " \
			"if test $fdt_file = undefined; then " \
				"echo WARNING: Could not determine dtb to use; " \
			"fi; " \
		"fi;\0"


#define CONFIG_ARP_TIMEOUT		200UL

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE		1024

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		256
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x10000000
#define CONFIG_SYS_MEMTEST_END		0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH	0x10800000

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_CMDLINE_EDITING
#define CONFIG_STACKSIZE		(128 * 1024)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_SIZE			(8 * 1024)

#ifdef CONFIG_SYS_BOOT_NAND
#define CONFIG_SYS_USE_NAND
#define CONFIG_ENV_IS_IN_NAND
/* NAND boot config */
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x200000
#else
#define CONFIG_ENV_IS_IN_MMC
#endif

#ifdef CONFIG_SYS_USE_NAND
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_TRIMFFS

/* UBI/UBIFS support */
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_UBI_SILENCE_MSG
#define CONFIG_UBIFS_SILENCE_MSG
#define CONFIG_RBTREE
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS
#define CONFIG_LZO

#define MTDIDS_DEFAULT		"nand0=nandflash-0"

/*
 * Partitions layout for NAND is:
 *     mtd0: 2M       (spl) First boot loader
 *     mtd1: 2M       (u-boot, dtb)
 *     mtd2: 8M       (kernel)
 *     mtd3: left     (rootfs)
 */
/* Default mtd partition table */
#define MTDPARTS_DEFAULT	"mtdparts=nandflash-0:"\
					"2m(spl),"\
					"2m(u-boot),"\
					"8m(kernel),"\
					"-(rootfs)"	/* ubifs */

/* NAND stuff */
#define CONFIG_NAND_MXS
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x40000000
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_ONFI_DETECTION

/* DMA stuff, needed for GPMI/MXS NAND support */
#define CONFIG_APBH_DMA
#define CONFIG_APBH_DMA_BURST
#define CONFIG_APBH_DMA_BURST8
#endif

#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_ENV_OFFSET		(0x3E0000)
#elif defined(CONFIG_ENV_IS_IN_NAND)
#undef CONFIG_ENV_SIZE
#define CONFIG_ENV_OFFSET		(0x180000)
#define CONFIG_ENV_SECT_SIZE		(128 << 10)
#define CONFIG_ENV_SIZE			CONFIG_ENV_SECT_SIZE
#endif

#define CONFIG_OF_LIBFDT

#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_CMD_CACHE
#endif

/* Framebuffer */
#define CONFIG_VIDEO
#ifdef CONFIG_VIDEO
#define CONFIG_VIDEO_IPUV3
#define CONFIG_CFB_CONSOLE
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_SPLASH_SOURCE
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#ifdef CONFIG_MX6DL
#define CONFIG_IPUV3_CLK 198000000
#else
#define CONFIG_IPUV3_CLK 264000000
#endif
#define CONFIG_IMX_HDMI
#define CONFIG_CMD_HDMIDETECT
#define CONFIG_IMX_VIDEO_SKIP
#define CONFIG_CMD_BMP
#elif defined(CONFIG_VIDEO_HDMI)
#define CONFIG_IMX_HDMI
#define CONFIG_CMD_HDMIDETECT
#endif

#define PMIC_I2C_BUS		1
#define MX6CB_CDISPLAY_I2C_BUS	2
#define MX6CB_CDISPLAY_I2C_ADDR	0x38

/* I2C Configs */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED		100000

/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR	0x08

#define CONFIG_NETCONSOLE

/* USB Configs */
#define CONFIG_CMD_USB
#ifdef CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_USB_MAX_CONTROLLER_COUNT	2 /* Enabled USB controller number */

#define CONFIG_CI_UDC
#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_DUALSPEED

/* Uncomment for USB Ethernet Gadget support */
/*
 * #define CONFIG_USB_ETHER
 * #define CONFIG_USB_ETH_CDC
 */

#define CONFIG_USB_GADGET
#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_MASS_STORAGE
#define CONFIG_USBDOWNLOAD_GADGET
#define CONFIG_USB_GADGET_VBUS_DRAW	2

#define CONFIG_G_DNL_VENDOR_NUM		0x18d1
#define CONFIG_G_DNL_PRODUCT_NUM	0x0d02
#define CONFIG_G_DNL_MANUFACTURER	"Variscite"

/* USB Device Firmware Update support */
#define CONFIG_CMD_DFU
#define CONFIG_DFU_FUNCTION
#define CONFIG_DFU_MMC
#define CONFIG_DFU_RAM
#ifdef CONFIG_SYS_USE_NAND
#define CONFIG_DFU_NAND
#endif
#if 0
#define CONFIG_SYS_DFU_DATA_BUF_SIZE SZ_32M
#define DFU_DEFAULT_POLL_TIMEOUT 300
#endif
#endif /* CONFIG_CMD_USB */

#if defined(CONFIG_ANDROID_SUPPORT)
#include "mx6var_som_android.h"
#endif

#endif	/* __MX6VAR_SOM_CONFIG_H */
