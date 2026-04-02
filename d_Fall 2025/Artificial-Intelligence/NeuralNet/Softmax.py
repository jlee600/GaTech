import numpy as np

class Softmax:

    # Do a softmax
    def forward(self, input):
        ## Your code here
        row = np.max(input, axis=1, keepdims=True)
        num = np.exp(input - row)
        den = np.sum(num, axis=1, keepdims=True)
        return num / den
