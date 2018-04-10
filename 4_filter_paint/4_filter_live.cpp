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
    
    blur(img_rgb,img_rgb,Size(3,3));
    
    Mat img_shifted;
    pyrMeanShiftFiltering(img_rgb,img_shifted, 10, 30);
    
    
    imshow("original", img_rgb);
    imshow("paint", img_shifted);

    
    
    
    return ;
}
