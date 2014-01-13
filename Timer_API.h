#include <msp430.h>

// TIME CONSTANTS
#define			ACLK_ONE_SEC		12000

// TIMER SOURCES
#define 		SOURCE_EXT_TCLK		TASSEL_0
#define 		SOURCE_ACLK			TASSEL_1
#define			SOURCE_MCLK			TASSEL_2
#define			SOURCE_EXT_INCLK	TASSEL_3

// TIMER DIVIDERS
#define			SOURCE_DIV_1		ID_0				// Normal source clock
#define			SOURCE_DIV_2		ID_1				// Source clock slowed by x2
#define			SOURCE_DIV_4		ID_2				// Source clock slowed by x4
#define			SOURCE_DIV_8		ID_3				// Source clock slowed by x8


// TIMER MODES
#define			STOP_MODE			MC_0				// Timer is stopped
#define			UP_MODE				MC_1				// Timer counts to up
#define			CONTINUOUS_MODE		MC_2				// Timer counts up to 0xFFFF
#define			UP_DOWN_MODE		MC_3				// Timer counts to TACCR0, then down to 0x0000

// EVENT MODE
#define			NO_INTERRUPTS		0x00				// No interrupts associated with this timer
#define			CC_INTERRUPT_ONLY	CCIE				// Capture/compare interrupt only
#define 		TMR_INTERRUPT_ONLY	TAIE				// Timer interrupt only
#define			BOTH_INTERRUPTS		CCIE|TAIE			// Capture/compare + Timer interrupts

// TIMER NUMBERS
#define			TIMER_A0			0x00
#define 		TIMER_A1			0x01
#define			TIMER_A2			0x02
#define 		TIMER_B0			0x03
#define			TIMER_B1			0x04
#define			TIMER_B2			0x05

// CAPTURE MODE SPECIFICS
#define			CAPTURE_NONE		CM_0
#define			CAPTURE_RISING		CM_1
#define 		CAPTURE_FALLING		CM_2
#define			CAPTURE_BOTH		CM_3

#define			CAPTURE_SYNC		SCS
#define 		CAPTURE_NOSYNC		0x00

#define			MODE_CAPTURE		CAP
#define			MODE_COMPARE		0x00


// CAPTURE/COMPARE INPUT SELECT
#define			CC_INPUT_CCIxA		CCIS_0				// Set to this if you don't want to use anything
#define			CC_INPUT_CCIxB		CCIS_1
#define			CC_INPUT_GND		CCIS_2
#define			CC_INPUT_VCC		CCIS_3

// OUTPUT MODES
#define			TIMER_OUT_OUTx		OUTMOD_0			// Outputs value of OUTx bit.
#define			TIMER_OUT_SET		OUTMOD_1			// Output sets when TxCCRy value is reached
#define			TIMER_OUT_TOGGLE_R	OUTMOD_2			// Output toggles at TxCCRy. Output reset at TxCCR0
#define			TIMER_OUT_SET_R		OUTMOD_3			// Output sets at TxCCRy. Output resets at TxCCR0
#define			TIMER_OUT_TOGGLE	OUTMOD_4			// Output toggles at TxCCRy
#define			TIMER_OUT_R			OUTMOD_5			// Output resets (low) at TxCCRy.
#define			TIMER_OUT_TOGGLE_S	OUTMOD_6			// Output toggles at TxCCRy. Output sets (high) at TxCCR0.
#define			TIMER_OUT_R_S		OUTMOD_7			// output resets at TxCCRy. Output sets at TxCCR0.


typedef struct
{
	unsigned int	timer_number;						// Choose from what processor has to offer

	unsigned int	timer_clock_source;					// Choose what drives the timer
	unsigned int	timer_clock_divider;				// Use to slow down the timer source clock

	unsigned int	start_time;							// Can set timer start to anything
	unsigned int	event_time;							// When event needs to happen, basically end time

	unsigned int	interrupt_mode;						// Any interrupts associated with this timer?
	unsigned int	timer_mode;							// How does this timer count? When does it reset?
	unsigned int	timer_output_mode;					// What happens at the output

	unsigned int	capture_or_compare;					// Capture or compare mode
	unsigned int	cc_input_select;					// Input select for capture/compare
	unsigned int	capture_synchronized;				// Synchronize capture input source with timer clock
	unsigned int	capture_mode;						// Capture mode

	volatile unsigned int *	CCTL_ptr;					// USER SET TO NULL. Filled at runtime
	volatile unsigned int * CTL_ptr;					// USER SET TO NULL. Filled at runtime
	volatile unsigned int * CR_ptr;						// USER SET TO NULL. Filled at runtime
	volatile unsigned int * TxR_ptr;					// USER SET TO NULL. Filled at runtime

} timer_prototype;


// FUNCTION PROTOTYPES

void initialize_timer(timer_prototype* timer);

void start_timer(timer_prototype* timer);

void stop_timer(timer_prototype* timer);

void timer_reset_INTERRUPT_status(timer_prototype* timer);

unsigned char timer_check_INTERRUPT_status(timer_prototype* timer);

void timer_TMR_INTERRUPT_OFF(timer_prototype* timer);

void timer_TMR_INTERRUPT_ON(timer_prototype* timer);

void timer_CC_INTERRUPT_OFF(timer_prototype* timer);

void timer_CC_INTERRUPT_ON(timer_prototype* timer);

int timer_get_current_value(timer_prototype* timer);

void timer_reset_current_value(timer_prototype* timer);
