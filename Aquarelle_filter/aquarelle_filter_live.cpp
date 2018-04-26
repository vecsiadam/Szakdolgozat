// Aquarelle-style filter
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

void Aquarelle_style_filter(Mat img);

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    
    //Create a windows
    namedWindow("Original", 1);
    namedWindow("Aquarelle-style filter", 1);
    
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
        
        Aquarelle_style_filter(frame);
        
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
void Aquarelle_style_filter(Mat img){
    
    //Avg smooth
    Size ksize = Size(3, 3);
    Mat avg;
    blur(img,avg,ksize);
    
    //Mean shift segmentation
    Mat img_shifted;
    double sp = 10;
    double sr = 30;
    pyrMeanShiftFiltering(avg,img_shifted, sp, sr);
    
    
    //Write and show images
    imshow("Original", img);
    imshow("Aquarelle-style filter", img_shifted);

    return ;
}
