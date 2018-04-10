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

void cartoon(Mat img_color);

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

void cartoon(Mat img_color){

    Mat img_rgb = img_color;
    
    Mat img_gray;
    Mat img_edges;
    
    Mat kernel;
    Point anchor;
    double delta;
    int ddepth;
    int kernel_size;
    
    anchor = Point( -1, -1 );
    delta = 0;
    ddepth = -1;
    
    cvtColor(img_rgb,img_gray, CV_BGR2GRAY);
    blur(img_gray,img_edges,Size(15,15));
    Canny(img_edges,img_edges, 50, 150,3);
    
    int ind = 0;
    kernel_size = 3 + 2*( ind%5 );
    kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    
    filter2D(img_edges,img_edges, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
    threshold(img_edges,img_edges, 50, 255, 0);
    
    cvtColor(img_edges,img_edges, CV_GRAY2BGR);
    
    Mat img_shifted;
    pyrMeanShiftFiltering(img_rgb,img_shifted, 5, 20);
    
    Mat img_comic;
    subtract(img_rgb, img_edges,img_comic);
    
    
    imshow("original", img_rgb);
    imshow("paint", img_shifted);
    
    
    return ;
}
