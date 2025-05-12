#Python script for Microsoft MakeCode (see https://makecode.microbit.org/)

IC = 13; #I2C slave address
READING_TIME = 1000 #time between each reading in ms

def on_forever():
    temp = input.temperature()
    buf = bytearray(1)
    buf.set_number(NumberFormat.UINT8_LE, 0, temp)
    pins.i2c_write_buffer(IC, buf, False)
    basic.pause(READING_TIME)
basic.forever(on_forever)
