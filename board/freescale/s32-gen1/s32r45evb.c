// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2018-2021 NXP
 */

#include "asm/arch-s32/siul-s32r45.h"
#include "board_common.h"
#if CONFIG_IS_ENABLED(DWC_ETH_QOS_S32CC)
#include "s32gen1_gmac_utils.h"
#endif
#include <asm/arch/soc.h>
#include <common.h>
#if CONFIG_IS_ENABLED(DWC_ETH_QOS_S32CC)
#include <linux/err.h>
#include <miiphy.h>
#endif

#define S32R_PHY_ADDR_1 0x01

void setup_iomux_uart(void)
{
#if (CONFIG_FSL_LINFLEX_MODULE == 0)

	/* Muxing for linflex0 */
	setup_iomux_uart0_pc09_pc10();

#else
#error "Unsupported UART pinmuxing configuration"
#endif
}

/**
 * Due to the hardware design of the reset circuit on S32R45,
 * the PHY does not properly reset after a software reboot.
 * PHY's control register is not correctly initialized afterwards
 * and GMAC clock settings fail to apply.
 */
int last_stage_init(void)
{
#if CONFIG_IS_ENABLED(DWC_ETH_QOS_S32CC)
	struct udevice *eth;
	struct mii_dev *bus;
	struct phy_device *phy;
	phy_interface_t mode;

	eth = eth_get_dev_by_name("eth_eqos");
	bus = miiphy_get_dev_by_name("eth_eqos");

	if (!IS_ERR_OR_NULL(eth) && !IS_ERR_OR_NULL(bus)) {
		mode = eqos_get_interface_s32cc(eth);

		/* phy_connect will reset the PHY */
		phy = phy_connect(bus, S32R_PHY_ADDR_1, eth, mode);
		if (phy)
			setup_clocks_enet_gmac(mode, eth);
	}
#endif
	return 0;
}