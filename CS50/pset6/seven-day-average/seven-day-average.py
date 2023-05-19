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
    # Initialize the new_cases and previous_cases dictionary
    new_cases = {}
    previous_cases = {}
    # Loop through the reader
    for row in reader:
        state = row['state']
        date = row['date']
        cases = int(row['cases'])

        # If this is the first time encountering this state, store the case count
        if state not in previous_cases:
            previous_cases[state] = cases
        else:
            new_cases_today = cases - previous_cases[state]
            # If this is the first time encountering this state, start a new list of new cases
            if state not in new_cases:
                new_cases[state] = [new_cases_today]
            else:
                new_cases[state].append(new_cases_today)
                if len(new_cases[state]) > 14:
                    new_cases[state].pop(0)
            previous_cases[state] = cases
    return new_cases


# TODO: Calculate and print out seven day average for given state
# Calculate and print out the 7-day average for each state in the states list
def comparative_averages(new_cases, states):
    # Loop through each state in the states list
    for state in states:
        # Calculate the 7-day average for this week and last week
        this_week = sum(new_cases[state][-7:]) / 7
        last_week = sum(new_cases[state][:7]) / 7
        # Calculate the percent change between this week and last week
        try:
            percent_change = (this_week - last_week) / last_week * 100
        except ZeroDivisionError:
            percent_change = float('inf')
        # Print the 7-day averages and percent change for this state
        print(f"{state}: This week: {this_week:.2f}, Last week: {last_week:.2f}, Change: {percent_change:.2f}%")


main()
