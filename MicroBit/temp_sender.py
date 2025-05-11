#Python script for Microsoft MakeCode (see https://makecode.microbit.org/)

temp = input.temperature()

def on_forever():
    buf = bytearray(1)
    buf.set_number(NumberFormat.UINT8_LE, 0, temp)
    pins.i2c_write_buffer(13, buf, False)
    basic.pause(1000)
basic.forever(on_forever)
