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
    blur(img_gray,img_edges,Size(3,3));
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
   
    // Wait until user press some key
    waitKey(0);
    
    return 0;
}

/*
def comic(img):
# do edge detection on a grayscale image
gray = cv2.cvtColor(img, cv2.COLOR_BGRA2GRAY)
edges = cv2.blur(gray, v) # this blur gets rid of some noise
edges = cv2.Canny(edges, 50, 150, apertureSize=3) # this is the edge detection

# the edges are a bit thin, this blur and threshold make them a bit fatter
kernel = numpy.ones((3,3), dtype=numpy.float) / 12.0
edges = cv2.filter2D(edges, 0, kernel)
edges = cv2.threshold(edges, 50, 255, 0)[1]

# and back to colour...
edges = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)

# this is the expensive operation, it blurs things but keeps track of
# colour boundaries or something, heck, just play with it
shifted = cv2.pyrMeanShiftFiltering(img, 5, 20)

# now compose with the edges, the edges are white so take them away
# to leave black
return cv2.subtract(shifted, edges)
*/
