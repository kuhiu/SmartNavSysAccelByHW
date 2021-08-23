import png
f = open('ramp.png', 'wb')      # binary mode is important
w = png.Writer(256, 1, greyscale=True)
w.write(f, [range(256)])
f.close()