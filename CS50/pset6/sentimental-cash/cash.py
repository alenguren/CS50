# TODO

from cs50 import get_float

while True:
    cents = get_float("Change owed: ")
    if cents > 0:
        break

cents = round(cents*100)

count = 0

# Determine how many quarters should give
while cents >= 25:
    cents = cents - 25
    count += 1

# Determine how many dimes should give
while cents >= 10:
    cents = cents - 10
    count += 1

# Determine how many nickels should give
while cents >= 5:
    cents = cents - 5
    count += 1

# Determine how many pennies should give
while cents >= 1:
    cents = cents - 1
    count += 1

print(count)