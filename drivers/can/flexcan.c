/*
 * Copyright (c) 2015 Benoît Taine.
 *
 * Base on Linux kernel source code.
 *
 * Copyright (c) 2005-2006 Varma Electronics Oy.
 * Copyright (c) 2009 Sascha Hauer, Pengutronix.
 * Copyright (c) 2010 Marc Kleine-Budde, Pengutronix.
 *
 * Based on code originally by Andrey Volkov <avolkov@varma-el.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * @file rbd.c
 * @author Benoît Taine (benoit.taine@openwide.fr)
 * @brief Freescale FlexCAN CAN controller driver.
 */

#define DRV_NAME		"flexcan"

#define MODULE_DESC		"Freescale FlexCAN CAN controller driver"
#define MODULE_AUTHOR		"Benoit Taine"
#define MODULE_LICENSE		"GPL"
#define MODULE_IPRIORITY	(VMM_CAN_CLASS_IPRIORITY + 1)
#define MODULE_INIT		flexcan_driver_init
#define MODULE_EXIT		flexcan_driver_exit

/* FLEXCAN module configuration register (CANMCR) bits */
#define FLEXCAN_MCR_MDIS		BIT(31)
#define FLEXCAN_MCR_FRZ			BIT(30)
#define FLEXCAN_MCR_FEN			BIT(29)
#define FLEXCAN_MCR_HALT		BIT(28)
#define FLEXCAN_MCR_NOT_RDY		BIT(27)
#define FLEXCAN_MCR_WAK_MSK		BIT(26)
#define FLEXCAN_MCR_SOFTRST		BIT(25)
#define FLEXCAN_MCR_FRZ_ACK		BIT(24)
#define FLEXCAN_MCR_SUPV		BIT(23)
#define FLEXCAN_MCR_SLF_WAK		BIT(22)
#define FLEXCAN_MCR_WRN_EN		BIT(21)
#define FLEXCAN_MCR_LPM_ACK		BIT(20)
#define FLEXCAN_MCR_WAK_SRC		BIT(19)
#define FLEXCAN_MCR_DOZE		BIT(18)
#define FLEXCAN_MCR_SRX_DIS		BIT(17)
#define FLEXCAN_MCR_BCC			BIT(16)
#define FLEXCAN_MCR_LPRIO_EN		BIT(13)
#define FLEXCAN_MCR_AEN			BIT(12)
#define FLEXCAN_MCR_MAXMB(x)		((x) & 0x7f)
#define FLEXCAN_MCR_IDAM_A		(0 << 8)
#define FLEXCAN_MCR_IDAM_B		(1 << 8)
#define FLEXCAN_MCR_IDAM_C		(2 << 8)
#define FLEXCAN_MCR_IDAM_D		(3 << 8)

/* FLEXCAN control register (CANCTRL) bits */
#define FLEXCAN_CTRL_PRESDIV(x)		(((x) & 0xff) << 24)
#define FLEXCAN_CTRL_RJW(x)		(((x) & 0x03) << 22)
#define FLEXCAN_CTRL_PSEG1(x)		(((x) & 0x07) << 19)
#define FLEXCAN_CTRL_PSEG2(x)		(((x) & 0x07) << 16)
#define FLEXCAN_CTRL_BOFF_MSK		BIT(15)
#define FLEXCAN_CTRL_ERR_MSK		BIT(14)
#define FLEXCAN_CTRL_CLK_SRC		BIT(13)
#define FLEXCAN_CTRL_LPB		BIT(12)
#define FLEXCAN_CTRL_TWRN_MSK		BIT(11)
#define FLEXCAN_CTRL_RWRN_MSK		BIT(10)
#define FLEXCAN_CTRL_SMP		BIT(7)
#define FLEXCAN_CTRL_BOFF_REC		BIT(6)
#define FLEXCAN_CTRL_TSYN		BIT(5)
#define FLEXCAN_CTRL_LBUF		BIT(4)
#define FLEXCAN_CTRL_LOM		BIT(3)
#define FLEXCAN_CTRL_PROPSEG(x)		((x) & 0x07)
#define FLEXCAN_CTRL_ERR_BUS		(FLEXCAN_CTRL_ERR_MSK)
#define FLEXCAN_CTRL_ERR_STATE \
	(FLEXCAN_CTRL_TWRN_MSK | FLEXCAN_CTRL_RWRN_MSK | \
	 FLEXCAN_CTRL_BOFF_MSK)
#define FLEXCAN_CTRL_ERR_ALL \
	(FLEXCAN_CTRL_ERR_BUS | FLEXCAN_CTRL_ERR_STATE)

/* FLEXCAN control register 2 (CTRL2) bits */
#define FLEXCAN_CTRL2_ECRWRE		BIT(29)
#define FLEXCAN_CTRL2_WRMFRZ		BIT(28)
#define FLEXCAN_CTRL2_RFFN(x)		(((x) & 0x0f) << 24)
#define FLEXCAN_CTRL2_TASD(x)		(((x) & 0x1f) << 19)
#define FLEXCAN_CTRL2_MRP		BIT(18)
#define FLEXCAN_CTRL2_RRS		BIT(17)
#define FLEXCAN_CTRL2_EACEN		BIT(16)

/* FLEXCAN memory error control register (MECR) bits */
#define FLEXCAN_MECR_ECRWRDIS		BIT(31)
#define FLEXCAN_MECR_HANCEI_MSK		BIT(19)
#define FLEXCAN_MECR_FANCEI_MSK		BIT(18)
#define FLEXCAN_MECR_CEI_MSK		BIT(16)
#define FLEXCAN_MECR_HAERRIE		BIT(15)
#define FLEXCAN_MECR_FAERRIE		BIT(14)
#define FLEXCAN_MECR_EXTERRIE		BIT(13)
#define FLEXCAN_MECR_RERRDIS		BIT(9)
#define FLEXCAN_MECR_ECCDIS		BIT(8)
#define FLEXCAN_MECR_NCEFAFRZ		BIT(7)

/* FLEXCAN error and status register (ESR) bits */
#define FLEXCAN_ESR_TWRN_INT		BIT(17)
#define FLEXCAN_ESR_RWRN_INT		BIT(16)
#define FLEXCAN_ESR_BIT1_ERR		BIT(15)
#define FLEXCAN_ESR_BIT0_ERR		BIT(14)
#define FLEXCAN_ESR_ACK_ERR		BIT(13)
#define FLEXCAN_ESR_CRC_ERR		BIT(12)
#define FLEXCAN_ESR_FRM_ERR		BIT(11)
#define FLEXCAN_ESR_STF_ERR		BIT(10)
#define FLEXCAN_ESR_TX_WRN		BIT(9)
#define FLEXCAN_ESR_RX_WRN		BIT(8)
#define FLEXCAN_ESR_IDLE		BIT(7)
#define FLEXCAN_ESR_TXRX		BIT(6)
#define FLEXCAN_EST_FLT_CONF_SHIFT	(4)
#define FLEXCAN_ESR_FLT_CONF_MASK	(0x3 << FLEXCAN_EST_FLT_CONF_SHIFT)
#define FLEXCAN_ESR_FLT_CONF_ACTIVE	(0x0 << FLEXCAN_EST_FLT_CONF_SHIFT)
#define FLEXCAN_ESR_FLT_CONF_PASSIVE	(0x1 << FLEXCAN_EST_FLT_CONF_SHIFT)
#define FLEXCAN_ESR_BOFF_INT		BIT(2)
#define FLEXCAN_ESR_ERR_INT		BIT(1)
#define FLEXCAN_ESR_WAK_INT		BIT(0)
#define FLEXCAN_ESR_ERR_BUS \
	(FLEXCAN_ESR_BIT1_ERR | FLEXCAN_ESR_BIT0_ERR | \
	 FLEXCAN_ESR_ACK_ERR | FLEXCAN_ESR_CRC_ERR | \
	 FLEXCAN_ESR_FRM_ERR | FLEXCAN_ESR_STF_ERR)
#define FLEXCAN_ESR_ERR_STATE \
	(FLEXCAN_ESR_TWRN_INT | FLEXCAN_ESR_RWRN_INT | FLEXCAN_ESR_BOFF_INT)
#define FLEXCAN_ESR_ERR_ALL \
	(FLEXCAN_ESR_ERR_BUS | FLEXCAN_ESR_ERR_STATE)
#define FLEXCAN_ESR_ALL_INT \
	(FLEXCAN_ESR_TWRN_INT | FLEXCAN_ESR_RWRN_INT | \
	 FLEXCAN_ESR_BOFF_INT | FLEXCAN_ESR_ERR_INT)

/* FLEXCAN interrupt flag register (IFLAG) bits */
/* Errata ERR005829 step7: Reserve first valid MB */
#define FLEXCAN_TX_BUF_RESERVED		8
#define FLEXCAN_TX_BUF_ID		9
#define FLEXCAN_IFLAG_BUF(x)		BIT(x)
#define FLEXCAN_IFLAG_RX_FIFO_OVERFLOW	BIT(7)
#define FLEXCAN_IFLAG_RX_FIFO_WARN	BIT(6)
#define FLEXCAN_IFLAG_RX_FIFO_AVAILABLE	BIT(5)
#define FLEXCAN_IFLAG_DEFAULT \
	(FLEXCAN_IFLAG_RX_FIFO_OVERFLOW | FLEXCAN_IFLAG_RX_FIFO_AVAILABLE | \
	 FLEXCAN_IFLAG_BUF(FLEXCAN_TX_BUF_ID))

/* FLEXCAN message buffers */
#define FLEXCAN_MB_CODE_RX_INACTIVE	(0x0 << 24)
#define FLEXCAN_MB_CODE_RX_EMPTY	(0x4 << 24)
#define FLEXCAN_MB_CODE_RX_FULL		(0x2 << 24)
#define FLEXCAN_MB_CODE_RX_OVERRRUN	(0x6 << 24)
#define FLEXCAN_MB_CODE_RX_RANSWER	(0xa << 24)

#define FLEXCAN_MB_CODE_TX_INACTIVE	(0x8 << 24)
#define FLEXCAN_MB_CODE_TX_ABORT	(0x9 << 24)
#define FLEXCAN_MB_CODE_TX_DATA		(0xc << 24)
#define FLEXCAN_MB_CODE_TX_TANSWER	(0xe << 24)

#define FLEXCAN_MB_CNT_SRR		BIT(22)
#define FLEXCAN_MB_CNT_IDE		BIT(21)
#define FLEXCAN_MB_CNT_RTR		BIT(20)
#define FLEXCAN_MB_CNT_LENGTH(x)	(((x) & 0xf) << 16)
#define FLEXCAN_MB_CNT_TIMESTAMP(x)	((x) & 0xffff)

#define FLEXCAN_MB_CODE_MASK		(0xf0ffffff)

#define FLEXCAN_TIMEOUT_US             (50)

/*
 * FLEXCAN hardware feature flags
 *
 * Below is some version info we got:
 *    SOC   Version   IP-Version  Glitch- [TR]WRN_INT Memory err RTR re-
 *                                Filter? connected?  detection  ception in MB
 *   MX25  FlexCAN2  03.00.00.00     no        no         no        no
 *   MX28  FlexCAN2  03.00.04.00    yes       yes         no        no
 *   MX35  FlexCAN2  03.00.00.00     no        no         no        no
 *   MX53  FlexCAN2  03.00.00.00    yes        no         no        no
 *   MX6s  FlexCAN3  10.00.12.00    yes       yes         no       yes
 *   VF610 FlexCAN3  ?               no       yes        yes       yes?
 *
 * Some SOCs do not have the RX_WARN & TX_WARN interrupt line connected.
 */
#define FLEXCAN_HAS_V10_FEATURES	BIT(1) /* For core version >= 10 */
#define FLEXCAN_HAS_BROKEN_ERR_STATE	BIT(2) /* [TR]WRN_INT not connected */
#define FLEXCAN_HAS_MECR_FEATURES	BIT(3) /* Memory error detection */

/* Structure of the message buffer */
struct flexcan_mb {
	u32 can_ctrl;
	u32 can_id;
	u32 data[2];
};

/* Structure of the hardware registers */
struct flexcan_regs {
	u32 mcr;		/* 0x00 */
	u32 ctrl;		/* 0x04 */
	u32 timer;		/* 0x08 */
	u32 _reserved1;		/* 0x0c */
	u32 rxgmask;		/* 0x10 */
	u32 rx14mask;		/* 0x14 */
	u32 rx15mask;		/* 0x18 */
	u32 ecr;		/* 0x1c */
	u32 esr;		/* 0x20 */
	u32 imask2;		/* 0x24 */
	u32 imask1;		/* 0x28 */
	u32 iflag2;		/* 0x2c */
	u32 iflag1;		/* 0x30 */
	u32 ctrl2;		/* 0x34 */
	u32 esr2;		/* 0x38 */
	u32 imeur;		/* 0x3c */
	u32 lrfr;		/* 0x40 */
	u32 crcr;		/* 0x44 */
	u32 rxfgmask;		/* 0x48 */
	u32 rxfir;		/* 0x4c */
	u32 _reserved3[12];	/* 0x50 */
	struct flexcan_mb cantxfg[64];	/* 0x80 */
	/* FIFO-mode:
	 *			MB
	 * 0x080...0x08f	0	RX message buffer
	 * 0x090...0x0df	1-5	reserverd
	 * 0x0e0...0x0ff	6-7	8 entry ID table
	 *				(mx25, mx28, mx35, mx53)
	 * 0x0e0...0x2df	6-7..37	8..128 entry ID table
	 *			  	size conf'ed via ctrl2::RFFN
	 *				(mx6, vf610)
	 */
	u32 _reserved4[408];
	u32 mecr;		/* 0xae0 */
	u32 erriar;		/* 0xae4 */
	u32 erridpr;		/* 0xae8 */
	u32 errippr;		/* 0xaec */
	u32 rerrar;		/* 0xaf0 */
	u32 rerrdr;		/* 0xaf4 */
	u32 rerrsynr;		/* 0xaf8 */
	u32 errsr;		/* 0xafc */
};

struct flexcan_devtype_data {
	u32 features;	/* hardware controller features */
};

struct flexcan_priv {
	struct can_priv can;
	struct napi_struct napi;

	void __iomem *base;
	u32 reg_esr;
	u32 reg_ctrl_default;

	struct clk *clk_ipg;
	struct clk *clk_per;
	struct flexcan_platform_data *pdata;
	const struct flexcan_devtype_data *devtype_data;
	struct regulator *reg_xceiver;

	/* Read and Write APIs */
	u32 (*read)(void __iomem *addr);
	void (*write)(u32 val, void __iomem *addr);
};

static struct flexcan_devtype_data fsl_p1010_devtype_data = {
	.features = FLEXCAN_HAS_BROKEN_ERR_STATE,
};
static struct flexcan_devtype_data fsl_imx28_devtype_data;
static struct flexcan_devtype_data fsl_imx6q_devtype_data = {
	.features = FLEXCAN_HAS_V10_FEATURES,
};
static struct flexcan_devtype_data fsl_vf610_devtype_data = {
	.features = FLEXCAN_HAS_V10_FEATURES | FLEXCAN_HAS_MECR_FEATURES,
};

static const struct can_bittiming_const flexcan_bittiming_const = {
	.name = DRV_NAME,
	.tseg1_min = 4,
	.tseg1_max = 16,
	.tseg2_min = 2,
	.tseg2_max = 8,
	.sjw_max = 4,
	.brp_min = 1,
	.brp_max = 256,
	.brp_inc = 1,
};

/*
 * FlexCAN module is essentially modelled as a little-endian IP in most
 * SoCs, i.e the registers as well as the message buffer areas are
 * implemented in a little-endian fashion.
 *
 * However there are some SoCs (e.g. LS1021A) which implement the FlexCAN
 * module in a big-endian fashion (i.e the registers as well as the
 * message buffer areas are implemented in a big-endian way).
 *
 * In addition, the FlexCAN module can be found on SoCs having ARM or
 * PPC cores. So, we need to abstract off the register read/write
 * functions, ensuring that these cater to all the combinations of module
 * endianness and underlying CPU endianness.
 */
static inline u32 flexcan_read_le(void __iomem *addr)
{
	return ioread32(addr);
}

static inline void flexcan_write_le(u32 val, void __iomem *addr)
{
	iowrite32(val, addr);
}

static inline u32 flexcan_read_be(void __iomem *addr)
{
	return ioread32be(addr);
}

static inline void flexcan_write_be(u32 val, void __iomem *addr)
{
	iowrite32be(val, addr);
}

static const struct vmm_devtree_nodeid flexcan_of_match[] = {
	{ .compatible = "fsl,imx6q-flexcan", .data = &fsl_imx6q_devtype_data, },
	{ .compatible = "fsl,imx28-flexcan", .data = &fsl_imx28_devtype_data, },
	{ .compatible = "fsl,p1010-flexcan", .data = &fsl_p1010_devtype_data, },
	{ .compatible = "fsl,vf610-flexcan", .data = &fsl_vf610_devtype_data, },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, flexcan_of_match);

static struct vmm_driver flexcan_driver = {
	.name		= DRV_NAME,
	.match_table	= flexcan_of_match,
	.probe		= flexcan_probe,
	.remove		= flexcan_remove,
};

static int __init flexcan_driver_init(void)
{
	return vmm_devdrv_register_driver(&flexcan_driver);
}

static void __exit flexcan_driver_exit(void)
{
	vmm_devdrv_unregister_driver(&flexcan_driver);
}

VMM_DECLARE_MODULE(MODULE_DESC,
	MODULE_AUTHOR,
	MODULE_LICENSE,
	MODULE_IPRIORITY,
	MODULE_INIT,
	MODULE_EXIT);
