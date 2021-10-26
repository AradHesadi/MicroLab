#define F_CPU = 8000000
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t overflows;
volatile uint8_t overflow_timer_0;
volatile uint8_t seconds[4];
volatile uint8_t state = 0;

// convert digit to 7seg pins
char array[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void reset_timer()
{
    seconds[0] = 0;
    seconds[1] = 0;
    seconds[2] = 0;
    seconds[3] = 0;
}

void initial_timer_1()
{
    TCCR1B |= _BV(CS11);

    TCNT1 = 0;

    TIMSK |= _BV(TOIE1);

    overflows = 0;
}

void initial_timer_0()
{
    TCCR0 |= _BV(CS11);

    TCNT0 = 0;

    TIMSK |= _BV(TOIE0);

    overflow_timer_0 = 0;
}

ISR(TIMER1_OVF_vect)
{
    overflows++;
}

ISR(TIMER0_OVF_vect)
{
    overflow_timer_0++;
}

int main(void)
{
    DDRC = 0xFF; 
    DDRD = 0xFF;

    PORTD = _BV(PORTD0);

    initial_timer_1();
    initial_timer_0();
    sei();

    reset_timer();

    while (1)
    {
        if (overflows >= 15)
        {
            if (TCNT1 >= 16960)
            {
                if (seconds[0] > 8)
                {
                    seconds[0] = 0;
                    seconds[1]++;
                }
                else
                {
                    seconds[0]++;
                }

                if (seconds[1] > 8)
                {
                    seconds[1] = 0;
                    seconds[2]++;
                }

                if (seconds[2] > 8)
                {
                    seconds[2] = 0;
                    seconds[3]++;
                }

                if (seconds[3] > 8)
                {
                    reset_timer();
                }

                TCNT1 = 0;
                overflows = 0;
            }
        }


        if (overflow_timer_0 >= 78)
        {
            if (TCNT0 >= 32)
            {
                PORTC = array[seconds[state]];
                PORTD = _BV(state);
                (state > 2) ? state = 0 : state++;

                TCNT0 = 0;
                overflow_timer_0 = 0;
            }
        }
    }
}