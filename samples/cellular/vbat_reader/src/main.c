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
		nrf_modem_lib_shutdown();
		return 0;
	}

	printk("Response: %s", response);

	/* AT%XRFTEST loop for band 12 (729-746 MHz) */
	printk("\nStarting AT%%XRFTEST loop for band 12 (729-746 MHz)\n");
	
	/* Band 12, LTE-M mode, test each MHz from 729 to 746 */
	for (int freq_mhz = 729; freq_mhz <= 746; freq_mhz++) {
		int freq_100khz = freq_mhz * 10;  /* Convert MHz to 100 kHz units */
		
		/* RX ON command: AT%XRFTEST=0,1,<band>,<freq>,<power>,<mode> */
		printk("\n--- Testing %d MHz ---\n", freq_mhz);
		printk("Sending AT%%XRFTEST=0,1,12,%d,-65,1 (RX ON)\n", freq_100khz);
		
		err = nrf_modem_at_cmd(response, sizeof(response), 
		                       "AT%%XRFTEST=0,1,12,%d,-65,1", freq_100khz);
		if (err) {
			printk("AT%%XRFTEST RX ON failed at %d MHz, error: %d\n", 
			       freq_mhz, err);
			/* Skip OFF command if ON failed, continue to next frequency */
			continue;
		}
		
		printk("Response: %s", response);
		
		/* Small delay between ON and OFF */
		k_msleep(100);
		
		/* RX OFF command: AT%XRFTEST=0,0 */
		err = nrf_modem_at_cmd(response, sizeof(response), "AT%%XRFTEST=0,0");
		if (err) {
			printk("AT%%XRFTEST RX OFF failed at %d MHz, error: %d\n", 
			       freq_mhz, err);
			/* Try to force OFF before continuing */
			k_msleep(100);
			nrf_modem_at_cmd(response, sizeof(response), "AT%%XRFTEST=0,0");
		}
	}
	
	printk("\nAT%%XRFTEST loop completed\n");

	printk("Shutting down modem\n");
	nrf_modem_lib_shutdown();

	return 0;
}
