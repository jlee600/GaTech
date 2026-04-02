import numpy as np
import matplotlib.pyplot as plt
import cv2
from skimage import io


class Prob3():
    def __init__(self):
        """Load input color image inputPS1Q3.jpg here and assign it as a class variable"""
        self.img = None
        ###### START CODE HERE ######
        self.img = io.imread('inputPS1Q3.jpg')
        ###### END CODE HERE ######
        
    def prob_3_1(self):
        """
        Swap red and green color channels here. Plot and return swapImg
        Returns:
            swapImg: RGB image with R and G channels swapped (3 channel image of size H x W x 3 with integer values lying in 0 - 255)
        """

        swapImg = None
        ###### START CODE HERE ######
        swapImg = self.img.copy()        
        swapped = np.stack([swapImg[:, :, 1], swapImg[:, :, 0], swapImg[:, :, 2]], axis=2)
        plt.figure()
        plt.imshow(swapImg)
        plt.show()
        ###### END CODE HERE ######
        return swapped


    def rgb2gray(self, rgb):
        """
        Converts and RGB image a grayscale image. Input is the RGB image (rgb) and you must return the grayscale image as gray.
        Returns:
            gray: grayscale image (single channel image of size H x W)
        """
        gray = None
        ###### START CODE HERE ######
        rgb_var = rgb.astype(float)
        gray = 0.2989 * rgb_var[:, :, 0] + 0.5870 * rgb_var[:, :, 1] + 0.114 * rgb_var[:, :, 2]
        gray = np.clip(gray, 0, 255).astype(np.uint8)
        ###### END CODE HERE ######
        return gray

    
    def prob_3_2(self):
        """
        This function should call your rgb2gray function to convert the input image to grayscale. Plot and return grayImg.
        Returns:
            grayImg: grayscale image (single channel image of size H x W with integer values lying between 0 - 255)
        """
        grayImg = None
        ###### START CODE HERE ######
        grayImg = self.rgb2gray(self.img)
        plt.figure()
        plt.imshow(grayImg, cmap='gray')
        plt.show()
        ###### END CODE HERE ######
        return grayImg
    
    def prob_3_3(self):
        """
        Convert the grayscale image to its negative. Plot and return negativeImg.
        
        Returns:
            negativeImg: negative image (single channel image of size H x W with integer values lying between 0 - 255)
        """
        negativeImg = None
        ###### START CODE HERE ######
        gray = self.rgb2gray(self.img)
        negativeImg = 255 - gray
        plt.figure()
        plt.imshow(negativeImg, cmap='gray')
        plt.show()
        ###### END CODE HERE ######
        return negativeImg
    
    def prob_3_4(self):
        """
        Create mirror image of grayscale image here. Plot and return mirrorImg.
        
        Returns:
            mirrorImg: mirror image (single channel image of size H x W with integer values lying between 0 - 255)
        """
        mirrorImg = None
        ###### START CODE HERE ######
        gray = self.rgb2gray(self.img)
        mirrorImg = np.fliplr(gray)
        plt.figure()
        plt.imshow(mirrorImg, cmap='gray')
        plt.show()
        ###### END CODE HERE ######
        return mirrorImg
    
    def prob_3_5(self):
        """
        Average grayscale image with mirror image here. Plot and return avgImg.
        
        Returns:
            avgImg: average of grayscale and mirror image (single channel image of size H x W with integer values lying between 0 - 255)
        """
        avgImg = None
        ###### START CODE HERE ######
        gray = self.rgb2gray(self.img).astype(np.float32)
        mirrorImg = np.fliplr(gray).astype(np.float32)
        avgImg = ((gray + mirrorImg) / 2).astype(np.uint8)
        plt.figure()
        plt.imshow(avgImg, cmap='gray')
        plt.show()
        ###### END CODE HERE ######
        return avgImg
    
    def prob_3_6(self):
        """
        Create noise matrix with the same size as the grayscale image. Add the noise to the grayscale image, and clip to ensure that max value is 255. 
        Plot this noisy image, and return the noisy image and the noise matrix.
        
        Returns:
            noisyImg: grayscale image after adding noise (single channel image of size H x W with integer values lying between 0 - 255)
            noise: random noise matrix of size H x W
        """
        noisyImg, noise = [None]*2
        ###### START CODE HERE ######
        gray = self.rgb2gray(self.img).astype(np.float32)
        noise = np.random.randint(0, 256, size=gray.shape).astype(np.float32)    
        noisyImg = gray + noise
        noisyImg = np.clip(noisyImg, 0, 255).astype(np.uint8)
        plt.figure()
        plt.imshow(noisyImg, cmap='gray')
        plt.show()
        ###### END CODE HERE ######
        return noisyImg, noise
        
        
if __name__ == '__main__': 
    
    p3 = Prob3()

    swapImg = p3.prob_3_1()
    grayImg = p3.prob_3_2()
    negativeImg = p3.prob_3_3()
    mirrorImg = p3.prob_3_4()
    avgImg = p3.prob_3_5()
    noisyImg,_ = p3.prob_3_6()