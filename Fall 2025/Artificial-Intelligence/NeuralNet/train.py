import numpy as np
import mnist_load
import FullyConnected
import Relu
import Softmax
import CrossEntropy
import pickle

# Constants
BATCH_SIZE = 200
HIDDEN_LAYER = 64
EPOCHS = 60
LEARNING_RATE = 0.1
CATEGORY_COUNT = 10

# Load the mnist data into numpy arrays
# These are already in a random order
X_train_int, y_train = mnist_load.load("training")
X_test_int, y_test = mnist_load.load("test")

# n is 60,000, input_count is 784
(n, input_count) = X_train_int.shape

# X_*_int has integers between 0 and 255, convert to floats from 0 to 1
X_train = X_train_int / 255.0
X_test = X_test_int / 255.00

# Log the shapes
print(f"Training: X = {X_train.shape}, y = {y_train.shape}")
print(f"Testing:  X = {X_test.shape}, y = {y_test.shape}")

# We are training in batches
batch_count = n // BATCH_SIZE
print(f"Processing in {batch_count} batches of {BATCH_SIZE} data points")

# Create the layers and put them in a dictionary
layers = {
    "fc1": FullyConnected.FullyConnected(input_count, HIDDEN_LAYER),
    "act1": Relu.Relu(),
    "fc2": FullyConnected.FullyConnected(HIDDEN_LAYER, CATEGORY_COUNT),
    "sm": Softmax.Softmax(),
}

# Figure out how many parameters we are tuning
parameter_count = layers["fc1"].parameter_count() + layers["fc2"].parameter_count()
print(f"Tuning {parameter_count:,d} parameters:")

# predict does a soft prediction using the layers in layer_dict
def predict(layer_dict, batch_X):
    n = batch_X.shape[0]
    A = layer_dict["fc1"].forward(batch_X)
    assert A.shape == (n, HIDDEN_LAYER)
    B = layer_dict["act1"].forward(A)
    assert B.shape == (n, HIDDEN_LAYER)
    C = layer_dict["fc2"].forward(B)
    assert C.shape == (n, CATEGORY_COUNT)
    y_hat = layer_dict["sm"].forward(C)
    assert y_hat.shape == (n, CATEGORY_COUNT)
    return y_hat

# Convert soft prediction into hard one
# and return the accuracy as a float between 0 and 1.
def compute_accuracy(proba, labels):
    ## Your code here
    acc = np.argmax(proba, axis=1) == labels
    return np.mean(acc)

# Arrays for analysis
mean_loss_log = np.zeros(EPOCHS)

# Start the training loop
for epoch in range(EPOCHS):

    # An array to hold the mean cross entropy loss for each batch
    batch_loss_log = np.zeros(batch_count)

    # The first batch starts at row o
    start_sample = 0

    for batch_index in range(batch_count):

        # Get the batch
        next_start_sample = start_sample + BATCH_SIZE
        batch_X = X_train[start_sample:next_start_sample]

        # Convert y_train into a one-hot-per-row
        batch_y = np.eye(CATEGORY_COUNT)[y_train[start_sample:next_start_sample]]
        assert batch_y.shape == (BATCH_SIZE, CATEGORY_COUNT)

        # Do a soft prediction
        y_hat = predict(layers, batch_X)
        assert y_hat.shape == (BATCH_SIZE, CATEGORY_COUNT)
        assert y_hat.min() > 0.0

        # Uncomment to test just initialization and forward phase
        # exit(-1)

        # Get the mean cross entropy loss
        batch_loss = CrossEntropy.loss(y_hat, batch_y)
        assert batch_loss > 0.0

        # Record it for the graph
        batch_loss_log[batch_index] = batch_loss

        # Do backpropogation to compute gradients
        dL_dc = (y_hat - batch_y) / BATCH_SIZE ## Your code here
        assert dL_dc.shape == (BATCH_SIZE, CATEGORY_COUNT)

        grad_fc2_w = layers["fc2"].weights_gradient(dL_dc) ## Your code here
        assert grad_fc2_w.shape == layers["fc2"].weights.shape

        grad_fc2_b = layers["fc2"].biases_gradient(dL_dc) ## Your code here
        assert grad_fc2_b.shape == layers["fc2"].biases.shape

        dL_db = layers["fc2"].input_gradients(dL_dc) ## Your code here
        assert dL_db.shape == (BATCH_SIZE, HIDDEN_LAYER)

        dL_da = layers["act1"].input_gradients(dL_db) ## Your code here
        assert dL_db.shape == (BATCH_SIZE, HIDDEN_LAYER)

        grad_fc1_w = layers["fc1"].weights_gradient(dL_da) ## Your code here
        assert grad_fc1_w.shape == layers["fc1"].weights.shape

        grad_fc1_b = layers["fc1"].biases_gradient(dL_da) ## Your code here
        assert grad_fc1_b.shape == layers["fc1"].biases.shape

        # Update the weights
        fc1_w = layers["fc1"].weights
        layers["fc1"].weights = fc1_w - LEARNING_RATE * grad_fc1_w ## Your code here
        fc1_b = layers["fc1"].biases
        layers["fc1"].biases = fc1_b - LEARNING_RATE * grad_fc1_b ## Your code here
        fc2_w = layers["fc2"].weights
        layers["fc2"].weights = fc2_w - LEARNING_RATE * grad_fc2_w ## Your code here
        fc2_b = layers["fc2"].biases
        layers["fc2"].biases = fc2_b - LEARNING_RATE * grad_fc2_b ## Your code here

        # Go on to the next batch
        start_sample = next_start_sample

    # Note mean loss
    mean_loss = batch_loss_log.mean()
    print(f"Epoch {epoch + 1} loss: {mean_loss:.6f}")
    mean_loss_log[epoch] = mean_loss

    # Every 20 epochs check the accuracy
    if (epoch + 1) % 20 == 0:

        # Check the accuracy on training data
        proba = predict(layers, X_train)
        assert proba.shape == (X_train.shape[0], CATEGORY_COUNT)
        accuracy = compute_accuracy(proba, y_train)
        print(f"*** Epoch {epoch + 1}: \n\tTrain Accuracy {100.0 * accuracy:.1f}%")

        # Check the accuracy on testin data
        proba = predict(layers, X_test)
        assert proba.shape == (X_test.shape[0], CATEGORY_COUNT)
        accuracy = compute_accuracy(proba, y_test)
        print(f"\t Test Accuracy {100.0 * accuracy:.1f}%")

# Save the weights and biases to a pickle file
with open("weights.pkl", "wb") as f:
    pickle.dump({
        "fc1_w": layers["fc1"].weights,
        "fc1_b": layers["fc1"].biases,
        "fc2_w": layers["fc2"].weights,
        "fc2_b": layers["fc2"].biases,
        "input_count": input_count,
        "hidden_size": HIDDEN_LAYER,
        "output_size": CATEGORY_COUNT
    }, f)


# Save the array of loss for each epoch
with open("losslog.pkl", "wb") as f:
    pickle.dump(mean_loss_log, f)
