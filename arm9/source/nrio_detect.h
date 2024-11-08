// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Adrian "asie" Siekierka, 2024

#ifndef _NRIO_DETECT_H_
#define _NRIO_DETECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define NRIO_USB_BOARD_TYPE_NONE 0
#define NRIO_USB_BOARD_TYPE_D12  12
#define NRIO_USB_BOARD_TYPE_D14  14

typedef struct nrio_usb_type {
    uint16_t board_type; ///< Board type
    uint16_t chip_id;    ///< Chip ID
    uint16_t chip_rev;   ///< Chip revision
} nrio_usb_type_t;

nrio_usb_type_t nrio_usb_detect(void);

#ifdef __cplusplus
}
#endif

#endif /* _NRIO_DETECT_H_ */

