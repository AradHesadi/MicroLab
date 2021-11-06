#include <avr/io.h>
#include <string.h>
#include <avr/delay.h>

char data[200] = "Hello World!";

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
        PORTC = data[i];
        PORTD = 0x05; 
        _delay_ms(10);
        PORTD = 0x04; 
    }
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

    reset_lcd();
    show_on_lcd(data);

    while (1)
    {
    }
    return 0;
}