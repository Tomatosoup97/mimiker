#include <mips/malta.h>
#include <bus.h>

static uint8_t cbus_read_1(bus_space_handle_t handle, bus_size_t offset) {
  return *(volatile uint8_t *)(handle + offset * sizeof(uint64_t));
}

static void cbus_write_1(bus_space_handle_t handle, bus_size_t offset,
                         uint8_t value) {
  *(volatile uint8_t *)(handle + offset * sizeof(uint64_t)) = value;
}

static bus_space_t cbus_bus_space = {
  .bs_read_1 = cbus_read_1,
  .bs_write_1 = cbus_write_1,
};

resource_t cbus_uart[1] = {{.r_bus_tag = &cbus_bus_space,
                            .r_bus_handle = MIPS_PHYS_TO_KSEG1(MALTA_CBUS_UART),
                            /* RT_MEMORY since CBUS UART is a device mapped in
                             * common physical address space of MIPS CPU. */
                            .r_type = RT_MEMORY,
                            .r_start = 0,
                            .r_end = 7}};
