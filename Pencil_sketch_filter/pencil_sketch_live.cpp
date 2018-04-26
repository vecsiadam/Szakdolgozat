//Pencil sketch filter
#include "opencv2/opencv.hpp"
#include <signal.h>
#include "opencv2/photo.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <stdlib.h>


using namespace std;
using namespace cv;
void pencil_sketch_filter(Mat img, Mat can);

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    //Create a window
    namedWindow("Original", 1);
    namedWindow("Pencile sketch", 1);
    Mat canvas = imread("canvas_video.jpg",0); //open canvas
    
    // fps counter declaration
    time_t start, end;
    int counter = 0;
    double sec;
    double fps;
    
    for(;;)
    {
        // Fps counter
        if (counter == 0){
            time(&start);
        }
        
        Mat frame;
        cap >> frame; // get a new frame from camera
        
        resize(frame, frame, Size(frame.cols/2, frame.rows/2));
        
        pencil_sketch_filter(frame,canvas);
        
        // Fps counter
        time(&end);
        counter++;
        sec = difftime(end, start);
        fps = counter/sec;
        if (counter > 30)
            printf("%.2f fps\n", fps);
        // Overflow protection
        if (counter == (INT_MAX - 1000))
            counter = 0;
        
        // Wait until user press some key
        if(waitKey(30) >= 0) break;
    }
    
    return 0;
}

void pencil_sketch_filter(Mat img, Mat can){
    
    //Convert image rgb to graysclae and median filtering
    Mat srcGray;
    Mat median;
    int kernel_size = 3;
    cvtColor(img, srcGray, CV_BGR2GRAY);
    medianBlur(srcGray, median, kernel_size);
    
    //Gaussian blur
    Mat gauss;
    Size ksize = Size(21, 21);
    double sigmaX = 0;
    double sigmaY = 0;
    GaussianBlur(median, gauss, ksize, sigmaX, sigmaY);
    
    //Divide median and Gaussian filter
    Mat img_blend;
    double scale_d = 245;
    divide(median, gauss, img_blend,scale_d);
    
    //Contrast strech
    double alpha=0;
    double beta =255;
    normalize(img_blend, img_blend, alpha, beta, CV_MINMAX);
    
    //Multiply canvas and image
    double scale_m = 1.0/256;
    multiply(img_blend,can,img_blend, scale_m);
    
    //Show live video
    imshow("Original", img);
    imshow("Pencile sketch", img_blend);
    return ;
}

