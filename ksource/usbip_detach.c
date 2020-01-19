// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2011 matt mooney <mfm@muteddisk.com>
 *               2005-2007 Takahiro Hirofuchi
 * 
 * Modified By
 * 	2020 dice14u - striped console print to make it exportable
 */

#include <usbip/vhci_driver.h>
#include <libudev.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

int usbip_detach(uint8_t port)
{
    struct usbip_imported_device *idev;
    int ret = usbip_vhci_driver_open();
    bool found = false;
    char path[PATH_MAX+1];
    if (ret < 0) {
        return -1;
    }
    for (int i = 0; i < vhci_driver->nports; i++) {
        idev = &vhci_driver->idev[i];
        if (idev->port == port) {
            found = true;
            if (idev->status != VDEV_ST_NULL)
                break;
            ret = 1; // this is success
            goto call_driver_close;
        }
    }
    if (!found) {
        ret = -2;
        goto call_driver_close;
    }

    /* remove the port state file */
    snprintf(path, PATH_MAX, VHCI_STATE_PATH"/port%d", port);
    remove(path);

    rmdir(VHCI_STATE_PATH);

    ret = usbip_vhci_detach_device(port);
    if (ret < 0) {
        ret = -3;
    } else {
        ret = 0;
    }

call_driver_close:
    usbip_vhci_driver_close();
    return ret;
}
