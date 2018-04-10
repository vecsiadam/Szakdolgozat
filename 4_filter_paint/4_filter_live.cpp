#include "opencv2/opencv.hpp"

#include <signal.h>
#include "opencv2/photo.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <stdlib.h>

using namespace cv;
void paint(Mat img_rgb);

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    
    namedWindow("Paint-style filter",1);
    
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        
        resize(frame, frame, Size(frame.cols/2, frame.rows/2));
        
        paint(frame);
        
        // Wait until user press some key
        if(waitKey(30) >= 0) break;
    }
    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
void paint(Mat img_color){
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
