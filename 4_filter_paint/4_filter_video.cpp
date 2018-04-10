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

void paint(Mat img_color);

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

        paint(frame);

        // Wait until user press some key
        if(waitKey(30) >= 0) break;
    }
    
   


    
    
    return 0;
}

void paint(Mat img_color){
    Mat img_rgb = img_color;
    
    blur(img_rgb,img_rgb,Size(3,3));    
    Mat img_shifted;
    pyrMeanShiftFiltering(img_rgb,img_shifted, 10, 30);
    
    
    imshow("original", img_rgb);
    imshow("paint", img_shifted);
    
    
    
    
    return ;
}
