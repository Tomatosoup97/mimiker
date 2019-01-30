#include <device.h>
#include <stdc.h>
#include <bus.h>
#include <fdt.h>
#include <klog.h>
#include <devclass.h>

extern uint8_t __malta_dtb_start[];

device_t *get_or_create_device(device_t *bus) {
  device_t *dev = NULL;
  device_t *child;

  TAILQ_FOREACH (child, &bus->children, link) {
  // TODO: pass name, unit? how to recognize its the desired device?
    if (1) {
      dev = child;
      // break;
    }
  }
  if (dev == NULL) {
    dev = make_device(bus, NULL);
  }
  return dev;
}

static void generic_hinted_child(device_t *bus, const void *fdt, int nodeoffset) {
    int proplen;
    char child_path[PATHBUF_LEN];
    const fdt_property_t *prop_ptr;
    device_t *child = get_or_create_device(bus);

    devprop_attr_val_t *path_attr = kmalloc(M_DEV, sizeof(devprop_attr_val_t), 0);
    devprop_res_val_t *irq = kmalloc(M_DEV, sizeof(devprop_res_val_t), 0);
    devprop_res_val_t *regs = kmalloc(M_DEV, sizeof(devprop_res_val_t), 0);

    fdt_get_path(fdt, nodeoffset, child_path, PATHBUF_LEN);
    path_attr->str_value = child_path;
    set_device_prop_attr(child, FDT_PATH, path_attr);

    prop_ptr = fdt_getprop(fdt, nodeoffset, "interrupts", &proplen);
    irq->uint8_value = (uint32_t) prop_ptr->data; // cast to uint8_t ?
    set_device_prop_res(child, IRQLINE, irq);

    prop_ptr = fdt_getprop(fdt, nodeoffset, "reg", &proplen);
    regs->uint8_value = (uint32_t) prop_ptr->data;
    set_device_prop_res(child, IOPORT, regs);

    return;
}

void bus_enumerate_hinted_children(device_t *bus) {
    const void *fdt = (const void *)__malta_dtb_start;
    char *path =  get_device_prop_attr(bus, FDT_PATH)->str_value;

    int bus_nodeoffset = fdt_path_offset(fdt, path);
    int child_nodeoffset;

    FDT_FOR_EACH_SUBNODE(child_nodeoffset, fdt, bus_nodeoffset) {
        generic_hinted_child(bus, fdt, child_nodeoffset);
    }
}
