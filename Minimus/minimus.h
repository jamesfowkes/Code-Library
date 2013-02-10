#ifndef _MINIMUS_H_
#define _MINIMUS_H_

#if (defined __AVR_AT90USB162__ || defined __AVR_ATmega32U2__)

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

	enum minimus_led_enum
	{
		LED1,
		LED2,
	};
	typedef enum minimus_led_enum MINIMUS_LED_ENUM;

	enum minimus_ledctrl_enum
	{
		LED_OFF,
		LED_ON,
		LED_TOGGLE
	};
	typedef enum minimus_ledctrl_enum MINIMUS_LEDCTRL_ENUM;

	/*
	 * Public Datatypes
	 */
	typedef void (*minimus_button_cb)(MINIMUS_BUTTON_ENUM, MINIMUS_BUTTONSTATE_ENUM);

	/*
	 * Public Function Prototypes
	 */
	void Minimus_Init(minimus_button_cb);
	void Minimus_USB_MsTick(void);

	void Minimus_LED_Control(MINIMUS_LED_ENUM eLED, MINIMUS_LEDCTRL_ENUM eControl);
	MINIMUS_BUTTONSTATE_ENUM Minimus_Button_Read();

#else
	#error "Minimus does not support target MCU!"
#endif

#endif
