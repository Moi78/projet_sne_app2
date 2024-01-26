#include "mbed.h"
#include <list>

#include "rgb_matrix.h"
#include "Adafruit_SSD1306.h"

#include "barre_15_led.h"
#include "capa_sensor.h"

I2C i2c(PB_9, PB_8);
UnbufferedSerial pc_serial{USBTX, USBRX, 115200};

DigitalOut led_bar{A0};

uint8_t fbo[128 * 64 + 3]; // FBO + Color buffer
uint16_t fbo_cursor = 0;

bool sync_flag = false;

void serial_fbo_isr() {
    static uint8_t prev = 0;
    static uint8_t sync_counter = 0;

    uint8_t data = 0;
    pc_serial.read(&data, 1);

    // Attente de 25 caracteres '5' => synchro
    if(!sync_flag) {
        if (data == '5' && prev != '5') {
            sync_counter = 1;
            fbo_cursor = 0;
        } else if (data == '5' && prev == '5') {
            sync_counter++;
        }

        if (sync_counter == 25) {
            sync_counter = 0;
            fbo_cursor = 0;
            sync_flag = true;
        }
    } else {
        fbo[fbo_cursor] = data;
        fbo_cursor = (fbo_cursor + 1) % sizeof(fbo);

        if(fbo_cursor == 0) {
            sync_flag = false;
        }
    }

    prev = data;
}

int main() {
    Matrix::RgbMatrix matrix{&i2c};

    Adafruit_SSD1306_I2c screen{i2c, PB_1, 0x78, 64, 128};
    screen.clearDisplay();

    int sequence[15] = {0};
    int color = 0xFFFFFF;
    for(int i = 0; i < 15; i++) {
        sequence[i] = color;

        color >>= 1;
    }

    led_clear(led_bar);
    pc_serial.attach(&serial_fbo_isr);

    while(true) {
        matrix.color_block(Matrix::RGBColor(fbo[0], fbo[1], fbo[2]));

        for (uint16_t y = 0; y < 64; y++) {
            for (uint16_t x = 0; x < 128; x++) {
                screen.drawPixel(x, y, fbo[x + y * 128 + 3] == '1' ? WHITE : BLACK);
            }
        }

        screen.display();
        ThisThread::sleep_for(2ms);
    }

    return 0;
}