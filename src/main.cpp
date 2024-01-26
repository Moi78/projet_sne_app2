#include "mbed.h"
#include <list>

#include "rgb_matrix.h"
#include "Adafruit_SSD1306.h"
#undef WHITE // Collision dans le naming...

I2C i2c(PB_9, PB_8);
UnbufferedSerial pc_serial{USBTX, USBRX, 115200};

uint8_t fbo[128 * 64 + 3]; // FBO + Color buffer
uint16_t fbo_cursor = 0;

bool sync_flag = false;

void serial_fbo_isr() {
    static uint8_t prev = 0;
    static uint8_t sync_counter = 0;

    uint8_t data = 0;
    pc_serial.read(&data, 1);

    // WARN /!\ : Bug dans l'ecran OLED. Il ne semble pas tenir compte des ordres pour
    // revenir a la (ligne, colonne) (0,0)

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
        // Sync OK ! Copie des data du serial port vers notre framebuffer

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

    pc_serial.attach(&serial_fbo_isr);

    // Cool rgb matrix effect
    Matrix::Color colors[9] = {
            Matrix::Color::RED,
            Matrix::Color::GREEN,
            Matrix::Color::BLUE,
            Matrix::Color::YELLOW,
            Matrix::Color::ORANGE,
            Matrix::Color::PINK,
            Matrix::Color::CYAN,
            Matrix::Color::PURPLE,
            Matrix::Color::WHITE,
    };

    std::array<Matrix::Color, 8*8> fb_matrix = {};
    for(int i = 0; i < 8 * 8; i++) {
        fb_matrix[i] = colors[i % 9];
    }
    matrix.write_image(fb_matrix);

    while(true) {
        //matrix.color_block(Matrix::RGBColor(fbo[0], fbo[1], fbo[2]));

        // Copie du framebuffer dans l'ecran
        for (uint16_t y = 0; y < 64; y++) {
            for (uint16_t x = 0; x < 128; x++) {
                screen.drawPixel(x, y, fbo[x + y * 128 + 3] == '1' ? 1 : 0);
            }
        }

        // Mise a jour de l'ecran
        screen.display();
        ThisThread::sleep_for(2ms);
    }

    return 0;
}