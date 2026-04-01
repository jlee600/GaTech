# 3.2 Snake Case to Camel Case (#1023.5)

from ensemble_test import autograder
from parameterized import parameterized

class SnakeToCamelTestSuite(autograder.LC3UnitTestCase):
    def setUp(self):
        super().setUp()
        
        self.loadFile("snakeToCamel.asm")
    
    @parameterized.expand([
        "snake_case",
        "cs_twenty_one_ten",
        "hello",
        "hello_world",
        "georgia_tech",
        "yellow_jackets"
    ],
        name_func=autograder.parameterized_name
    )
    def test_snake_to_camel_case(self, string : str):
        """
        (3.2) Snake Case to Camel Case Test
        """
        # write the actual string into the file
        snake_string_address = self.readMemValue("SNAKESTRING")
        self.writeString(snake_string_address, string)
        self.writeMemValue("LENGTH", len(string))

        # run the code
        self.runCode()
        self.assertHalted()

        # grab the address of the final answer
        answer_address = self.readMemValue("RESULTADDR")

        # determine the answer for myself
        camel_case_string = "".join(x.capitalize() for x in string.lower().split("_"))
        camel_case_string = string[0] + camel_case_string[1:]

        # and assert
        self.assertString(
            answer_address,
            camel_case_string
        )
    
    def test_snake_to_camel_case_preserve(self):
        """
        (3.2) Snake Case to Camel Case: Preserve Parameters
        """
        string = "snake_case"

        # get original values
        orig_snakestring = self.readMemValue("SNAKESTRING")
        orig_resultaddr = self.readMemValue("RESULTADDR")
        orig_underscore = self.readMemValue("UNDERSCORE")

        # write parameters
        self.writeString(orig_snakestring, string)
        self.writeMemValue("LENGTH", len(string))

        # run
        self.runCode()
        self.assertHalted()

        # assert values unchanged
        self.assertMemValue("SNAKESTRING", orig_snakestring, "Address at mem[SNAKESTRING] was unexpectedly overwritten during execution")
        self.assertMemValue("RESULTADDR", orig_resultaddr, "Address at mem[RESULTADDR] was unexpectedly overwritten during execution")
        self.assertMemValue("UNDERSCORE", orig_underscore, "Value at mem[UNDERSCORE] was unexpectedly overwritten during execution")
