.. _vbat_reader_sample:

Cellular: VBAT Reader
#####################

.. contents::
   :local:
   :depth: 2

The VBAT Reader sample demonstrates how to send the AT%XVBAT command to the nRF9151 device to read the battery voltage and print the response to the console.

Requirements
************

The sample supports the following development kits:

.. table-from-sample-yaml::

.. include:: /includes/tfm.txt

Overview
********

The VBAT Reader sample initializes the modem library and sends the AT%XVBAT command to query the battery voltage.
The response from the modem is then printed to the console using printk.

The AT%XVBAT command returns the battery voltage measurement from the modem.

Building and running
********************

.. |sample path| replace:: :file:`samples/cellular/vbat_reader`

.. include:: /includes/build_and_run_ns.txt


Testing
=======

After programming the sample to your development kit, test it by performing the following steps:

1. Press the reset button on the nRF91 Series DK to reboot the kit and start the VBAT Reader sample.
#. :ref:`Connect to the nRF91 Series DK with the Serial Terminal app <serial_terminal_connect>`.
#. Observe the output showing the AT%XVBAT command response.


Sample output
=============

The sample will output something similar to:

.. code-block:: console

   VBAT Reader sample started
   Sending AT%XVBAT command...
   Response: %XVBAT: 3500
   OK


Dependencies
************

This sample uses the following |NCS| libraries:

* :ref:`nrfxlib:nrf_modem`

In addition, it uses the following secure firmware component:

* :ref:`Trusted Firmware-M <ug_tfm>`
