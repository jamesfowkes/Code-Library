// The reset pin can either act as low-triggered reset, or NMI
enum lib_sys_resetmode_enum
{
	RST,
	NMI
};
typedef enum lib_sys_resetmode_enum LIB_SYS_RESETMODE_ENUM;

// The reset pin can either have pullup or pulldown resistors
enum lib_sys_resistor_enum
{
	PULLUP,
	PULLDOWN,
};
typedef enum lib_sys_resistor_enum LIB_SYS_RESISTOR_ENUM;

bool lib_sys_setresetmode(LIB_SYS_RESETMODE_ENUM eMode);
bool lib_sys_setresetresistor(LIB_SYS_RESISTOR_ENUM eResistor, bool enable);


