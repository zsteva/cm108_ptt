/**
 *  Hacked-together cm108-ptt library. Nearly 100% copied from Hamlib.
 *  Do-not-trust and will-do-evil-things generally apply.
 *
 *  Have fun with it :D
 */

/*
 *  Hamlib Interface - CM108 HID communication low-level support
 *  Copyright (c) 2000-2012 by Stephane Fillod
 *  Copyright (c) 2011 by Andrew Errington
 *  CM108 detection code Copyright (c) Thomas Sailer used with permission
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/**
 * \addtogroup rig_internal
 * @{
 */

/**
 * \brief CM108 GPIO
 * \file cm108.c
 */
#include <stdlib.h>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <libudev.h>
#include <sys/param.h>

#include <linux/hidraw.h>
#include "cm108.h"


/**
 * Find the matching hidraw device for the given USB_VID and USB_PID.
 * Returns the path of the found hidraw device or a null pointer if none was found.
 */
const char *cm108_find_device()
{
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;
	const char *found_path;

	udev = udev_new();
	if(!udev) {
		printf("Can't create udev\n");
		exit(2);
	}

	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "hidraw");
	udev_enumerate_scan_devices(enumerate);

	devices = udev_enumerate_get_list_entry(enumerate);

	udev_list_entry_foreach(dev_list_entry, devices) {
		const char *path, *hidraw;

		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);

		hidraw = udev_device_get_devnode(dev);

		dev = udev_device_get_parent_with_subsystem_devtype(
		       dev,
		       "usb",
		       "usb_device");
		if (!dev) {
			printf("Unable to find parent usb device.");
			exit(1);
		}

		if((strcmp(udev_device_get_sysattr_value(dev, "idVendor"), CM108_VID) == 0) 
		    && (strcmp(udev_device_get_sysattr_value(dev, "idProduct"), CM108_PID) == 0)) {
			found_path = hidraw;
			goto clean;
		}
	}

clean:
	udev_enumerate_unref(enumerate);
	udev_unref(udev);	
	
	return found_path;
}

/**
 * \brief Open CM108 HID port (/dev/hidrawX)
 * \param port
 * \return file descriptor
 */

int cm108_open(const char* path)
{
	int fd;

	fd = open(path, O_RDWR);
    
	if (fd < 0) {
		return -1;
	}

#ifdef HAVE_LINUX_HIDRAW_H
	// CM108 detection copied from Thomas Sailer's soundmodem code

	struct hidraw_devinfo hiddevinfo;

	if (!ioctl(fd, HIDIOCGRAWINFO, &hiddevinfo)
	&&
	  (
	    (hiddevinfo.vendor == 0x0d8c	// CM108/109/119
		&& hiddevinfo.product >= 0x0008
		&& hiddevinfo.product <= 0x000f
	    )
	    ||
	    (hiddevinfo.vendor == 0x0c76 &&	// SSS1621/23
		(hiddevinfo.product == 0x1605 ||
		hiddevinfo.product == 0x1607 ||
		hiddevinfo.product == 0x160b)
	    )
	  )
	)
	{
	}
	else
	{
		close(fd);
		return -2;
	}
#endif

	return fd;
}

/**
 * \brief Close CM108 HID port
 * \param port
 */
int cm108_close(int fd)
{
	return close(fd);
}

/**
 * \brief Set or unset Push to talk bit on CM108 GPIO
 * \param p
 * \param pttx RIG_PTT_ON --> Set PTT
 * \return RIG_OK or < 0 error
 */
void cm108_ptt_set(int fd, int state)
{

	// For a CM108 USB audio device PTT is wired up to one of the GPIO
	// pins.  Usually this is GPIO3 (bit 2 of the GPIO register) because it
	// is on the corner of the chip package (pin 13) so it's easily accessible.
	// Some CM108 chips are epoxy-blobbed onto the PCB, so no GPIO
	// pins are accessible.  The SSS1623 chips have a different pinout, so
	// we make the GPIO bit number configurable.

		// Build a packet for CM108 HID to turn GPIO bit on or off.
		// Packet is 4 bytes, preceded by a 'report number' byte
		// 0x00 report number
		// Write data packet (from CM108 documentation)
		// byte 0: 00xx xxxx     Write GPIO
		// byte 1: xxxx dcba     GPIO3-0 output values (1=high)
		// byte 2: xxxx dcba     GPIO3-0 data-direction register (1=output)
		// byte 3: xxxx xxxx     SPDIF


		char out_rep[] = {
			0x00, // report number
			// HID output report
			0x00,
			(state == 1) ? 1 : 0, // set GPIO
			1, // Data direction register (1=output)
			0x00
		};

		ssize_t nw;

		// Send the HID packet
		nw = write(fd, out_rep, sizeof(out_rep));

}
