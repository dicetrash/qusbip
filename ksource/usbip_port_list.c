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
#include <limits.h>
#include <libudev.h>
#include <stdint.h>

#include "utils.h"
#include "usbip_device_list.h"

#define MAX_INTERFACES 10

struct usbip_devices {
    uint8_t port;
    uint32_t devid;
    uint8_t busnum;
    uint8_t devnum;
    char* product_name;
    char* host;
    char* remote_port;
    char* remote_busid;
    struct usbip_usb_device udev;
    struct usbip_devices* next;
};

void usbip_devices_free(struct usbip_devices* device) {
    free(device->host);
    free(device->remote_port);
    free(device->remote_busid);
    free(device->product_name);
    free(device);
}

/*
 * Read the given port's record.
 *
 * To avoid buffer overflow we will read the entire line and
 * validate each part's size. The initial buffer is padded by 4 to
 * accommodate the 2 spaces, 1 newline and an additional character
 * which is needed to properly validate the 3rd part without it being
 * truncated to an acceptable length.
 */
static int read_record(int rhport, char *host, unsigned long host_len,
        char *port, unsigned long port_len, char *busid)
{
    int part;
    FILE *file;
    char path[PATH_MAX+1];
    char *buffer, *start, *end;
    char delim[] = {' ', ' ', '\n'};
    int max_len[] = {(int)host_len, (int)port_len, SYSFS_BUS_ID_SIZE};
    size_t buffer_len = host_len + port_len + SYSFS_BUS_ID_SIZE + 4;

    buffer = (char*)malloc(buffer_len);
    if (!buffer)
        return -1;

    snprintf(path, PATH_MAX, VHCI_STATE_PATH"/port%d", rhport);

    file = fopen(path, "r");
    if (!file) {
        err("fopen");
        free(buffer);
        return -1;
    }

    if (fgets(buffer, buffer_len, file) == NULL) {
        err("fgets");
        free(buffer);
        fclose(file);
        return -1;
    }
    fclose(file);

    /* validate the length of each of the 3 parts */
    start = buffer;
    for (part = 0; part < 3; part++) {
        end = strchr(start, delim[part]);
        if (end == NULL || (end - start) > max_len[part]) {
            free(buffer);
            return -1;
        }
        start = end + 1;
    }

    if (sscanf(buffer, "%s %s %s\n", host, port, busid) != 3) {
        err("sscanf");
        free(buffer);
        return -1;
    }

    free(buffer);

    return 0;
}


struct usbip_devices* usbip_port_list(void)
{
    int i;
    char product_name[100];
    char host[NI_MAXHOST] = "unknown host";
    char serv[NI_MAXSERV] = "unknown port";
    char remote_busid[SYSFS_BUS_ID_SIZE] = {0};

    struct usbip_devices* last = NULL;
    struct usbip_devices* current = NULL;
    struct usbip_devices* first = NULL;


    if (usbip_vhci_driver_open() < 0) {
		err("open vhci_driver");
        return first;
    }

    for (i= 0; i < vhci_driver->nports; i++) {
        struct usbip_imported_device current_device = vhci_driver->idev[i];
        if (current_device.status == VDEV_ST_NULL || current_device.status == VDEV_ST_NOTASSIGNED) {
            continue;
        }
        int rec = read_record(current_device.port, host, sizeof(host), serv, sizeof(serv), remote_busid);
        current = (struct usbip_devices*)malloc(sizeof(struct usbip_devices));
        current->port = current_device.port;
        current->devid = current_device.devid;
        current->busnum = current_device.busnum;
        current->devnum = current_device.devnum;
        current->udev = current_device.udev;
        if (!rec) {
            current->host = strdup(host);
            current->remote_port = strdup(serv);
            current->remote_busid = strdup(remote_busid);
        } else {
            current->host = NULL;
            current->remote_port = NULL;
            current->remote_busid = NULL;
        }

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
