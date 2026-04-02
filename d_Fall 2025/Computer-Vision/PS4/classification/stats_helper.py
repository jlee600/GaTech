import glob
import os
from typing import Tuple

import numpy as np
from PIL import Image
from sklearn.preprocessing import StandardScaler


def compute_mean_and_std(dir_name: str) -> Tuple[np.ndarray, np.array]:
    """
    Compute the mean and the standard deviation of the pixel values in the dataset.

    Note: convert the image in grayscale and then scale to [0,1] before computing
    mean and standard deviation

    Hints: use StandardScalar (check import statement)

    Args:
    -   dir_name: the path of the root dir
    Returns:
    -   mean: mean value of the dataset (np.array containing a scalar value)
    -   std: standard deviation of th dataset (np.array containing a scalar value)
    """

    mean = None
    std = None

    ############################################################################
    # Student code begin
    ############################################################################
    # raise NotImplementedError('forward function of SimpleNet not implemented')
    pth = []
    for ext in ['*.jpg', '*.jpeg', '*.png', '*.bmp']:
        pth.extend(glob.glob(os.path.join(dir_name, '**', ext), recursive=True))
    val = []
    
    for p in pth:
        img = Image.open(p).convert('L')
        img_np = np.array(img) / 255.0
        val.append(img_np.flatten())

    temp = np.concatenate(val)
    fit = temp.reshape(-1, 1)

    scaler = StandardScaler()
    scaler.fit(fit)
    
    mean = scaler.mean_
    std = scaler.scale_

    ############################################################################
    # Student code end
    ############################################################################
    return mean, std
