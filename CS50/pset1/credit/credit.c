#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long creditcard; // To determine the input
    do
    {
        creditcard = get_long("Input a Credit Card Number: ");
    }
    while (creditcard < 0); // Credit card number must be a positive number

    // ccd stands for credit card digit
    int ccd1, ccd2, ccd3, ccd4, ccd5, ccd6, ccd7, ccd8;
    ccd1 = ((creditcard % 100) / 10) * 2;
    ccd2 = ((creditcard % 10000) / 1000) * 2;
    ccd3 = ((creditcard % 1000000) / 100000) * 2;
    ccd4 = ((creditcard % 100000000) / 10000000) * 2;
    ccd5 = ((creditcard % 10000000000) / 1000000000) * 2;
    ccd6 = ((creditcard % 1000000000000) / 100000000000) * 2;
    ccd7 = ((creditcard % 100000000000000) / 10000000000000) * 2;
    ccd8 = ((creditcard % 10000000000000000) / 1000000000000000) * 2;

    // to get the second digit in case of
    ccd1 = (ccd1 % 100 / 10) + (ccd1 % 10);
    ccd2 = (ccd2 % 100 / 10) + (ccd2 % 10);
    ccd3 = (ccd3 % 100 / 10) + (ccd3 % 10);
    ccd4 = (ccd4 % 100 / 10) + (ccd4 % 10);
    ccd5 = (ccd5 % 100 / 10) + (ccd5 % 10);
    ccd6 = (ccd6 % 100 / 10) + (ccd6 % 10);
    ccd7 = (ccd7 % 100 / 10) + (ccd7 % 10);
    ccd8 = (ccd8 % 100 / 10) + (ccd8 % 10);

    int sum1 = ccd1 + ccd2 + ccd3 + ccd4 + ccd5 + ccd6 + ccd7 + ccd8; // To determine the first sum operation

    //It's time to find the digits that weren't multiplied before

    int ccd9, ccd10, ccd11, ccd12, ccd13, ccd14, ccd15, ccd16;

    ccd9 = (creditcard % 10);
    ccd10 = ((creditcard % 1000) / 100);
    ccd11 = ((creditcard % 100000) / 10000);
    ccd12 = ((creditcard % 10000000) / 1000000);
    ccd13 = ((creditcard % 1000000000) / 100000000);
    ccd14 = ((creditcard % 100000000000) / 10000000000);
    ccd15 = ((creditcard % 10000000000000) / 1000000000000);
    ccd16 = ((creditcard % 1000000000000000) / 100000000000000);

    int sum2 = ccd9 + ccd10 + ccd11 + ccd12 + ccd13 + ccd14 + ccd15 + ccd16; // To determine the second sum operation

    int sum3 = sum1 + sum2; // To determine the last sum operation

    int length = 0;
    long visa = creditcard;
    long master = creditcard;
    long amex = creditcard;
    if ((sum3 % 10) != 0)
    {
        printf("%s\n", "INVALID");
        return 0;
    }

    // It's time to identify between, VISA, Mastercard and American Express

    while (creditcard > 0)
    {
        creditcard = creditcard / 10;
        length++;
    }

    //Check if it is VISA
    while (visa >= 10)
    {
        visa /= 10;
    }
    if (visa == 4 && (length == 13 || length == 16))
    {
        printf("%s\n", "VISA");
        return 0;
    }

    //Check if it is AMEX
    while (amex >= 10000000000000)
    {
        amex /= 10000000000000;
    }
    if (length == 15 && (amex == 34 || amex == 37))
    {
        printf("%s\n", "AMEX");
        return 0;
    }

    //Check if it is MASTERCARD
    while (master >= 100000000000000)
    {
        master /= 100000000000000;
    }
    if (length == 16 && (master == 51 || master == 52 || master == 53 || master == 54 || master == 55))
    {
        printf("%s\n", "MASTERCARD");
        return 0;
    }
    //If the Credit Card Number does not meet the previous conditions it will print Invalid
    else
    {
        printf("%s\n", "INVALID");
        return 0;
    }
}