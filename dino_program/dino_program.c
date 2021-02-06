/*	for cc65, for NES
 *	simple platformer game
 *	doug fraker 2015
 *	feel free to reuse any code here
 */





#include "DEFINE.c"





void main (void) {
	All_Off(); // turn off screen
	Draw_Background();
	X1 = 0x10;
	Y1 = 0x20; // middle of screen
	Load_Palette();
	Wait_Vblank();
	All_On(); // turn on screen
    Reset_Scroll();
    X_speed = 0x40;
	while (1){ // infinite loop
		while (NMI_flag == 0); // wait till NMI
        if ((Frame_Count & 0xf) == 2){
            ++run_stage;
		    run_stage &= 1; // keep it 0 or 1          
        }
		//every_frame();	// moved this to the nmi code in reset.s for greater stability
		Get_Input();
		move_logic();
		update_Sprites();
		NMI_flag = 0;
        
	}
}
	
// inside the startup code, the NMI routine will ++NMI_flag and ++Frame_Count at each V-blank
	
	
	
	
void All_Off (void) {
	PPU_CTRL = 0;
	PPU_MASK = 0; 
}
	
void All_On (void) {
	PPU_CTRL = 0x90; // screen is on, NMI on
	PPU_MASK = 0x1e; 
}
	
void Reset_Scroll (void) {
	PPU_ADDRESS = 0;
	PPU_ADDRESS = 0;
	SCROLL = 0;
	SCROLL = 0;
}
	
void Load_Palette (void) {
	PPU_ADDRESS = 0x3f;
	PPU_ADDRESS = 0x00;
	for( index = 0; index < sizeof(PALETTE); ++index ){
		PPU_DATA = PALETTE[index];
	}
}

void update_Sprites (void) {
	state4 = state << 3; // shift left 2 = multiply 4
	index4 = 0;
	for (index = 0; index < 16; ++index ){
        move = 0;
        if (index > 12) {
            if (run_stage == 1) {
                move = 4;
            }
        }
		SPRITES[index4] = MetaSprite_Y[index] + Y1; // relative y + master y
		++index4;
		SPRITES[index4] = MetaSprite_Tile_R[index+move]; // tile numbers
		++index4;
		SPRITES[index4] = MetaSprite_Attrib_R[index]; // attributes, all zero here
		++index4;
		SPRITES[index4] = MetaSprite_X[index] + X1; // relative x + master x
		++index4;
	}
}
void move_logic (void) {

	collision = 0;
	if (Y1 > 0x73)
		collision = 1;
	if (collision == 0){
		Y_speed += 4; // gravity
	}
	else {
		Y_speed = 0;	// collision = stop falling
	}
	
	// Jump - we already figured if we are on a platform, only jump if on a platform
	if (collision > 0){
		if (((joypad1 & A_BUTTON) != 0) && ((joypad1old & A_BUTTON) == 0)){
			Y_speed = -0x38; // 0xc8
		}
	}
	
	// max speeds
    if (Y_speed > 0x20)
		Y_speed = 0x20;
		
	// move player
	Horiz_scroll_Old = Horiz_scroll;
	Horiz_scroll += (X_speed >> 4); // use the high nibble
    if (Horiz_scroll_Old > Horiz_scroll) // if pass 0, switch nametables
			++Nametable;
	Nametable &= 1; // keep it 1 or 0
	
	Y1 += (Y_speed >> 3); // use the high nibble
}

void Draw_Background(void) {
 	PPU_ADDRESS = 0x22;  	//	set an address in the PPU of 0x21ca
    PPU_ADDRESS = 0x20;  	//	about the middle of the screen
    for( index = 0; index < 32; ++index ){
	    PPU_DATA = 0x03;
    }
    PPU_ADDRESS = 0x26; // address of nametable #1 = 0x2400
	PPU_ADDRESS = 0x20;
    for( index = 0; index < 32; ++index ){
	    PPU_DATA = 0x03;
    }
    PPU_ADDRESS = 0x22;
    PPU_ADDRESS = 0x5f;
	PPU_DATA = 0x03;
 	PPU_ADDRESS = 0x26;  	//	set an address in the PPU of 0x21ca
    PPU_ADDRESS = 0x4f;
	PPU_DATA = 0x04;
}
