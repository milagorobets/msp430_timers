#include "Timer_API.h"

void initialize_timer(timer_prototype* timer)
{
	// SET UP REGISTER POINTERS
	// Timer B
	if (timer->timer_number > 0x02)
	{
		timer->CTL_ptr = &TBCTL;
		timer->TxR_ptr = &TBR;

		if (timer -> timer_number == 0x03)
		{
			timer->CCTL_ptr 	= &TBCCTL0;
			timer->CR_ptr 		= &TBCCR0;
		}
		else if (timer -> timer_number == 0x04)
		{
			timer->CCTL_ptr 	= &TBCCTL1;
			timer->CR_ptr 		= &TBCCR1;
		}
		else if (timer -> timer_number == 0x05)
		{
			timer->CCTL_ptr 	= &TBCCTL2;
			timer->CR_ptr		= &TBCCR2;
		}

	}
	// Timer A
	else
	{
		timer->CTL_ptr = &TACTL;
		timer->TxR_ptr = &TAR;

		if (timer -> timer_number == 0x00)
		{
			timer->CCTL_ptr 	= &TACCTL0;
			timer->CR_ptr 		= &TACCR0;
		}
		else if (timer -> timer_number == 0x01)
		{
			timer->CCTL_ptr 	= &TACCTL1;
			timer->CR_ptr 		= &TACCR1;
		}
		else if (timer -> timer_number == 0x02)
		{
			timer->CCTL_ptr 	= &TACCTL2;
			timer->CR_ptr		= &TACCR2;
		}

	}

	// Reset the TxCTL register
	*(timer->CTL_ptr) |= TBCLR;					// Same thing as TACLR, 0x4

	// Set options in TxCTL
	*(timer->CTL_ptr) |= (timer->timer_clock_source) | (timer->timer_clock_divider);

	// Set timer-specific info
	*(timer->CR_ptr) = timer->event_time;

	*(timer->CCTL_ptr) |= (timer->timer_output_mode) | (timer->capture_mode) | (timer->cc_input_select) | (timer->capture_synchronized) | (timer->capture_or_compare);

	// Enable interrupts if specified
	if ((timer->interrupt_mode) & TAIE)
	{
		timer_TMR_INTERRUPT_ON(timer);
	}
	else
	{
		timer_TMR_INTERRUPT_OFF(timer);
	}

	if ((timer->interrupt_mode) & CCIE)
	{
		timer_CC_INTERRUPT_ON(timer);
	}
	else
	{
		timer_CC_INTERRUPT_OFF(timer);
	}

	// Clear interrupt flags
	*(timer->CTL_ptr) 	&= ~TAIFG;
	*(timer->CCTL_ptr) 	&= ~CCIFG;


}

void start_timer(timer_prototype* timer)
{
	*(timer->TxR_ptr) = timer->start_time;
	*(timer->CTL_ptr) |= timer->timer_mode;
}

void stop_timer(timer_prototype* timer)
{
	// Set mode control to 00; halts the timer
	*(timer->CTL_ptr) &= ~MC_0;
}

void timer_reset_INTERRUPT_status(timer_prototype* timer)
{
	*(timer->CCTL_ptr) &= ~CCIFG;
}

unsigned char timer_check_INTERRUPT_status(timer_prototype* timer)
{
	return (*(timer->CTL_ptr) & CCIFG);
}

void timer_TMR_INTERRUPT_OFF(timer_prototype* timer)
{
	*(timer->CTL_ptr) &= ~ TAIE;
}

void timer_TMR_INTERRUPT_ON(timer_prototype* timer)
{
	*(timer->CTL_ptr) |= TAIE;
}

void timer_CC_INTERRUPT_OFF(timer_prototype* timer)
{
	*(timer->CCTL_ptr) &= ~ CCIE;
}

void timer_CC_INTERRUPT_ON(timer_prototype* timer)
{
	*(timer->CCTL_ptr) |= CCIE;
}

int timer_get_current_value(timer_prototype* timer)
{
	return (*(timer->TxR_ptr));
}

void timer_reset_current_value(timer_prototype* timer)
{
	*(timer->TxR_ptr) = 0x00;
}
