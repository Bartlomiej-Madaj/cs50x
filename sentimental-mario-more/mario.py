height = 0
while height < 1 or height > 8:
    try:
        height = int(input("Height: "))
    except ValueError:
        print("Not an integer")

for i in range(height):
    print(f"{' ' * (height - i -1)}{(i+1) * '#'}  {(i+1) * '#'}", end="")
    print()