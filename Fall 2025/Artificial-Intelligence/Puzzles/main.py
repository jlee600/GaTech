import sys
import utilities
import time
import bfs
import bidirectional
import bestfirst

# Check for right number of command-line arguments
if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} <bfs|bidirectional|bestfirst> <infile>")
    exit()

# Get command-line arguments
method = sys.argv[1]

# In case the user used the filename
method = method.removesuffix(".py")

inpath = sys.argv[2]

# Read in the initial state
with open(inpath, "r") as infile:
    (initial_state, size) = utilities.read_puzzle(infile)

# Print initial state for debugging purposes
print(f"Initial state:\n{utilities.state_string(initial_state, size)}")

# Measure the execution time
start_time = time.perf_counter()

# Pick a solve_puzzle function based on the command-line argument
if method == "bfs":
    solution = bfs.solve_puzzle(initial_state, size)
elif method == "bidirectional":
    solution = bidirectional.solve_puzzle(initial_state, size)
elif method == "bestfirst":
    solution = bestfirst.solve_puzzle(initial_state, size)
else:
    print(f"**Error: Invalid method '{method}'")
    exit()

execution_time = time.perf_counter() - start_time

# Print stats
print(f"Execution time: {execution_time:.4f} seconds")

# Did it find a solution?
if len(solution)> 0:
    print(f"Solution has {len(solution)} steps")
else:
    print("***Error: No solution found! ***")
    exit()

# Check solution by executing the actions in the solution
current_state = initial_state
for action in solution:
    # print(f"Current state = \n{utilities.state_string(current_state, size)}, rotating: {action}")
    current_state = utilities.rotate(current_state, size, action, True)

# Does the final state match the goal state?
goal_state = tuple(range(size * size))
if current_state == goal_state:
    print("Solution correct.")
else:
    print("*** Error: Solution is incorrect! ***")
