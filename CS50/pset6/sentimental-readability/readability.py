# TODO
from cs50 import get_string

text = get_string("text: ")

letters = 0
words = 1
sentences = 0

for char in text:
    # Add letters if character is alphabetical
    if char.isalpha():
        letters += 1
    # Add words if we found a whitespace
    elif char.isspace():
        words += 1
    # Add senteces if we found . ! ?
    elif char in ['.', '!', '?']:
        sentences += 1

# Variables for the Coleman-Liau index
L = letters / words * 100
S = sentences / words * 100

# Coleman-Liau index formula
index = round(0.0588 * L - 0.296 * S - 15.8)

if index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")
