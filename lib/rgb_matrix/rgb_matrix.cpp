#include "rgb_matrix.h"

namespace Matrix {

    RgbMatrix::RgbMatrix(I2C *com_driver) {
        m_com_driver = com_driver;

        if (!check_dev_presence()) {
            printf("ERROR: RGB Matrix not present!\n");
        }

        for (auto &e: m_fbo) {
            e = Color::BLACK;
        }
    }

    void RgbMatrix::write_pixel(const Pixel &pxl) {
        m_fbo[pxl.coord.x + pxl.coord.y * MatrixCfg::matrix_w] = pxl.color;
        write_image(m_fbo);
    }

    void RgbMatrix::write_pixel_noblit(const Pixel &pxl) {
        m_fbo[pxl.coord.x + pxl.coord.y * MatrixCfg::matrix_w] = pxl.color;
    }

    void RgbMatrix::blit() {
        write_image(m_fbo);
    }

    void RgbMatrix::write_image(const std::array<Color, MatrixCfg::matrix_h * MatrixCfg::matrix_w> &image) {
        uint8_t data[72] = {0};
        data[0] = 0x05; // Custom frame
        data[1] = 0xFF; // Whatever duration LSB
        data[2] = 0xFF; // Whatever duration MSB
        data[3] = 0x01; // Forever
        data[4] = 1;    // Frame count
        data[5] = 0;    // Frame index
        memcpy(data + 8, image.data(), image.size());

        m_com_driver->write(MatrixCfg::matrix_addr << 1, (char *) &data, 72);
    }

    bool RgbMatrix::check_dev_presence() {
        uint8_t cmd = 0x00; // GET_DEV_ID
        m_com_driver->write(MatrixCfg::matrix_addr << 1, (char *) &cmd, 1);

        uint16_t response[2] = {0};
        m_com_driver->read(MatrixCfg::matrix_addr << 1, (char *) &response, 4);

        return (response[0] && response[1]);
    }

    void RgbMatrix::display_builtin(Matrix::I2cBuiltins builtin, Matrix::Color color, uint8_t index) {
        if(builtin == I2cBuiltins::COLOR_CKWISE) {
            uint8_t data[6] = {
                    static_cast<uint8_t>(builtin),
                    0x01,                           // Is CW
                    0x01,                           // Is big
                    0xFF,
                    0x07,
                    0x01
            };

            m_com_driver->write(MatrixCfg::matrix_addr << 1, (char*)&data, 6);
        } else {
            uint8_t data[6] = {
                    static_cast<uint8_t>(builtin), // I2C Command
                    index,                         // Command argument
                    0xFF,                          // Disp Duration (LSB)
                    0x07,                          // Disp Duration (MSB)
                    0x01,                          // Display it forever flag
                    static_cast<uint8_t>(color)    // Color
            };

            if(builtin == I2cBuiltins::COLOR_WAVE) {
                data[1] = data[5]; // Color is accepted as the argument of the effect
            }

            m_com_driver->write(MatrixCfg::matrix_addr << 1, (char*)&data, 6);
        }
    }

    void RgbMatrix::color_block(uint32_t rgb) {
        uint8_t data[7] = {
                0x0D,                         // I2C Command
                (uint8_t)(rgb >> 16),         // Red
                (uint8_t)((rgb >> 8) & 0xFF), // Green
                (uint8_t)(rgb & 0xFF),        // Blue
                0xFF,                         // Duration LSB
                0xFF,                         // Duration MSB
                0x01                          // Display forever flag
        };

        m_com_driver->write(MatrixCfg::matrix_addr << 1, (char*)&data, 7);
    }

}