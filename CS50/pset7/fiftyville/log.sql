-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Let's see what we have with Crime Scene Reports about day mentioned
SELECT description FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28;

-- The interviews mention the bakery so let's see what happend in the bakery
SELECT transcript FROM interviews WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE "%bakery%";

-- Witness 1 told a clue about the thief's car, let's investigate the license plate into the bakery security logs
SELECT bakery_security_logs.activity, bakery_security_logs.license_plate, people.name FROM people JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate WHERE bakery_security_logs.year = 2021 AND bakery_security_logs.month = 7 AND bakery_security_logs.DAY = 28 AND bakery_security_logs.hour = 10 AND bakery_security_logs.minute >=15 AND bakery_security_logs.minute <= 25;

-- Witness 2 told a clue about the thief withdrawing money from an ATM, let's investigate the ATM activity
 SELECT people.name, atm_transactions.transaction_type FROM people JOIN bank_accounts ON bank_accounts.person_id = people.id JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number WHERE atm_transactions.year = 2021 AND atm_transactions.month = 7 AND atm_transactions.day = 28 AND atm_location LIKE "%Leggett Street%" AND atm_transactions.transaction_type LIKE "%withdraw%";

-- Witness 3 told that the thief called someone, let's investigate the phone calls in fiftyville database
-- SELECT caller, caller_name, receiver, receiver_name FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;

UPDATE phone_calls SET caller_name = people.name FROM people WHERE phone_calls.caller = people.phone_number;

UPDATE phone_calls SET receiver_name = people.name FROM people WHERE phone_calls.receiver = people.phone_number;

SELECT caller, caller_name, receiver, receiver_name FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;

-- Witness 3 told that the thief was planning to tak a flight, let's investigate the flights
-- SELECT id, hour, minute, origin_airport_id, destination_airport_id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC LIMIT 1;

UPDATE flights SET origin_airport_id = airports.city FROM airports WHERE flights.origin_airport_id = airports.id;

UPDATE flights SET destination_airport_id = airports.city FROM airports WHERE flights.destination_airport_id = airports.id;

SELECT id, hour, minute, origin_airport_id, destination_airport_id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC LIMIT 1;

-- Now that we know the flight destination we can check for the passengers in that flight
SELECT flights.destination_airport_id, name, phone_number, license_plate FROM people JOIN passengers ON people.passport_number = passengers.passport_number JOIN flights ON flights.id = passengers.flight_id WHERE flights.id = 36 ORDER BY flights.hour ASC;

-- With this information let's discover who is the thief!
SELECT name FROM people JOIN passengers ON people.passport_number = passengers.passport_number JOIN flights ON flights.id = passengers.flight_id WHERE (flights.year = 2021 AND flights.month = 7 AND flights.day = 29 AND flights.id = 36) AND name IN (SELECT phone_calls.caller_name FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND name IN (SELECT people.name FROM people JOIN bank_accounts ON bank_accounts.person_id = people.id JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number WHERE atm_transactions.year = 2021 AND atm_transactions.month = 7 AND atm_transactions.day = 28 AND atm_location LIKE "%Leggett Street%" AND atm_transactions.transaction_type LIKE "%withdraw%") AND name IN (SELECT people.name FROM people JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate WHERE bakery_security_logs.year = 2021 AND bakery_security_logs.month = 7 AND bakery_security_logs.DAY = 28 AND bakery_security_logs.hour = 10 AND bakery_security_logs.minute >=15 AND bakery_security_logs.minute <= 25);