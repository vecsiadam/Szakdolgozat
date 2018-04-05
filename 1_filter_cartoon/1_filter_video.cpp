#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <signal.h>
#include <stdlib.h>


using namespace std;
using namespace cv;

void cartoon(Mat img_rgb);

int main(int argc, char** argv)
{

    VideoCapture cap("tiger.mov"); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    
    //Create a window
    namedWindow("original", 1);
    namedWindow("cartoon", 1);
    //namedWindow("median", 1);
    //namedWindow("adaptivethreshold", 1);
    
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        
        resize(frame, frame, Size(frame.cols/2, frame.rows/2));

        cartoon(frame);

        // Wait until user press some key
        if(waitKey(30) >= 0) break;
    }
    
    return 0;
}

void cartoon(Mat img_rgb){
    // downsample image using Gaussian pyramid
    Mat img_color = img_rgb;
    Mat srcGray;
    Mat dst ;

    
    cvtColor(img_rgb, srcGray, CV_BGR2GRAY);
    
    // Remove the pixel noise with a good Median filter, before we start detecting edges.
    medianBlur(srcGray, srcGray, 7);
    
    Size size = img_color.size();
    Mat mask = Mat(size, CV_8U);
    Mat edges = Mat(size, CV_8U);
    
    // Generate a nice edge mask, similar to a pencil line drawing.
    Laplacian(srcGray, edges, CV_8U, 5);
    threshold(edges, mask, 80, 255, THRESH_BINARY_INV);
    
    
    memset((char*)dst.data, 0, dst.step * dst.rows);
    img_color.copyTo(dst, mask);
    
    
    imshow("cartoon1", dst);
    imshow("original",img_color );
    
    return ;
}
