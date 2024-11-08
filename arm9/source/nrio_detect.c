// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Adrian "asie" Siekierka, 2024

#include <nds.h>
#include "nrio_detect.h"

#define EXMEMCNT_SRAM_TIME_SHIFT            (0)
#define EXMEMCNT_SRAM_TIME_6_CYCLES         (2 << EXMEMCNT_SRAM_TIME_SHIFT)
#define EXMEMCNT_SRAM_TIME_18_CYCLES        (3 << EXMEMCNT_SRAM_TIME_SHIFT)

#define EXMEMCNT_ROM_TIME1_SHIFT            (2)
#define EXMEMCNT_ROM_TIME2_SHIFT            (4)
#define EXMEMCNT_ROM_TIME1_6_CYCLES         (2 << EXMEMCNT_ROM_TIME1_SHIFT)
#define EXMEMCNT_ROM_TIME1_18_CYCLES        (3 << EXMEMCNT_ROM_TIME1_SHIFT)
#define EXMEMCNT_ROM_TIME2_6_CYCLES         (0 << EXMEMCNT_ROM_TIME2_SHIFT)
#define EXMEMCNT_ROM_TIME2_4_CYCLES         (1 << EXMEMCNT_ROM_TIME2_SHIFT)

#define NRIO_D12_DATA (*((volatile uint16_t*) 0x9FDFFFE))
#define NRIO_D12_CMD  (*((volatile uint16_t*) 0x9FFFFFE))

#define NRIO_D12_CMD_READ_ID         0xFD

static inline uint16_t nrio_d12_read_chip_id(void) {
    uint16_t result;
    NRIO_D12_CMD = NRIO_D12_CMD_READ_ID;
    result = NRIO_D12_DATA & 0xFF;
    result |= (NRIO_D12_DATA & 0xFF) << 8;
    return result;
}

#define NRIO_D14_REG(index) GBA_BUS[(index) * 0x10000]

#define NRIO_D14_CHIP_IDL  NRIO_D14_REG(0x70)
#define NRIO_D14_CHIP_IDH  NRIO_D14_REG(0x72)

static inline uint32_t nrio_d14_read_chip_id(void) {
    return ((NRIO_D14_CHIP_IDH << 16) | NRIO_D14_CHIP_IDL) & 0xFFFFFF;
}

nrio_usb_type_t nrio_usb_detect(void) {
	
    nrio_usb_type_t result = {0, 0, 0};

    // Configure GBA cartridge bus
    sysSetCartOwner(BUS_OWNER_ARM9);
	
    REG_EXMEMCNT = (REG_EXMEMCNT & ~0x1F) | EXMEMCNT_ROM_TIME1_6_CYCLES
        | EXMEMCNT_ROM_TIME2_4_CYCLES | EXMEMCNT_SRAM_TIME_6_CYCLES;

    // Check if the chip is an ISP1581/82/83
    uint32_t d14_chip_id = nrio_d14_read_chip_id();
    if (d14_chip_id >= 0x158100 && d14_chip_id < 0x158400) {
        result.board_type = NRIO_USB_BOARD_TYPE_D14;
        result.chip_id = d14_chip_id >> 8;
        result.chip_rev = d14_chip_id & 0xFF;
        return result;
    }

    // The D12 supports a maximum readout speed of 2 MHz.
    REG_EXMEMCNT = (REG_EXMEMCNT & ~0x1F) | EXMEMCNT_ROM_TIME1_18_CYCLES
        | EXMEMCNT_ROM_TIME2_6_CYCLES | EXMEMCNT_SRAM_TIME_18_CYCLES;

    // Check if the chip is an PDIUSBD12 (do other compatible chips exist?)
    uint16_t d12_chip_id = nrio_d12_read_chip_id();
    uint8_t d12_open_bus = NRIO_D12_DATA;
    if (!((d12_chip_id >> 8) == d12_open_bus && (d12_chip_id & 0xFF) == d12_open_bus)) {
        result.board_type = NRIO_USB_BOARD_TYPE_D12;
        result.chip_id = d12_chip_id;
        return result;
    }

    return result;
}

