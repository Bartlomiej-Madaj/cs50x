def main():

    # Prompt the user for a credit card number while number is correct

    # card_number = 0
    # digit_numbers = 0
    # while (digit_numbers != 15 and digit_numbers != 16 and digit_numbers != 13):
    #     try:
    #         card_number = int(input("Number: "))
    #         digit_numbers = len(str(card_number))
    #     except ValueError:
    #         print("Not an integer")

    card_number = int(input("Number: "))
    digit_numbers = len(str(card_number))
    first_two_digits = int(str(card_number)[:2])

    card_name = ""
    if (digit_numbers == 15 and first_two_digits in [34, 37]):
        card_name = "AMEX"
    elif (digit_numbers == 16 and first_two_digits range(51,56)):
        card_name = "MASTERCARD"
    elif ((digit_numbers == 13 or digit_numbers == 16) and int(str(first_two_digits)[:1]) == 4):
        card_name = "VISA"

    sum1 = 0
    for i in range(digit_numbers - 2, -1, -2):
        digit = int(str(card_number)[i]) * 2
        sum1 += digit // 10 + digit % 10

    sum2 = 0
    for i in range(digit_numbers - 1, -1, -2):
        sum2 += int(str(card_number)[i])

    sum = sum1 + sum2

    if int(str(sum)[-1]) == 0 and len(card_name) > 0:
        print(f"{card_name}")
    else:
        print("INVALID")


main()

