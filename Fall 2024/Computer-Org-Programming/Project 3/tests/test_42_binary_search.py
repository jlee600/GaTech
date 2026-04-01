# 4.2 Binary Search (#704)

from ensemble_test import autograder
from parameterized import parameterized

def search(array, target):
    try:
        return array.index(target)
    except ValueError:
        return -1

class BinarySearchTestSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()
        
        self.loadFile("binarySearch.asm")
        self.defineSubroutine("RIGHTSHIFT", { 0: "val", 1: "amt" }, 0)
    
    @parameterized.expand([
        [[-5, -3, 1, 4, 5, 6], -3], # original problem
        [[10, 20, 30, 40, 50], 40],
        [[1, 2, 4, 8, 16, 32], 9], # no match
        [[], 10], # empty list
        [[10], 10], # singleton
        *([range(8, 20, 2), i] for i in range(4, 24, 2))
    ])
    def test_binary_search(self, array, target):
        """
        (4.2) Binary Search Test
        """

        self.writeMemValue("LENGTH", len(array))
        self.writeMemValue("TARGET", target)

        arr_addr = self.readMemValue("ARRAY")
        self.writeArray(arr_addr, array)

        self.runCode()
        self.assertHalted()

        self.assertMemValue("RESULT", search(array, target))

    @parameterized.expand([
        [[-5, -3, 1, 4, 5, 6], -3],
        [[10, 20, 30, 40, 50], 40],
        [[1, 2, 4, 8, 16, 32], 9],
        [[], 10],
        [[10], 10],
    ])
    def test_binary_search_bounds(self, array, target):
        """
        (4.2) Binary Search: Bounds Checking
        """
        self.writeMemValue("LENGTH", len(array))
        self.writeMemValue("TARGET", target)

        arr_addr = self.readMemValue("ARRAY")
        # write around array:
        self.writeArray(arr_addr - 2, [target] * (len(array) + 4))
        # write array:
        self.writeArray(arr_addr, array)

        self.runCode()
        self.assertHalted()
        
        self.assertMemValue("RESULT", search(array, target))

    def test_binary_search_preserve(self):
        """
        (3.2) Binary Search: Preserve Parameters
        """
        array = [-5, -3, 1, 4, 5, 6]
        target = -3

        # get original values
        orig_array = self.readMemValue("ARRAY")

        # write parameters
        self.writeMemValue("LENGTH", len(array))
        self.writeMemValue("TARGET", target)
        self.writeArray(orig_array, array)

        # run
        self.runCode()
        self.assertHalted()

        # assert values unchanged
        self.assertMemValue("ARRAY", orig_array, "Address at mem[ARRAY] was unexpectedly overwritten during execution")

    ## Everything below here asserts binary search calls RIGHTSHIFT the correct number of times.
    ## Beware spaghetti below.

    def _count_expected_rshift(self, array, target):
        low = 0
        high = len(array) - 1
        calls = 0
        while low <= high:
            mid = (low + high) // 2
            calls += 1

            if array[mid] == target:
                break
            elif array[mid] < target:
                low = mid + 1
            else:
                high = mid - 1
        return calls

    def _count_actual_rshift(self):
        rshift_calls = 0
        self.sim.add_breakpoint("RIGHTSHIFT")

        start = self.sim.instructions_run
        while self.sim.instructions_run - start < autograder.INSTRUCTION_RUN_LIMIT:
            self.sim.run(autograder.INSTRUCTION_RUN_LIMIT)
            if self.sim.hit_halt(): break
            rshift_calls += 1

        return rshift_calls

    @parameterized.expand([
        [[-5, -3, 1, 4, 5, 6], -3],
        [[10, 20, 30, 40, 50], 40],
        [[1, 2, 4, 8, 16, 32], 9],
        [[10], 10],
        [[], 10],
        *([range(8, 20, 2), i] for i in range(4, 24, 2))
    ])
    def test_binary_search_called_rshift(self, array, target):
        """
        (4.2): Binary Search: Calls Right Shift
        """
        self.writeMemValue("LENGTH", len(array))
        self.writeMemValue("TARGET", target)
        arr_addr = self.readMemValue("ARRAY")
        self.writeArray(arr_addr, array)

        expected = self._count_expected_rshift(array, target)
        actual = self._count_actual_rshift()
        self.assertEqual(expected, actual, autograder._simple_assert_msg(
            "Incorrect number of calls to RIGHTSHIFT",
            expected, actual
        ))