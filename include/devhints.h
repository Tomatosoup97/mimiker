#ifndef _SYS_DEVHINTS_H_
#define _SYS_DEVHINTS_H_

#include <device.h>
#include <stdc.h>
#include <bus.h>
#include <fdt.h>
#include <klog.h>

void bus_enumerate_hinted_children(device_t *bus);

void generic_hinted_child(device_t *bus, const void *fdt, int nodeoffset);

#endif /* !_SYS_DEVHINTS_H_ */
