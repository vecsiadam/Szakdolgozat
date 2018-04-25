//Cartoon filter
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
void cartoon(Mat img_rgb);

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    //Create a window
    namedWindow("Original", 1);
    namedWindow("Cartoon filter",1);
    
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
        
        cartoon(frame);
        
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
void cartoon(Mat img){

    Mat dst ;
    
    //Median filtering
    Mat srcGray;
    Mat median;
    int kernel_size = 7;
    cvtColor(img, srcGray, CV_BGR2GRAY);
    medianBlur(srcGray, median, kernel_size);
    
    //Laplacian edge detectation
    Mat edges;
    int kernel_lap = 5;
    Laplacian(median, edges, CV_8U, kernel_lap);
    
    
    //Thresholding
    Mat mask;
    double thresh = 90;
    double maxval = 250;
    threshold(edges, mask, thresh, maxval, THRESH_BINARY_INV);
    
    
    //Copy mask to the image
    memset((char*)dst.data, 0, dst.step * dst.rows);
    img.copyTo(dst, mask);
    
    //Show live video
    imshow("Cartoon-filter", dst);
    imshow("Original",img );
    
    return ;
}
