import sys
import utilities
import random

# Check command line arguments
if len(sys.argv) != 3 :
    print(f"Usage: {sys.argv[0]} <size> <steps> ")
    exit()


# Read command line arguments
SIZE = int(sys.argv[1])
if SIZE < 2:
    print("Error: bad row count")
    exit()


STEPS = int(sys.argv[2])

# File names to create
base = f"{SIZE}_{STEPS}"
problem_path = f"problems/{base}_prob.txt"
solution_path = f"problems/{base}_sol.txt"

# Make the final state
current_state = tuple(range(SIZE * SIZE))

# Keept track of the states we have passed through
# (loops might make the puzzle super-easy)
visited_states = set()

# The solution will be assembled here
# as the reverse of the process we used to create the problem_path
# Note: not necessarily the shortest solutio
solution = []

# Possible rows and columns
idx_choices = list(range(SIZE))


for _ in range(STEPS):

    # Note this current state so we don't revisit it
    visited_states.add(current_state)

    # Come up with a next state we haven't seen before'
    while current_state in visited_states :
        idx = random.choice(idx_choices)

        # Rotate it *backwards*
        current_state = utilities.rotate(current_state, SIZE, idx, False)

    # Insert it in the solution
    solution.insert(0, idx)

with open(problem_path, "w") as pfile:
    out_string = utilities.state_string(current_state, SIZE)
    pfile.write(out_string)
    print("", file=pfile)
print(f"Problem written to '{problem_path}'")

with open(solution_path, "w") as sfile:
    for idx in solution:
        print(f"{idx} ", file=sfile, end="")
    print("", file=sfile)

print(f"Solution written to '{solution_path}'")
