menu = {
    "Baja Taco": 4.00,
    "Burrito": 7.50,
    "Bowl": 8.50,
    "Nachos": 11.00,
    "Quesadilla": 8.50,
    "Super Burrito": 8.50,
    "Super Quesadilla": 9.50,
    "Taco": 3.00,
    "Tortilla Salad": 8.00
}

def main():
    total = 0
    while True:
        user_input = input("Item:")
        if len(user_input) == 0:
            break
        elif user_input in menu:
            total += menu[user_input]
            print(f"Total: ${round(total, 2)}")
        else:
            continue

main()