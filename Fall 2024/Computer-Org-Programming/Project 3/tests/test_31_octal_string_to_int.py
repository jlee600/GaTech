# 3.1 Octal String to Integer (#8.5)

from ensemble_test import autograder
from parameterized import parameterized

class OctalStringToIntTestSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()
        
        self.loadFile("octalStringToInt.asm")
    
    @parameterized.expand([
        "2110",
        "712",
        "1000",
        "777",
        "1234",
        "2"
    ],
        name_func=autograder.parameterized_name
    )
    def test_octal_string_to_int(self, string):
        """
        (3.1) Octal String to Integer Test
        """
        # write the octal string
        octal_address = self.readMemValue("OCTALSTRING")
        self.writeString(octal_address, string)
        self.writeMemValue("LENGTH", len(string))

        self.runCode()
        self.assertHalted()

        # determine the final answer
        answer = int(string, 8)

        # assert it's true
        answer_address = self.readMemValue("RESULTADDR")
        self.assertMemValue(
            answer_address,
            answer
        )
    
    def test_octal_string_to_int_preserve(self):
        """
        (3.1) Octal String to Integer: Preserve Parameters
        """
        string = "2110"

        # get original values
        orig_ascii = self.readMemValue("NEGATIVE_ASCII_ZERO")
        orig_octalstring = self.readMemValue("OCTALSTRING")
        orig_resultaddr = self.readMemValue("RESULTADDR")

        # write parameters
        self.writeString(orig_octalstring, string)
        self.writeMemValue("LENGTH", len(string))

        # run
        self.runCode()
        self.assertHalted()

        # assert values unchanged
        self.assertMemValue("NEGATIVE_ASCII_ZERO", orig_ascii, "Value at mem[NEGATIVE_ASCII_ZERO] was unexpectedly overwritten during execution")
        self.assertMemValue("OCTALSTRING", orig_octalstring, "Address at mem[OCTALSTRING] was unexpectedly overwritten during execution")
        self.assertMemValue("RESULTADDR", orig_resultaddr, "Address at mem[RESULTADDR] was unexpectedly overwritten during execution")

