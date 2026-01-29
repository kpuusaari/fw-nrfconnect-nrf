/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <stdio.h>
#include <nrf_modem_at.h>
#include <modem/nrf_modem_lib.h>

int main(void)
{
	int err;
	char response[128];

	printk("VBAT Reader sample started\n");

	err = nrf_modem_lib_init();
	if (err) {
		printk("Modem library initialization failed, error: %d\n", err);
		return 0;
	}

	printk("Sending AT%%XVBAT command...\n");
	
	err = nrf_modem_at_cmd(response, sizeof(response), "AT%%XVBAT");
	if (err) {
		printk("AT%%XVBAT command failed, error: %d\n", err);
		return 0;
	}

	printk("Response: %s", response);

	return 0;
}
