import sys
import random
from pyfiglet import Figlet

figlet = Figlet()
fonts = figlet.getFonts()

if len(sys.argv) == 1:
    font = random.choice(fonts)
elif len(sys.argv) == 3:
    if sys.argv[1] not in ["-f", "--font"] or sys.argv[2] not in fonts:
        sys.exit("Invalid usage")
    font = sys.argv[2]
else:
    sys.exit("Invalid usage")

text = input("Input: ")
figlet.setFont(font=font)
print(figlet.renderText(text))