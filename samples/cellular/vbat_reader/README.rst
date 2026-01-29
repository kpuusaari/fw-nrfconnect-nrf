.. _vbat_reader_sample:

Cellular: VBAT Reader and RF Test
##################################

.. contents::
   :local:
   :depth: 2

The VBAT Reader sample demonstrates how to send the AT%XVBAT command to an nRF91 Series device to read the battery voltage and print the response to the console. Additionally, it performs RF receiver tests across LTE Band 12 frequencies.

Requirements
************

The sample supports the following development kits:

.. table-from-sample-yaml::

.. include:: /includes/tfm.txt

Overview
********

The VBAT Reader sample initializes the modem library and performs two main operations:

1. **Battery Voltage Query**: Sends the AT%XVBAT command to query the battery voltage. The response from the modem is printed to the console using printk.

2. **RF Receiver Test**: Performs a systematic test of the RF receiver across LTE Band 12 downlink frequencies (729-746 MHz). For each 1 MHz frequency step in this range, the sample:
   
   - Sends an AT%XRFTEST RX ON command to activate the receiver
   - Prints the response showing received signal measurements
   - Sends an AT%XRFTEST RX OFF command to deactivate the receiver

The AT%XVBAT command returns the battery voltage measurement from the modem, while the AT%XRFTEST command enables RF receiver testing in LTE-M mode.

Building and running
********************

.. |sample path| replace:: :file:`samples/cellular/vbat_reader`

.. include:: /includes/build_and_run_ns.txt

To build the sample for the nRF9151 DK, use the following command:

.. code-block:: console

   west build -b nrf9151dk/nrf9151/ns


Testing
=======

After programming the sample to your development kit, test it by performing the following steps:

1. Press the reset button on the nRF91 Series DK to reboot the kit and start the sample.
#. :ref:`Connect to the nRF91 Series DK with the Serial Terminal app <serial_terminal_connect>`.
#. Observe the output showing the AT%XVBAT command response.
#. Observe the AT%XRFTEST loop testing each frequency from 729 MHz to 746 MHz in Band 12.


Sample output
=============

The sample will output something similar to:

.. code-block:: console

   VBAT Reader sample started
   Sending AT%XVBAT command...
   Response: %XVBAT: 3500
   OK

   Starting AT%XRFTEST loop for band 12 (729-746 MHz)
   Frequency: Power (dBm), Headroom (dBFS)
   729 MHz: -65.00 dBm, -15 dBFS
   730 MHz: -66.00 dBm, -15 dBFS
   731 MHz: -65.50 dBm, -15 dBFS
   732 MHz: -67.25 dBm, -16 dBFS
   733 MHz: -66.75 dBm, -15 dBFS
   [... continues for each MHz from 734 to 746 ...]

   AT%XRFTEST loop completed
   Shutting down modem

The power values are converted from q8 format (divided by 256) to display the received signal strength in dBm.
The headroom values indicate the receiver's dynamic range margin in dBFS (decibels relative to full scale).


Dependencies
************

This sample uses the following |NCS| libraries:

* :ref:`nrfxlib:nrf_modem`

In addition, it uses the following secure firmware component:

* :ref:`Trusted Firmware-M <ug_tfm>`
