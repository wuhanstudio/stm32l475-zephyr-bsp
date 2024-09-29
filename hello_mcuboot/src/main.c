/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

int main(void)
{
	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

	const char *buildString = "This build " CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION " was compiled at " __DATE__ ", " __TIME__ ".";
	printf("%s\n", buildString);

	return 0;
}
