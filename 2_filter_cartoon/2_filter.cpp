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

    //if fail to read the image
    if ( img_rgb.empty() )
    {
        cout << "Error loading the image" << endl;
        return -1;
    }
    
    //Create a window
    namedWindow("original", 1);
    namedWindow("cartoon", 1);
    //namedWindow("median", 1);
    //namedWindow("adaptivethreshold", 1);

    
    
    // downsample image using Gaussian pyramid
    Mat img_color = img_rgb;
   
    for(int i=0; i < 2;i++){
    pyrDown( img_color,img_color);
    }
    
    // repeatedly apply small bilateral filter instead of
    // applying one large filter
     //  bilateralFilter ( img_color, img_color, 9, 9, 7 );
    
    
    // upsample image to original size
    for(int k=0; k < 2;k++){
        pyrUp( img_color,img_color);
    }
    
    // convert to grayscale and apply median blur
    Mat img_gray;
    Mat img_blur;
    cvtColor(img_rgb,img_gray, CV_BGR2GRAY);
    medianBlur(img_gray,img_blur, 7);
    
    // detect and enhance edges
    Mat img_edge;
    adaptiveThreshold(img_blur,img_edge, 225, ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,9,2); //original maxValue= 250

    // convert back to color, bit-AND with color image
    cvtColor(img_edge, img_edge, CV_GRAY2RGB);
    Mat img_cartoon;
    bitwise_and(img_color,img_edge,img_cartoon);
    
    // display
    imshow("original", img_rgb);
    imshow("cartoon", img_cartoon);
    //imshow("median", img_blur);
    //imshow("adaptivethreshold", img_edge);


    // Wait until user press some key
    waitKey(0);
    
    return 0;
}
