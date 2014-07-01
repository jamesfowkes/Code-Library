/****************************************************************************
 Title	:   HD44780U LCD library
 Author:    James Fowes <jamesfowkes@gmail.com> adapted from Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury

 DESCRIPTION
       Basic routines for interfacing a HD44780U-based text lcd display

       Adapted from Peter Fleury's AVR lcd library. Made generic for any micro.
	   
       Originally based on Volker Oth's lcd library,
       changed lcd_init(), added additional constants for lcd_command(),
       added 4-bit I/O mode, improved and optimized code.

 USAGE
       See the C include lcd.h file for a description of each function
       
*****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "util_memory_placement.h"

#include "lcd_platform_defs.h"
#include "lib_io.h"
#include "lcd.h"

/*
** Defines and Typedefs
*/

#define lcd_e_high()    IO_On(*s_pData->enPort, s_pPins->enPin)
#define lcd_e_low()     IO_Off(*s_pData->enPort, s_pPins->enPin)
#define lcd_e_toggle()  toggle_e()
#define lcd_rw_high()   IO_On(*s_pData->rwPort, s_pPins->rwPin)
#define lcd_rw_low()    IO_Off(*s_pData->rwPort, s_pPins->rwPin)
#define lcd_rs_high()   IO_On(*s_pData->rsPort, s_pPins->rsPin)
#define lcd_rs_low()    IO_Off(*s_pData->rsPort, s_pPins->rsPin)

#define KS0073_EXTENDED_FUNCTION_REGISTER_ON  0x2C   /* |0|010|1100 4-bit mode, extension-bit RE = 1 */
#define KS0073_EXTENDED_FUNCTION_REGISTER_OFF 0x28   /* |0|010|1000 4-bit mode, extension-bit RE = 0 */
#define KS0073_4LINES_MODE                    0x09   /* |0|000|1001 4 lines mode */

enum lcd_io_state
{
	LCD_OUTPUT,
	LCD_INPUT,
};
typedef enum lcd_io_state LCD_IO_STATE;

/*
** private variables
*/
static LCD_PORT * s_pData;
static LCD_PORT * s_pDirection;
static LCD_PIN * s_pPins;
static LCD_CONFIG * s_pConfig;

static LCD_IO_STATE s_IOState = LCD_INPUT;

/* 
** function prototypes 
*/

static void toggle_e(void);
static void setIOState(LCD_IO_STATE eState);

/*
** local functions
*/

/* toggle Enable Pin to initiate write */
static void toggle_e(void)
{
    lcd_e_high();
    lcd_e_delay();
    lcd_e_low();
}

static void setIOState(LCD_IO_STATE eState)
{
	if (eState != s_IOState)
	{
		switch(eState)
		{
		case LCD_OUTPUT:
			IO_Output(*(s_pDirection->port0.w), s_pPins->pin0);
			IO_Output(*(s_pDirection->port1.w), s_pPins->pin1);
			IO_Output(*(s_pDirection->port2.w), s_pPins->pin2);
			IO_Output(*(s_pDirection->port3.w), s_pPins->pin3);
			break;
		case LCD_INPUT:
			IO_Input(*(s_pDirection->port0.w), s_pPins->pin0);
			IO_Input(*(s_pDirection->port1.w), s_pPins->pin1);
			IO_Input(*(s_pDirection->port2.w), s_pPins->pin2);
			IO_Input(*(s_pDirection->port3.w), s_pPins->pin3);
			break;
		default:
			break;
		}
		
		s_IOState = eState;
	}
}

/*************************************************************************
Low-level function to write byte to LCD controller
Input:    data   byte to write to LCD
          rs     1: write data    
                 0: write instruction
Returns:  none
*************************************************************************/
static void lcd_write(uint8_t data,uint8_t rs) 
{

    if (rs) {   /* write data        (RS=1, RW=0) */
       lcd_rs_high();
    } else {    /* write instruction (RS=0, RW=0) */
       lcd_rs_low();
    }
    lcd_rw_low();

	setIOState(LCD_OUTPUT);
	
	/* output high nibble first */
	if (data & 0x10) { IO_On(*(s_pData->port0.w), s_pPins->pin0); } else { IO_Off(*(s_pData->port0.w), s_pPins->pin0); }
	if (data & 0x20) { IO_On(*(s_pData->port1.w), s_pPins->pin1); } else { IO_Off(*(s_pData->port1.w), s_pPins->pin1); }
	if (data & 0x40) { IO_On(*(s_pData->port2.w), s_pPins->pin2); } else { IO_Off(*(s_pData->port2.w), s_pPins->pin2); }
	if (data & 0x80) { IO_On(*(s_pData->port3.w), s_pPins->pin3); } else { IO_Off(*(s_pData->port3.w), s_pPins->pin3); }
	
	lcd_e_toggle();
	
	/* output low nibble */
	if (data & 0x01) { IO_On(*(s_pData->port0.w), s_pPins->pin0); } else { IO_Off(*(s_pData->port0.w), s_pPins->pin0); }
	if (data & 0x02) { IO_On(*(s_pData->port1.w), s_pPins->pin1); } else { IO_Off(*(s_pData->port1.w), s_pPins->pin1); }
	if (data & 0x04) { IO_On(*(s_pData->port2.w), s_pPins->pin2); } else { IO_Off(*(s_pData->port2.w), s_pPins->pin2); }
	if (data & 0x08) { IO_On(*(s_pData->port3.w), s_pPins->pin3); } else { IO_Off(*(s_pData->port3.w), s_pPins->pin3); }
	lcd_e_toggle();        
	
	/* all data pins high (inactive) */
	IO_On(*(s_pData->port0.w), s_pPins->pin0);
	IO_On(*(s_pData->port1.w), s_pPins->pin1);
	IO_On(*(s_pData->port2.w), s_pPins->pin2);
	IO_On(*(s_pData->port3.w), s_pPins->pin3);
}

/*************************************************************************
Low-level function to read byte from LCD controller
Input:    rs     1: read data    
                 0: read busy flag / address counter
Returns:  byte read from LCD controller
*************************************************************************/
static uint8_t lcd_read(uint8_t rs) 
{
    uint8_t data = 0;
    
    
    if (rs)
    {
        lcd_rs_high();                       /* RS=1: read data      */
    }
    else
    {
        lcd_rs_low();                        /* RS=0: read busy flag */
    }
    
    lcd_rw_high();                           /* RW=1  read mode      */
    
	/* configure data pins as input */
	setIOState(LCD_INPUT);
	
	/* read high nibble first */
	lcd_e_high();
	delay(50);
	data |= IO_Read(*(s_pData->port0.r), s_pPins->pin0) ? 0x10 : 0x00;
	data |= IO_Read(*(s_pData->port1.r), s_pPins->pin1) ? 0x20 : 0x00;
	data |= IO_Read(*(s_pData->port2.r), s_pPins->pin2) ? 0x40 : 0x00;
	data |= IO_Read(*(s_pData->port3.r), s_pPins->pin3) ? 0x80 : 0x00;
		
	lcd_e_low();
	lcd_e_delay();                       /* Enable 500ns low       */

	/* read low nibble */    
	lcd_e_high();
	delay(50);
	data |= IO_Read(*(s_pData->port0.r), s_pPins->pin0) ? 0x01 : 0x00;
	data |= IO_Read(*(s_pData->port1.r), s_pPins->pin1) ? 0x02 : 0x00;
	data |= IO_Read(*(s_pData->port2.r), s_pPins->pin2) ? 0x04 : 0x00;
	data |= IO_Read(*(s_pData->port3.r), s_pPins->pin3) ? 0x08 : 0x00;

	lcd_e_low();

    return data;
}

/*************************************************************************
loops while lcd is busy, returns address counter
*************************************************************************/
static uint8_t lcd_waitbusy(void)

{
    /* wait until busy flag is cleared */
	#ifndef TEST_HARNESS
	register uint8_t c;
    while ( (c=lcd_read(0)) & (1<<LCD_BUSY)) {PINB |= (1 << 5);}
    #endif
	
    /* the address counter is updated 4us after the busy flag is cleared */
    delay(2);

    /* now read the address counter */
    return (lcd_read(0));  // return address counter
    
}/* lcd_waitbusy */


/*************************************************************************
Move cursor to the start of next line or to the first line if the cursor 
is already on the last line.
*************************************************************************/
static inline void lcd_newline(uint8_t pos)
{
    register uint8_t addressCounter = 0;

	switch(s_pConfig->lines)
	{
	case 1:
		addressCounter = 0;
		break;
	case 2:
		if ( pos < (s_pConfig->line2Start) )
			addressCounter = s_pConfig->line2Start;
		else
			addressCounter = s_pConfig->line1Start;
		break;
	case 4:
	
		if (s_pConfig->type == LCD_CONTROLLER_KS0073)
		{
			if ( pos < s_pConfig->line2Start )
				addressCounter = s_pConfig->line2Start;
			else if ( (pos >= s_pConfig->line2Start) && (pos < s_pConfig->line3Start) )
				addressCounter = s_pConfig->line3Start;
			else if ( (pos >= s_pConfig->line3Start) && (pos < s_pConfig->line4Start) )
				addressCounter = s_pConfig->line4Start;
			else 
				addressCounter = s_pConfig->line1Start;
		}
		else
		{
			if ( pos < s_pConfig->line3Start )
				addressCounter = s_pConfig->line2Start;
			else if ( (pos >= s_pConfig->line2Start) && (pos < s_pConfig->line4Start) )
				addressCounter = s_pConfig->line3Start;
			else if ( (pos >= s_pConfig->line3Start) && (pos < s_pConfig->line2Start) )
				addressCounter = s_pConfig->line4Start;
			else 
				addressCounter = s_pConfig->line1Start;
		}
	}
	
    lcd_command((1<<LCD_DDRAM)+addressCounter);

}/* lcd_newline */


/*
** PUBLIC FUNCTIONS 
*/

/*************************************************************************
Send LCD controller instruction command
Input:   instruction to send to LCD controller, see HD44780 data sheet
Returns: none
*************************************************************************/
void lcd_command(uint8_t cmd)
{
    lcd_waitbusy();
    lcd_write(cmd,0);
}


/*************************************************************************
Send data byte to LCD controller 
Input:   data to send to LCD controller, see HD44780 data sheet
Returns: none
*************************************************************************/
void lcd_data(uint8_t data)
{
    lcd_waitbusy();
    lcd_write(data,1);
}



/*************************************************************************
Set cursor to specified position
Input:    x  horizontal position  (0: left most position)
          y  vertical position    (0: first line)
Returns:  none
*************************************************************************/
void lcd_gotoxy(uint8_t x, uint8_t y)
{
	switch(s_pConfig->lines)
	{
	case 1:
		lcd_command((1<<LCD_DDRAM)+s_pConfig->line1Start+x);
		break;
	case 2:
		if ( y==0 ) 
			lcd_command((1<<LCD_DDRAM)+s_pConfig->line1Start+x);
		else
			lcd_command((1<<LCD_DDRAM)+s_pConfig->line2Start+x);
		break;
	case 4:
		if ( y==0 )
			lcd_command((1<<LCD_DDRAM)+s_pConfig->line1Start+x);
		else if ( y==1)
			lcd_command((1<<LCD_DDRAM)+s_pConfig->line2Start+x);
		else if ( y==2)
			lcd_command((1<<LCD_DDRAM)+s_pConfig->line3Start+x);
		else /* y==3 */
			lcd_command((1<<LCD_DDRAM)+s_pConfig->line4Start+x);
		break;
	}
}/* lcd_gotoxy */


/*************************************************************************
*************************************************************************/
int lcd_getxy(void)
{
    return lcd_waitbusy();
}


/*************************************************************************
Clear display and set cursor to home position
*************************************************************************/
void lcd_clrscr(void)
{
    lcd_command(1<<LCD_CLR);
}


/*************************************************************************
Set cursor to home position
*************************************************************************/
void lcd_home(void)
{
    lcd_command(1<<LCD_HOME);
}


/*************************************************************************
Display character at current cursor position 
Input:    character to be displayed                                       
Returns:  none
*************************************************************************/
void lcd_putc(char c)
{
    uint8_t pos;


    pos = lcd_waitbusy();   // read busy-flag and address counter
    if (c=='\n')
    {
        lcd_newline(pos);
    }
    else
    {
		if (s_pConfig->wrapLines)
		{
			switch(s_pConfig->lines)
			{
			case 1:
				if ( pos == s_pConfig->line1Start+s_pConfig->dispLength ) {
					lcd_write((1<<LCD_DDRAM)+s_pConfig->line1Start,0);
				}
				break;
			case 2:
				if ( pos == s_pConfig->line1Start+s_pConfig->dispLength ) {
					lcd_write((1<<LCD_DDRAM)+s_pConfig->line2Start,0);    
				}else if ( pos == s_pConfig->line2Start+s_pConfig->dispLength ){
					lcd_write((1<<LCD_DDRAM)+s_pConfig->line1Start,0);
				}
				break;
			case 4:
				if ( pos == s_pConfig->line1Start+s_pConfig->dispLength ) {
					lcd_write((1<<LCD_DDRAM)+s_pConfig->line2Start,0);    
				}else if ( pos == s_pConfig->line2Start+s_pConfig->dispLength ) {
					lcd_write((1<<LCD_DDRAM)+s_pConfig->line3Start,0);
				}else if ( pos == s_pConfig->line3Start+s_pConfig->dispLength ) {
					lcd_write((1<<LCD_DDRAM)+s_pConfig->line4Start,0);
				}else if ( pos == s_pConfig->line4Start+s_pConfig->dispLength ) {
					lcd_write((1<<LCD_DDRAM)+s_pConfig->line1Start,0);
				}
				break;
			}
			lcd_waitbusy();
		}
		lcd_write(c, 1);
    }

}/* lcd_putc */


/*************************************************************************
Display string without auto linefeed 
Input:    string to be displayed
Returns:  none
*************************************************************************/
void lcd_puts(const char *s)
/* print string on lcd (no auto linefeed) */
{
    register char c;

    while ( (c = *s++) ) {
        lcd_putc(c);
    }

}/* lcd_puts */

/*************************************************************************
Display string without auto linefeed 
Input:    string to be displayed
Returns:  none
*************************************************************************/
void lcd_putsn(const char *s, char n)
/* print string on lcd (no auto linefeed) */
{
    register char c;

    while ( (c = *s++) && (n-- > 0)) {
        lcd_putc(c);
    }

}/* lcd_puts */

/*************************************************************************
Display string from program memory without auto linefeed 
Input:     string from program memory be be displayed                                        
Returns:   none
*************************************************************************/
void lcd_puts_p(const char *progmem_s)
/* print string from program memory on lcd (no auto linefeed) */
{
    register char c;

    while ( (c = pgm_read_byte(progmem_s++)) ) {
        lcd_putc(c);
    }

}/* lcd_puts_p */


/*************************************************************************
Initialize display and select type of cursor 
Input:    dispAttr LCD_DISP_OFF            display off
                   LCD_DISP_ON             display on, cursor off
                   LCD_DISP_ON_CURSOR      display on, cursor on
                   LCD_DISP_CURSOR_BLINK   display on, cursor on flashing
Returns:  none
*************************************************************************/
void lcd_init(uint8_t dispAttr, LCD_PORT * pData, LCD_PORT * pDirection, LCD_PIN * pPins, LCD_CONFIG * pConfig)
{

	if (pData == NULL) { return; }
	if (pDirection == NULL) { return; }
	if (pPins == NULL) { return; }
	if (pConfig == NULL) { return; }
	
	s_pData = pData;
	s_pDirection = pDirection;
	s_pPins = pPins;
	s_pConfig = pConfig;
	
    /*
     *  Initialize LCD to 4 bit I/O mode
     */
     
	/* configure all port bits as output */
	setIOState(LCD_OUTPUT);
	
	IO_Output(*(s_pDirection->rsPort), s_pPins->rsPin);
	IO_Output(*(s_pDirection->rwPort), s_pPins->rwPin);
	IO_Output(*(s_pDirection->enPort), s_pPins->enPin);

    delay(16000);        /* wait 16ms or more after power-on       */
    
    /* initial write to lcd is 8bit */
	IO_On(*(s_pData->port0.w), s_pPins->pin0);
	IO_On(*(s_pData->port1.w), s_pPins->pin1);

    lcd_e_toggle();
    delay(4992);         /* delay, busy flag can't be checked here */
   
    /* repeat last command */ 
    lcd_e_toggle();      
    delay(64);           /* delay, busy flag can't be checked here */
    
    /* repeat last command a third time */
    lcd_e_toggle();      
    delay(64);           /* delay, busy flag can't be checked here */

    /* now configure for 4bit mode */
	IO_Off(*(s_pData->port0.w), s_pPins->pin0);
    lcd_e_toggle();
    delay(64);           /* some displays need this additional delay */
    
    /* from now the LCD only accepts 4 bit I/O, we can use lcd_command() */    

	if ((s_pConfig->type == LCD_CONTROLLER_KS0073) && (s_pConfig->lines == 4))
    {
		/* Display with KS0073 controller requires special commands for enabling 4 line mode */
		lcd_command(KS0073_EXTENDED_FUNCTION_REGISTER_ON);
		lcd_command(KS0073_4LINES_MODE);
		lcd_command(KS0073_EXTENDED_FUNCTION_REGISTER_OFF);
	}
	else
	{
		lcd_command(s_pConfig->lines == 1 ? LCD_FUNCTION_4BIT_1LINE : LCD_FUNCTION_4BIT_2LINES);      /* function set: display lines  */
	}

    lcd_command(LCD_DISP_OFF);              /* display off                  */
    lcd_clrscr();                           /* display clear                */ 
    lcd_command(LCD_MODE_DEFAULT);          /* set entry mode               */
    lcd_command(dispAttr);                  /* display/cursor control       */

}/* lcd_init */
