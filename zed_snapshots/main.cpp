#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

// int main(int, char**) {
//     cv::VideoCapture vcap;
//     cv::Mat image;

//     // This works on a D-Link CDS-932L
//     // const std::string videoStreamAddress = "http://<username:password>@<ip_address>/video.cgi?.mjpg";
//     // const std::string videoStreamAddress = "http://admin:fred2017@192.168.1.2/video.cgi?.mjpg";
//     // const std::string videoStreamAddress = "http://admin:fred2017@129.49.32.159/video.cgi?.mjpg";
//     // const std::string videoStreamAddress = "http://admin:fred2017@129.49.32.159/video.cgi?.mjpg";
//     const std::string videoStreamAddress = "http://admin:fred2017@192.168.1.2/video.cgi?.mjpg";
//     // const std::string videoStreamAddress = "http://186.116.132.10:83/";

//     //open the video stream and make sure it's opened
//     if(!vcap.open(videoStreamAddress)) {
//         std::cout << "Error opening video stream or file" << std::endl;
//         return -1;
//     }

//     while(true) {
//         if(!vcap.read(image)) {
//             std::cout << "No frame" << std::endl;
//             cv::waitKey();
//         }
//         cv::imshow("Output Window", image);
//         // imwrite( "image7.jpg", image );
//         cv::waitKey(1);
        
//     }

//     // for(int i = 0; i < 10;i++) {
//     //     if(!vcap.read(image)) {
//     //         std::cout << "No frame" << std::endl;
//     //         cv::waitKey();
//     //     }
//     //     cv::imshow("Output Window", image);
//     //     // imwrite( "capture.jpg", image );
//     //     // if(cv::waitKey(50000) >= 0) break;
//     // }   
// }





#include "opencv2/opencv.hpp"
using namespace cv;

int countCameras()
{
   cv::VideoCapture temp_camera;
   int maxTested = 10;
   for (int i = 1; i < maxTested; i++){
     cv::VideoCapture temp_camera(i);
     bool res = (!temp_camera.isOpened());
     temp_camera.release();
     if (res)
     {
       return i;
     }
   }
   return maxTested;
}

int main(int argc, char** argv)
{
    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.

    // for(int i = 0; i < 10; i++) {
    //     if(cap.open(i))
    //         std::cout << "hi " << i;
    // }
    // printf("hi %d\n", countCameras());

    printf("Hello world\n");
    printf("arg 0: arg1: \n");
    printf("Saving: %s\n", argv[1]);

    //ls -al /dev/video*
    if(!cap.open("/dev/video1"))
        return 0;

    for(;;) {
      Mat frame;
      cap >> frame;
      if( frame.empty() ) break; // end of video stream
      imshow("IDLE Press escape to start:)", frame);
      if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC 
    }
    int counter = 1;
    mkdir(argv[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    for(;;)
    {
          Mat frame;
          cap >> frame;
          if( frame.empty() ) break; // end of video stream
          imshow("Capturing", frame);
          //imwrite( "zed1.jpg", frame );
          char filename[50];
          sprintf(filename, "%s/%s_%d.jpg", argv[1], argv[1], counter);

          imwrite(filename, frame );
          counter++;
          if(counter == 300) break;
          if( waitKey(10) == 27 ) break; // stop capturing by pressing ESC 
    }
    // the camera will be closed automatically upon exit
    // cap.close();
    return 0;
}


// #include <stdio.h>
// #include <opencv2/opencv.hpp>

// using namespace cv;

// int main(int argc, char** argv )
// {
//     if ( argc != 2 )
//     {
//         printf("usage: DisplayImage.out <Image_Path>\n");
//         return -1;
//     }

//     Mat image, grayimage;
//     image = imread( argv[1], 1 );

//     if ( !image.data )
//     {
//         printf("No image data \n");
//         return -1;
//     }

//     cvtColor(image, grayimage, cv::COLOR_RGB2GRAY);
//     namedWindow("Display Image", WINDOW_AUTOSIZE );
//     imshow("Display Image", grayimage);

//     waitKey(0);

//     return 0;
// }

