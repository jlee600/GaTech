import math
from typing import Tuple

import numpy as np

from ps3_code import fundamental_matrix, two_view_data
from ps3_code.least_squares_fundamental_matrix import solve_F


def calculate_num_ransac_iterations(prob_success: float, 
                                    sample_size: int, 
                                    ind_prob_correct: float) -> int:
    """
    Calculate the number of RANSAC iterations needed for a given guarantee of success.

    Args:
    -   prob_success: float representing the desired guarantee of success
    -   sample_size: int representing the number of samples included in each RANSAC iteration
    -   ind_prob_success: float representing the probability that each element in a sample is correct

    Returns:
    -   num_samples: int the number of RANSAC iterations needed

    """
    num_samples = None

    ##############################
    # TODO: Student code goes here

    prob = ind_prob_correct ** sample_size
    num = math.log(1 - prob_success)
    de = math.log(1 - prob)
    num_samples = int(math.ceil(num / de))

    # raise NotImplementedError
    ##############################

    return num_samples


def find_inliers(x_0s: np.ndarray, 
                 F: np.ndarray, 
                 x_1s: np.ndarray, 
                 threshold: float) -> np.ndarray:
    """ Find the inliers' indices for a given model.

    There are multiple methods you could use for calculating the error
    to determine your inliers vs outliers at each pass. However, we suggest
    using the magnitude of the line to point distance function we wrote for the
    optimization in part 2.

    Args:
    -   x_0s: A numpy array of shape (N, 3) representing the coordinates
                   of possibly matching points from the left image
    -   F: The proposed fundamental matrix of shape (3, 3)
    -   x_1s: A numpy array of shape (N, 3) representing the coordinates
                   of possibly matching points from the right image
    -   threshold: float representing the maximum error for a point correspondence to be
                    considered an inlier
    Each row in x_1s and x_0s is a proposed correspondence (e.g. row #42 of x_0s is a point that
    corresponds to row #42 of x_1s)

    Returns:
    -    inliers: 1D array of the indices of the inliers in x_0s and x_1s

    """
    inliers = None

    ##############################
    # TODO: Student code goes here

    out = []

    for i in range(x_0s.shape[0]):
        x_0 = x_0s[i]
        x_1 = x_1s[i]

        line_0 = np.dot(F, x_1)
        dist_0 = fundamental_matrix.point_line_distance(line_0, x_0)

        line_1 = np.dot(F.T, x_0)
        dist_1 = fundamental_matrix.point_line_distance(line_1, x_1)

        if abs(dist_0) < threshold and abs(dist_1) < threshold:
            out.append(i)

    inliers = np.array(out)
    # raise NotImplementedError
    ##############################

    return inliers


def ransac_fundamental_matrix(x_0s: int, 
                              x_1s: int) -> Tuple[
                                  np.ndarray, np.ndarray, np.ndarray]:
    """Find the fundamental matrix with RANSAC.

    Use RANSAC to find the best fundamental matrix by
    randomly sampling interest points. You will call your
    solve_F() from part 2 of this assignment
    and calculate_num_ransac_iterations().

    You will also need to define a new function (see above) for finding
    inliers after you have calculated F for a given sample.

    Tips:
        0. You will need to determine your P, k, and p values.
            What is an acceptable rate of success? How many points
            do you want to sample? What is your estimate of the correspondence
            accuracy in your dataset?
        1. A potentially useful function is numpy.random.choice for
            creating your random samples
        2. You will want to call your function for solving F with the random
            sample and then you will want to call your function for finding
            the inliers.
        3. You will also need to choose an error threshold to separate your
            inliers from your outliers. We suggest a threshold of 1.
        4. You can use the `preprocess_data` function in `two_view_data` to make
           x_0s, and x_1s homogeneous.

    Args:
    -   x_0s: A numpy array of shape (N, 2) representing the coordinates
                   of possibly matching points from the left image
    -   x_1s: A numpy array of shape (N, 2) representing the coordinates
                   of possibly matching points from the right image
    Each row is a proposed correspondence (e.g. row #42 of x_0s is a point that
    corresponds to row #42 of x_1s)

    Returns:
    -   best_F: A numpy array of shape (3, 3) representing the best fundamental
                matrix estimation
    -   inliers_x_0: A numpy array of shape (M, 2) representing the subset of
                   corresponding points from the left image that are inliers with
                   respect to best_F
    -   inliers_x_1: A numpy array of shape (M, 2) representing the subset of
                   corresponding points from the right image that are inliers with
                   respect to best_F

    """

    best_F = None
    inliers_x_0 = None
    inliers_x_1 = None

    ##############################
    # TODO: Student code goes here

    run = calculate_num_ransac_iterations(0.999, 9, 0.9)
    x0, x1 = two_view_data.preprocess_data(x_0s.copy(), x_1s.copy())

    thsh = 0.75
    best_i = None

    for i in range(run):
        indices = np.random.choice(x_0s.shape[0], 9, replace=False)
        sample_x0s = x_0s[indices]
        sample_x1s = x_1s[indices]

        f = solve_F(sample_x0s, sample_x1s)
        out = find_inliers(x0, f, x1, 1.0)
        num_inliers = len(out)
        
        if num_inliers > thsh:
            thsh = num_inliers
            best_i = out
    
    if best_i is not None and len(best_i) > 0:
        inliers_x_0 = x_0s[best_i]
        inliers_x_1 = x_1s[best_i]
        best_F = solve_F(inliers_x_0, inliers_x_1)

    else:
        best_F = np.eye(3)
        inliers_x_0 = np.array([])
        inliers_x_1 = np.array([])

    # raise NotImplementedError
    ##############################

    return best_F, inliers_x_0, inliers_x_1
