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
    namedWindow("paint", 1);
   
    blur(img_rgb,img_rgb,Size(3,3));

    Mat img_shifted;
    pyrMeanShiftFiltering(img_rgb,img_shifted, 15, 50);
    
    
    imwrite( "Paint_filter.jpg", img_shifted );
    
    imshow("original", img_rgb);
    imshow("paint", img_shifted);
   
    // Wait until user press some key
    waitKey(0);
    
    return 0;
}
