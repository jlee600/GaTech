# 2.1 Minimum of Array (#153.5)

from ensemble_test import autograder
from parameterized import parameterized
import random

random.seed(2110)

class MinArrayTestSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()

        self.loadFile("minArray.asm")

    @parameterized.expand([
        [[1, 2, 3, 4, 5, 6]],
        [[62, 90, 56, 49, 99]],
        [[-4, -10, -11, -24, -32]],
        [[random.randint(-500, 500) for i in range(10)]],
        [[random.randint(-2110, 2110) for i in range(20)]],
    ])
    def test_min_array(self, array):
        """
        (2.1) Minimum of Array Test
        """
        self.writeMemValue("LENGTH", len(array))

        array_address = self.readMemValue("ARRAY")
        self.writeArray(array_address, array)

        self.runCode()
        self.assertHalted()

        answer_address = self.readMemValue("ANSWERADDR")
        self.assertMemValue(
            answer_address,
            min(array)
        )

    def test_min_array_preserve(self):
        """
        (2.1) Minimum of Array: Preserve Parameters
        """
        array = [1, 2, 3, 4, 5, 6]

        # get original values
        orig_array = self.readMemValue("ARRAY")
        orig_answer = self.readMemValue("ANSWERADDR")

        # write parameters
        self.writeMemValue("LENGTH", len(array))
        self.writeArray(orig_array, array)

        # run
        self.runCode()
        self.assertHalted()

        # assert values unchanged
        self.assertMemValue("ARRAY", orig_array, "Address at mem[ARRAY] was unexpectedly overwritten during execution")
        self.assertMemValue("ANSWERADDR", orig_answer, "Address at mem[ANSWERADDR] was unexpectedly overwritten during execution")
