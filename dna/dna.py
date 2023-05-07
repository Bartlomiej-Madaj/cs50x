import csv
import sys

def main():


    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return

    database = list()
    strs = list()
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for row in reader:
            database.append(row)
        for key in row:
                if key == "name":
                    continue
                strs.append(key)


    with open(sys.argv[2], 'r') as file:
        sequence = file.read()


    matches = dict()
    for str in strs:
        counts = longest_match(sequence, str)
        matches[str] = counts

    names = list()
    for person in database:
        for str in strs:
            if int(person[str]) == int(matches[str]):
                names.append(person['name'])

    name_count = {}
    for name in names:
        if name in name_count:
            name_count[name] += 1
        else:
            name_count[name] = 1
    name = max(name_count, key=name_count.get)
    print(name)

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
