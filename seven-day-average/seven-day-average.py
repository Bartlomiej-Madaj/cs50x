import csv
import requests


def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)
    print(f"new_cases: {new_cases}")

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
                break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


# TODO: Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader):
    new_cases = dict()
    for row in reader:
        if row["state"] not in new_cases:
            new_cases[row["state"]] = [int(row["cases"])]
        if row["state"] in new_cases:
            length = len(new_cases[row["state"]])
            previous_cases = new_cases[row["state"]][length - 1]
            new_cases[row["state"]].append(int(row["cases"]) - previous_cases)
            # previous_cases = int(row["cases"])
            if len(new_cases[row["state"]]) > 14:
                new_cases[row["state"]].pop(0)
    return new_cases


# TODO: Calculate and print out seven day average for given state
def comparative_averages(new_cases, states):
    for state in states:
        print(state)
        prev_week_sum = sum(new_cases[state][:6])
        curr_week_sum = sum(new_cases[state][7:])
        # print(f"prev_week_sum: {prev_week_sum}, curr_week_sum: {curr_week_sum} ")
        try:
            average = round(curr_week_sum / 7)
            ratio = round((curr_week_sum / prev_week_sum - 1) * 100)
            if ratio > 0:
                text = f"an increase {abs(ratio)}%"
            else:
                text = f"a decrease {abs(ratio)}%"
            print(f"{state} had a 7-day average of {average} and {text}")
            if prev_week_sum == 0:
                raise ZeroDivisionError("denominator equal 0")
        except ZeroDivisionError as e:
            print(e)

main()
