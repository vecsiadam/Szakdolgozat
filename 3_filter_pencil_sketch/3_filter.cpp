#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


using namespace std;
using namespace cv;


int main(int argc, char** argv)
{

    Mat img_rgb = imread("image.jpg");
    Mat canvas = imread("canvas.jpg");

    //if fail to read the image
    if ( img_rgb.empty() )
    {
        cout << "Error loading the image" << endl;
        return -1;
    }
    
    //Create a window
    namedWindow("Original", 1);
    namedWindow("Pencil sketch", 1);

    
    //Step 1: Convert the color image to grayscale
    Mat img_gray;
    cvtColor(img_rgb,img_gray, CV_BGR2GRAY);
    medianBlur(img_gray,img_gray, 3);

    
    //Step 2: Obtain a negative
   // Mat img_gray_inv=255-img_gray;
    
    //Step 3: Apply a Gaussian blur
    Mat img_blur;
    GaussianBlur(img_gray, img_blur,Size(21, 21),0, 0);
    Mat img_blend;
    
    divide(img_gray,img_blur,img_blend,245);
    
    normalize(img_blend, img_blend, 0, 255, CV_MINMAX);

    cvtColor(canvas,canvas, CV_BGR2GRAY);

    multiply(img_blend,canvas,img_blend, 1./256);
    

    //cvtColor(img_blend,img_blend, CV_GRAY2RGB);
    
     imwrite( "pencil_sketch.jpg", img_blend );
    // display
    //imshow("gray", img_gray);
    //imshow("gray inv", img_gray_inv);
    
    imshow("Pencil sketch", img_blend);
    imshow("Original", img_rgb);


    // Wait until user press some key
    waitKey(0);
    
    return 0;
}
