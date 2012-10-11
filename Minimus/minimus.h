#ifndef _MINIMUS_H_
#define _MINIMUS_H_

#if (defined __AVR_AT90USB162__ || defined __AVR_ATmega32U2__)

	/* IO for Minumus AVR USB key */
	#define BUTTON1_PRT PORTC
	#define BUTTON1_DDR DDRC
	#define BUTTON1_PIN PINC1

	#define BUTTON2_PRT PORTD
	#define BUTTON2_DDR DDRD
	#define BUTTON2_PIN PIND7

	/* Button Enumeration */
	enum minimus_button_enum
	{
			BUTTON1,
			BUTTON2
	};
	typedef enum minimus_button_enum MINIMUS_BUTTON_ENUM;

	enum minimus_buttonstate_enum
	{
			BUTTONUP,
			BUTTONDN
	};
	typedef enum minimus_buttonstate_enum MINIMUS_BUTTONSTATE_ENUM;

	/*
	 * Public Datatypes
	 */
	typedef void (*minimus_button_cb)(MINIMUS_BUTTON_ENUM, MINIMUS_BUTTONSTATE_ENUM);

	/*
	 * Public Function Prototypes
	 */
	void Minimus_Init(minimus_button_cb);

#else
	#error "Minimus does not support target MCU!"
#endif

#endif
