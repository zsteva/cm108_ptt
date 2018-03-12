#!/usr/bin/env python

import sys, usb.core

dev = usb.core.find(idVendor=0x1b3f, idProduct=0x2008)
if dev is None:
    sys.exit("No Panic button found in the system");

try:
    if dev.is_kernel_driver_active(0) is True:
        dev.detach_kernel_driver(0)
except usb.core.USBError as e:
    sys.exit("Kernel driver won't give up control over device: %s" % str(e))

try:
    #dev.set_configuration()
    dev.reset()
except usb.core.USBError as e:
    sys.exit("Cannot set configuration the device: %s" % str(e))

print("dev", dev[0])
endpoint = dev[0][(3,0)][0]
while 1:
    try:
        data = dev.read(endpoint.bEndpointAddress, endpoint.wMaxPacketSize, timeout=10000)
        if data is not None and len(data) > 2:
            if data[0] == 6 and data[2] == 19:
                # Panic button was pressed!
                print("OH MY GOD! OH MY GOD! DOUBLE RAINBOW!!!")
    except usb.core.USBError as e:
        if e.errno != 110: # 110 is a timeout.
            sys.exit("Error readin data: %s" % str(e))
