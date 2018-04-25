// Aquarelle-style filter
#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <ctime>

using namespace std;
using namespace cv;



int main(int argc, char** argv)
{
    //Loading image
    int start_all=clock();
    int start_s=clock();
    Mat img = imread("image.jpg");
    //if fail to read the image
    if ( img.empty() )
    {
        cout << "Error loading the image" << endl;
        return -1;
    }
    int stop_s=clock();
    cout << "Image loading time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    
    //Create a windows
    start_s=clock();
    namedWindow("Original", 1);
    namedWindow("Aquarelle-style filter", 1);
    stop_s=clock();
    cout << "Creating windows time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    //Avg smooth
    start_s=clock();
    Size ksize = Size(3, 3);
    Mat avg;
    blur(img,avg,ksize);
    stop_s=clock();
    cout << "Avarage blur time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    //Mean shift segmentation
    start_s=clock();
    Mat img_shifted;
    double sp = 15;
    double sr = 50;
    pyrMeanShiftFiltering(avg,img_shifted, sp, sr);
    stop_s=clock();
    cout << "Mean shift segmentation time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    
    //Write and show images
    start_s=clock();
    imwrite( "Paint_filter.jpg", img_shifted );
    imshow("Original", img);
    imshow("Aquarelle-style filter", img_shifted);
    stop_s=clock();
    cout << "Write and show image time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    
    int stop_all=clock();
    cout << "Image processing time in ms: " << (stop_all-start_all)/double(CLOCKS_PER_SEC)*1000 << endl;

    
    // Wait until user press some key
    waitKey(0);
    
    return 0;
}
