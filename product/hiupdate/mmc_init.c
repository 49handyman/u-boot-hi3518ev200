static int mmc_stor_init(void)
{
	struct mmc *mmc;

#ifdef CONFIG_ARCH_HI3519
	mmc = find_mmc_device(2);
#else
	mmc = find_mmc_device(0);
#endif
	if (NULL == mmc) {
		printf("No mmc driver found!\n");
		return -1;
	}

	if ((0 == (unsigned int)mmc->block_dev.vendor[0])
			|| (0 == (unsigned int)mmc->block_dev.product[0])) {
		printf("*No SD card found!\n");
		return -1;
	}

	return 0;
}

static void mmc_stor_exit(void)
{
}
