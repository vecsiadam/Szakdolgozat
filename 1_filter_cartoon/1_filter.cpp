#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{

    Mat img = imread("image.jpg");
    Mat dst ;

    //if fail to read the image
    if ( img.empty() )
    {
        cout << "Error loading the image" << endl;
        return -1;
    }
    
    //Create a window
    namedWindow("original", 1);
    namedWindow("cartoon1", 1);


    Mat srcGray;
    cvtColor(img, srcGray, CV_BGR2GRAY);
    
    // Remove the pixel noise with a good Median filter, before we start detecting edges.
    medianBlur(srcGray, srcGray, 7);
    
    Size size = img.size();
    Mat mask = Mat(size, CV_8U);
    Mat edges = Mat(size, CV_8U);

    // Generate a nice edge mask, similar to a pencil line drawing.
    Laplacian(srcGray, edges, CV_8U, 5);
    threshold(edges, mask, 80, 255, THRESH_BINARY_INV);
    
    
    memset((char*)dst.data, 0, dst.step * dst.rows);
    img.copyTo(dst, mask);

    
    imshow("cartoon1", dst);
    imshow("original",img );

    // Wait until user press some key
    waitKey(0);
    
    return 0;
}
