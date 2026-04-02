import numpy as np
import mnist_load
from PIL import Image

# Load the mnist data into numpy arrays
X_test_int, y_test = mnist_load.load("test")

i = np.random.randint(0, X_test_int.shape[0])
image = X_test_int[i, :]
image_to_save = image.reshape((28, 28))

label = y_test[i]

im = Image.fromarray(image_to_save)
im.save(f"image_{i}_{label}.png")
