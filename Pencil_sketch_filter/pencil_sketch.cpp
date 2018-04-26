//Pencil sketch filter
#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <ctime>


using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    //Loading images
    int start_all=clock();
    int start_s=clock();
    Mat img = imread("image.jpg");
    Mat canvas = imread("canvas.jpg");

    //if fail to read the image
    if ( img.empty() )
    {
        cout << "Error loading the image" << endl;
        return -1;
    }
    int stop_s=clock();
    cout << "Image loading time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    
    //Create a window
    start_s=clock();
    namedWindow("Original", 1);
    namedWindow("Pencil sketch image", 1);
    stop_s=clock();
    cout << "Creating a window time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;

    
    //Convert image rgb to graysclae and median filtering
    start_s=clock();
    Mat srcGray;
    Mat median;
    int kernel_size = 3;
    cvtColor(img, srcGray, CV_BGR2GRAY);
    medianBlur(srcGray, median, kernel_size);
    stop_s=clock();
    cout << "Convert rgb iamge to gray and median filter time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    
    //Gaussian blur
    start_s=clock();
    Mat gauss;
    Size ksize = Size(21, 21);
    double sigmaX = 0;
    double sigmaY = 0;
    GaussianBlur(median, gauss, ksize, sigmaX, sigmaY);
    stop_s=clock();
    cout << "Gaussian filter time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;

    
    //Divide median and Gaussian filter
    start_s=clock();
    Mat img_blend;
    double scale_d = 245;
    divide(median, gauss, img_blend,scale_d);
    stop_s=clock();
    cout << "Gaussian filter and median filter divide time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    
    //Contrast strech
    start_s=clock();
    double alpha=0;
    double beta =255;
    normalize(img_blend, img_blend, alpha, beta, CV_MINMAX);
    stop_s=clock();
    cout << "Contrast strech time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    
    //Multiply canvas and image
    start_s=clock();
    double scale_m = 1.0/256;
    cvtColor(canvas, canvas, CV_BGR2GRAY);
    multiply(img_blend,canvas,img_blend, scale_m);
    stop_s=clock();
    cout << "Multiply the canvas and the smooth image time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    //Write and show images
    start_s=clock();
    imwrite( "pencil_sketch.jpg", img_blend );
    imshow("Pencil sketch filter", img_blend);
    imshow("Original", img);
    stop_s=clock();
    cout << "Write and show images time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    int stop_all=clock();
    cout << "Image processing time in ms: " << (stop_all-start_all)/double(CLOCKS_PER_SEC)*1000 << endl;

    // Wait until user press some key
    waitKey(0);
    
    return 0;
}
