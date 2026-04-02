import numpy as np
import cv2
from ps2_code.student_harris import get_gradients

def get_magnitudes_and_orientations(dx, dy):
    """
    This function will return the magnitudes and orientations of the
    gradients at each pixel location.
    
    Helpful functions: np.arctan2
    
    Args:
    -   dx: A numpy array of shape (m,n), representing x gradients in the image
    -   dy: A numpy array of shape (m,n), representing y gradients in the image

    Returns:
    -   magnitudes: A numpy array of shape (m,n), representing magnitudes of the
            gradients at each pixel location
    -   orientations: A numpy array of shape (m,n), representing angles of
            the gradients at each pixel location. angles should range from
            -PI to PI.

    """
    magnitudes = []#placeholder
    orientations = []#placeholder

    #############################################################################
    # TODO: YOUR CODE HERE                                                      #                                          #
    #############################################################################

    magnitudes = np.sqrt(dx**2 + dy**2)
    orientations = np.arctan2(dy, dx)

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################
    return magnitudes, orientations

def get_feat_vec(x,y,magnitudes, orientations,feature_width):
    """
    This function returns the feature vector for a specific interest point.

    To start with, you might want to use normalized patches as your
    local feature. This should be simple to code and works OK.

    However, to get full credit you will need to implement the more effective
    SIFT descriptor (See Szeliski 4.1.2 or the original publications at
    http://www.cs.ubc.ca/~lowe/keypoints/)

    Your implementation does not need to exactly match the SIFT reference.
    Here are the key properties your (baseline) descriptor should have:
    (1) a 4x4 grid of cells where each cell is a square of feature_width/4 * feature_width/4 pixels
        It is simply the terminology used in the feature literature to describe the spatial
        bins where gradient distributions will be described. The grid will extend
        feature_width/2 to the left of the "center", and feature_width/2 - 1 to the right
    (2) each cell should have a histogram of the local distribution of
        gradients in 8 orientations. Appending these histograms together will
        give you 4x4 x 8 = 128 dimensions. The bin centers for the histogram
        should be at -7pi/8,-5pi/8,...5pi/8,7pi/8. The histograms should be appended
        to the feature vector left to right then row by row (reading order).
    (3) Each feature should be normalized to unit length.
    (4) Each feature should be raised to a power less than one(use .9)

    For our tests, you do not need to perform the interpolation in which each gradient
    measurement contributes to multiple orientation bins in multiple cells
    As described in Szeliski, a single gradient measurement creates a
    weighted contribution to the 4 nearest cells and the 2 nearest
    orientation bins within each cell, for 8 total contributions. This type
    of interpolation probably will help, though, so feel free to try it.
    The autograder will only check for each gradient contributing to a single bin.

    Useful function: np.histogram

    Args:
    -   x: a float, the x-coordinate of the interest point
    -   y: A float, the y-coordinate of the interest point
    -   magnitudes: A numpy array of shape (m,n), representing image gradients
            at each pixel location
    -   orientations: A numpy array of shape (m,n), representing gradient
            orientations at each pixel location
    -   feature_width: integer representing the local feature width in pixels.
            You can assume that feature_width will be a multiple of 4 (i.e. every
                cell of your local SIFT-like feature will have an integer width
                and height). This is the initial window size we examine around
                each keypoint.

    Returns:
    -   fv: A numpy array of shape (feat_dim) representing a feature vector.
            "feat_dim" is the feature_dimensionality (e.g. 128 for standard SIFT).
            These are the computed features.

    """

    fv = []#placeholder
    #############################################################################
    # TODO: YOUR CODE HERE                                                      #                                          #
    #############################################################################

    half_width = feature_width // 2
    height, width = magnitudes.shape
    pv = []

    x = int(x) - half_width 
    y = int(y) - half_width
    x1 = x + feature_width
    y1 = y + feature_width

    if x < 0 or y < 0 or x1 > width or y1 > height:
        return np.zeros(128)
    
    patchM = magnitudes[y:y1, x:x1]
    patchO = orientations[y:y1, x:x1]

    tmep = feature_width // 4
    for i in range(4):
        for j in range(4):
            y2 = i * tmep
            x2 = j * tmep
            m = patchM[y2:y2+tmep, x2:x2+tmep]
            o = patchO[y2:y2+tmep, x2:x2+tmep]
            
            hist, null = np.histogram(o, bins=8, range=(-np.pi, np.pi), weights=m)
            pv.append(hist.astype(np.float64))
    fv = np.concatenate(pv).astype(np.float64)
    fv /= (np.linalg.norm(fv) + 1e-12)
    # fv /= (np.linalg.norm(fv) + 1e-10)
    fv = np.power(fv, 0.9, dtype=np.float64)
    # fv = fv / (np.linalg.norm(fv) + 1e-10)

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################
    return fv


def get_features(image, x, y, feature_width):
    """
    This function returns the SIFT features computed at each of the input points
    You should code the above helper functions first, and use them below.
    You should also use your implementation of image gradients from before.

    Useful function: get_gradients, get_magnitudes_and_orientations


    Args:
    -   image: A numpy array of shape (m,n), the image
    -   x: A numpy array of shape (k,), the x-coordinates of interest points
    -   y: A numpy array of shape (k,), the y-coordinates of interest points
    -   feature_width: integer representing the local feature width in pixels.
            You can assume that feature_width will be a multiple of 4 (i.e. every
                cell of your local SIFT-like feature will have an integer width
                and height). This is the initial window size we examine around
                each keypoint.

    Returns:
    -   fvs: A numpy array of shape (k, feat_dim) representing all feature vectors.
            "feat_dim" is the feature_dimensionality (e.g. 128 for standard SIFT).
            These are the computed features.
    """

    assert image.ndim == 2, 'Image must be grayscale'
    fvs = []
    #############################################################################
    # TODO: YOUR CODE HERE                                                      #                                          #
    #############################################################################

    dx, dy = get_gradients(image)
    magnitudes, orientations = get_magnitudes_and_orientations(dx, dy)
    
    out = []
    for i, j in zip(x, y):
        fv = get_feat_vec(i, j, magnitudes, orientations, feature_width)
        out.append(fv)
    
    fvs = np.vstack(out)

    #############################################################################
    #                             END OF YOUR CODE                              #
    #############################################################################
    return fvs
