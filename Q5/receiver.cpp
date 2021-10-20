#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/delay.h>
#include <string.h>

char recived_data[200];

void cmd(char config)
{
    PORTC = config;
    PORTD = 0x01; 
    _delay_ms(10);
    PORTD = 0x00; 
}

void show_on_lcd(char data[])
{
    for (int i = 0; i < strlen(data); i++)
    {
        if (i % 32 == 0)
        {
            cmd(0x01); 
            cmd(0x80); 
        }
        else if (i % 16 == 0)
        {
            cmd(0xC0);
        }
        PORTC = data[i];
        PORTD = 0x05; 
        _delay_ms(10);
        PORTD = 0x04; 
    }

    cmd(0x01);
    cmd(0x80);
}

void reset_lcd()
{
    cmd(0x01); 
    cmd(0x06); 
    cmd(0x0c); 
    cmd(0x38);
}

int main()
{
    DDRC = 0xFF;
    DDRD = 0xFF;
    DDRA = 0x00;
    DDRB = 0x00;

    reset_lcd();

    while (PINA == 0x00)
    {
    }

    recived_data[0] |= (PINA & _BV(0)) << 0;

    for (int i = 0; i <= 200; i++)
    {
        recived_data[i] = 0;
        for (int j = 0; j < 8; j++)
        {
            recived_data[i] |= (PINA & _BV(0)) << j;
            _delay_ms(0.5);
        }
    }

    while (1)
    {
        show_on_lcd(recived_data);
    }
    return 0;
}