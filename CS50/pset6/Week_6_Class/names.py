import sys

names = ["Bill", "Charlie", "Fred", "George", "Ginny", "Percy", "Ron"]

name = input("Name: ")

for name in names:
    print("Found")
    sys.exit(1)

print("Not found")
sys.exit(1)