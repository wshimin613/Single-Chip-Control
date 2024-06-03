#!/usr/bin/python3
import serial
import time
import subprocess

PC = subprocess.Popen("ls -f /vm_data/xml/ | grep PC | sed s/-1.xml//g | sort | head -n 1", shell=True, stdout=subprocess.PIPE).stdout.read().decode().strip()
com = subprocess.Popen("/bin/sh /vm_data/node_MCU/port.sh", shell=True, stdout=subprocess.PIPE).stdout
USB = '/dev/' + com.read().decode().strip()
esp = serial.Serial(USB, 9600)   #使用USB連接電腦
esp.write('0'.encode())

while True:
    esp_value = esp.readline().decode('utf-8').strip()
    print(esp_value)
    if esp_value == 'vm1_open':
        vm_status = subprocess.Popen("/bin/sh /vm_data/node_MCU/open_vm.sh " + PC + '-1', shell=True, stdout=subprocess.PIPE).stdout.read().decode().strip()
        esp.write(vm_status.encode())
        #print(vm_status)
    elif esp_value == 'vm2_open':
        vm_status = subprocess.Popen("/bin/sh /vm_data/node_MCU/open_vm.sh " + PC + '-2', shell=True, stdout=subprocess.PIPE).stdout.read().decode().strip()
        esp.write(vm_status.encode())
        #print(vm_status)
    elif esp_value == 'vm1_destroy':
        vm_status = subprocess.Popen("/bin/sh /vm_data/node_MCU/close_vm.sh " + PC + '-1', shell=True, stdout=subprocess.PIPE).stdout.read().decode().strip()
        esp.write(vm_status.encode())
        #print(vm_status)
    elif esp_value == 'vm2_destroy':
        vm_status = subprocess.Popen("/bin/sh /vm_data/node_MCU/close_vm.sh " + PC + '-2', shell=True, stdout=subprocess.PIPE).stdout.read().decode().strip()
        esp.write(vm_status.encode())
        #print(vm_status)
    elif esp_value == 'check':
        update_status = subprocess.Popen("/bin/sh /vm_data/node_MCU/update_status.sh", shell=True, stdout=subprocess.PIPE).stdout.read().decode().strip()
        esp.write(update_status.encode())
        #print(update_status)
        time.sleep(1)
    time.sleep(1)
    esp.write('0'.encode())
esp.close()
