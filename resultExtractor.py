import subprocess
import itertools
import pandas as pd
import os
import threading
from datetime import datetime

def run_program_and_extract_value(command):
    # Prepare the PowerShell command
    powershell_command = ["powershell", "-Command"] + command

    # Run the command in PowerShell
    result = subprocess.run(powershell_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

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
def isConfigValid(config):
    path, pop_size, mut_rate, fit_rate, reset, preserve_selection, multimut_pop, multi_mut_coeff, mut_res_num, mut_res_coeff, fittest, cross_mut, solver_type = config
    if multimut_pop == 0 and multi_mut_coeff != 0:
        return False
    if mut_res_num == 0 and mut_res_coeff != 0:
        return False
    # if reset < mut_res_num:
    #     return False
    if pop_size < 250:
        return False
    if reset ==20:
        return False
    if preserve_selection == "true":
        if fit_rate+multimut_pop*fit_rate>1:
            return False
    return True

print(f"Starting...{datetime.now()}")

# Configuration template
config_template = r"""LogLevel {log_level}
SudokuPath {sudoku_path}
PopulationSize {population_size}
MutationRate {mutation_rate}
FittestRate {fittest_rate}
ResetCounter {reset_counter}"""

config_template = r"""LogLevel ERR
SudokuPath {sudoku_path}
PopulationSize {population_size}
MutationRate {mutation_rate}
FittestRate {fittest_rate}
ResetCounter {reset_counter}
PreserveSelection {preserve_selection}
MultiMutationCoeff {multimut_pop} {multi_mut_coeff}
MutationReset {mut_res_num} {mut_res_coeff}
Fittest {fittest}
SolverType {solver_type}
MultiCrossover true
ExperimentMutation true"""


population_size = [1000]
mutation_rate = [0.05]
fittest_rate = [0.05]
reset_counter = [8000]
preserve_selection = [True]
multimut_pop = [8]
multi_mut_coeff = [4]
mut_res_num = [500]
mut_res_coeff = [0.8]
fittest = [0]
cross_mut = [False]
solver_type = [0]
sudoku_path = ["./testData/1.txt","./testData/1mid.txt","./testData/1hard.txt","./testData/solverhard.txt"]

# Generate all combinations of parameters
combinations = list(itertools.product(sudoku_path, population_size, mutation_rate, fittest_rate, reset_counter,preserve_selection
                                      ,multimut_pop,multi_mut_coeff,mut_res_num,mut_res_coeff,fittest,cross_mut,solver_type))

# List to store results
results = []

# Counter to track the number of runs
run_counter = 0
save_interval = 15  # Save every 30 runs
filename="C:\\inz\\GeneticSudokuSolver\\rowpermutationtornament.csv"
# Check if results.csv exists and load existing results
if os.path.exists(filename):
    df_existing = pd.read_csv(filename)
    results = df_existing.to_dict('records')
    last_run = df_existing.iloc[-1]
    print(combinations[0])
    last_combination = (last_run['sudoku_path'], last_run['population_size'], last_run['mutation_rate'], last_run['fittest_rate'], last_run['reset_counter'], last_run['preserve_selection'], last_run['multimut_pop'], last_run['multi_mut_coeff'], last_run['mut_res_num'], last_run['mut_res_coeff'], last_run['fittest'], last_run['cross_mut'], last_run['solver_type'])
    last_iteration = last_run['iteration']
    print(last_combination)
    # print(combinations[200])
    start_index = combinations.index(last_combination)
    print(f"Loaded {len(results)} existing results. Last run: {last_combination}, iteration {last_iteration}")
    start_iteration = last_iteration + 1
    df_existing=[]
else:
    print("No existing results found.")
    start_index = 0
    start_iteration = 1

# Run the program for each combination
for index in range(start_index, len(combinations)):
    if not isConfigValid(combinations[index]):
        continue
    path, pop_size, mut_rate, fit_rate, reset, preserve_selection, multimut_pop, multi_mut_coeff, mut_res_num, mut_res_coeff, fittest, cross_mut, solver_type = combinations[index]
    config = config_template.format(
        sudoku_path=path,
        population_size=pop_size,
        mutation_rate=mut_rate,
        fittest_rate=fit_rate,
        reset_counter=reset,
        preserve_selection=preserve_selection,
        multimut_pop=multimut_pop,
        multi_mut_coeff=multi_mut_coeff,
        mut_res_num=mut_res_num,
        mut_res_coeff=mut_res_coeff,
        fittest=fittest,
        cross_mut=cross_mut,
        solver_type=solver_type
    )
    for i in range(start_iteration if index == start_index else 1, 101):
        print(f"\rProcessing combination , {path}, {pop_size}, {mut_rate}, {fit_rate}, {reset}, iteration {i}/5   {run_counter}  left:{index/len(combinations)}  {index}/{len(combinations)}", end="")
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
                "sudoku_path": path,
                "population_size": pop_size,
                "mutation_rate": mut_rate,
                "fittest_rate": fit_rate,
                "reset_counter": reset,
                "preserve_selection": preserve_selection,
                "multimut_pop": multimut_pop,
                "multi_mut_coeff": multi_mut_coeff,
                "mut_res_num": mut_res_num,
                "mut_res_coeff": mut_res_coeff,
                "fittest": fittest,
                "cross_mut": cross_mut,
                "solver_type": solver_type,
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
            file_exists = os.path.isfile(filename)
            df.to_csv(filename, mode='a', header=not file_exists, index=False)
            results = []  # Clear the results list to free memory

    # Reset start_iteration for subsequent combinations
    start_iteration = 1

# Save the final DataFrame
df = pd.DataFrame(results)
df.to_csv(filename, mode='a', header=False, index=False)

#add hour
print(f"\nProcessing complete.{datetime.now()} ")