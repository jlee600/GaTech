import numpy as np

class Relu:

    # Make a copy of input. In the copy: any of the numbers
    # that are less than zero, set to zero.
    # Note which ones you set to zero so you can
    # use that in input_gradients
    def forward(self, input):
        output = np.copy(input)
        ## Your code here
        self.mask = output < 0
        output[self.mask] = 0
        return output

    def input_gradients(self, dL):
        ## Your code here
        out = np.copy(dL)
        out[self.mask] = 0
        return out
