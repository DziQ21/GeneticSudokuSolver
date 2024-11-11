import subprocess
import itertools
import pandas as pd
import os
import threading
from datetime import datetime

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

print(f"Starting...{datetime.now()}")

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
sudoku_path = ["./testData/1mid.txt"]

# Generate all combinations of parameters
combinations = list(itertools.product(log_level, sudoku_path, population_size, mutation_rate, fittest_rate, reset_counter))

# List to store results
results = []

# Counter to track the number of runs
run_counter = 0
save_interval = 30  # Save every 30 runs

# Check if results.csv exists and load existing results
if os.path.exists("C:\\inz\\GeneticSudokuSolver\\resultsMid.csv"):
    df_existing = pd.read_csv("C:\\inz\\GeneticSudokuSolver\\resultsMid.csv")
    results = df_existing.to_dict('records')
    last_run = df_existing.iloc[-1]
    last_combination = (last_run['log_level'], last_run['sudoku_path'], last_run['population_size'], last_run['mutation_rate'], last_run['fittest_rate'], last_run['reset_counter'])
    last_iteration = last_run['iteration']
    start_index = combinations.index(last_combination)
    print(f"Loaded {len(results)} existing results. Last run: {last_combination}, iteration {last_iteration}")
    start_iteration = last_iteration + 1
else:
    print("No existing results found.")
    start_index = 0
    start_iteration = 1

# Run the program for each combination
for index in range(start_index, len(combinations)):
    log, path, pop_size, mut_rate, fit_rate, reset = combinations[index]
    config = config_template.format(
        log_level=log,
        sudoku_path=path,
        population_size=pop_size,
        mutation_rate=mut_rate,
        fittest_rate=fit_rate,
        reset_counter=reset
    )
    for i in range(start_iteration if index == start_index else 1, 11):
        print(f"\rProcessing combination {log}, {path}, {pop_size}, {mut_rate}, {fit_rate}, {reset}, iteration {i}/10   {run_counter}    ", end="")
        # Save the configuration to a file
        config_filename = "config2137.txt"
        with open(config_filename, "w") as config_file:
            config_file.write(config)

        # Run the program with the configuration file
        command = ["./build/SudokuSolver.exe", config_filename]
        
        result = run_program_and_extract_value(command)
        if result is not None:
            mean_values, best_values, best_is_zero = result
            results.append({
                "log_level": log,
                "sudoku_path": path,
                "population_size": pop_size,
                "mutation_rate": mut_rate,
                "fittest_rate": fit_rate,
                "reset_counter": reset,
                "iteration": i,
                "mean_values": mean_values,
                "best_values": best_values,
                "best_is_zero": best_is_zero
            })
            if best_is_zero:
                print(f"Found solution for config: {config}")

        run_counter += 1

        # Save the DataFrame every few runs
        if run_counter % save_interval == 0:
            df = pd.DataFrame(results)
            df.to_csv("resultsMid.csv", index=False)

    # Reset start_iteration for subsequent combinations
    start_iteration = 1

# Save the final DataFrame
df = pd.DataFrame(results)
df.to_csv("resultsMid.csv", index=False)
#add hour
print(f"\nProcessing complete.{datetime.now()} ")