#include <RGBmatrixPanel.h>

#define CLK  8   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

char temp[16];

uint8_t query_status[]    = {0x01, 0x03, 0x00, 0x0A, 0x00, 0x01, 0xA4, 0x08};
uint8_t query_bvolt[]     = {0x01, 0x03, 0x01, 0x01, 0x00, 0x01, 0xD4, 0x36};
uint8_t query_svolt[]     = {0x01, 0x03, 0x01, 0x07, 0x00, 0x01, 0x34, 0x37};
uint8_t query_scurrent[]  = {0x01, 0x03, 0x01, 0x08, 0x00, 0x01, 0x04, 0x34};
uint8_t query_spower[]    = {0x01, 0x03, 0x01, 0x09, 0x00, 0x01, 0x55, 0xF4};
uint8_t query_lvolt[]     = {0x01, 0x03, 0x01, 0x04, 0x00, 0x01, 0xC4, 0x37};
uint8_t query_lcurrent[]  = {0x01, 0x03, 0x01, 0x05, 0x00, 0x01, 0x95, 0xF7};
uint8_t command_load_on[] = {0x01, 0x06, 0x01, 0x0A, 0x00, 0x01, 0x69, 0xF4};
uint8_t command_load_off[]= {0x01, 0x05, 0x01, 0x0A, 0x00, 0x00, 0xA8, 0x34};

#define IMAGE_4h_logo_WIDTH 50
#define IMAGE_4h_logo_HEIGHT 28
const uint8_t IMAGE_4h_logo[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,5,2,2,5,2,2,5,2,2,5,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

};

void doQuery(uint8_t data[], uint8_t len, uint8_t resultbuf[]) {
    for (uint8_t i=0; i < len; i++) {
        Serial1.write(data[i]);
    }
    delay(100);
    uint8_t i=0;
    //while (Serial1.available()) Serial.write(Serial1.read());
    for (uint8_t i=0; (i < 10) && (Serial1.available() > 0); i++) resultbuf[i] = Serial1.read();
}

uint16_t queryInt(uint8_t data[], uint8_t len) {
    doQuery(data, len, (uint8_t*)temp);
    return ((uint16_t)temp[3]<<8) | temp[4];
}

void displayImage(const uint8_t data[], uint8_t width, uint8_t height, int8_t x, int8_t y) {
    uint8_t mheight = min(matrix.height() - y, height);
    uint8_t mwidth = min(matrix.width() - x, width);
    for (uint8_t r=(y<0?-y:0); r < mheight; r++) {
        for (uint8_t c=(x<0?-x:0); c < mwidth; c++) {
            matrix.drawPixel(c+x, r+y, matrix.Color333(data[3*(r*width + c)], data[3*(r*width + c)+1], data[3*(r*width + c)+2]));
        }
    }
}


void setup() {
    // put your setup code here, to run once:
    matrix.begin();
    
    matrix.setTextSize(1);     // size 1 == 8 pixels high
    matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

    matrix.setCursor(0, 0);
    strcpy(temp, "Bat:00.0V");
    matrix.print(temp);
    Serial.begin(9600);
    Serial1.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:


    
    uint16_t bvolt = queryInt(query_bvolt, 8);
    uint16_t svolt = queryInt(query_svolt, 8);
    uint16_t scurrent = queryInt(query_scurrent, 8);
    uint16_t lvolt = queryInt(query_lvolt, 8);
    uint16_t lcurrent = queryInt(query_lcurrent, 8);
    uint32_t spower = (svolt * scurrent) / 100;
    uint32_t lpower = (lvolt * lcurrent) / 100;
    Serial.print("bvolt: "); Serial.print(bvolt);
    Serial.print(", svolt: "); Serial.print(svolt);
    Serial.print(", scurrent: "); Serial.print(scurrent);
    Serial.print(", lvolt: "); Serial.print(lvolt);
    Serial.print(", lcurrent: "); Serial.println(lcurrent);
    strcpy(temp, "BAT: 00.0V");
    temp[8] = bvolt % 10 + '0'; bvolt /= 10;
    temp[6] = bvolt % 10 + '0'; bvolt /= 10;
    temp[5] = bvolt % 10 + '0';
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.setCursor(0, 0);
    matrix.println(temp);

    strcpy(temp, "PWR:  0.0W");
    temp[8] = spower % 10 + '0'; spower /= 10;
    temp[6] = spower % 10 + '0'; spower /= 10;
    if (spower > 0) {
        temp[5] = spower % 10 + '0'; spower /= 10;
        if (spower > 0) temp[4] = spower % 10 + '0';
    }
    matrix.println(temp);

    strcpy(temp, "LPR:  0.0W");
    temp[8] = lpower % 10 + '0'; lpower /= 10;
    temp[6] = lpower % 10 + '0'; lpower /= 10;
    if (lpower > 0) {
        temp[5] = lpower % 10 + '0'; lpower /= 10;
        if (lpower > 0) temp[4] = lpower % 10 + '0';
    }
    matrix.print(temp);

    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'l':
                Serial.println("Enabling load");
                doQuery(command_load_on, 8, (uint8_t*)temp);
                break;
            case 'L':
                Serial.println("Disabling load");
                doQuery(command_load_off, 8, (uint8_t*)temp);
                break;
        }
        while (Serial.available()) Serial.read();
    }

    delay(2000);

    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.setCursor(0, 0);
    matrix.println("This board");
    matrix.println("is powered");
    matrix.println("by solar");
    matrix.println("energy!");

    delay(2000);

    matrix.fillScreen(matrix.Color333(0, 0, 0));
    displayImage(IMAGE_4h_logo, IMAGE_4h_logo_WIDTH, IMAGE_4h_logo_HEIGHT, 10, 2);

    delay(2000);

    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.setCursor(0, 0);
    matrix.println("Sponsored");
    matrix.println("by");
    matrix.setTextColor(matrix.Color333(7, 0, 0));
    matrix.setCursor(14, 16);
    matrix.print("ROOFEX");
    matrix.setTextColor(matrix.Color333(3, 3, 3));
    matrix.setCursor(23, 24);
    matrix.print("NRG");
    matrix.setTextColor(matrix.Color333(7, 7, 7));

    delay(2000);
}