#ifndef DRIVER_VGA_H_
#define DRIVER_VGA_H_ 1

#include "../../../common/sysutils.h"

// Text mode 80x25 0x3h
#define TEXTMODE_BUFFER (uint8_t*) 0xB8000
#define TEXTMODE_WIDTH  (uint8_t) 90
#define TEXTMODE_HEIGHT (uint8_t) 60
#define TEXTMODE_SIZE   (uint16_t) (TEXTMODE_WIDTH * TEXTMODE_HEIGHT)

// Graphic mode 640x480 16bp planar 0x12h
#define GRAPHMODE_BUFFER  (uint8_t*) 0xA0000
#define GRAPHMODE_WIDTH   (uint16_t) 640
#define GRAPHMODE_HEIGHT  (uint16_t) 480
#define GRAPHMODE_SIZE    (uint32_t) (GRAPHMODE_WIDTH * GRAPHMODE_HEIGHT / 2)

/* VGA registers */

#define	ATTRIBUTE_INDEX             0x3C0
#define	ATTRIBUTE_WRITE             0x3C0
#define	ATTRIBUTE_READ              0x3C1

#define	MISCELLANEOUS_WRITE         0x3C2
#define SEQUENCER_INDEX             0x3C4
#define SEQUENCER_DATA              0x3C5
#define	DIGANALOG_READ_INDEX        0x3C7
#define	DIGANALOG_WRITE_INDEX       0x3C8
#define	DIGANALOG_DATA              0x3C9
#define	MISCELLANEOUS_READ          0x3CC

#define GRAPHICS_INDEX              0x3CE
#define GRAPHICS_DATA               0x3CF

#define CATHODERAY_INDEX            0x3D4
#define CATHODERAY_DATA             0x3D5
#define	INPUT_STATE_READ            0x3DA

#define	NUM_SEQUENCER_REGS          5
#define	NUM_CATHODERAY_REGS         25
#define	NUM_GRAPHICS_REGS           9
#define	NUM_ATTRIBUTE_REGS          21

#define REG_SEQUENCER_MASK          0x02
#define REG_SEQUENCER_CHARSET       0x03
#define REG_SEQUENCER_MEMORY        0x04

#define REG_GRAPHICS_MAP_READ       0x04
#define GRAPHICS_GRAPHICS_MODE      0x05
#define GRAPHICS_MISCELLANEOUS      0x06


/** @brief Planar video mode registers, 640x480, 16 colors */
uint8_t video_mode[61] = {
    /* Miscellaneous Register */
	0xE3,

    /* Sequencer Registers */
	0x03, 0x01, 0x0F, 0x00, 0x06,

    /* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59,
	0xEA, 0x8C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	0xFF,

    /* Graphics Controller Registers */
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,

    /* Attribute Controller Registers */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x01, 0x00, 0x0F, 0x00, 0x00
};


/** @brief Linear text mode registers, 90x60, 16 colors */
uint8_t text_mode[61] = {
    /* Miscellaneous Register */
	0xE7,

    /* Sequencer Registers */
	0x03, 0x01, 0x03, 0x00, 0x02,

    /* CRTC */
	0x6B, 0x59, 0x5A, 0x82, 0x60, 0x8D, 0x0B, 0x3E,
	0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x2D, 0x08, 0xE8, 0x05, 0xA3,
	0xFF,

    /* Graphics Controller Registers */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
	0xFF,

    /* Attribute Controller Registers */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x0C, 0x00, 0x0F, 0x08, 0x00
};


const uint32_t color_palette[16] = {
    0x000000, // Black
    0x2A0000, // Dark Red
    0x002A00, // Dark Green
    0x2A2A00, // Brown
    0x00002A, // Dark Blue
    0x2A002A, // Magenta
    0x002A2A, // Dark Cyan
    0x2A2A2A, // Dark Gray

    0x151515, // Light Gray
    0x00003F, // Light Blue
    0x003F00, // Light Green
    0x003F3F, // Light Cyan
    0x3F0000, // Light Red
    0x3F003F, // Light Pink
    0x3F3F00, // Light Yellow
    0x3F3F3F  // Light White
};


/** @brief VGA Font 8x8px bitmap, based on the extended ASCII code page 437 */
uint8_t small_font[256 * 8] = {

    /* Non-printable characters */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0   NULL  ( Null character )
	0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E, // 1   SOH   ( Start of Header )
	0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0xE7, 0xFF, 0x7E, // 2   STX   ( Start of Text )
	0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00, // 3   ETX   ( End of Text, hearts card suit )
	0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00, // 4   EOT   ( End of Transmission, diamonds card suit )
	0x38, 0x7C, 0x38, 0xFE, 0xFE, 0x92, 0x10, 0x7C, // 5   ENQ   ( Enquiry, clubs card suit )
	0x00, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x7C, // 6   ACK   ( Acknowledgement, spade card suit )
	0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00, // 7   BEL   ( Bell )
	0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF, // 8   BS    ( Backspace )
	0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00, // 9   HT    ( Horizontal Tab )
	0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF, // 10  LF    ( Line feed )
	0x0F, 0x07, 0x0F, 0x7D, 0xCC, 0xCC, 0xCC, 0x78, // 11  VT    ( Vertical Tab, male symbol)
	0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18, // 12  FF    ( Form feed, female symbol)
	0x3F, 0x33, 0x3F, 0x30, 0x30, 0x70, 0xF0, 0xE0, // 13  CR    ( Carriage return )
	0x7F, 0x63, 0x7F, 0x63, 0x63, 0x67, 0xE6, 0xC0, // 14  SO    ( Shift Out )
	0x99, 0x5A, 0x3C, 0xE7, 0xE7, 0x3C, 0x5A, 0x99, // 15  SI    ( Shift In )
	0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00, // 16  DLE   ( Data link escape )
	0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00, // 17  DC1   ( Device control 1 )
	0x18, 0x3C, 0x7E, 0x18, 0x18, 0x7E, 0x3C, 0x18, // 18  DC2   ( Device control 2 )
	0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00, // 19  DC3   ( Device control 3 )
	0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00, // 20  DC4   ( Device control 4 )
	0x3E, 0x63, 0x38, 0x6C, 0x6C, 0x38, 0x86, 0xFC, // 21  NAK   ( NAK Negative-acknowledge )
	0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00, // 22  SYN   ( Synchronous idle )
	0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF, // 23  ETB   ( End of trans. block )
	0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00, // 24  CAN   ( Cancel )
	0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00, // 25  EM    ( End of medium )
	0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00, // 26  SUB   ( Substitute )
	0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00, // 27  ESC   ( Escape )
	0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00, // 28  FS    ( File separator )
	0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00, // 29  GS    ( Group separator )
	0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00, // 30  RS    ( Record separator )
	0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00, // 31  US    ( Unit separator )

    /* Printable characters */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 32  space ( Space )
	0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00, // 33  !     ( Exclamation mark )
	0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, // 34  "     ( Double quotes ; Quotation mark ; speech marks )
	0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00, // 35  #     ( Number sign )
	0x18, 0x7E, 0xC0, 0x7C, 0x06, 0xFC, 0x18, 0x00, // 36  $     ( Dollar sign )
	0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00, // 37  %     ( Percent sign )
	0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00, // 38  &     ( Ampersand )
	0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, // 39  '     ( Single quote or Apostrophe )
	0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00, // 40  (     ( round brackets or parentheses, opening round bracket )
	0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00, // 41  )     ( parentheses or round brackets, closing parentheses )
	0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, // 42  *     ( Asterisk )
	0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, // 43  +     ( Plus sign )
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, // 44  ,     ( Comma )
	0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, // 45  -     ( Hyphen , minus sign )
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, // 46  .     ( Dot, full stop )
	0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00, // 47  /     ( Slash , forward slash , fraction bar , division slash )
	0x7C, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0x7C, 0x00, // 48  0     ( number zero )
	0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00, // 49  1     ( number one )
	0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00, // 50  2     ( number two )
	0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00, // 51  3     ( number three )
	0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00, // 52  4     ( number four )
	0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00, // 53  5     ( number five )
	0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00, // 54  6     ( number six )
	0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00, // 55  7     ( number seven )
	0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00, // 56  8     ( number eight )
	0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00, // 57  9     ( number nine )
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, // 58  :     ( Colon )
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30, // 59  ;     ( Semicolon )
	0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00, // 60  <     ( Less-than sign )
	0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00, // 61  =     ( Equals sign )
	0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00, // 62  >     ( Greater-than sign ; Inequality )
	0x3C, 0x66, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x00, // 63  ?     ( Question mark )
	0x7C, 0xC6, 0xDE, 0xDE, 0xDC, 0xC0, 0x7C, 0x00, // 64  @     ( At sign )
	0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00, // 65  A     ( Capital letter A )
	0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00, // 66  B     ( Capital letter B )
	0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00, // 67  C     ( Capital letter C )
	0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, // 68  D     ( Capital letter D )
	0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0x00, // 69  E     ( Capital letter E )
	0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0x00, // 70  F     ( Capital letter F )
	0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3A, 0x00, // 71  G     ( Capital letter G )
	0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00, // 72  H     ( Capital letter H )
	0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, // 73  I     ( Capital letter I )
	0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00, // 74  J     ( Capital letter J )
	0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00, // 75  K     ( Capital letter K )
	0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00, // 76  L     ( Capital letter L )
	0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00, // 77  M     ( Capital letter M )
	0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00, // 78  N     ( Capital letter N )
	0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00, // 79  O     ( Capital letter O )
	0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00, // 80  P     ( Capital letter P )
	0x7C, 0xC6, 0xC6, 0xC6, 0xD6, 0x7C, 0x0E, 0x00, // 81  Q     ( Capital letter Q )
	0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00, // 82  R     ( Capital letter R )
	0x7C, 0xC6, 0xE0, 0x78, 0x0E, 0xC6, 0x7C, 0x00, // 83  S     ( Capital letter S )
	0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, // 84  T     ( Capital letter T )
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00, // 85  U     ( Capital letter U )
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00, // 86  V     ( Capital letter V )
	0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00, // 87  W     ( Capital letter W )
	0xC6, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0xC6, 0x00, // 88  X     ( Capital letter X )
	0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00, // 89  Y     ( Capital letter Y )
	0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0x00, // 90  Z     ( Capital letter Z )
	0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00, // 91  [     ( square brackets or box brackets, opening bracket )
	0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00, // 92  \     ( Backslash , reverse slash )
	0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00, // 93  ]     ( box brackets or square brackets, closing bracket )
	0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, // 94  ^     ( Circumflex accent or Caret )
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, // 95  _     ( underscore , understrike , underbar or low line )
	0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, // 96  `     ( Grave accent )
	0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00, // 97  a     ( Lowercase letter a , minuscule a )
	0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00, // 98  b     ( Lowercase letter b , minuscule b )
	0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00, // 99  c     ( Lowercase letter c , minuscule c )
	0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00, // 100 d     ( Lowercase letter d , minuscule d )
	0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00, // 101 e     ( Lowercase letter e , minuscule e )
	0x38, 0x6C, 0x64, 0xF0, 0x60, 0x60, 0xF0, 0x00, // 102 f     ( Lowercase letter f , minuscule f )
	0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8, // 103 g     ( Lowercase letter g , minuscule g )
	0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00, // 104 h     ( Lowercase letter h , minuscule h )
	0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00, // 105 i     ( Lowercase letter i , minuscule i )
	0x0C, 0x00, 0x1C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, // 106 j     ( Lowercase letter j , minuscule j )
	0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00, // 107 k     ( Lowercase letter k , minuscule k )
	0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00, // 108 l     ( Lowercase letter l , minuscule l )
	0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xD6, 0x00, // 109 m     ( Lowercase letter m , minuscule m )
	0x00, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, // 110 n     ( Lowercase letter n , minuscule n )
	0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00, // 111 o     ( Lowercase letter o , minuscule o )
	0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0, // 112 p     ( Lowercase letter p , minuscule p )
	0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E, // 113 q     ( Lowercase letter q , minuscule q )
	0x00, 0x00, 0xDC, 0x76, 0x62, 0x60, 0xF0, 0x00, // 114 r     ( Lowercase letter r , minuscule r )
	0x00, 0x00, 0x7C, 0xC0, 0x70, 0x1C, 0xF8, 0x00, // 115 s     ( Lowercase letter s , minuscule s )
	0x10, 0x30, 0xFC, 0x30, 0x30, 0x34, 0x18, 0x00, // 116 t     ( Lowercase letter t , minuscule t )
	0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, // 117 u     ( Lowercase letter u , minuscule u )
	0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00, // 118 v     ( Lowercase letter v , minuscule v )
	0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00, // 119 w     ( Lowercase letter w , minuscule w )
	0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00, // 120 x     ( Lowercase letter x , minuscule x )
	0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8, // 121 y     ( Lowercase letter y , minuscule y )
	0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00, // 122 z     ( Lowercase letter z , minuscule z )
	0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00, // 123 {     ( braces or curly brackets, opening braces )
	0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00, // 124 |     ( vertical-bar, vbar, vertical line or vertical slash )
	0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00, // 125 }     ( curly brackets or braces, closing curly brackets )
	0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 126 ~     ( Tilde ; swung dash )

	0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00, // 127 DEL   ( Delete )

    // Extended characters
	0x7C, 0xC6, 0xC0, 0xC6, 0x7C, 0x0C, 0x06, 0x7C,
	0x00, 0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
	0x1C, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
	0x7E, 0x81, 0x3C, 0x06, 0x3E, 0x66, 0x3B, 0x00,
	0xCC, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0xE0, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0x30, 0x30, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0x00, 0x00, 0x7C, 0xC6, 0xC0, 0x78, 0x0C, 0x38,
	0x7E, 0x81, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00,
	0xCC, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
	0xE0, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00,
	0xCC, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
	0x7C, 0x82, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00,
	0xE0, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
	0xC6, 0x10, 0x7C, 0xC6, 0xFE, 0xC6, 0xC6, 0x00,
	0x30, 0x30, 0x00, 0x78, 0xCC, 0xFC, 0xCC, 0x00,
	0x1C, 0x00, 0xFC, 0x60, 0x78, 0x60, 0xFC, 0x00,
	0x00, 0x00, 0x7F, 0x0C, 0x7F, 0xCC, 0x7F, 0x00,
	0x3E, 0x6C, 0xCC, 0xFE, 0xCC, 0xCC, 0xCE, 0x00,
	0x78, 0x84, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,
	0x00, 0xCC, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,
	0x00, 0xE0, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,
	0x78, 0x84, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
	0x00, 0xE0, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
	0x00, 0xCC, 0x00, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,
	0xC3, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00,
	0xCC, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x00,
	0x18, 0x18, 0x7E, 0xC0, 0xC0, 0x7E, 0x18, 0x18,
	0x38, 0x6C, 0x64, 0xF0, 0x60, 0xE6, 0xFC, 0x00,
	0xCC, 0xCC, 0x78, 0x30, 0xFC, 0x30, 0xFC, 0x30,
	0xF8, 0xCC, 0xCC, 0xFA, 0xC6, 0xCF, 0xC6, 0xC3,
	0x0E, 0x1B, 0x18, 0x3C, 0x18, 0x18, 0xD8, 0x70,
	0x1C, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00,
	0x38, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,
	0x00, 0x1C, 0x00, 0x78, 0xCC, 0xCC, 0x78, 0x00,
	0x00, 0x1C, 0x00, 0xCC, 0xCC, 0xCC, 0x76, 0x00,
	0x00, 0xF8, 0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0x00,
	0xFC, 0x00, 0xCC, 0xEC, 0xFC, 0xDC, 0xCC, 0x00,
	0x3C, 0x6C, 0x6C, 0x3E, 0x00, 0x7E, 0x00, 0x00,
	0x38, 0x6C, 0x6C, 0x38, 0x00, 0x7C, 0x00, 0x00,
	0x18, 0x00, 0x18, 0x18, 0x30, 0x66, 0x3C, 0x00,
	0x00, 0x00, 0x00, 0xFC, 0xC0, 0xC0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFC, 0x0C, 0x0C, 0x00, 0x00,
	0xC6, 0xCC, 0xD8, 0x36, 0x6B, 0xC2, 0x84, 0x0F,
	0xC3, 0xC6, 0xCC, 0xDB, 0x37, 0x6D, 0xCF, 0x03,
	0x18, 0x00, 0x18, 0x18, 0x3C, 0x3C, 0x18, 0x00,
	0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00,
	0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00,
	0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88,
	0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,
	0xDB, 0xF6, 0xDB, 0x6F, 0xDB, 0x7E, 0xD7, 0xED,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18, 0x18,
	0x18, 0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18,
	0x36, 0x36, 0x36, 0x36, 0xF6, 0x36, 0x36, 0x36,
	0x00, 0x00, 0x00, 0x00, 0xFE, 0x36, 0x36, 0x36,
	0x00, 0x00, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18,
	0x36, 0x36, 0xF6, 0x06, 0xF6, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
	0x00, 0x00, 0xFE, 0x06, 0xF6, 0x36, 0x36, 0x36,
	0x36, 0x36, 0xF6, 0x06, 0xFE, 0x00, 0x00, 0x00,
	0x36, 0x36, 0x36, 0x36, 0xFE, 0x00, 0x00, 0x00,
	0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18,
	0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x37, 0x30, 0x3F, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3F, 0x30, 0x37, 0x36, 0x36, 0x36,
	0x36, 0x36, 0xF7, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0x00, 0xF7, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36,
	0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0x36, 0x36, 0xF7, 0x00, 0xF7, 0x36, 0x36, 0x36,
	0x18, 0x18, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0x36, 0x36, 0x36, 0x36, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0x00, 0xFF, 0x18, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0x3F, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x1F, 0x18, 0x1F, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x00, 0x3F, 0x36, 0x36, 0x36,
	0x36, 0x36, 0x36, 0x36, 0xFF, 0x36, 0x36, 0x36,
	0x18, 0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x76, 0xDC, 0xC8, 0xDC, 0x76, 0x00,
	0x00, 0x78, 0xCC, 0xF8, 0xCC, 0xF8, 0xC0, 0xC0,
	0x00, 0xFC, 0xCC, 0xC0, 0xC0, 0xC0, 0xC0, 0x00,
	0x00, 0x00, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x00,
	0xFC, 0xCC, 0x60, 0x30, 0x60, 0xCC, 0xFC, 0x00,
	0x00, 0x00, 0x7E, 0xD8, 0xD8, 0xD8, 0x70, 0x00,
	0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xC0,
	0x00, 0x76, 0xDC, 0x18, 0x18, 0x18, 0x18, 0x00,
	0xFC, 0x30, 0x78, 0xCC, 0xCC, 0x78, 0x30, 0xFC,
	0x38, 0x6C, 0xC6, 0xFE, 0xC6, 0x6C, 0x38, 0x00,
	0x38, 0x6C, 0xC6, 0xC6, 0x6C, 0x6C, 0xEE, 0x00,
	0x1C, 0x30, 0x18, 0x7C, 0xCC, 0xCC, 0x78, 0x00,
	0x00, 0x00, 0x7E, 0xDB, 0xDB, 0x7E, 0x00, 0x00,
	0x06, 0x0C, 0x7E, 0xDB, 0xDB, 0x7E, 0x60, 0xC0,
	0x38, 0x60, 0xC0, 0xF8, 0xC0, 0x60, 0x38, 0x00,
	0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,
	0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00,
	0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x7E, 0x00,
	0x60, 0x30, 0x18, 0x30, 0x60, 0x00, 0xFC, 0x00,
	0x18, 0x30, 0x60, 0x30, 0x18, 0x00, 0xFC, 0x00,
	0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0x70,
	0x18, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x76, 0xDC, 0x00, 0x76, 0xDC, 0x00, 0x00,
	0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
	0x0F, 0x0C, 0x0C, 0x0C, 0xEC, 0x6C, 0x3C, 0x1C,
	0x58, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00,
	0x70, 0x98, 0x30, 0x60, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void configureScreen(uint8_t *registers, bool isVideoMode);

#endif /* DRIVER_VGA_H_ */
