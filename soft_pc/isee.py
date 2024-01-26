import serial
from PIL import Image
from PIL import ImageGrab
from colorthief import ColorThief

# Convertis une image donnée vers quelque chose d'affichable sur l'écran OLED
def to_displayable(img):
    displayable = Image.new('1', (128,64), 0)
    displayable.paste(img, (0, 0))
    return displayable

# Capture l'écran et envoie la capture sur le port série
def send_screen_to_oled(ser_dev):
    image = ImageGrab.grab(bbox=(0, 0, 1920, 1080))

    ct = ColorThief(image)
    dominant_color = ct.get_color(5)

    image.thumbnail((128, 64), Image.LANCZOS)
    image = image.convert('1')

    disp_buff = to_displayable(image).load()

    buffer = bytearray()
    buffer.extend(25 * b'5')
    buffer.append(dominant_color[0])
    buffer.append(dominant_color[1])
    buffer.append(dominant_color[2])
                                                                                                                   
    for y in range(0, 64):                                                                                             
        for x in range(0, 128):                                                                                        
            buffer.extend(b'1' if disp_buff[x, y] == 255 else b'0')                                                    
                                                                                                                   
    ser_dev.write(buffer)

# Ouverture du port série en 115 200 bauds
ser = serial.Serial("/dev/ttyACM0", 115200)

# Mainloop
while(True):
    send_screen_to_oled(ser)
