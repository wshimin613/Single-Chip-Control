#!/bin/bash

PC=${1}
host_number=$( ls -f /vm_data/xml/ | grep PC | sed s/-1.xml//g | sort | head -n 1 )
check_status=$( virsh list | grep ${PC} )

if [ -n "${check_status}" ];then				#exist
	virsh destroy "${PC}" &> /dev/null
	if [ "${PC}" == "${host_number}-1" ];then
		echo "10"
	else
		echo "20"
	fi
fi
