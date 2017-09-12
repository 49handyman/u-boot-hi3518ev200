/******************************************************************************
 *	Flash Memory Controller v100 Device Driver
 *	Copyright (c) 2014 - 2015 by Hisilicon
 *	All rights reserved.
 * ***
 *	Create by hisilicon
 *
 *****************************************************************************/

#ifndef __HIFMC100_H__
#define __HIFMC100_H__

/*****************************************************************************/
/* These macroes are for debug only, reg read is slower then dma read */
#undef HIFMC100_SPI_NOR_SUPPORT_REG_READ
/* #define HIFMC100_SPI_NOR_SUPPORT_REG_READ */

#undef HIFMC100_SPI_NOR_SUPPORT_REG_WRITE
/* #define HIFMC100_SPI_NOR_SUPPORT_REG_WRITE */

/*****************************************************************************/
#define HIFMC100_DMA_MAX_SIZE			(4096)
#define HIFMC100_DMA_MASK			(HIFMC100_DMA_MAX_SIZE - 1)

/*****************************************************************************/
#define SPI_NOR_CR_SHIFT	8	/* Config Register shift(bit) */

#define SPI_NOR_CR_4BYTE_SHIFT	5
#define SPI_NOR_CR_4BYTE_MASK		(1 << SPI_NOR_CR_4BYTE_SHIFT)
#define SPI_NOR_GET_4BYTE_BY_CR(cr)	(((cr) & SPI_NOR_CR_4BYTE_MASK) \
						>> SPI_NOR_CR_4BYTE_SHIFT)

#define SPI_NOR_CR_QE_SHIFT		1
#define SPI_NOR_CR_QE_MASK		(1 << SPI_NOR_CR_QE_SHIFT)
#define SPI_NOR_GET_QE_BY_CR(cr)	(((cr) & SPI_NOR_CR_QE_MASK) \
						>> SPI_NOR_CR_QE_SHIFT)

#ifdef CONFIG_SPI_BLOCK_PROTECT
#define DEBUG_SPI_NOR_BP	0

#define SPI_NOR_SR_SRWD_SHIFT	7
#define SPI_NOR_SR_SRWD_MASK	(1 << SPI_NOR_SR_SRWD_SHIFT)

#define SPI_NOR_SR_BP0_SHIFT	2
#define SPI_NOR_SR_BP_WIDTH_4	0xf
#define SPI_NOR_SR_BP_MASK_4	(SPI_NOR_SR_BP_WIDTH_4 << SPI_NOR_SR_BP0_SHIFT)

#define SPI_NOR_SR_BP_WIDTH_3	0x7
#define SPI_NOR_SR_BP_MASK_3	(SPI_NOR_SR_BP_WIDTH_3 << SPI_NOR_SR_BP0_SHIFT)

#define SPI_NOR_SR_TB_SHIFT	3
#define SPI_NOR_SR_TB_MASK	(1 << SPI_NOR_SR_TB_SHIFT)

#define SPI_NOR_SR_TB_SHIFT_S	5
#define SPI_NOR_SR_TB_MASK_S	(1 << SPI_NOR_SR_TB_SHIFT_S)

#define SPI_BP_BOTTOM_RDCR_SET_S(config)	((config) | \
					(0x01 << SPI_NOR_SR_TB_SHIFT_S))
#define SPI_BP_BOTTOM_RDCR_SET(config)		((config) | \
					(0x01 << SPI_NOR_SR_TB_SHIFT))

#define SPI_BP_BOTTOM_RDSR_SET_1(bp_num)	(0x1 << (2 + bp_num))
#define SPI_BP_BOTTOM_RDSR_SET_0(bp_num)	(~(0x1 << (2 + bp_num)))

#define LOCK_LEVEL_MAX(bp_num)			(((0x01) << bp_num) - 1)

#define SPI_CMD_RDCR_MX                 0x15    /* MXIC Read Config Register */

#endif /* CONFIG_SPI_BLOCK_PROTECT */

#define SPI_NOR_SR_WIP_MASK	(1 << 0)

/*****************************************************************************/
struct hifmc_host {
	struct spi_flash spi_nor_flash[1];
	struct mtd_info_ex *spi_nor_info;
	struct hifmc_spi spi[CONFIG_SPI_NOR_MAX_CHIP_NUM];

	void *regbase;
	void *iobase;

	void (*set_system_clock)(struct spi_op *op, int clk_en);
	void (*set_host_addr_mode)(struct hifmc_host *host, int enable);

#ifdef CONFIG_SPI_BLOCK_PROTECT
	unsigned int start_addr;
	unsigned int end_addr;
	unsigned char cmp;
	unsigned int bp_num;
	/* the BT bit location, decide the data num count */
	unsigned int bt_loc;
	unsigned char level;
#endif
};

#ifdef CONFIG_SPI_BLOCK_PROTECT
unsigned short hifmc100_set_spi_lock_info(struct hifmc_host *host);
void hifmc100_get_bp_lock_level(struct hifmc_host *host);
void hifmc100_spi_lock(struct hifmc_host *host, unsigned char level);
void hifmc100_spi_flash_lock(unsigned char cmp, unsigned char level,
			unsigned char op);
unsigned short hifmc100_handle_bp_rdcr_info(struct hifmc_host *host,
			u_char cmd);
unsigned char hifmc100_bp_to_level(struct hifmc_host *host);
unsigned short hifmc100_handle_bp_rdsr_info(struct hifmc_host *host,
			u_char cmd);
extern u_char spi_general_get_flash_register(struct hifmc_spi *spi,
			u_char cmd);
#endif

#define SPIFLASH_TO_HOST(_spiflash)	((struct hifmc_host *)(_spiflash))

/*****************************************************************************/
extern unsigned char hifmc_ip_user;

/*****************************************************************************/
void hifmc100_read_ids(struct hifmc_spi *, u_char, u_char *);

extern int hifmc_spi_nor_probe(struct mtd_info_ex *mtd, struct hifmc_spi *spi);

/*****************************************************************************/

#endif /* End of __HIFMC100_H__ */

