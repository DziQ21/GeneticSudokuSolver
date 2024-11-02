import subprocess
import itertools

def run_program_and_extract_value(command):
    # Run the command
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    # Check if the command was successful
    if result.returncode != 0:
        print(f"Error running command: {result.stderr}")
        return None

    # Extract the output
    output = result.stdout

    # Process the output to extract the desired value
    # This is just an example, modify it according to your needs
    value = None
    for line in output.splitlines():
        if "desired_value" in line:
            value = line.split(":")[1].strip()
            break

    return output




config_template = r"""
LogLevel {log_level}
SudokuPath {sudoku_path}
PopulationSize {population_size}
MutationRate {mutation_rate}
FittestRate {fittest_rate}
ResetCounter {reset_counter}
"""

log_level =["ERR"]
population_size = [100, 200, 400, 500 , 1000 , 2000]
mutation_rate=[0.1, 0.08, 0.06, 0.02]
fittest_rate = [0.2, 0.3, 0.4, 0.5]
reset_counter = [100, 200, 300 ,20 ,10,15,50]
sudoku_path = ["./data/1.txt"]

combinations = itertools.product(log_level, sudoku_path, population_size, mutation_rate, fittest_rate, reset_counter)

for log, path, pop_size, mut_rate, fit_rate, reset in combinations:
    config = config_template.format(
        log_level=log,
        sudoku_path=path,
        population_size=pop_size,
        mutation_rate=mut_rate,
        fittest_rate=fit_rate,
        reset_counter=reset
    )

    command = ["./build/SudokuSolver",config]
    value = run_program_and_extract_value(command)
    if value:
        print(f"Extracted value: {value}")
    break