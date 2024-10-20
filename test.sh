#!/bin/bash

# Define the executable path for your philosophers simulation (change this if needed)
EXE="./philo"

# Function to print colored output
print_result() {
    local result=$1
    local color=$2
    case $color in
        green)
            echo -e "\033[0;32m$result\033[0m"
            ;;
        red)
            echo -e "\033[0;31m$result\033[0m"
            ;;
        yellow)
            echo -e "\033[0;33m$result\033[0m"
            ;;
        *)
            echo "$result"
            ;;
    esac
}

# Function to run a test case
run_test() {
    local test_num=$1
    local num_philos=$2
    local time_to_die=$3
    local time_to_eat=$4
    local time_to_sleep=$5
    local num_times_each=$6
    local timeout=7  # 7 seconds timeout for each test
    local infinite_run=false  # Flag for infinite run (no times to eat specified)

    # Input and Output file name
    local log_file="test${test_num}_log.txt"

    # Save the input parameters in the log file
    echo "Input: $num_philos philosophers, $time_to_die ms to die, $time_to_eat ms to eat, $time_to_sleep ms to sleep, $num_times_each times to eat (if applicable)" > "$log_file"

    # If no number of times each philosopher must eat is provided, set flag to skip timeout
    if [ -z "$num_times_each" ]; then
        infinite_run=true
    fi

    # Run the test
    if $infinite_run; then
        echo "Running infinite test case... (7-second cap)" >> "$log_file"
        # Run the test with a timeout of 7 seconds
        { 
            timeout $timeout bash -c "$EXE $num_philos $time_to_die $time_to_eat $time_to_sleep" >> "$log_file" 2>&1
        } &

        # Get the process ID of the background job
        pid=$!

        # Wait for the process to finish
        wait $pid
        status=$?

        # If the process was terminated before 7 seconds, consider it failed
        if [[ $status -ne 0 ]]; then
            print_result "Test Case $test_num: FAILED (terminated early)" red
            echo "Process terminated early, considered a failure." >> "$log_file"
            print_result "Check logs in $log_file" red
            return 1
        else
            print_result "Test Case $test_num: SUCCESS (ran for 7 seconds without issue)" green
            echo "Test ran for the full 7 seconds, considered successful." >> "$log_file"
            print_result "Check logs in $log_file" green
        fi
    else
        # Normal test case, with a timeout of 5 seconds
        { 
            timeout 5 bash -c "$EXE $num_philos $time_to_die $time_to_eat $time_to_sleep $num_times_each" >> "$log_file" 2>&1
        } &

        # Get the process ID of the background job
        pid=$!

        # Wait for the process to finish
        wait $pid
        status=$?

        # Check if the test ran longer than 5 seconds or had an issue
        if [[ $status -eq 124 ]]; then
            print_result "Test Case $test_num: FAILED (timeout)" red
            echo "Timeout occurred, took longer than 5 seconds." >> "$log_file"
            print_result "Check logs in $log_file" red
            return 1
        fi

        # Check if the output file is empty (indicating a crash or no output)
        if [[ ! -s "$log_file" ]]; then
            print_result "Test Case $test_num: FAILED (no output)" red
            return 1
        fi

        # Assuming success if no errors were found and no timeout
        print_result "Test Case $test_num: SUCCESS" green
        print_result "Check logs in $log_file" green
        echo ""  # Add a newline between test case results
    fi
}

# Test Case 1: Basic test with 5 philosophers, no limit on eating times
# Expected: Test should run without any timeout or errors, philosophers should eventually eat and sleep.
run_test 1 5 800 200 100

# Test Case 2: 3 philosophers, each must eat 3 times
# Expected: The simulation should stop after all philosophers have eaten 3 times.
run_test 2 3 800 200 100 3

# Test Case 3: 2 philosophers, each philosopher must eat 1 time
# Expected: The simulation should stop after each philosopher eats exactly once.
run_test 3 2 600 300 200 1

# Test Case 4: Edge case with only 1 philosopher
# Expected: The single philosopher should eat and sleep indefinitely for 7 seconds, considered success.
# run_test 4 1 500 150 150

# Test Case 5: High number of philosophers (10 philosophers, higher time to die)
# Expected: Philosophers should be able to eat, sleep, and not die within the given time.
run_test 5 10 1000 150 100

# Test Case 6: Extreme case where philosophers must eat many times (5 philosophers, must eat 100 times)
# Expected: The simulation should run with 5 philosophers eating 100 times each without timing out.
run_test 6 5 800 200 100 100

# Test Case 7: Very short times (2 philosophers, 200ms to die, 50ms to eat, 50ms to sleep)
# Expected: Philosophers should still be able to complete a full cycle of eating, sleeping, and thinking.
run_test 7 2 200 50 50

# Test Case 8: Long times (3 philosophers, 2000ms time to die, 500ms time to eat, 500ms time to sleep)
# Expected: The simulation should run smoothly with philosophers eating and sleeping.
run_test 8 3 2000 500 500

# Test Case 9: Large number of philosophers (20 philosophers, each must eat 2 times)
# Expected: The simulation should stop after all 20 philosophers eat twice each.
run_test 9 20 1000 200 100 2

# Test Case 10: Edge case with philosophers having maximum possible eating times
# Expected: The simulation should run with 5 philosophers eating 100 times each without timing out.
run_test 10 5 800 200 100 100
