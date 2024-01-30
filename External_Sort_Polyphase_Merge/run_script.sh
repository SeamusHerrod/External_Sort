#!/bin/bash

# Define the directory containing the input files
INPUT_DIR="Inputs"

#Define the directory containing the output files
OUTPUT_DIR="Outputs"

#Define the directory containing the log files 
LOG_DIR="Logs"
# Define the log file
LOG_FILE="time_log.txt"

# Define your CPU clock rate
CPU_CLOCK_RATE="3400.000"

# Loop over all input files in the directory
for i in {1..10}
do
    # Define the input and output files
    INPUT_FILE="${INPUT_DIR}/input_${i}.txt"
    OUTPUT_FILE_1="result_${i}_1.txt"
    OUTPUT_FILE_2="result_${i}_2.txt"
    LOG_FILE_CHECK="${LOG_DIR}/log_${i}.txt"
    
    CHECK_OUT1="${OUTPUT_DIR}/output_${i}_1.txt"
    CHECK_OUT2="${OUTPUT_DIR}/output_${i}_2.txt"

    # Run the command
    /usr/bin/time -v -o ${LOG_FILE} ./Lab2 ${INPUT_FILE} ${OUTPUT_FILE_1} ${OUTPUT_FILE_2}

    # Run the grading script for each output file
    python3 GradingScript.py ${OUTPUT_FILE_1} ${CHECK_OUT1} ${LOG_FILE} ${LOG_FILE_CHECK} ${CPU_CLOCK_RATE}
    python3 GradingScript.py ${OUTPUT_FILE_2} ${CHECK_OUT2} ${LOG_FILE} ${LOG_FILE_CHECK} ${CPU_CLOCK_RATE}

    echo "Iteration ${i} finished."

done