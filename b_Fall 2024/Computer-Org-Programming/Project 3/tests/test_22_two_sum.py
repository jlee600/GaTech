# 2.2 Two Sum (#1)

from ensemble_test import autograder
from parameterized import parameterized
from random import randint

class TwoSumTestSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()
        
        self.loadFile("twoSum.asm")
    
    @parameterized.expand([
        [[2, 7, 11, 15], 9],
        [[i for i in range(8)], 2],
        [[2, 4, 9, 17, 21, 40, 92, 112], 109]
    ])
    def test_two_sum(self, array, target):
        """
        (2.2) Two Sum Test
        """
        self.writeMemValue("LENGTH", len(array))
        self.writeMemValue("TARGET", target)

        array_address = self.readMemValue("ARRAY")
        self.writeArray(array_address, array)

        self.runCode()
        self.assertHalted()

        answer_array = [-10, -10]
        for i in range(len(array)) :
            for j in range(i + 1, len(array)) :
                if array[i] + array[j] == target :
                    answer_array[0] = i
                    answer_array[1] = j

        self.assertArray(
            "ANSWER",
            answer_array
        )

    def test_two_sum_preserve(self):
        """
        (2.2) Two Sum: Preserve Parameters
        """
        array = [2, 7, 11, 15]
        target = 9

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
