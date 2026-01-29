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
	
	/* Outer loop: test different input power levels */
	int power_levels[] = {-65, -75, -85, -95, -105, -115, -125};
	int num_power_levels = sizeof(power_levels) / sizeof(power_levels[0]);
	
	for (int p = 0; p < num_power_levels; p++) {
		int input_power = power_levels[p];
		
		printk("\n=== Testing with input power: %d dBm ===\n", input_power);
		printk("Frequency: Power (dBm), Headroom (dBFS)\n");
		
		/* Band 12, LTE-M mode, test each MHz from 729 to 746 */
		for (int freq_mhz = 729; freq_mhz <= 746; freq_mhz++) {
			int freq_100khz = freq_mhz * 10;  /* Convert MHz to 100 kHz units */
			int antenna_power_q8;
			int headroom_dbfs;
			
			/* RX ON command: AT%XRFTEST=0,1,<band>,<freq>,<power>,<mode> */
			err = nrf_modem_at_cmd(response, sizeof(response), 
			                       "AT%%XRFTEST=0,1,12,%d,%d,1", freq_100khz, input_power);
			if (err) {
				printk("%d MHz: RX ON failed (error: %d)\n", freq_mhz, err);
				/* Skip OFF command if ON failed, continue to next frequency */
				continue;
			}
			
			/* Parse response: %XRFTEST: <antenna_power>,<headroom> */
			if (sscanf(response, "%%XRFTEST: %d,%d", &antenna_power_q8, &headroom_dbfs) == 2) {
				/* Convert antenna_power from q8 format to dBm (divide by 256) */
				/* Since printk doesn't support %f, split into integer and fractional parts */
				int power_dbm_int = antenna_power_q8 / 256;
				int power_dbm_frac = ((antenna_power_q8 % 256) * 100) / 256;
				if (power_dbm_frac < 0) {
					power_dbm_frac = -power_dbm_frac;
				}
				printk("%d MHz: %d.%02d dBm, %d dBFS\n", freq_mhz, power_dbm_int, power_dbm_frac, headroom_dbfs);
			} else {
				printk("%d MHz: Failed to parse response\n", freq_mhz);
			}
			
			/* Small delay between ON and OFF */
			k_msleep(100);
			
			/* RX OFF command: AT%XRFTEST=0,0 */
			err = nrf_modem_at_cmd(response, sizeof(response), "AT%%XRFTEST=0,0");
			if (err) {
				/* Try to force OFF before continuing */
				k_msleep(100);
				nrf_modem_at_cmd(response, sizeof(response), "AT%%XRFTEST=0,0");
			}
		}
	}
	
	printk("\nAT%%XRFTEST loop completed\n");

	printk("Shutting down modem\n");
	nrf_modem_lib_shutdown();

	return 0;
}
