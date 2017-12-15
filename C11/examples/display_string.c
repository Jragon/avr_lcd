#include <avr/io.h>
#include "lcd.h"

int main(void)
{
    init_lcd();    
    
	display_string("Hello World");
	while(1);
}