#ifndef _SYS_BINARIES_H
#define _SYS_BINARIES_H 1

#define BMP_SIZE(x, y) (((x) * (y)) / 2)

#include "../common/common.h"


/* The poem text :) */
extern char bootpoem[];

/* The butterfly ascii logo */
extern char butterfly[];

/// Wallpapers 480px

extern uint8_t bigeye_480[BMP_SIZE(480, 480)];
extern uint8_t myfall_480[BMP_SIZE(480, 480)];
extern uint8_t myfood_480[BMP_SIZE(480, 480)];
extern uint8_t mylamb_480[BMP_SIZE(480, 480)];
extern uint8_t mylife_480[BMP_SIZE(480, 480)];
extern uint8_t mymind_480[BMP_SIZE(480, 480)];
extern uint8_t theman_480[BMP_SIZE(480, 480)];

/// Wallpapers 640px

extern uint8_t candle_640[BMP_SIZE(640, 480)];
extern uint8_t choice_640[BMP_SIZE(640, 480)];
extern uint8_t clouds_640[BMP_SIZE(640, 480)];
extern uint8_t myhill_640[BMP_SIZE(640, 480)];
extern uint8_t mypain_640[BMP_SIZE(640, 480)];
extern uint8_t mypath_640[BMP_SIZE(640, 480)];
extern uint8_t myroad_640[BMP_SIZE(640, 480)];
extern uint8_t mywork_640[BMP_SIZE(640, 480)];
extern uint8_t wchess_640[BMP_SIZE(640, 480)];


/* Mouse pointer bitmap */
uint8_t mouse_bitmap[9 * 18] = {
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
};


#endif /* _SYS_BINARIES_H */
