#include <cs50.h>
#include <stdio.h>
#include <math.h>

int get_digits(long card_number);
bool check_digits(long card_number, long digits);
int get_first_two_digits(int number_of_digits, long card_number);
string get_card_name(int number_of_digits, int first_two_digits);
bool check_validation_card(int number_of_digits, long card_number);
void printName(bool is_valid, string card_name);

int main(void)
{
    int number_of_digits = 0;
    string name_card;
    long card_number;

    do
    {
        card_number = get_long("Number: ");
        number_of_digits = get_digits(card_number);
    }
    while (number_of_digits != 15 && number_of_digits != 16 && number_of_digits != 13);

    int first_two_digits = get_first_two_digits(number_of_digits, card_number);

    string card_name = get_card_name(number_of_digits, first_two_digits);

    bool is_valid = check_validation_card(number_of_digits, card_number);

    printName(is_valid, card_name);
}

int get_digits(long card_number)
{
    long thirteen_digit = pow(10, 12);
    long fifteen_digit = pow(10, 14);
    long sixteen_digit = pow(10, 15);
    int number_of_digits = 0;

    if (check_digits(card_number, thirteen_digit))
    {
        number_of_digits = 13;
    }
    if (check_digits(card_number, fifteen_digit))
    {
        number_of_digits = 15;
    }
    if (check_digits(card_number, sixteen_digit))
    {
        number_of_digits = 16;
    }

    return number_of_digits;
}

bool check_digits(long card_number, long digits)
{
    return card_number / digits > 0 && card_number / digits < 10;
}

int get_first_two_digits(int number_of_digits, long card_number)
{
    int first_two_digits;

    if (number_of_digits == 15)
    {
        first_two_digits = card_number / pow(10, 13);
    }
    if (number_of_digits == 16)
    {
        first_two_digits = card_number / pow(10, 14);
    }
    if (number_of_digits == 13)
    {
        first_two_digits = card_number / pow(10, 11);
    }
    return first_two_digits;
}

string get_card_name(int number_of_digits, int first_two_digits)
{
    int visa_first_digit = first_two_digits / 10;
    string card_name;
    if (number_of_digits == 15 && (first_two_digits == 34 || first_two_digits == 37))
    {
        card_name = "AMEX";
    }
    if (number_of_digits == 16 && (first_two_digits >= 51 && first_two_digits <= 55))
    {
        card_name = "MASTERCARD";
    }
    if ((number_of_digits == 13 || number_of_digits == 16) && visa_first_digit == 4)
    {
        card_name = "VISA";
    }
    return card_name;
}

bool check_validation_card(int number_of_digits, long card_number)
{
    int even_digits_number = 0;
    int uneven_digits_number = 0;
    int even_digits_sum = 0;
    int uneven_digits_sum = 0;

    even_digits_number = number_of_digits / 2;
    if (number_of_digits % 2 == 0)
    {
        uneven_digits_number = number_of_digits / 2;
    }
    else
    {
        uneven_digits_number = number_of_digits / 2 + 1;
    }

    for (int i = 0, p = 2; i < even_digits_number; i++, p += 2)
    {
        int products_digit;
        int multiplied_digit = (card_number % (long)pow(10, p)) / (long)pow(10, p - 1) * 2;
        if (multiplied_digit > 9)
        {
            int first_digit = (multiplied_digit % 100) / 10;
            int second_digit = multiplied_digit % 10;
            products_digit = first_digit + second_digit;
        }
        else
        {
            products_digit = multiplied_digit;
        }
        even_digits_sum += products_digit;
    }

    for (int i = 0, p = 1; i < uneven_digits_number; i++, p += 2)
    {
        uneven_digits_sum += (card_number % (long)pow(10, p)) / (long)pow(10, p - 1);
    }

    int sum_digits = even_digits_sum + uneven_digits_sum;

    bool is_valid = false;

    if (sum_digits % 10 == 0)
    {
        is_valid = true;
    }
    return is_valid;
}

void printName(bool is_valid, string card_name)
{
    if (is_valid)
    {
        printf("%s\n", card_name);
    }
    else
    {
        printf("INVALID\n");
    }
}