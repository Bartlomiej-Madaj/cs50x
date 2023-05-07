from pyfiglet import Figlet
import random
import sys

figlet = Figlet()
fonts = figlet.getFonts()
if len(sys.argv) == 3 and sys.argv[1] in ["-f", "--font"] and sys.argv[2] in fonts:
    font_name = sys.argv[2]
    figlet.setFont(font=font_name)
elif len(sys.argv) == 2:
    figlet.setFont(font=random.choice(fonts))
else:
    sys.exit()

text = str(input("Input: "))

print(f"Output: \n{figlet.renderText(text)}")
