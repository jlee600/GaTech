import numpy as np
import matplotlib.pyplot as plt
import cv2
from skimage import color, io

class Prob4():
    def __init__(self):
        """Load input color image indoor.png and outdoor.png here as class variables."""

        self.indoor = None
        self.outdoor = None
        ###### START CODE HERE ######
        self.indoor = io.imread('indoor.png')
        self.outdoor = io.imread('outdoor.png')
        ###### END CODE HERE ######

    
    def prob_4_1(self):
        """Plot R,G,B channels separately and also their corresponding LAB space channels separately for both the indoor and outdoor image.
           Use the "gray" colormap options for plotting each channel."""
        
        ###### START CODE HERE ######
        imgs = [('Indoor', self.indoor), ('Outdoor', self.outdoor)]

        # RGB
        fig_rgb, axes_rgb = plt.subplots(2, 3, figsize=(12, 6))
        for row, (name, img) in enumerate(imgs):
            img = img[..., :3]
            R, G, B = img[..., 0], img[..., 1], img[..., 2]
            for col, (ch, label) in enumerate(zip([R, G, B], ['R', 'G', 'B'])):
                axes_rgb[row, col].imshow(ch, cmap="gray")
                axes_rgb[row, col].set_title(f'{name} {label}', fontsize=9)
                axes_rgb[row, col].axis("off")
        fig_rgb.suptitle("RGB Channels", fontsize=12)
        plt.show()

        # LAB
        fig_lab, axes_lab = plt.subplots(2, 3, figsize=(12, 6))
        for row, (name, img) in enumerate(imgs):
            img = img[..., :3]
            lab = color.rgb2lab(img.astype(np.float32) / 255.0)
            L, a, b = lab[..., 0], lab[..., 1], lab[..., 2]
            for col, (ch, label) in enumerate(zip([L, a, b], ['L', 'a', 'b'])):
                axes_lab[row, col].imshow(ch, cmap="gray")
                axes_lab[row, col].set_title(f'{name} {label}', fontsize=9)
                axes_lab[row, col].axis("off")
        fig_lab.suptitle("LAB Channels", fontsize=12)
        plt.show()
        ###### END CODE HERE ######
        return

    def prob_4_2(self):
        """
        Convert the loaded RGB image to HSV and return HSV matrix without using inbuilt functions. Return the HSV image as HSV. Plot the HSV image.
        Make sure to use a 3 channeled RGB image with floating point values lying between 0 - 1 for the conversion to HSV.

        Returns:
            HSV image (3 channeled image of size H x W x 3 with floating point values lying between 0 - 1 in each channel)
        """
        
        HSV = None
        ###### START CODE HERE ######
        rgb = io.imread('inputPS1Q4.jpg').astype(np.float32)
        rgb = rgb[..., :3] / 255.0

        Hh, Ww, _ = rgb.shape
        HSV = np.zeros((Hh, Ww, 3), dtype=np.float32)

        for i in range(Hh):
            for j in range(Ww):
                R = float(rgb[i, j, 0])
                G = float(rgb[i, j, 1])
                B = float(rgb[i, j, 2])

                V = max(R, G, B)
                m = min(R, G, B)
                C = V - m

                HSV[i, j, 2] = V
                if V == 0.0:
                    S = 0.0
                else:
                    S = C / V
                HSV[i, j, 1] = S

                if C == 0.0:
                    Hval = 0.0
                else:
                    if V == R:
                        Hprime = (G - B) / C
                    elif V == G:
                        Hprime = (B - R) / C + 2.0
                    else:  
                        Hprime = (R - G) / C + 4.0

                    if Hprime < 0.0:
                        Hval = Hprime / 6.0 + 1.0
                    else:
                        Hval = Hprime / 6.0
                HSV[i, j, 0] = Hval

        plt.imshow(HSV)
        plt.title("HSV Image")
        plt.axis("off")
        plt.show()
        ###### END CODE HERE ######
        return HSV

        
if __name__ == '__main__':
    
    p4 = Prob4()
    p4.prob_4_1()
    HSV = p4.prob_4_2()





