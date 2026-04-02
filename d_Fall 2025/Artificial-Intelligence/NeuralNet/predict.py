import numpy as np
import FullyConnected
import Relu
import Softmax
import pickle
from PIL import Image
import sys

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} <image>")
    exit(-1)

# Constants
BATCH_SIZE = 200
HIDDEN_LAYER = 64
CATEGORY_COUNT = 10

# Read in the image
img = Image.open(sys.argv[1])
x = np.array(img)
x = np.expand_dims(np.reshape(x, (28 * 28)) / 255.0, axis=0)
input_count = x.shape[1]

# Create the layers
layers = {
    "fc1": FullyConnected.FullyConnected(input_count, HIDDEN_LAYER),
    "act1": Relu.Relu(),
    "fc2": FullyConnected.FullyConnected(HIDDEN_LAYER, CATEGORY_COUNT),
    "sm": Softmax.Softmax(),
}

# Load in the weights and biases
with open("weights.pkl", "rb") as f:
    layers["fc1"].weights = pickle.load(f)
    layers["fc1"].biases = pickle.load(f)
    layers["fc2"].weights = pickle.load(f)
    layers["fc2"].biases = pickle.load(f)

# Do the prediction
A = layers["fc1"].forward(x)
B = layers["act1"].forward(A)
C = layers["fc2"].forward(B)
y_hat = layers["sm"].forward(C)

# Print your guess
print(f"{np.argmax(y_hat)}")
