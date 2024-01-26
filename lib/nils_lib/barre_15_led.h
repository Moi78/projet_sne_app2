#ifndef BARRE_15_LED_H
#define BARRE_15_LED_H

#include "mbed.h"
#include <cstdio>

void push_color(int color, DigitalOut& sign);
void led_clear(DigitalOut& sign);
void led_tab(int tab[15], int iter, DigitalOut& sign);

#endif
