//Cartoon-style filter
#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <signal.h>
#include <stdlib.h>
#include <time.h> // to calculate time needed
#include <limits.h> // to get INT_MAX, to protect against overflow


using namespace std;
using namespace cv;

void cartoon_style_filter(Mat img_rgb);

int main(int argc, char** argv)
{

    VideoCapture cap("tiger.mov"); // loading the video
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    
    //Create a window
    namedWindow("Original", 1);
    namedWindow("Cartoon-style filter", 1);
    
    // fps counter declaration
    time_t start, end;
    int counter = 0;
    double sec;
    double fps;
    
    for(;;)
    {
        // fps counter
        if (counter == 0){
            time(&start);
        }
        
        Mat frame;
        cap >> frame; // get a new frame from camera
        
        resize(frame, frame, Size(frame.cols/2, frame.rows/2));

        cartoon_style_filter(frame);
        
        // fps counter
        time(&end);
        counter++;
        sec = difftime(end, start);
        fps = counter/sec;
        if (counter > 30)
            printf("%.2f fps\n", fps);
        // overflow protection
        if (counter == (INT_MAX - 1000))
            counter = 0;

        // Wait until user press some key
        if(waitKey(30) >= 0) break;
    }
 
    return 0;
}

void cartoon_style_filter(Mat img_rgb){
    //Downsample image using Gaussian pyramid
    Mat img = img_rgb;
    Mat img_res;
    for(int i=0; i < 2;i++){
        pyrDown( img, img);
    }
    
    //Bilateral filtering
    int d = 9;
    double sigmaColor = 9.0;
    double sigmaSpace = 7.0;
    for(int i=0; i < 7;i++){
        bilateralFilter ( img, img_res, d, sigmaColor, sigmaSpace );
    }
    
    // Upsample image to original size
    for(int k=0; k < 2;k++){
        pyrUp( img_res,img_res);
    }
    //Convert to grayscale and apply median blur
    Mat srcGray;
    Mat median;
    int kernel_size = 7;
    cvtColor(img_rgb, srcGray, CV_BGR2GRAY);
    medianBlur(srcGray, median, kernel_size);
    
    //Detect and enhance edges
    Mat img_edge;
    double maxValue = 225;
    int blockSize = 9;
    double C = 2;
    adaptiveThreshold(median, img_edge, maxValue, ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY, blockSize, C);
    
    
    //Convert back to color, bit-AND with color image
    cvtColor(img_edge, img_edge, CV_GRAY2RGB);
    Mat img_cartoon;
    bitwise_and(img_res,img_edge,img_cartoon);
    
    
    //Show videos
    imshow("Original", img_rgb);
    imshow("Cartoon-style filter", img_cartoon);
    
    
    return ;
}

