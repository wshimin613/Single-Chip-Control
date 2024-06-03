#!/bin/bash

PC_1=$( ls -f /vm_data/xml/ | grep PC | sed s/.xml//g | sort | head -n 1 )
PC_2=$( ls -f /vm_data/xml/ | grep PC | sed s/.xml//g | sort | tail -n 1 )
check_vm_1=$( virsh list | grep ${PC_1} )
check_vm_2=$( virsh list | grep ${PC_2} )

if [ -z "${check_vm_1}" ];then
	vm_1='0'
else
	vm_1='1'
fi

if [ -z "${check_vm_2}" ];then
	vm_2='0'
else
	vm_2='1'
fi

if [ ${vm_1} == 0 ] && [ ${vm_2} == 0 ];then
	echo '300'
elif [ ${vm_1} == 1 ] && [ ${vm_2} == 0 ];then
	echo '310'
elif [ ${vm_1} == 0 ] && [ ${vm_2} == 1 ];then
	echo '301'
else 
	echo '311'
fi
