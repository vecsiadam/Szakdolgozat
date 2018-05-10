/**
 * Cartoon-style filter
 */

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <ctime>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // Image loading
    int start_all = clock();
    int start_s = clock();
    Mat img_rgb = imread("image.jpg");

    // Check image reading
    if (img_rgb.empty())
    {
        cout << "Error loading the image" << endl;
        return -1;
    }

    int stop_s = clock();
    cout << "Image loading time ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    // Create a window
    start_s = clock();
    namedWindow("Original", 1);
    namedWindow("Cartoon-Style filter", 1);
    stop_s = clock();
    cout << "Creating windows time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    // Downsample image using Gaussian pyramid
    start_s = clock();
    Mat img = img_rgb;
    Mat img_res;
    for(int i = 0; i < 2; i++){
        pyrDown(img, img);
    }
    stop_s = clock();
    cout << "Gaussian pyramid down in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    // Bilateral filtering
    start_s = clock();
    int d = 9;
    double sigmaColor = 9.0;
    double sigmaSpace = 7.0;
    for(int i = 0; i < 7; i++){
        bilateralFilter(img, img_res, d, sigmaColor, sigmaSpace);
    }
    stop_s = clock();
    cout << "Bilateral filter time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;

    // Upsample image to original size
    start_s = clock();
    for(int k = 0; k < 2; k++){
        pyrUp(img_res, img_res);
    }
    stop_s = clock();
    cout << "Gaussian pyramid up time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;

    
    // Convert to grayscale and apply median blur
    start_s = clock();
    Mat srcGray;
    Mat median;
    int kernel_size = 7;
    cvtColor(img_rgb, srcGray, CV_BGR2GRAY);
    medianBlur(srcGray, median, kernel_size);
    stop_s = clock();
    cout << "Convert rgb img to gray and median filter  time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;

    // Detect and enhance edges
    start_s = clock();
    Mat img_edge;
    double maxValue = 225;
    int blockSize = 9;
    double C = 2;
    adaptiveThreshold(median, img_edge, maxValue, ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY, blockSize, C);
    stop_s = clock();
    cout << "Adaptive threshold time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    // Convert back to color, bit-AND with color image
    start_s = clock();
    cvtColor(img_edge, img_edge, CV_GRAY2RGB);
    Mat img_cartoon;
    bitwise_and(img_res,img_edge,img_cartoon);
    stop_s = clock();
    cout << "Convert back to color, bit-AND time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    // Write and show images 
    start_s = clock();
    imwrite("Cartoon_filter.jpg", img_cartoon );
    imshow("Original", img_rgb);
    imshow("Cartoon-Style filter", img_cartoon);
    stop_s = clock();
    cout << "Write and show images time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    int stop_all = clock();
    cout << "Image processing time in ms: " << (stop_all-start_all)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    // Wait until user press some key
    waitKey(0);
    
    return 0;
}
