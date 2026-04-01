import cv2
import numpy as np
import matplotlib.pyplot as plt
from scipy.ndimage.filters import maximum_filter
import pdb


def get_gaussian_kernel(ksize, sigma):
    """
    Generate a Gaussian kernel to be used later (in get_interest_points for calculating
    image gradients and a second moment matrgrad_x).
    You can call this function to get the 2D gaussian filter.

    Hints:
    1) Make sure the value sum to 1
    2) Some useful functions: cv2.getGaussianKernel

    Args:
    -   ksize: kernel size
    -   sigma: kernel standard deviation

    Returns:
    -   kernel: numpy nd-array of size [ksize, ksize]
    """

    kernel = None
    #############################################################################
    # TODO: YOUR GAUSSIAN KERNEL CODE HERE                                      #
    #############################################################################

    x = cv2.getGaussianKernel(ksize, sigma)
    kernel = np.dot(x, x.T)
    kernel = kernel / np.sum(kernel)

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################

    return kernel

def my_filter2D(image, filter, bias = 0):
    """
    Compute a 2D convolution. Pad the border of the image using 0s.
    Any type of automatic convolution is not allowed (i.e. np.convolve, cv2.filter2D, etc.)

    Hints:
        Padding width should be half of the filter's shape (correspondingly)
        The conv_image shape should be same as the input image
        Helpful functions: cv2.copyMakeBorder

    Args:
    -   image: A numpy array of shape (m,n) or (m,n,c),
                depending if image is grayscale or colored
    -   filter: filter that will be used in the convolution with shape (a,b)
    -   bias: An bias value added to every output

    Returns:
    -   conv_image: image resulting from the convolution with the filter
    """
    conv_image = None

    #############################################################################
    # TODO: YOUR MY FILTER 2D CODE HERE                                         #
    #############################################################################

    img = image.astype(np.float64)
    filter = np.asarray(filter, dtype=np.float64)
    filter = np.flipud(np.fliplr(filter)) 
    filter_height, filter_width = filter.shape
    pad_height = filter_height // 2
    pad_width = filter_width // 2
    
    if len(img.shape) == 3:
        h, w, c = img.shape
        padded = cv2.copyMakeBorder(img, pad_height, pad_height, pad_width, pad_width, cv2.BORDER_CONSTANT, value=0)
        conv_image = np.zeros((h, w, c), dtype=np.float64)

        for i in range(h):
            for j in range(w):
                for k in range(c):
                    temp = padded[i:i + filter_height, j:j + filter_width, k]
                    conv_image[i, j, k] = np.sum(temp * filter) + bias
    elif len(img.shape) == 2:
        h, w = img.shape
        padded = cv2.copyMakeBorder(img, pad_height, pad_height, pad_width, pad_width, cv2.BORDER_CONSTANT, value=0)
        conv_image = np.zeros((h, w), dtype=np.float64)

        for i in range(h):
            for j in range(w):
                temp = padded[i:i + filter_height, j:j + filter_width]
                conv_image[i, j] = np.sum(temp * filter) + bias

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################
    return conv_image

def get_gradients(image):
    """
    Compute smoothed gradients grad_x & grad_y. This will be done using a sobel filter,
    which is of shape (3, 3). Sobel filters can be used to approximate the image
    gradient, and it will be a different filter for the x and y directions.

    Helpful functions: my_filter2D from above

    Args:
    -   image: A numpy array of shape (m,n) containing the image

    Returns:
    -   grad_x: numpy nd-array of shape (m,n) containing the image convolved with differentiated kernel in the x direction
    -   grad_y: numpy nd-array of shape (m,n) containing the image convolved with differentiated kernel in the y direction

    Note: Remember that the image gradient in the x-direction corresponds to vertical edge detection and vice versa for y.
    """

    grad_x, grad_y = None, None
    #############################################################################
    # TODO: YOUR IMAGE GRADIENTS CODE HERE                                      #
    #############################################################################

    grad_x = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], dtype=np.float64)
    grad_x = my_filter2D(image, grad_x)
    grad_y = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]], dtype=np.float64)
    grad_y = my_filter2D(image, grad_y)

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################

    return grad_x, grad_y


def remove_border_vals(image, x, y, c, window_size = 16):
    """
    Remove interest points that are too close to a border to allow SIFTfeature
    extraction. Make sure you remove all points where a window around
    that point cannot be formed.

    Args:
    -   image: image: A numpy array of shape (m,n,c),
        image may be grayscale of color (your choice)
    -   x: numpy array of shape (N,)
    -   y: numpy array of shape (N,)
    -   c: numpy array of shape (N,)
    -   window_size: int of the window size that we want to remove. (i.e. make sure all
        points in a window_size by window_size area can be formed around a point)
        Set this to 16 for unit testing. Treat the center point of this window as the bottom right
        of the center-most 4 pgrad_xels. This will be the same window used for SIFT.

    Returns:
    -   x: A numpy array of shape (N-#removed vals,) containing x-coordinates of interest points
    -   y: A numpy array of shape (N-#removed vals,) containing y-coordinates of interest points
    -   c (optional): numpy nd-array of dim (N-#removed vals,) containing the strength
    """

    #############################################################################
    # TODO: YOUR REMOVE BORDER VALS CODE HERE                                   #
    #############################################################################

    h, w = image.shape[:2]
    half_window = window_size // 2
    filter = (x >= half_window) & (x < w - half_window) & (y >= half_window) & (y < h - half_window)
    x = x[filter]
    y = y[filter]

    if c is not None:
        c = c[filter]

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################
    return x, y, c

def second_moments(grad_x, grad_y, ksize = 7, sigma = 10):
    """
    Given image gradients, grad_x and grad_y, compute x2, xy2, y2 using a gaussian filter.
    Second moments, AKA the variance, provide a measure of how spread out the values are in a distribution.
    These moments are computed by convolving the image gradients with a Gaussian filter.

    Helpful functions: my_filter2D, get_gaussian_kernel

    Args:
    -   grad_x: numpy nd-array of shape (m,n) containing the gradient of the image with respect to x
    -   grad_y: numpy nd-array of shape (m,n) containing the gradient of the image with respect to y
    -   ksize: size of gaussian filter (set this to 7 for unit testing)
    -   sigma: deviation of gaussian filter (set this to 10 for unit testing)

    Returns:
    -   x2: A numpy nd-array of shape (m,n) containing the second moment in the x direction twice
    -   y2: A numpy nd-array of shape (m,n) containing the second moment in the y direction twice
    -   xy2: (optional): numpy nd-array of dim (m,n) containing the second moment in the x then the y direction
    """

    x2, y2, xy2 = None, None, None
    #############################################################################
    # TODO: YOUR SECOND MOMENTS CODE HERE                                       #
    #############################################################################

    x = grad_x ** 2
    y = grad_y ** 2
    grad_xy = grad_x * grad_y

    g = get_gaussian_kernel(ksize, sigma)
    x2 = my_filter2D(x, g)
    y2 = my_filter2D(y, g)
    xy2 = my_filter2D(grad_xy, g)

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################

    return x2, y2, xy2

def corner_response(x2, y2, xy2, alpha):

    """
    Given second moments function below, calculate corner resposne.

    R = det(M) - alpha(trace(M)^2)
    where M = [[x2, xy2],
                [xy2, y2]]

    Args:
    -   x2: A numpy nd-array of shape (m,n) containing the second moment in the x direction twice
    -   y2: A numpy nd-array of shape (m,n) containing the second moment in the y direction twice
    -   xy2: (optional): numpy nd-array of dim (m,n) containing the second moment in the x then the y direction
    -   alpha: empirical constant in Corner Resposne equaiton (set this to 0.05 for unit testing)

    Returns:
    -   R: Corner response score for each pgrad_xel
    """

    R = None
    #############################################################################
    # TODO: YOUR CORNER RESPONSE CODE HERE                                       #
    #############################################################################

    det = (x2 * y2) - (xy2 ** 2)
    trace = x2 + y2
    R = det - alpha * (trace ** 2)

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################

    return R

def non_max_suppression(R, neighborhood_size = 7):
    """
    Implement non maxima suppression.
    Take a matrgrad_x and return a matrgrad_x of the same size but only the max values in a neighborhood that are not zero.
    We also do not want very small local maxima so remove all values that are below the median for the original R matrgrad_x.

    The input to this function is corner response matrgrad_x and the output is a filtered version of this
    matrgrad_x, where some of the responses have been set to 0.

    Helpful functions: scipy.ndimage.filters.maximum_filter

    Args:
    -   R: numpy nd-array of shape (m, n)
    -   neighborhood_size: int, the size of neighborhood to find local maxima (set this to 7 for unit testing)

    Returns:
    -   R_local_pts: numpy nd-array of shape (m, n) where only local maxima are non-zero
    """

    R_local_pts = None

    #############################################################################
    # TODO: YOUR NON MAX SUPPRESSION CODE HERE                                  #
    #############################################################################

    m = maximum_filter(R, size=neighborhood_size)
    R_local_pts = np.where((R == m) & (R > np.median(R)), R, 0)

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################

    return R_local_pts


def get_interest_points(image, n_pts = 1500):
    """
    Using your helper functions above, implement the Harris corner detector
    (See Szeliski 4.1.1). You will calculate the image gradients and second moments,
    use these to determine pgrad_xels with high corner response, and filter them via
    non maximum suppression and removing border values. You should return the
    top n_pts based on confidence score.

    Helpful functions:
        get_gradients, second_moments, corner_response, non_max_suppression, remove_border_vals

    Args:
    -   image: A numpy array of shape (m,n,c),
                image may be grayscale of color (your choice)
    -   n_pts: integer, number of interest points to obtain

    Returns:
    -   x: A numpy array of shape (n_pts) containing x-coordinates of interest points
    -   y: A numpy array of shape (n_pts) containing y-coordinates of interest points
    -   R_local_pts: A numpy array of shape (m,n) containing cornerness response scores after
            non-maxima suppression and before removal of border scores
    -   confidences (optional): numpy nd-array of dim (n_pts) containing the strength
            of each interest point
    """

    x, y, R_local_pts, confidences = None, None, None, None

    #############################################################################
    # TODO: YOUR HARRIS CORNER DETECTOR CODE HERE                               #
    #############################################################################

    if len(image.shape) == 3:
        image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    image = image.astype(np.float64) / 255.0

    grad_x, grad_y = get_gradients(image)
    x2, y2, xy2 = second_moments(grad_x, grad_y)
    R = corner_response(x2, y2, xy2, alpha=0.05)
    R_local_pts = non_max_suppression(R)

    y, x = np.nonzero(R_local_pts)
    confidences = R_local_pts[y, x]

    x, y, confidences = remove_border_vals(image, x, y, confidences, window_size=16)
    temp = np.argsort(confidences)[-n_pts:]
    x = x[temp]
    y = y[temp]
    confidences = confidences[temp]

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################

    return x,y, R_local_pts, confidences
