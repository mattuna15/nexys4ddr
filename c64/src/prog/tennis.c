//
// This implements a simple tennis game
//

#include "memorymap.h"
#include "zeropage.h"   // Variables to be stored in the zero-page.
#include "tennis.h"
#include "tennis_ball.h"
#include "tennis_player.h"

static const char bitmap_ball[32] = {
   0x01, 0x80,
   0x0F, 0xF0,
   0x3F, 0xFC,
   0x3F, 0xFC,
   0x7F, 0xFE,
   0x7F, 0xFE,
   0x7F, 0xFE,
   0xFF, 0xFF,
   0xFF, 0xFF,
   0x7F, 0xFE,
   0x7F, 0xFE,
   0x7F, 0xFE,
   0x3F, 0xFC,
   0x3F, 0xFC,
   0x0F, 0xF0,
   0x01, 0x80};

static const char bitmap_player[32] = {
   0x00, 0x00,
   0x00, 0x00,
   0x00, 0x00,
   0x00, 0x00,
   0x00, 0x00,
   0x00, 0x00,
   0x00, 0x00,
   0x00, 0x00,
   0x01, 0x80,
   0x0F, 0xF0,
   0x3F, 0xFC,
   0x3F, 0xFC,
   0x7F, 0xFE,
   0x7F, 0xFE,
   0x7F, 0xFE,
   0xFF, 0xFF};

static const char bitmap_wall[32] = {
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0,
   0x03, 0xC0};

void __fastcall__ my_memcpy(void)
{
loop:
   __asm__("DEY");
   __asm__("LDA (%b),Y", ZP_SRC_LO);
   __asm__("STA (%b),Y", ZP_DST_LO);
   __asm__("TYA");
   __asm__("BNE %g", loop);
} // end of my_memcpy

void __fastcall__ my_memset(void)
{
   __asm__("TAX");
loop:
   __asm__("DEY");
   __asm__("TXA");
   __asm__("STA (%b),Y", ZP_DST_LO);
   __asm__("TYA");
   __asm__("BNE %g", loop);
} // end of my_memset

static void __fastcall__ clearScreen(void)
{
   // Clear the screen
   __asm__("LDA #$00");
   __asm__("STA %b", ZP_SCREEN_POS_LO);
   __asm__("LDA #$80");
   __asm__("STA %b", ZP_SCREEN_POS_HI);
   __asm__("LDA #$00");
   __asm__("TAY");
clear:
   __asm__("LDA #$20");
   __asm__("STA (%b),Y", ZP_SCREEN_POS_LO);
   __asm__("INY");
   __asm__("BNE %g", clear);
   __asm__("LDA %b", ZP_SCREEN_POS_HI);
   __asm__("CLC");
   __asm__("ADC #$01");
   __asm__("STA %b", ZP_SCREEN_POS_HI);
   __asm__("CMP #$84");
   __asm__("BNE %g", clear);
   __asm__("RTS");
} // end of clearScreen

void __fastcall__ vga_init(void)
{
   // Configure text color
   __asm__("LDA #%b", COL_LIGHT);
   __asm__("STA %w",  VGA_ADDR_FGCOL);
   __asm__("LDA #%b", COL_DARK);
   __asm__("STA %w",  VGA_ADDR_BGCOL);

   // Configure sprite 0 as the ball
   __asm__("LDA #<%w", VGA_ADDR_SPRITE_0_BITMAP);
   __asm__("STA %b", ZP_DST_LO);
   __asm__("LDA #>%w", VGA_ADDR_SPRITE_0_BITMAP);
   __asm__("STA %b", ZP_DST_HI);
   __asm__("LDA #<%v", bitmap_ball);
   __asm__("STA %b", ZP_SRC_LO);
   __asm__("LDA #>%v", bitmap_ball);
   __asm__("STA %b", ZP_SRC_HI);
   __asm__("LDA #$20");
   __asm__("TAY");
   my_memcpy();

   // Configure sprite 1 as the left player
   __asm__("LDA #<%w", VGA_ADDR_SPRITE_1_BITMAP);
   __asm__("STA %b", ZP_DST_LO);
   __asm__("LDA #>%w", VGA_ADDR_SPRITE_1_BITMAP);
   __asm__("STA %b", ZP_DST_HI);
   __asm__("LDA #<%v", bitmap_player);
   __asm__("STA %b", ZP_SRC_LO);
   __asm__("LDA #>%v", bitmap_player);
   __asm__("STA %b", ZP_SRC_HI);
   __asm__("LDA #$20");
   __asm__("TAY");
   my_memcpy();

   // Configure sprite 2 as the right player
   __asm__("LDA #<%w", VGA_ADDR_SPRITE_2_BITMAP);
   __asm__("STA %b", ZP_DST_LO);
   __asm__("LDA #>%w", VGA_ADDR_SPRITE_2_BITMAP);
   __asm__("STA %b", ZP_DST_HI);
   __asm__("LDA #<%v", bitmap_player);
   __asm__("STA %b", ZP_SRC_LO);
   __asm__("LDA #>%v", bitmap_player);
   __asm__("STA %b", ZP_SRC_HI);
   __asm__("LDA #$20");
   __asm__("TAY");
   my_memcpy();

   // Configure sprite 3 as the wall
   __asm__("LDA #<%w", VGA_ADDR_SPRITE_3_BITMAP);
   __asm__("STA %b", ZP_DST_LO);
   __asm__("LDA #>%w", VGA_ADDR_SPRITE_3_BITMAP);
   __asm__("STA %b", ZP_DST_HI);
   __asm__("LDA #<%v", bitmap_wall);
   __asm__("STA %b", ZP_SRC_LO);
   __asm__("LDA #>%v", bitmap_wall);
   __asm__("STA %b", ZP_SRC_HI);
   __asm__("LDA #$20");
   __asm__("TAY");
   my_memcpy();

   // Configure ball
   __asm__("LDA #<%w", WALL_XPOS/2);
   __asm__("STA %w", VGA_ADDR_SPRITE_0_X);
   __asm__("LDA #>%w", WALL_XPOS/2);
   __asm__("STA %w", VGA_ADDR_SPRITE_0_X_MSB);
   __asm__("LDA #%b", WALL_YPOS/2);
   __asm__("STA %w", VGA_ADDR_SPRITE_0_Y);
   __asm__("LDA #%b", COL_RED);
   __asm__("STA %w", VGA_ADDR_SPRITE_0_COL);
   __asm__("LDA #$01");
   __asm__("STA %w", VGA_ADDR_SPRITE_0_ENA);

   // Configure left player
   __asm__("LDA #<%w", WALL_XPOS/2);
   __asm__("STA %w", VGA_ADDR_SPRITE_1_X);
   __asm__("LDA #>%w", WALL_XPOS/2);
   __asm__("STA %w", VGA_ADDR_SPRITE_1_X_MSB);
   __asm__("LDA #%b", WALL_YPOS);
   __asm__("STA %w", VGA_ADDR_SPRITE_1_Y);
   __asm__("LDA #%b", COL_LIGHT);
   __asm__("STA %w", VGA_ADDR_SPRITE_1_COL);
   __asm__("LDA #$01");
   __asm__("STA %w", VGA_ADDR_SPRITE_1_ENA);

   // Configure right player
   __asm__("LDA #<%w", (3*WALL_XPOS)/2);
   __asm__("STA %w", VGA_ADDR_SPRITE_2_X);
   __asm__("LDA #>%w", WALL_XPOS/2);
   __asm__("STA %w", VGA_ADDR_SPRITE_2_X_MSB);
   __asm__("LDA #%b", WALL_YPOS);
   __asm__("STA %w", VGA_ADDR_SPRITE_2_Y);
   __asm__("LDA #%b", COL_LIGHT);
   __asm__("STA %w", VGA_ADDR_SPRITE_2_COL);
   __asm__("LDA #$01");
   __asm__("STA %w", VGA_ADDR_SPRITE_2_ENA);

   // Configure wall
   __asm__("LDA #<%w", WALL_XPOS);
   __asm__("STA %w", VGA_ADDR_SPRITE_3_X);
   __asm__("LDA #>%w", WALL_XPOS);
   __asm__("STA %w", VGA_ADDR_SPRITE_3_X_MSB);
   __asm__("LDA #%b", WALL_YPOS);
   __asm__("STA %w", VGA_ADDR_SPRITE_3_Y);
   __asm__("LDA #%b", COL_WHITE);
   __asm__("STA %w", VGA_ADDR_SPRITE_3_COL);
   __asm__("LDA #$01");
   __asm__("STA %w", VGA_ADDR_SPRITE_3_ENA);
} // end of vga_init

// Entry point after CPU reset
void __fastcall__ reset(void)
{
   __asm__("SEI");                           // Disable all interrupts
   __asm__("LDX #$FF");
   __asm__("TXS");                           // Reset stack pointer

   clearScreen();

   // Initialize ball position and velocity
   ball_reset();

   vga_init();

   __asm__("LDA #%b", WALL_YPOS+16);
   __asm__("STA %w", VGA_ADDR_YLINE); // The line number for interrupt
   __asm__("LDA #$01");
   __asm__("STA %w", VGA_ADDR_MASK); // Enable IRQ
   __asm__("CLI");

   // Just busy wait here. Everything is processed during IRQ.
loop:
   __asm__("JMP %g", loop);
} // end of reset

// Maskable interrupt
void __fastcall__ irq(void)
{
   __asm__("LDA %w", VGA_ADDR_IRQ);  // Read IRQ status
   __asm__("STA %w", VGA_ADDR_IRQ);  // Clear IRQ assertion.

   __asm__("LDA %w", VGA_COLL);  // Read collision status
   __asm__("BEQ %g", noColl);
   __asm__("CMP #$03");
   __asm__("BNE %g", noColl);

   // Collision between ball (0) and left player (1)

   // Let CB be the centre of the ball, and
   // let CP be the centre of the player, and
   // let CC be the point of contact.
   // Since the ball and the player both are circles of the
   // same size, then CC is the midpoint between CB and CP.
   //
   // Let v be the velocity vector of the ball, and 
   // let dv be the change (acceleration) in this velocity vector.
   //
   // Some algebra gives the following equation
   // dv = (-2v*BP)/|BP|^2 * BP
   //
   // Note that v*BP must be negative, indicating that the ball
   // is travelling into the player. If v*BP is positive, then
   // no need to update v.
   //
   // Algorithmic complexity:
   // * v*BP requires two multiplications
   // * |BP|^2 requires two multiplcations
   // * * BP requires two multiplications
   // * / requires one division
   // So a total of six multiplications and one division.
   //
   // One way to organize the calculations is as follows:
   // Let w be the new velocity, i.e. w = v+dv. Then
   // w = A*v, where A11=-A22=(BPy^2-BPx^2)/(BPy^2+BPx^2) and A12=A21=(-2*BPx*BPy)/(BPy^2+BPx^2).
   //
   // This requires 7 multiplications and two divisions.
   // A-I = -2*(BPx,BPy)^T*(BPx,BPy)
   //
   //
   // Example:
   // CB = (0x4A, 0xD5)
   // CP = (0x50, 0xDA) + (0,8)
   // v  = vertical = (0, +1)
   //
   // We calculate BP = (-6, -13). We should have |BP| = 16, but we only
   // have |BP| = 14. That's ok :-)
   //
   // We then calculate v*BP = -13, and so:
   // dv = 13/128 * BP
   //
   // The matrix A is: 1/205 * ((133, -156), (-156, 133)), and so
   // w = 1/205 * (-156, -133).

noColl:
   ball_move();
   player_move();

   __asm__("LDA %w", VGA_ADDR_SPRITE_0_Y);
   __asm__("CMP #%b", WALL_YPOS+16);
   __asm__("BCC %g", end);

   // Ball fell out of bottom of screen.
   ball_reset();

end:
   __asm__("RTI");
} // end of irq

// Non-maskable interrupt
void __fastcall__ nmi(void)
{
   // Not used.
   __asm__("RTI");
} // end of nmi

