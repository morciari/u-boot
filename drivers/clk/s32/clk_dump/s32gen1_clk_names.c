// SPDX-License-Identifier: BSD-3-Clause
/*
 * Copyright 2020 NXP
 */
#include <dt-bindings/clock/s32gen1-clock.h>
#include <linux/kernel.h>
#include <s32gen1_clk_dump.h>

#define CLK_NAME(ID_DEF)     \
	S32GEN1_CLK_NAME_INIT(ID_DEF, #ID_DEF)

static struct s32gen1_clk_blk s32gen1_clk_blks[] = {
	/* Oscillators */
	CLK_NAME(S32GEN1_CLK_FIRC),
	CLK_NAME(S32GEN1_CLK_SIRC),
	CLK_NAME(S32GEN1_CLK_FXOSC),
	/* ARM PLL */
	CLK_NAME(S32GEN1_CLK_ARM_PLL_MUX),
	CLK_NAME(S32GEN1_CLK_ARM_PLL_VCO),
	CLK_NAME(S32GEN1_CLK_ARM_PLL_PHI0),
	/* ARM DFS */
	CLK_NAME(S32GEN1_CLK_ARM_PLL_DFS1),
	CLK_NAME(S32GEN1_CLK_ARM_PLL_DFS2),
	/* ARM - MC_CGM1 */
	CLK_NAME(S32GEN1_CLK_MC_CGM1_MUX0),
	CLK_NAME(S32GEN1_CLK_A53_CORE),
	CLK_NAME(S32GEN1_CLK_A53_CORE_DIV2),
	CLK_NAME(S32GEN1_CLK_A53_CORE_DIV10),
	/* ARM - MC_CGM0 */
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX0),
	CLK_NAME(S32GEN1_CLK_XBAR_2X),
	CLK_NAME(S32GEN1_CLK_XBAR),
	CLK_NAME(S32GEN1_CLK_XBAR_DIV2),
	CLK_NAME(S32GEN1_CLK_XBAR_DIV3),
	CLK_NAME(S32GEN1_CLK_XBAR_DIV4),
	CLK_NAME(S32GEN1_CLK_XBAR_DIV6),
	/* PERIPH PLL */
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_MUX),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_VCO),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_PHI0),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_PHI1),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_PHI2),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_PHI3),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_PHI4),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_PHI5),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_PHI7),
	/* PERIPH DFS */
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_DFS1),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_DFS2),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_DFS3),
	CLK_NAME(S32GEN1_CLK_PERIPH_PLL_DFS5),
	/* PERIPH - MC_CGM0 */
	CLK_NAME(S32GEN1_CLK_SERDES_REF),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX3),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX4),
	CLK_NAME(S32GEN1_CLK_FTM0_EXT_REF),
	CLK_NAME(S32GEN1_CLK_FTM0_REF),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX5),
	CLK_NAME(S32GEN1_CLK_FTM1_EXT_REF),
	CLK_NAME(S32GEN1_CLK_FTM1_REF),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX6),
	CLK_NAME(S32GEN1_CLK_FLEXRAY_PE),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX7),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX8),
	CLK_NAME(S32GEN1_CLK_LIN_BAUD),
	CLK_NAME(S32GEN1_CLK_LINFLEXD),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX10),
	CLK_NAME(S32GEN1_CLK_GMAC0_TX),
	CLK_NAME(S32GEN1_CLK_GMAC0_EXT_TS),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX9),
	CLK_NAME(S32GEN1_CLK_GMAC0_TS),
	CLK_NAME(S32GEN1_CLK_GMAC0_EXT_TX),
	CLK_NAME(S32GEN1_CLK_GMAC0_EXT_REF),
	CLK_NAME(S32GEN1_CLK_SERDES0_LANE0_CDR),
	CLK_NAME(S32GEN1_CLK_SERDES0_LANE0_TX),
	CLK_NAME(S32GEN1_CLK_GMAC0_EXT_RX),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX11),
	CLK_NAME(S32GEN1_CLK_GMAC0_RX),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX15),
	CLK_NAME(S32GEN1_CLK_GMAC0_REF_DIV),
	CLK_NAME(S32GEN1_CLK_GMAC0_REF),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX16),
	CLK_NAME(S32GEN1_CLK_SPI),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX12),
	CLK_NAME(S32GEN1_CLK_QSPI_2X),
	CLK_NAME(S32GEN1_CLK_QSPI),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX14),
	CLK_NAME(S32GEN1_CLK_SDHC),
	/* DDR PLL */
	CLK_NAME(S32GEN1_CLK_DDR_PLL_MUX),
	CLK_NAME(S32GEN1_CLK_DDR_PLL_VCO),
	CLK_NAME(S32GEN1_CLK_DDR_PLL_PHI0),
	CLK_NAME(S32GEN1_CLK_MC_CGM5_MUX0),
	CLK_NAME(S32GEN1_CLK_DDR),
	/* ACCEL PLL */
	CLK_NAME(S32GEN1_CLK_ACCEL_PLL_MUX),
	CLK_NAME(S32GEN1_CLK_ACCEL_PLL_VCO),
	/* CLKOUT */
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX1),
	CLK_NAME(S32GEN1_CLK_CLKOUT0),
	CLK_NAME(S32GEN1_CLK_MC_CGM0_MUX2),
	CLK_NAME(S32GEN1_CLK_CLKOUT1),
};

const char *clk_name(const char *name)
{
	if (!name)
		return NULL;

	return name + sizeof(S32GEN1_CLK_PREFIX) - 1;
}

struct s32gen1_clk_blk *s32gen1_get_clk_blk(u32 id)
{
	if (id < ARRAY_SIZE(s32gen1_clk_blks))
		return &s32gen1_clk_blks[id];

	id -= ARRAY_SIZE(s32gen1_clk_blks);

	return s32gen1_get_plat_clk_blk(id);
}

const char *s32gen1_get_clock_name(struct s32gen1_clk_blk *clk)
{
	if (!clk)
		return NULL;

	if (!(clk >= &s32gen1_clk_blks[0] &&
	      clk <= (&s32gen1_clk_blks[0] + ARRAY_SIZE(s32gen1_clk_blks))))
		return s32gen1_get_plat_clock_name(clk);

	return clk_name(clk->name);
}
