#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t timer1_overflow = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;

char cast_numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void initial_timer_1()
{
    TCCR1B |= _BV(CS11);

    TCNT1 = 0;

    TIMSK |= _BV(TOIE1);

    timer1_overflow = 0;
}

ISR(TIMER1_OVF_vect)
{
    timer1_overflow++;
}

void cmd(char config)
{
    PORTC = config;
    PORTD = 0x01; 
    _delay_ms(1);
    PORTD = 0x00; 
}

void show_on_lcd(char data)
{
    PORTC = data;
    PORTD = 0x05;
    _delay_ms(1);
    PORTD = 0x04; 
}

void reset_lcd()
{
    cmd(0x01); 
    cmd(0x06); 
    cmd(0x0c); 
    cmd(0x38); 
}

void show_clock()
{
    reset_lcd();
    show_on_lcd(cast_numbers[hours / 10]);
    show_on_lcd(cast_numbers[hours % 10]);
    show_on_lcd(':');
    show_on_lcd(cast_numbers[minutes / 10]);
    show_on_lcd(cast_numbers[minutes % 10]);
    show_on_lcd(':');
    show_on_lcd(cast_numbers[seconds / 10]);
    show_on_lcd(cast_numbers[seconds % 10]);
}

void setup_alarm()
{
    while (PINA != _BV(2))
    {
        if (PINA == _BV(0) && hours < 24)
        {
            hours++;
            show_clock();
            _delay_ms(500);
        }

        if (PINA == _BV(1) && hours > 0)
        {
            hours--;
            show_clock();
            _delay_ms(500);
        }
    }

    _delay_ms(1000);

    while (PINA != _BV(2))
    {
        if (PINA == _BV(0) && minutes < 59)
        {
            minutes++;
            show_clock();
            _delay_ms(500);
        }

        if (PINA == _BV(1) && minutes > 0)
        {
            minutes--;
            show_clock();
            _delay_ms(500);
        }
    }

    _delay_ms(1000);
    while (PINA != _BV(2))
    {
        if (PINA == _BV(0) && seconds < 59)
        {
            seconds++;
            show_clock();
            _delay_ms(500);
        }

        if (PINA == _BV(1) && seconds > 0)
        {
            seconds--;
            show_clock();
            _delay_ms(500);
        }
    }
}

void finished_alarm()
{
    reset_lcd();
    char message[] = "finished";
    for (int i = 0; i < sizeof(message); i++)
    {
        show_on_lcd(message[i]);
    }
    _delay_ms(1000);
}

int main()
{
    DDRC = 0xFF;
    DDRD = 0xFF;
    DDRA = 0x00;

    reset_lcd();
    show_clock();
    initial_timer_1();
    sei();

    setup_alarm();
    while (1)
    {
        if (timer1_overflow >= 15)
        {
            if (TCNT1 >= 16960)
            {
                if (seconds >= 1)
                {
                    seconds--;
                }
                if (seconds <= 0 && minutes >= 1)
                {
                    seconds = 59;
                    minutes--;
                }
                if (seconds <= 0 && minutes <= 0 && hours >= 1)
                {
                    minutes = 59;
                    seconds = 59;
                    hours--;
                }
                if (hours <= 0 && minutes <= 0 && seconds <= 0)
                {
                    finished_alarm();
                    show_clock();
                    setup_alarm();
                }

                show_clock();
                TCNT1 = 0;           
                timer1_overflow = 0; 
            }
        }
    }
    return 0;
}