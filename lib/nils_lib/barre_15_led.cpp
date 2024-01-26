#include "barre_15_led.h"

void push_color(int color, DigitalOut& sign) 
{
    for (int i = 0; i < 24; i++) 
    {
        if ((color & 0x800000) == 0x800000) 
        {
            sign = 1;
            wait_ns(550);
            sign = 0;
            wait_ns(200);
        } 
        else if ((color & 0x800000) == 0) 
        {
            sign = 1;
            wait_ns(200);
            sign = 0;
            wait_ns(550);
        }
        color = color << 1;
    }
}

void led_clear(DigitalOut& sign)
{
    for(int k = 0; k < 15; k++)
    {
        push_color(0x000000, sign);
    }
    wait_us(300);
}

void led_tab(int tab[15], int iter, DigitalOut& sign)
{
    if(iter == 0) {
        return;
    }

    for(int j = 0; j < iter; j++)
    {
        push_color(tab[j], sign);
    }
}