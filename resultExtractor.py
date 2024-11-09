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

    mean_values = []
    best_values = []
    best_is_zero = False

    for line in output.splitlines():
        if "mean:" in line and "best =" in line:
            mean_value = float(line.split("mean:")[1].split(",")[0].strip())
            best_value = int(line.split("best =")[1].strip())
            mean_values.append(mean_value)
            best_values.append(best_value)
            if best_value == 0:
                best_is_zero = True

    return mean_values, best_values, best_is_zero

    return value

# Configuration template
config_template = r"""LogLevel {log_level}
SudokuPath {sudoku_path}
PopulationSize {population_size}
MutationRate {mutation_rate}
FittestRate {fittest_rate}
ResetCounter {reset_counter}"""

log_level = ["ERR"]
population_size = [100, 200, 400, 500, 1000, 2000]
mutation_rate = [0.1, 0.08, 0.06, 0.02]
fittest_rate = [0.2, 0.3, 0.4, 0.5]
reset_counter = [100, 200, 300, 20, 10, 15, 50]
sudoku_path = ["./testData/1.txt"]

# Generate all combinations of parameters
combinations = itertools.product(log_level, sudoku_path, population_size, mutation_rate, fittest_rate, reset_counter)

# Run the program for each combination
for log, path, pop_size, mut_rate, fit_rate, reset in combinations:
    config = config_template.format(
        log_level=log,
        sudoku_path=path,
        population_size=pop_size,
        mutation_rate=mut_rate,
        fittest_rate=fit_rate,
        reset_counter=reset
    )
    for i in range(10):
    # Save the configuration to a file
        config_filename = "config2137.txt"
        with open(config_filename, "w") as config_file:
            config_file.write(config)

        # Run the program with the configuration file
        command = ["./build/SudokuSolver", config_filename]
        value = run_program_and_extract_value(command)
        if value[2]:
            print(f"found solution  value  for config: {config}")
