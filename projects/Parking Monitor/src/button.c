/******************************************************************************
 * File           : Button driver implementation file
 *****************************************************************************/
#include "button.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
uint8_t button_flag;
uint8_t last_state_button;

// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------
bool is_button_pressed(void);

void button_init()
{
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
	
	button_flag = 0;
	last_state_button = 0;
}

bool has_state_button_changed()
{
	bool current_state = is_button_pressed();
	
	if(current_state != last_state_button)
	{
		button_flag = 1;
		last_state_button = current_state;
		return true;
	}
	
	return false;
}

uint8_t get_button_flag(void)
{
	if(button_flag == 1)
	{
		button_flag = 0;
		return 1;
	}
	
	return 0;
}

uint8_t get_last_state_button(void)
{
	return last_state_button;
}

bool is_button_pressed()
{
	if(STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET)
	{
		return true;
	}
	
	return false;
}
