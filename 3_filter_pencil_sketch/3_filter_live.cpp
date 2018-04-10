#include "opencv2/opencv.hpp"

#include <signal.h>
#include "opencv2/photo.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <stdlib.h>

using namespace cv;
void cartoon(Mat img_rgb);

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    
    namedWindow("Original", 1);
    namedWindow("Pencile sketch", 1);
    Mat canvas = imread("canvas_video.jpg",0);
    
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        
        resize(frame, frame, Size(frame.cols/2, frame.rows/2));
        
        Mat img_gray;
        cvtColor(frame,img_gray, CV_BGR2GRAY);
        
        medianBlur(img_gray,img_gray, 3);
        
        Mat img_blur;
        GaussianBlur(img_gray, img_blur,Size(21, 21),0, 0);
        
        Mat img_blend;
        divide(img_gray,img_blur,img_blend,245);
        normalize(img_blend, img_blend, 0, 255, CV_MINMAX);
        
        
        multiply(img_blend,canvas,img_blend, 1./256);
        
        // cvtColor(img_blend,img_blend, CV_GRAY2RGB);
        
        imshow("Original", frame);
        imshow("Pencile sketch", img_blend);
        
        
        
        // Wait until user press some key
        if(waitKey(30) >= 0) break;
    }
}

