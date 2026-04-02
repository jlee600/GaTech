# Does Breadth-First Search to find the goal state
import utilities
from typing import List

# For your search queue
from collections import deque

## Feel free to add helper functions here

def solve_puzzle(initial_state:tuple[int,...], size:int) -> List[int]:
    """
    This function uses Breadth-First Search (BFS) to find the goal state from the initial state.
    It returns a list of operations to transform the initial state into the goal state.

    Parameters:
        initial_state: The initial state of the puzzle.
        size: The size of the puzzle.

    Returns:
        A list of operations to transform the initial state into the goal state. If no solution is found, returns an empty list.
    """

    goal_state = tuple(range(size * size))
    
    # Do search to find a path
    ## Your code here
    ret = []
    if initial_state == goal_state:
        return ret
    
    q = deque()
    backTracking = {}
    q.append(initial_state)

    while q:
        curr = q.popleft()

        for a in range(size):
            next_q = utilities.rotate(curr, size, a, True)
            # print(utilities.state_string(next_q, size))
            # print()
            if next_q == initial_state or next_q in backTracking:
                continue

            q.append(next_q)
            backTracking[next_q] = curr, a
            # print(backTracking)
            # return path

            if next_q == goal_state:
                while next_q != initial_state:
                    s, a = backTracking[next_q]
                    ret.append(a)
                    next_q = s

                ret.reverse()
                return ret

    return ret
