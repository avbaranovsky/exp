# seg16 - convert text into sequence of 16-segment images
# pip install --upgrade Pillow
import sys
import getopt
from PIL import Image


def getcode(ch):
    if 65 <= ch <= 90 or \
       192 <= ch <= 223 or \
       48 <= ch <= 57 or \
       ch == 32 or ch == 43 or ch == 45 or ch == 62 or ch == 95 or ch == 126:
        return ch
    elif ch == 168 or ch == 184:
        return 197
    elif 97 <= ch <= 122:
        return ch-97+65
    elif 224 <= ch <= 255:
        return ch-224+192
    else:
        return 42


def process_line(line, result):
    max_chars = result.size[0]//36
    l = len(line)
    if l > max_chars:
        line = line[:max_chars-3]
        line = line + ">>>"

    ba = bytearray(line, '1251')
    x = 0
    for b in ba:
        code = getcode(b)
        image = Image.open('gif/' + "%0.3d" % code + '.gif')
        result.paste(image, (x, 0))
        x = x+36


def main(argv):
    opts, args = getopt.getopt(argv, "hs:o:i:")
    size = 1024
    output = "result.gif"
    input_string="Hello World"
    for opt, arg in opts:
        if opt == '-h':
            print('str2led16.py -i <input string> -o <output file> -s<size>')
            sys.exit()
        elif opt == "-s":
            size = arg
        elif opt == "-o":
            output = arg
        elif opt == "-i":
            input_string = arg
    result_image = Image.new("RGBA", (size, 51))
    process_line(input_string, result_image)
    result_image.save(output)


if __name__ == "__main__":
    main(sys.argv[1:])







