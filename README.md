# cm108_ptt
A simple hacked-together PTT library for CM108 USB soundcards

This is code from the Hamlib, which I simplified and made work standalone.
It has a very hackish style and should not be used for anything but
testing as it may have all kinds of weird bugs.

To build a test-tool which just pushes the PTT button for one second enter:

``` make ```

# IMPORTANT: Create a corresponding udev rule!
Create a rule file (for example /etc/udev/rules.d/cm108.rules) and paste the line that fits for your chip version:

older chips:
``` SUBSYSTEMS=="usb", ATTRS{idVendor}=="0d8c", ATTRS{idProduct}=="000c", MODE="0666" ```

newer chips:
``` SUBSYSTEMS=="usb", ATTRS{idVendor}=="0d8c", ATTRS{idProduct}=="013c", MODE="0666" ```

# ALSO IMPORTANT: newer chip revisions have a different instruction set
Please check cm108.c and cm108.h
