# seg16 - convert text into sequence of 16-segment images
from PIL import Image

im = Image.open('gif/065.gif')
print(im.format, im.size, im.mode)



