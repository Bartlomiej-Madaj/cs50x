import re
def main():
    text = input("Text: ")

    amount_letters = sum(1 for char in text if char.isalpha())
    # for char in text:
    #     if char.isalpha():
    #         amount_letters += 1

    amount_words = len(text.split())

    # amount_sentences = len(re.sub(r'[!?]+', ".", text).split('.')) - 1
    amount_sentences = len(re.findall(r'[.!?]+', text))

    L = float(amount_letters / amount_words * 100)
    S = float(amount_sentences / amount_words * 100)

    index = 0.0588 * L - 0.296 * S - 15.8

    if (index < 1):
        print("Before Grade 1")
    elif (index >= 16):
        print("Grade 16+")
    else:
        print(f"Grade {round(index)}")

main()

