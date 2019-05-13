#ifndef LCD_H
#define	LCD_H

#include <avr/io.h>
#include "always.h"
#include "sbit.h"

#ifndef PIN
#define PIN(n,x,y) n SBIT(x,y) // sbit n = x^y
#endif


//#include<reg52.h>

// Set exactly one of these screen types to TRUE so that code is generated properly.
// If no type is enabled, you will get errors!
//
//	Format			Status		Notes			All part numbers are Hantronix (www.hantronix.com)
//	===============		=======		=====================	==================================================>>
#define	LCD_MODE_1x8		(FALSE)					// HDM08111H-1
#define	LCD_MODE_1x16_A		(FALSE)		// Row=1, Col=1..16	// HDM16116H-7, HDM16116H-L, HDM16116L*, HDM16116L-7, HDM16116L-L, HDM16116L-1, HDM16116H-2, HDM16116L-2
#define	LCD_MODE_1x16_B		(FALSE)		// Row=1..2, Col=1..8	// HDM16116H, HDM16116L* note that this part number could be either A or B
#define	LCD_MODE_1x40		(FALSE)					// HDM40108H-2
#define	LCD_MODE_2x8		(FALSE)					// HDM08216H-1, HDM08216L, HDM16216H-2
#define	LCD_MODE_2x12		(FALSE)					// HDM12216H, HDM12216L
#define	LCD_MODE_2x16		(TRUE)					// HDM16216H-2, HDM16216H-4, HDM16216H-5, HDM16216H-B, HDM16216H-D, HDM16216L-2, HDM16216L-5, HDM16216L-7, HDM16216L-D, HDM16216L-B, HDM16216H-I
#define	LCD_MODE_2x20		(FALSE)					// HDM20216H-3, HDM20216L, HDM20216L-1, HDM20216L-L, HDM20216H-L
#define	LCD_MODE_2x24		(FALSE)					// HDM24216H-2 HDM24216L-2
#define	LCD_MODE_2x40		(FALSE)					// HDM40216H-4, HDM40216L, HDM40216L-1
#define	LCD_MODE_4x16		(FALSE)					// HDM16416H, HDM16416L
#define	LCD_MODE_4x20		(FALSE)					// HDM40216H-4, HDM40216L, HDM40216L-1
#define LCD_MODE_4x24		(FALSE)		// UNSUPPORTED!		// HDM24416H, HDM24416L
#define	LCD_MODE_4x40		(FALSE)		// UNSUPPORTED!		// HDM40416H, HDM40416L-4, HDM40416H-5

#define	LCD_CURSOR_BLINK	(TRUE)					// Blink/Noblink cursor mode
#define	LCD_CURSOR_ON		(TRUE)					// Cursor visible
#define	LCD_CURSOR_INCREMENT	(TRUE)					// Set left-to-right cursor movementuu
#define	LCD_CURSOR_SHIFT	(FALSE)					// Shift display on entry

#define	LCD_DISPLAY_5x10	(FALSE)					// Select 5x7 or 5x10 character matrix
#define	LCD_DISPLAY_ON		(TRUE)					// Turn display on/off

#define	LCD_ALLOW_USER_CHARS	(TRUE)					// Controls whether display uses ASCII for control chars or uses user-defined chars in lcd_putc()
#define	LCD_ENABLE_GETC		(FALSE)					// Save code space by setting to FALSE
#define	LCD_ENABLE_GOTOXY	(TRUE)					//   any functions which you will not
#define	LCD_ENABLE_PRINTF	(TRUE)					//   need in your application.
#define	LCD_ENABLE_UNSCROLL	(FALSE)
#define	LCD_ENABLE_SCROLL	(TRUE)
#define	LCD_ENABLE_CLEAR	(TRUE)

#if (LCD_ENABLE_SCROLL)
#define	LCD_ENABLE_GOTOXY	(TRUE)					// Over-ride turning it off because these function
#endif
#if (LCD_ENABLE_UNSCROLL)
#define	LCD_ENABLE_GOTOXY	(TRUE)					//   need access to the gotoxy function.
#endif
#if (!LCD_ALLOW_USER_CHARS)
#define	LCD_ENABLE_GOTOXY	(TRUE)					//   need access to the gotoxy function.
#endif

#define	LCD_DATA_PORT		PORTB					// Port on which LCD data lines are connected
#define	LCD_TRIS_PORT		DDRB					// Need to specify the corresponding TRIS

#define	LCD_4_BIT_MODE		(TRUE)					// 4-bit or 8-bit interface selection

#if LCD_4_BIT_MODE
#define	LCD_D4_BIT		4	//0					// Attachment of D0 to data port bus - note
#endif									//   that they MUST be contiguous and in order!

#if LCD_4_BIT_MODE

#if LCD_D4_BIT == 0
#define	LCD_TRIS_DATAMASK	((char)(0x0f))			// Define the bitmask used to read/write the data bits
#else 
#define	LCD_TRIS_DATAMASK	((char)(0xf0))			// Define the bitmask used to read/write the data bits
#endif

#else
#define	LCD_TRIS_DATAMASK	((byte)(0xff))			// Define all bits used for 8-bit mode
#endif

#if LCD_ALLOW_USER_CHARS
#else
#define	LCD_DESTRUCTIVE_BS	(TRUE)					// Backspace '\b' erases character
#endif
							    
//static volatile bit LCD_E RB1;
//static volatile bit LCD_RW RB2;
//static volatile bit LCD_RS RB3;
//static volatile bit LCD_TRIS_E TRISB1;
//static volatile bit LCD_TRIS_RW TRISB2;
//static volatile bit LCD_TRIS_RS TRISB3;


#define LCD_E SBIT (PORTA, 7)					// Pin for LCD /E signal
//PIN (LCD_RW,		P, 5);					// not used
#define LCD_RS SBIT(PORTA, 6)					// Pin for LCD /E signal
/*
PIN (SEL0,			P1,0);
PIN (SEL1,			P1,1);
PIN (SEL2,			P1,2);
*/					// Add definitions for direction control
#define LCD_TRIS_E SBIT(DDRA, 7)					// Pin for LCD /E signal
//PIN (LCD_TRIS_RW,	TRISC, 4);
#define LCD_TRIS_RS SBIT(DDRA, 6)					// Pin for LCD /E signal

/////////////////// DO NOT CHANGE ANYTHING BELOW THIS LINE! //////////////

#define	LCD_8_BIT_MODE		(!LCD_4_BIT_MODE)

#if LCD_MODE_1x8
#define LCD_MAXROWS		((byte)(1))				// Number of LCD rows
#define	LCD_MAXCOLS		((byte)(8))				// Number of LCD columns
#define	LCD_MULTI_LINE		(FALSE)					// Operate in 1-line or 2-line mode for controller
#endif
#if LCD_MODE_1x16_A
#define LCD_MAXROWS		((byte)(1))
#define	LCD_MAXCOLS		((byte)(16))
#define	LCD_MULTI_LINE		(FALSE)
#endif
#if LCD_MODE_1x16_B
#define LCD_MAXROWS		((byte)(1))
#define	LCD_MAXCOLS		((byte)(16))
#define	LCD_MULTI_LINE		(TRUE)
#endif
#if LCD_MODE_1x40
#define LCD_MAXROWS		((byte)(1))
#define	LCD_MAXCOLS		((byte)(40))
#define	LCD_MULTI_LINE		(TRUE)					// Not sure about this
#endif
#if LCD_MODE_2x8
#define LCD_MAXROWS		((byte)(2))
#define	LCD_MAXCOLS		((byte)(8))
#define	LCD_MULTI_LINE		(TRUE)
#endif
#if LCD_MODE_2x12
#define LCD_MAXROWS		((byte)(2))
#define	LCD_MAXCOLS		((byte)(12))
#define	LCD_MULTI_LINE		(TRUE)
#endif
#if LCD_MODE_2x16
#define LCD_MAXROWS		((byte)(2))
#define	LCD_MAXCOLS		((byte)(16))
#define	LCD_MULTI_LINE		(TRUE)
#endif
#if LCD_MODE_2x20
#define LCD_MAXROWS		((byte)(2))
#define	LCD_MAXCOLS		((byte)(20))
#define	LCD_MULTI_LINE		(TRUE)
#endif
#if LCD_MODE_2x24
#define LCD_MAXROWS		((byte)(2))
#define	LCD_MAXCOLS		((byte)(24))
#define	LCD_MULTI_LINE		(TRUE)
#endif
#if LCD_MODE_2x40
#define LCD_MAXROWS		((byte)(2))
#define	LCD_MAXCOLS		((byte)(40))
#define	LCD_MULTI_LINE		(TRUE)
#endif
#if LCD_MODE_4x16
#define LCD_MAXROWS		((byte)(4))
#define	LCD_MAXCOLS		((byte)(16))
#define	LCD_MULTI_LINE		(TRUE)
#endif
#if LCD_MODE_4x20
#define LCD_MAXROWS		((byte)(4))
#define	LCD_MAXCOLS		((byte)(20))
#define	LCD_MULTI_LINE		(TRUE)
#endif

#define	LCD_COMMAND_CLEAR	((byte)(0x01))				// Clear screen, home cursor, unshift display
#define	LCD_COMMAND_HOME	((byte)(0x02))				// Home cursor, unshift display
#define	LCD_COMMAND_BACKSPACE	((byte)(0x10))				// Move cursor left one (destructive based on LCD_DESTRUCTIVE_BS)
#define	LCD_COMMAND_FWDSPACE	((byte)(0x14))				// Move cursor right one
#define	LCD_COMMAND_PANLEFT	((byte)(0x18))				// Move screen left one
#define	LCD_COMMAND_PANRIGHT	((byte)(0x1C))				// Move screen right one

#ifndef LCD_MAXROWS
/*#define*/ error ("NO LCD MODE SPECIFIED - You need to give a 1x8, 2x8 etc. format!");
#endif

// Mandatory functions:
typedef unsigned char byte;
void lcd_init (void);							// Initialize LCD bus and Mode
void lcd_put(byte c);							// Write character to LCD
void lcd_command(byte c);						// Write command to LCD controller
byte lcd_lineof(byte CursorAddress);					// Calculate cursor row from it's address
byte lcd_cursorpos (void);						// Return address of cursor position

// Optional functions:

#if LCD_ENABLE_GETC
byte lcd_getc (void);							// Read character at cursor
#endif
#if LCD_ENABLE_GOTOXY
void lcd_gotoxy (byte row, byte col);					// Position cursor
void lcd_getxy (byte *row, byte *col);					// Return row and column of cursor position
#endif
#if LCD_ENABLE_PRINTF
void lcd_printf (const char* message);					// Write message to LCD (C string type)
#endif
#if LCD_ENABLE_UNSCROLL
void lcd_unscroll (void);						// Roll scroll backwards one line
#endif
#if LCD_ENABLE_SCROLL
void lcd_scroll (void);							// Scroll up one line
#endif
#if LCD_ENABLE_CLEAR
void lcd_clear (void);							// Clear LCD screen
#endif
#if LCD_ALLOW_USER_CHARS
void lcd_define_char (byte c, const byte *bitmap);			// Define user-defined char
#endif

#define CONST_US_DELAY 10 //this many microsecs delay between asserting pins etc.
#endif


