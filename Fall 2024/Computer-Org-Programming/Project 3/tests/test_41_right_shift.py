# 4.1 Right Shift (#29.1)

from ensemble_test import autograder
from parameterized import parameterized

def rshift(val, shift):
    # Zero-extended right shift
    return (val & 0xFFFF) >> shift

class RightShiftTestSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()
        
        self.loadFile("binarySearch.asm")
        self.defineSubroutine("RIGHTSHIFT", { 0: "val", 1: "amt" }, 0)
    
    @parameterized.expand([
        [10, 2],
        [1515, 0],
        [2024, 10],
        [0xAAAA, 0xA],
        [-15, 1],
        *([1234, i] for i in range(18)),
        *([-5678, i] for i in range(18))
    ],
        name_func=autograder.parameterized_name
    )
    def test_right_shift(self, val, amt):
        """
        (4.1) Right Shift Test
        """
        self.callSubroutine("RIGHTSHIFT", [val, amt])
        self.assertReturned()

        expected = rshift(val, amt)
        self.assertReturnValue(expected)