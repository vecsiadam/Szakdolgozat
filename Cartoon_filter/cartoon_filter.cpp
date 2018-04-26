//Cartoon filter
#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <ctime>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    //Image loading
    int start_all=clock();
    int start_s=clock();
    Mat img = imread("image.jpg");
    Mat dst ;
    //if fail to read the image
    if ( img.empty() )
    {
        cout << "Error loading the image" << endl;
        return -1;
    }
    int stop_s=clock();
    cout << "Image loading time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    //Create a window
    start_s=clock();
    namedWindow("Original", 1);
    namedWindow("Cartoon-filter", 1);
    stop_s=clock();
    cout << "Create windows time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    //Median filtering
    start_s=clock();
    Mat srcGray;
    Mat median;
    int kernel_size = 7;
    cvtColor(img, srcGray, CV_BGR2GRAY);
    medianBlur(srcGray, median, kernel_size);
    stop_s=clock();
    cout << "Median filtering time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;

    //Laplacian edge detectation
    start_s=clock();
    Mat edges;
    int kernel_lap = 5;
    Laplacian(median, edges, CV_8U, kernel_lap);
    stop_s=clock();
    cout << "Laplacian edge detectation time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
   
    //Thresholding
    start_s=clock();
    Mat mask;
    double thresh = 90;
    double maxval = 250;
    threshold(edges, mask, thresh, maxval, THRESH_BINARY_INV);
    stop_s=clock();
    cout << "Thresholding time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    //Copy mask to the image
    start_s=clock();
    memset((char*)dst.data, 0, dst.step * dst.rows);
    img.copyTo(dst, mask);
    stop_s=clock();
    cout << "Copy mask to the image time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
   
    
    //Write and show images
    start_s=clock();
    imwrite( "Cartoon1_filter1.jpg", dst );
    imshow("Cartoon-filter", dst);
    imshow("Original",img );
    stop_s=clock();
    cout << "Write and show images time in ms: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    int stop_all=clock();
    cout << "Image processing time in ms: " << (stop_all-start_all)/double(CLOCKS_PER_SEC)*1000 << endl;
    
    // Wait until user press some key
    waitKey(0);
    
    return 0;
}
