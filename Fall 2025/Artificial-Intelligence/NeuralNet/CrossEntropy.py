import numpy as np

# Return the mean cross entropy loss
def loss(predictions, one_hot):
    # Your code here
    sam = -np.sum(one_hot * np.log(predictions), axis=1)
    return np.mean(sam)

