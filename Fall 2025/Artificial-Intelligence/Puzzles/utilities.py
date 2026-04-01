"""
Utility functions for you to use.
Don't edit this file.
"""


def state_string(data_list, size: int) -> str:
    """
    Return a string representation of the state for debugging purposes.

    Parameters:
    data_list (list): A tuple (or list) of integers representing the state.
    size (int): The size of the square grid. data_list should be of length size^2

    Returns:
    str: A string representation of the state.
    """
    result = ""
    for row in range(size):
        start = row * size
        for col in range(size):
            result += f"{data_list[start + col]:>3} "
        if row < size - 1:
            result += "\n"
    return result


def rotate(
    current_state: tuple[int, ...], size: int, indx: int, is_up: bool
) -> tuple[int, ...]:
    """
    Perform one rotation of current_state and return the result.

    Parameters:
        current_state: The current state of the puzzle.
        size: The size of the puzzle.
        indx: The index of the rotation. (0-based, evens rotate horizontally)
        is_up: True means we are rotating forward in the game. False is used to work backwards.

    Returns:
        tuple[int,...]: The rotated state.
    """
    if indx % 2 == 0:
        # Rotate horizontally
        first_cell = indx * size
        step_count = size
        step_size = 1
    else:
        # Rotate vertically
        first_cell = indx
        step_count = size
        step_size = size

    last_cell = first_cell + step_size * (step_count - 1)

    result = list(current_state)
    if is_up:
        result[first_cell] = current_state[last_cell]
        for i in range(1, step_count):
            to_loc = first_cell + i * step_size
            from_loc = to_loc - step_size
            result[to_loc] = current_state[from_loc]
    else:
        result[last_cell] = current_state[first_cell]
        for i in range(0, step_count - 1):
            to_loc = first_cell + i * step_size
            from_loc = to_loc + step_size
            result[to_loc] = current_state[from_loc]
    return tuple(result)


def read_puzzle(file) -> tuple[tuple[int, ...], int]:
    """Reads a puzzle from a file.

    Args:
        file: The file handle to read from.

    Returns:
        tuple[int,...]: The puzzle.
        int: The size of the puzzle.
    """
    result = []
    col_count = 0
    row_count = 0
    for line in file:
        num_count = 0
        for num_str in line.split(" "):
            # Skip non-numbers
            if len(num_str) == 0 or not num_str.isdigit():
                continue

            x = int(num_str)
            result.append(x)
            num_count += 1
        if num_count > 0:
            row_count += 1
            if col_count == 0:
                col_count = num_count
            elif col_count != num_count:
                raise ValueError("Invalid puzzle format")
    if row_count != col_count:
        raise ValueError("Invalid puzzle format")
    # We use tuples because (unlike lists) they can be used as keys in dictionaries or in sets
    return (tuple(result), row_count)
