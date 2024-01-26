#ifndef PROJETSN_APP2_RGB_MATRIX_H
#define PROJETSN_APP2_RGB_MATRIX_H

#include "mbed.h"
#include <cstdint>

namespace MatrixCfg {
    constexpr uint8_t matrix_addr = 0x65;
    constexpr uint8_t matrix_w = 8;
    constexpr uint8_t matrix_h = 8;
}

namespace Matrix {
    static uint32_t RGBColor(uint32_t r, uint32_t g, uint32_t b) {
        return (r << 16) | (g << 8) | b;
    }

    struct PixelCoord {
        uint8_t x;
        uint8_t y;
    };

    enum Color {
        RED = 0x00,
        ORANGE = 0x12,
        YELLOW = 0x18,
        GREEN = 0x52,
        CYAN = 0x7F,
        BLUE = 0xAA,
        PURPLE = 0xC3,
        PINK = 0xDC,
        WHITE = 0xFE,
        BLACK = 0xFF
    };

    enum I2cBuiltins {
        LED_BAR = 0x01,
        EMOJI = 0x02,
        COLOR_BAR = 0x09,
        COLOR_WAVE = 0x0A,
        COLOR_CKWISE = 0x0B,
    };

    struct Pixel {
        PixelCoord coord;
        Color color;
    };

    class RgbMatrix {
    public:
        RgbMatrix(I2C *com_driver);

        ~RgbMatrix();

        void write_pixel(const Pixel &pxl);
        void write_pixel_noblit(const Pixel &pxl);

        void display_builtin(I2cBuiltins builtin, Color color, uint8_t index);
        void color_block(uint32_t rgb);

        void write_image(const std::array<Color, MatrixCfg::matrix_h * MatrixCfg::matrix_w> &image);
        void blit();

    private:
        bool check_dev_presence();

        I2C *m_com_driver;
        std::array<Color, MatrixCfg::matrix_h * MatrixCfg::matrix_w> m_fbo;
    };
}

#endif //PROJETSN_APP2_RGB_MATRIX_H
