// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2011 matt mooney <mfm@muteddisk.com>
 *               2005-2007 Takahiro Hirofuchi
 * Copyright (C) 2015-2016 Samsung Electronics
 *               Igor Kotrasinski <i.kotrasinsk@samsung.com>
 *               Krzysztof Opasiak <k.opasiak@samsung.com>
 * Modified By
 * 	2020 dice14u - striped console and made it return a result for remote list
 */
#include <netdb.h>
#include <usbip/usbip_common.h>
#include "usbip_network.h"
#include "utils.h"

#define MAX_INTERFACES 10

struct usbip_external_list {
    char* product_name;
    char* path;
    char* busid;
    char* interfaces[MAX_INTERFACES];
    int num_interfaces;
    struct usbip_external_list* next;
};

void usbip_external_list_free(struct usbip_external_list* device) {
    int interface_count = lowest(device->num_interfaces, MAX_INTERFACES);
    for(int i = 0; i < interface_count; i++) {
        free(device->interfaces[i]);
    }
    free(device->product_name);
    free(device->busid);
    free(device->path);
    free(device);
};

static struct usbip_external_list* get_exported_devices(char *host, int sockfd)
{
    struct usbip_external_list* last = NULL;
    struct usbip_external_list* current = NULL;
    struct usbip_external_list* first = NULL;
	char product_name[100];
	char class_name[100];
	struct op_devlist_reply reply;
	uint16_t code = OP_REP_DEVLIST;
	struct usbip_usb_device udev;
	struct usbip_usb_interface uintf;
	unsigned int i;
	int rc, j;
	int status;

	rc = usbip_net_send_op_common(sockfd, OP_REQ_DEVLIST, 0);
	if (rc < 0) {
		dbg("usbip_net_send_op_common failed");
        return NULL;
	}

	rc = usbip_net_recv_op_common(sockfd, &code, &status);
	if (rc < 0) {
		err("Exported Device List Request failed - %s\n",
		    usbip_op_common_status_string(status));
        return NULL;
	}

	memset(&reply, 0, sizeof(reply));
	rc = usbip_net_recv(sockfd, &reply, sizeof(reply));
	if (rc < 0) {
		dbg("usbip_net_recv_op_devlist failed");
        return NULL;
	}
	PACK_OP_DEVLIST_REPLY(0, &reply);
	dbg("exportable devices: %d\n", reply.ndev);

	if (reply.ndev == 0) {
		info("no exportable devices found on %s", host);
        return NULL;
	}

	for (i = 0; i < reply.ndev; i++) {
		memset(&udev, 0, sizeof(udev));
		rc = usbip_net_recv(sockfd, &udev, sizeof(udev));
		if (rc < 0) {
			dbg("usbip_net_recv failed: usbip_usb_device[%d]", i);
            return NULL;
		}
		usbip_net_pack_usb_device(0, &udev);

		usbip_names_get_product(product_name, sizeof(product_name),
					udev.idVendor, udev.idProduct);

        current = (struct usbip_external_list*)malloc(sizeof(struct usbip_external_list));
        current->next = NULL;
        current->path = strdup(udev.path);
        current->busid = strdup(udev.busid);
        current->product_name = strdup(product_name);
        current->num_interfaces = lowest(udev.bNumInterfaces, MAX_INTERFACES);

        for (j = 0; j < udev.bNumInterfaces && j < 10; j++) {
            rc = usbip_net_recv(sockfd, &uintf, sizeof(uintf));
            if (rc < 0) {
                err("usbip_net_recv failed: usbip_usb_intf[%d]",
                        j);

                return NULL;
            }
            usbip_net_pack_usb_interface(0, &uintf);

            usbip_names_get_class(class_name, sizeof(class_name),
                    uintf.bInterfaceClass,
                    uintf.bInterfaceSubClass,
                    uintf.bInterfaceProtocol);
            current->interfaces[j] = strdup(class_name);
        }
        if (last != NULL) {
            last->next = current;
        }
        if (first == NULL) {
            first = current;
        }
        last = current;
	}

    return first;
}

struct usbip_external_list* usbip_external_devices(char *host)
{
    struct usbip_external_list* rc;
    int sockfd;

    sockfd = usbip_net_tcp_connect(host, usbip_port_string);
    if (sockfd < 0) {
        err("could not connect to %s:%s: %s", host,
            usbip_port_string, gai_strerror(sockfd));
        return NULL;
    }
    dbg("connected to %s:%s", host, usbip_port_string);

    rc = get_exported_devices(host, sockfd);
    if (rc < 0) {
        err("failed to get device list from %s", host);
        return NULL;
    }

    close(sockfd);

    return rc;
}
