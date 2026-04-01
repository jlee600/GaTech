# 2.3 OR Adjacent Elements (#3173)

from ensemble_test import autograder
from parameterized import parameterized

class OrPairsTestSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()

        self.loadFile("orAdj.asm")
    
    @parameterized.expand([
        [[1, 3, 7, 15]],
        *([[i, i*2, i-1, 40]] for i in range(7)),
        *([[17, i, i*7]] for i in range(19))
    ])
    def test_or_pairs(self, array):
        """
        (2.3) OR Adjacent Elements Test
        """

        # write the length variable into file
        self.writeMemValue("LENGTH", len(array))

        # write the array as well
        array_address = self.readMemValue("ARRAY")
        self.writeArray(array_address, array)

        # run the code
        self.runCode()
        self.assertHalted()

        # actually determine the final array
        answer_array = [array[i] | array[i + 1] for i in range(len(array) - 1)]

        # assert array
        target_address = self.readMemValue("TARGET")
        self.assertArray(
            target_address,
            answer_array
        )

    def test_or_pairs_preserve(self):
        """
        (2.2) OR Adjacent Elements: Preserve Parameters
        """
        array = [1, 3, 7, 15]

        # get original values
        orig_array = self.readMemValue("ARRAY")
        orig_target = self.readMemValue("TARGET")

        # write parameters
        self.writeMemValue("LENGTH", len(array))
        self.writeArray(orig_array, array)

        # run
        self.runCode()
        self.assertHalted()

        # assert values unchanged
        self.assertMemValue("ARRAY", orig_array, "Address at mem[ARRAY] was unexpectedly overwritten during execution")
        self.assertMemValue("TARGET", orig_target, "Address at mem[TARGET] was unexpectedly overwritten during execution")
