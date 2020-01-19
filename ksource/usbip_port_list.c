// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2011 matt mooney <mfm@muteddisk.com>
 *               2005-2007 Takahiro Hirofuchi
 *
 * Modified By
 * 	2020 dice14u - striped console and made it return a result
 */

#include <usbip/vhci_driver.h>
#include <usbip/usbip_common.h>

#include "usbip_device_list.h"

struct usbip_devices {
    uint8_t port;
    uint32_t devid;
    uint8_t busnum;
    uint8_t devnum;
    char* product_name;
    struct usbip_devices* next;
};

void usbip_devices_free(struct usbip_devices* device) {
    free(device->product_name);
    free(device);
}

struct usbip_devices* usbip_port_list(void)
{
    int i;
    char product_name[100];
    struct usbip_devices* last = NULL;
    struct usbip_devices* current = NULL;
    struct usbip_devices* first = NULL;


    if (usbip_vhci_driver_open() < 0) {
		err("open vhci_driver");
        return first;
    }

    for (i= 0; i < vhci_driver->nports; i++) {
        current = (struct usbip_devices*)malloc(sizeof(struct usbip_devices));
        struct usbip_imported_device current_device = vhci_driver->idev[i];
        current->port = current_device.port;
        current->devid = current_device.devid;
        current->busnum = current_device.busnum;
        current->devnum = current_device.devnum;
        usbip_names_get_product(product_name, sizeof(product_name),
                    current_device.udev.idVendor, current_device.udev.idProduct);
        current->product_name = strdup(product_name);
        current->next = NULL;
        if (last != NULL) {
            last->next = current;
        }
        if (first == NULL) {
            first = current;
        }
        last = current;
    }

    usbip_vhci_driver_close();

    return first;
};
