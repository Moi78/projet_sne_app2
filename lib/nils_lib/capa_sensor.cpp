#include "capa_sensor.h"

static char btn_reg_addr = TOUCH_BUTTON_VALUE_REG_ADDR;
static char slide_reg_addr = TOUCH_SLIDER_VALUE_REG_ADDR;

int get_val_button_sensor(I2C& touch_sensor)                           // Les valeurs de sortie sont : 1 bouton gauche, 2 bouton droit et 3 les deux simultanément
{
    int val_button_sensor;
    touch_sensor.write(ADDR_TOUCH_SLIDER, &btn_reg_addr, 1);
    touch_sensor.read(ADDR_TOUCH_SLIDER, (char *)&val_button_sensor, 1);

    return (val_button_sensor);
}

int get_val_slide_sensor(I2C& touch_sensor)
{
    int val_slide_sensor = 0;
    touch_sensor.write(ADDR_TOUCH_SLIDER, &slide_reg_addr, 1);
    touch_sensor.read(ADDR_TOUCH_SLIDER, (char *)&val_slide_sensor, 1);

    return (val_slide_sensor);
}

int convert_for_barre_led(int val_slide) // Récupérer la variable retourner par la fonction get_val_slide_sensor
{
    int nb_iter = (val_slide * 15)/100;
    
    return(nb_iter); // Nombre d'itérations nécessaire pour proportionner l'allumage des LEDs
}