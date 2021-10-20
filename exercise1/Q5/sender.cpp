#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/delay.h>
#include <string.h>

char read_from_eeprom[200];

char data[200] = "One Line Commiunication project, This message sends from microcontroller 'sender' to 'receiver' with only one line communicate wire and shows a message in LCD.";

int main()
{
    DDRC = 0xFF;
    PORTC = 0x00;
    DDRD = 0xFF;
    PORTD = 0x00;

    eeprom_write_block(data, 0, strlen(data));
    eeprom_read_block(read_from_eeprom, 0, strlen(data));

    _delay_ms(100);

    for (int i = 0; i <= 200; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            PORTC = (read_from_eeprom[i] >> j) & _BV(0);
            _delay_ms(0.5);
        }
    }

    while (1)
    {
        PORTC = 0x00;
    }
    return 0;
}