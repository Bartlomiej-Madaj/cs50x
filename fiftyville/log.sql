-- Keep a log of any SQL queries you execute as you solve the mystery.

-- reading the raport
SELECT description
FROM crime_scene_reports
WHERE
year = 2021
AND month = 7
AND day = 28
AND street = "Humphrey Street";

--reading the transcript of the interviews
SELECT name, transcript
FROM interviews
WHERE month = 7
AND day = 28
AND year = 2021
AND transcript LIKE '%bakery%';

-- Checking the recordings from the parking lot cameras
SELECT name, phone_number, passport_number, activity
FROM people
JOIN bakery_security_logs
ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute <= 25
AND bakery_security_logs.minute >= 15
AND activity = "exit";

--Checking ATM readings
SELECT name, phone_number, passport_number, transaction_type
FROM people
JOIN bank_accounts
ON people.id = bank_accounts.person_id
JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number
WHERE year = 2021
AND month = 7
AND day = 28
AND atm_location = "Leggett Street"
AND transaction_type = "withdraw";

--checking phone bills
SELECT name, phone_number, receiver, passport_number
FROM people
JOIN phone_calls
ON people.phone_number = phone_calls.caller
WHERE year = 2021
AND month = 7
AND day = 28
AND duration < 60;

--Checking the departures from Fiftyville on the 29th
SELECT name, phone_number, city, hour, minute
FROM people
JOIN passengers
ON people.passport_number = passengers.passport_number
JOIN flights
ON flights.id = passengers.flight_id
JOIN airports
ON airports.id = flights.destination_airport_id
WHERE flights.day = 29
AND flights.month = 7
AND flights.origin_airport_id = (SELECT id FROM airports WHERE full_name LIKE '%fiftyville%')
ORDER BY hour;

--Checking all of the above conditions and identifying the name of the thief.
SELECT name
FROM people WHERE name IN (SELECT name
FROM people
JOIN passengers
ON people.passport_number = passengers.passport_number
JOIN flights
ON flights.id = passengers.flight_id
JOIN airports
ON airports.id = flights.destination_airport_id
WHERE flights.day = 29
AND flights.month = 7
AND flights.origin_airport_id = (SELECT id FROM airports WHERE full_name LIKE '%fiftyville%')
ORDER BY hour)
AND name IN ( SELECT name
FROM people
JOIN phone_calls
ON people.phone_number = phone_calls.caller
WHERE year = 2021
AND month = 7
AND day = 28
AND duration < 60)
AND name IN (SELECT name
FROM people
JOIN bank_accounts
ON people.id = bank_accounts.person_id
JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number
WHERE year = 2021
AND month = 7
AND day = 28
AND atm_location = "Leggett Street"
AND transaction_type = "withdraw")
AND name IN (SELECT name
FROM people
JOIN bakery_security_logs
ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.year = 2021
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute <= 25
AND bakery_security_logs.minute >= 15);

-- The thief could be either Diana or Bruce based on the result of the above checks. According to one of the testimonies, the thief said on the phone that they wanted to take the earliest flight out of Fiftyville, and considering this, the thief is likely Bruce, who fled to New York City.

--identification of an accomplice in a theft
SELECT name
FROM people
WHERE phone_number = (SELECT receiver
FROM phone_calls
WHERE caller = "(367) 555-5533"
AND year = 2021
AND month = 7
AND day = 28
AND duration < 60);

--  Robin is an accomplice.



