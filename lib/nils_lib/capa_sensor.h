#ifndef CAPA_SENSOR_H
#define CAPA_SENSOR_H

#include "mbed.h"
#include <cstdio>

#define TOUCH_BUTTON_VALUE_REG_ADDR 0X00
#define TOUCH_SLIDER_VALUE_REG_ADDR 0X01

#define ADDR_TOUCH_SLIDER 0x88 << 1

int get_val_button_sensor(I2C& touch_sensor); // Fonctions qui permettent de récupérer les données de l'actionneur
int get_val_slide_sensor(I2C& touch_sensor);

int convert_for_barre_led(int val_slide); // Fonctions pour les 15 LEDs

#endif