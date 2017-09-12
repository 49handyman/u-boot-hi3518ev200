/******************************************************************************
 *	Flash Memory Controller v100 Driver
 *	Copyright (c) 2015 by Hisilicon.
 *	All rights reserved.
 * ***
 *	Create by PBQ. 2015-4-28
 *
 *****************************************************************************/

#ifndef __HIFMC100_NAND_H__
#define __HIFMC100_NAND_H__

/******************************************************************************/
/* These macroes are for debug only, reg option is slower then dma option */
#undef HIFMC100_NAND_SUPPORT_REG_READ
/* #define HIFMC100_NAND_SUPPORT_REG_READ */

#undef HIFMC100_NAND_SUPPORT_REG_WRITE
/* #define HIFMC100_NAND_SUPPORT_REG_WRITE */

/*****************************************************************************/
#define CONFIG_SUPPORT_YAFFS
#define HIFMC100_NAND_OOBSIZE_FOR_YAFFS		32

/*****************************************************************************/
#define REG_CNT_HIGH_BLOCK_NUM_SHIFT		10

#define REG_CNT_BLOCK_NUM_MASK			0x3ff
#define REG_CNT_BLOCK_NUM_SHIFT			22

#define REG_CNT_PAGE_NUM_MASK			0x3f
#define REG_CNT_PAGE_NUM_SHIFT			16

#define REG_CNT_WRAP_MASK			0xf
#define REG_CNT_WRAP_SHIFT			12

#define REG_CNT_ECC_OFFSET_MASK			0xfff
#define REG_CNT_ECC_8BIT_OFFSET			1054
#define REG_CNT_ECC_16BIT_OFFSET		1056
#define REG_CNT_ECC_24BIT_OFFSET		1082

/*****************************************************************************/
#define NAND_BUFFER_LEN				(2048 + 128)

#define HIFMC100_ADDR_CYCLE_MASK		0x4

/*****************************************************************************/
struct hifmc_host {
	struct nand_chip *chip;
	struct mtd_info  *mtd;

	struct hifmc_cmd_op cmd_op;
	void __iomem *regbase;

	/* Controller config option nand flash */
	unsigned int nand_cfg;
	unsigned int nand_cfg_ecc0;

	unsigned int offset;
	/* This is maybe an un-aligment address, only for malloc or free */
	char *buforg;
	char *buffer;

	unsigned int dma_buffer;
	unsigned int dma_oob;

	unsigned int addr_cycle;
	unsigned int addr_value[2];
	unsigned int cache_addr_value[2];

	unsigned int column;
	unsigned int block_page_mask;

	unsigned int ecctype;
	unsigned int pagesize;
	unsigned int oobsize;

	int  need_rr_data;
#define HIFMC100_READ_RETRY_DATA_LEN         128
	char rr_data[HIFMC100_READ_RETRY_DATA_LEN];
	struct read_retry_t *read_retry;

	int  version;

	/* BOOTROM read two bytes to detect the bad block flag */
#define HIFMC100_BAD_BLOCK_POS              0
	unsigned char *bbm;  /* nand bad block mark */
	unsigned short *epm;  /* nand empty page mark */
	unsigned int flags;

#define HIFMC100_PS_UC_ECC        0x01 /* page has ecc error */
#define HIFMC100_PS_BAD_BLOCK     0x02 /* bad block */
#define HIFMC100_PS_EMPTY_PAGE    0x04 /* page is empty */
#define HIFMC100_PS_EPM_ERROR     0x0100 /* empty page mark word has error. */
#define HIFMC100_PS_BBM_ERROR     0x0200 /* bad block mark word has error. */
	unsigned int page_status;

	void (*send_cmd_pageprog)(struct hifmc_host *host);
	void (*send_cmd_status)(struct hifmc_host *host);
	void (*send_cmd_readstart)(struct hifmc_host *host);
	void (*send_cmd_erase)(struct hifmc_host *host);
	void (*send_cmd_readid)(struct hifmc_host *host);
	void (*send_cmd_reset)(struct hifmc_host *host);

	void (*enable_ecc_randomizer)(struct hifmc_host *host,
			int ecc_en, int randomizer_en);

	void (*detect_ecc)(struct hifmc_host *host);
};

/*****************************************************************************/
int hifmc100_nand_init(struct nand_chip *chip);

/*****************************************************************************/
extern void hifmc_spl_ids_register(void);

extern void hifmc100_nand_controller_enable(int enable);

#endif /* End of__HIFMC100_NAND_H__ */

