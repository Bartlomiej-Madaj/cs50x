input = str(input("Greeting: "))

if "hello" in input.lower():
    print("$0")
elif input.lower()[0] == "h":
    print("$20")
else:
    print("$100")
