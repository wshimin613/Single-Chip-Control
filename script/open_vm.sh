#!/bin/bash

PC=${1}
host_number=$( ls -f /vm_data/xml/ | grep PC | sed s/-1.xml//g | sort | head -n 1 )
path="/vm_data/xml/"
search='/vm_data/script/search-core.sh'
cpu='/vm_data/script/cpuauto.sh'
check_status=$( virsh list | grep ${PC} )

if [ -z "${check_status}" ];then			#null
	/bin/virsh nodedev-detach pci_0000_00_1c_0 &> /dev/null
	/bin/virsh nodedev-detach pci_0000_00_1c_2 &> /dev/null
	/bin/virsh nodedev-detach pci_0000_00_1c_4 &> /dev/null
	/bin/virsh nodedev-detach pci_0000_00_1b_0 &> /dev/null
	/bin/sh ${search} &> /dev/null
	/bin/sh ${search} &> /dev/null
	virsh create "${path}${PC}.xml" &> /dev/null; vm_status=$?
	/bin/sh ${cpu} &> /dev/null
	if [ "${vm_status}" != 0 ];then
		if [ "${PC}" == "${host_number}-1" ];then
			echo "13"			#vm1 error
		else
			echo "23"			#vm2 error
		fi
	else
		if [ "${PC}" == "${host_number}-1" ];then
			echo "11"			#vm1 starting
		else
			echo "21"			#vm2 starting
		fi
	fi
else
	if [ "${PC}" == "${host_number}-1" ];then
		echo "12"				#vm1 opened
	else
		echo "22"				#vm2 opened
	fi
fi
