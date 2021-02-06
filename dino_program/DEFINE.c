// let's define some stuff

#define PPU_CTRL		*((unsigned char*)0x2000)
#define PPU_MASK		*((unsigned char*)0x2001)
#define PPU_STATUS		*((unsigned char*)0x2002)
#define OAM_ADDRESS		*((unsigned char*)0x2003)
#define SCROLL			*((unsigned char*)0x2005)
#define PPU_ADDRESS		*((unsigned char*)0x2006)
#define PPU_DATA		*((unsigned char*)0x2007)
#define OAM_DMA			*((unsigned char*)0x4014)


#define RIGHT		0x01
#define LEFT		0x02
#define DOWN		0x04
#define UP			0x08
#define START		0x10
#define SELECT		0x20
#define B_BUTTON	0x40
#define A_BUTTON	0x80
#define RAND_MAX    0x05

// Globals
// our startup code initialized all values to zero
#pragma bss-name(push, "ZEROPAGE")
unsigned char NMI_flag;
unsigned char Frame_Count;
unsigned char index;
unsigned char index4;
unsigned char X1;
unsigned char Y1;
unsigned char state;
unsigned char state4;
unsigned char joypad1;
unsigned char joypad1old;
unsigned char joypad1test; 
unsigned char joypad2; 
unsigned char joypad2old;
unsigned char joypad2test;
unsigned char Horiz_scroll; 
unsigned char Horiz_scroll_Old; 
unsigned int Scroll_Adjusted_X;
// got rid of Vert_scroll, now always 0
unsigned char Nametable;
unsigned char NametableB;
unsigned char walk_count; // changes the animation
signed char X_speed; // signed char = -128 to 127
signed char Y_speed; // signed char = -128 to 127
unsigned char direction; // 0 = R, 1 = L
unsigned char collision_Index;
unsigned char high_byte;
unsigned char collision;
unsigned char temp;
unsigned char hide;
unsigned char move;
unsigned char run_stage;
unsigned char Scroll_count;
#pragma bss-name(push, "OAM")
unsigned char SPRITES[256];
// OAM equals ram addresses 200-2ff
#pragma bss-name(push, "MAP")
unsigned char C_MAP[256];
unsigned char C_MAP2[256];
// MAP equals ram addresses 300-4ff, collision map, metatiles


//#include "BG/card1.h"
//#include "BG/card2.h"
//#include "BG/N1.csv"
//#include "BG/N2.csv"
// collision maps called C1 and C2
// now their value is 0-11, which will index to this array...

const unsigned char PLATFORM[]={ // which metatiles act like platforms
	0, 1, 0, 1, 0, 1, 
	0, 0, 0, 0, 0, 0, 0};
const unsigned char HIDE[]={ // which metatiles act like platforms
	0, 0, 4, 4, 0, 0, 
	0, 0, 0, 0, 0, 0, 0};
//const unsigned char PALETTE[]={
//0x0f, 0x16, 0x06, 0x0f,  0, 8, 0x18, 0x39,  0, 0, 0x10, 0x20,  0, 0x0a, 0x1a, 0x2a,
//0x0f, 0x37, 0x16, 0x0f, 0x0f, 0x37, 0x16, 0x0f,  0x0f, 0x37, 0x16, 0x0f,  0, 0, 5, 0x15};

const unsigned char PALETTE[]={ 0x30,0x00,0x00,0x30, 0x0f,0x17,0x38,0x07, 0x0f,0x21,0x20,0x31, 0x0f,0x06,0x39,0x17, 
0x30,0x00,0x00,0x30, 0x0f, 0x37, 0x16, 0x0f,  0x0f, 0x37, 0x16, 0x0f,  0, 0, 5, 0x15};


const unsigned char Walk_Moves[] = {
	0, 1, 0, 2}; // just a way to reuse the #0 state tiles
    
	
const unsigned char MetaSprite_Y[] = {0,  0,  0,  0,
                                      8,  8,  8,  8,
                                      16, 16, 16, 16,
                                      24, 24, 24, 24 }; // relative y coordinates

const unsigned char MetaSprite_Tile_R[] = { // tile numbers, right
    0x00,0x00,0x01,0x02,    //dino body line1
    0x03,0x04,0x05,0x06,    //dino body line2
    0x07,0x08,0x09,0x0a,   //dino body line3
    0x0b,0x0c,0x0d,0x00,  //dino state1
    0x11,0x12,0x13,0x00,  //dino state2
    };
const unsigned char MetaSprite_Attrib_R[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // attributes = not flipped

const unsigned char MetaSprite_X[] = {0, 8, 16, 24,0, 8, 16, 24,0, 8, 16, 24,0, 8, 16, 24}; // relative x coordinates
// we are using 4 sprites, each one has a relative position from the top left sprite


// Prototypes
void All_Off(void);
void All_On(void);
void Reset_Scroll (void);
void Load_Palette(void);
void Wait_Vblank(void);
void update_Sprites (void);
void Collision_Down(void);
void move_logic(void);
void Draw_Background(void);
void __fastcall__ UnRLE(const unsigned char *data);
void Get_Input(void);
void __fastcall__ memcpy (void* dest, const void* src, int count);

void gray_line(void);
