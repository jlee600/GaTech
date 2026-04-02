# Does bidirectional search to find operations to get from initial state to goal state
import utilities
from typing import List
from collections import deque


## Feel free to add helper functions here

def solve_puzzle(initial_state:tuple[int,...], size:int) -> List[int]:
    """
    This function uses Bidirectional Search to find the goal state from the initial state.
    It returns a list of operations to transform the initial state into the goal state.

    Parameters:
        initial_state: The initial state of the puzzle.
        size: The size of the puzzle.

    Returns:
        A list of operations to transform the initial state into the goal state.
        If no solution is found, returns an empty list.
    """

    # Where am I trying to get?
    goal_state = tuple(range(size * size))

    # Do search necessary to find a path from initial state to goal state
    ## Your code here
    ret = []

    forwardQ = deque()
    backwardQ = deque()
    forwardQ.append(initial_state)
    backwardQ.append(goal_state)

    forward_bt = {}
    backward_bt = {}
    center = None

    forward = True
    while forwardQ and backwardQ:
        if forward:
            for i in range(len(forwardQ)):
                curr = forwardQ.popleft()
                
                for a in range(size):
                    next_q = utilities.rotate(curr, size, a, True)
                    if next_q in forward_bt:
                        continue
                    forward_bt[next_q] = curr, a
                    if next_q in backward_bt:
                        center = next_q
                        break
                    forwardQ.append(next_q)
                
                if center:
                    break
        else: 
            for i in range(len(backwardQ)):
                curr = backwardQ.popleft()
                
                for a in range(size):
                    next_q = utilities.rotate(curr, size, a, False)
                    if next_q in backward_bt:
                        continue
                    backward_bt[next_q] = curr, a
                    if next_q in forward_bt:
                        center = next_q
                        break
                    backwardQ.append(next_q)
                
                if center:
                    break
        
        if center:
            break
        forward = not forward
    
    if center is None:
        return ret
    ret = backtrack(initial_state, center, forward_bt, True) + backtrack(goal_state, center, backward_bt, False)

    return ret

def backtrack(sOri, sNew, bt, rev):
    q = []
    s = sNew

    while s != sOri:
        s, a = bt[s]
        q.append(a)
    if rev:
        q.reverse()
    return q