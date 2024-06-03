#!/bin/bash

ttyUSB=$( ls /dev/ttyUSB* | cut -d '/' -f 3 )

for ttyUSB in ${ttyUSB}
do
	com=$(udevadm info -n ${ttyUSB} | grep 'ID_USB_DRIVER' | cut -d '=' -f 2)
	if [ "${com}" == 'ch340' ] || [ "${com}" == 'ch341' ] || [ "${com}" == 'ftdi_sio' ];then
		echo ${ttyUSB}
	fi
done
