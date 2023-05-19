# TODO
from cs50 import get_int


def main():
    height = get_height()
    for i in range(0, height, 1):
        for j in range(0, height+i+3, 1):
            # If the sum of the indices i and j is less than the height - 1,
            # print a space
            if (j == height or j == height+1 or i+j < height - 1):
                print(" ", end="")
            # Otherwise, print a "#" symbol
            else:
                print("#", end="")
        # Move to the next line for the next iteration of the outer loop
        print()


def get_height():
    # Loop until a valid height is provided
    while True:
        try:
            n = get_int("Height: ")
            if n >= 1 and n <= 8:
                return n
        except ValueError:
            print("Not an integer")


main()