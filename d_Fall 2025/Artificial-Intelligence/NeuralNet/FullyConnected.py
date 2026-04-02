import numpy as np

# Implements a fully connected layer
class FullyConnected:

    def __init__(self, input_count, output_count) -> None:
        # Initialize weights and biases using He method
        ## Your code here
        self.weights = np.random.normal(0, np.sqrt(2/input_count), (input_count, output_count))# Your code here
        self.biases = np.zeros(output_count)# Your code here

    # Do the forward operation and stash the input in an instance
    # variable so you can use it in weights_gradients
    def forward(self, input):
        ## Your code here
        self.input = input
        return np.dot(input, self.weights) + self.biases

    def input_gradients(self, dL):
        ## Your code here
        return np.dot(dL, self.weights.T)

    def weights_gradient(self, dL):
        ## Your code here
        return np.dot(self.input.T, dL)

    def biases_gradient(self, dL):
        ## Your code here
        return np.sum(dL, axis=0)
    
    # How many total numbers in weights and baises?
    def parameter_count(self):
        ## Your code here
        return self.weights.size + self.biases.size
