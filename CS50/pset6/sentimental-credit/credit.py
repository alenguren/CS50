# TODO
from cs50 import get_int

# Loop to continue accepting credit card numbers until a negative number is entered
while True:
    # Prompt the user to enter a credit card number
    creditcard = get_int("Input a Credit Card Number: ")
    # Break the loop if the user enters a negative number
    if creditcard < 0:
        break

    # Check if the credit card number is valid using Luhn's algorithm
    ccd1, ccd2, ccd3, ccd4, ccd5, ccd6, ccd7, ccd8 = (
        int(creditcard % 100 / 10) * 2,
        int(creditcard % 10000 / 1000) * 2,
        int(creditcard % 1000000 / 100000) * 2,
        int(creditcard % 100000000 / 10000000) * 2,
        int(creditcard % 10000000000 / 1000000000) * 2,
        int(creditcard % 1000000000000 / 100000000000) * 2,
        int(creditcard % 100000000000000 / 10000000000000) * 2,
        int(creditcard % 10000000000000000 / 1000000000000000) * 2,
    )

    # Add the digits of each doubled number together
    ccd1 = int(ccd1 % 100 // 10) + (ccd1 % 10)
    ccd2 = int(ccd2 % 100 // 10) + (ccd2 % 10)
    ccd3 = int(ccd3 % 100 // 10) + (ccd3 % 10)
    ccd4 = int(ccd4 % 100 // 10) + (ccd4 % 10)
    ccd5 = int(ccd5 % 100 // 10) + (ccd5 % 10)
    ccd6 = int(ccd6 % 100 // 10) + (ccd6 % 10)
    ccd7 = int(ccd7 % 100 // 10) + (ccd7 % 10)
    ccd8 = int(ccd8 % 100 // 10) + (ccd8 % 10)

    sum1 = ccd1 + ccd2 + ccd3 + ccd4 + ccd5 + ccd6 + ccd7 + ccd8

    ccd9, ccd10, ccd11, ccd12, ccd13, ccd14, ccd15, ccd16 = (
        creditcard % 10,
        int(creditcard % 1000 / 100),
        int(creditcard % 100000 / 10000),
        int(creditcard % 10000000 / 1000000),
        int(creditcard % 1000000000 / 100000000),
        int(creditcard % 100000000000 / 10000000000),
        int(creditcard % 10000000000000 / 1000000000000),
        int(creditcard % 1000000000000000 / 100000000000000),
    )

    sum2 = ccd9 + ccd10 + ccd11 + ccd12 + ccd13 + ccd14 + ccd15 + ccd16
    sum3 = sum1 + sum2

    if sum3 % 10 != 0:
        print("INVALID")
        break

    length = 0
    visa = creditcard
    master = creditcard
    amex = creditcard

    # It's time to identify between, VISA, Mastercard and American Express
    length = len(str(creditcard))

    # Check if it is VISA
    while visa >= 10:
        visa = int(visa/10)

    # Check if it is AMEX
    while amex >= 10000000000000:
        amex = int(amex / 10000000000000)

    # Check if it is MASTERCARD
    while master >= 100000000000000:
        master = int(master / 100000000000000)

    if visa == 4 and (length == 13 or length == 16):
        print("VISA")
        break

    elif length == 15 and (amex == 34 or amex == 37):
        print("AMEX")
        break

    elif length == 16 and (master == 51 or master == 52 or master == 54 or master == 55):
        print("MASTERCARD")
        break

    # If the Credit Card Number does not meet the previous conditions it will print Invalid
    else:
        print("INVALID")
        break

else:
    print("INVALID")