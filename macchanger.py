#!/usr/bin/env python3

import subprocess
import optparse
import re

def get_args():
    parser = optparse.OptionParser()
    parser.add_option("-i", "--interface", dest="interface", help="Interface whose MAC has to be changed")
    parser.add_option("-m", "--mac", dest="new_mac", help="New MAC address")
    options, args = parser.parse_args()

    if not options.interface:
        parser.error("[-] Please specify a valid interface.")
    elif not options.new_mac:
        parser.error("[-] Please specify a valid MAC address.")
    return options

def change_mac(interface, new_mac):
    print("[+] Changing MAC address " + "for " + interface + " to " + new_mac)
    subprocess.call(["sudo", "ifconfig", interface, "down"])
    subprocess.call(["sudo", "ifconfig", interface, "hw", "ether", new_mac])
    subprocess.call(["sudo", "ifconfig", interface, "up"])

def get_mac(interface):
    ifconfig_result = subprocess.check_output(["ifconfig", interface]).decode()
    mac_search_result = re.search(r"\w\w:\w\w:\w\w:\w\w:\w\w:\w\w", ifconfig_result)

    if mac_search_result:
        return (mac_search_result.group(0))
    else:
        print("[-] Failed to read MAC .")

options = get_args()
current_mac = get_mac(options.interface)
print("[+] Current MAC = ",current_mac)
change_mac(options.interface, options.new_mac)

c_mac = get_mac(options.interface)
if c_mac == options.new_mac:
    print("[+] MAC address changed successfully to " + c_mac)
else:
    print("[-] Failed to changed to specified MAC address.")


