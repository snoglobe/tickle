#pragma once

#define TBLACK   0x222533ff
#define TDGREY   0x6275baff
#define TLGREY   0xa3c0e6ff
#define TWHITE   0xfafffcff
#define TBEIGE   0xffab7bff
#define TPINK    0xff6c7aff
#define TRED     0xdc435bff
#define TBLUE    0x3f48c2ff
#define TLBLUE   0x448de7ff
#define TGREEN   0x2bdb72ff
#define TLGREEN  0xa7f547ff
#define TYELLOW  0xffeb33ff
#define TGOLD    0xf58931ff
#define TORANGE  0xdb4b3dff
#define TMAROON  0xa63d57ff
#define TDPURPLE 0x36354dff

static inline int IGetPColor(int c) {
    switch(c){
        case 0:
            return (TBLACK); // black
        case 1: 
            return (TWHITE); // white
        case 2:
            return (TRED); // red
        case 3:
            return (TMAROON); // maroon
        case 4:
            return (TORANGE); // orange
        case 5:
            return (TGOLD); // gold
        case 6:  
            return (TYELLOW); // yellow
        case 7:
            return (TLGREEN); // light green
        case 8:
            return (TGREEN); // green
        case 9:
            return (TLBLUE); // light blue
        case 10:
            return (TBLUE); // blue
        case 11:
            return (TDPURPLE); // purple
        case 12:
            return (TPINK); // pink
        case 13:
            return (TBEIGE); // beige
        case 14:
            return (TLGREY); // light brown
        case 15:
            return (TDGREY); // brown
        case 17:
            return (0x00000000); // transparent
        default: 
            printf("invalid color %d", c);
            exit(EXIT_FAILURE);
    }
}