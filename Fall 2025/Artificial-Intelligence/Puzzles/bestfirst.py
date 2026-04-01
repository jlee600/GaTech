import utilities
import heapq
from typing import List

def distance_to_goal(a: tuple[int, ...], size) -> float:
    """
    Calculates the distance between the current state and the goal state.

    Parameters:
        a: The current state of the puzzle.
        size: The size of the puzzle.

    Returns:
        A number. Small numbers indicate that a is close to the goal state.
    """
    ## Your code here
    max_val = 0.0
    for idx, val in enumerate(a):
        sc = 0.0
        col_diff = 0.0
        row_diff = 0.0
        
        if val == idx:
            continue
        
        row = idx // size
        goal_row = val // size
        col = idx % size
        goal_col = val % size
        
        if row != goal_row and col % 2 == 0:
            sc += 0.001
            col = (col + 1) % size
        if goal_col < col:
            goal_col += size
        col_diff = goal_col - col

        if col != goal_col and row % 2 == 1:
            sc += 0.001
            row = (row + 1) % size
        if goal_row < row:
            goal_row += size
        row_diff = goal_row - row

        sc += col_diff + row_diff
        max_val = max(max_val, sc)
            
    return max_val

def solve_puzzle(initial_state:tuple[int,...], size:int) -> List[int]:
    """
    This function ue
    
    return scses Best-First Search to find the goal state from the initial state.
    It returns a list of operations to transform the initial state into the goal state.

    Parameters:
        initial_state: The initial state of the puzzle.
        size: The size of the puzzle.

    Returns:
        The shortest possible list of operations to transform the initial state into the goal state.
        If no solution is found, returns an empty list.
    """
    goal_state = tuple(range(size * size))

    # Do search to find a path
    ## Your code here
    ret = []
    if initial_state == goal_state:
        return ret    
    q = []
    backTracking = {}
    heapq.heappush(q, (0, initial_state))
    backTracking[initial_state] = None, None

    while q:
        dist, curr = heapq.heappop(q)

        for a in range(size):
            next_q = utilities.rotate(curr, size, a, True)
            if next_q == initial_state or next_q in backTracking:
                continue
                        
            backTracking[next_q] = curr, a
            if next_q == goal_state:
                ret = backtrack(initial_state, next_q, backTracking)
                return ret
            
            d = distance_to_goal(next_q, size)
            heapq.heappush(q, (d, next_q))
    return ret

def backtrack(sOri, sNew, bt):
    q = []
    s = sNew

    while s != sOri:
        s, a = bt[s]
        q.append(a)
    
    q.reverse()
    return q