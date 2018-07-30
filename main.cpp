/*
 * Hough lines and circles transform - detecting lines and cirlces

 */
#include <iostream>
#include <opencv2/opencv.hpp>
 
using namespace std;
using namespace cv;

const int low_thres_max = 245;
int low_thres;

const int high_thres_max = 245;
int high_thres;

const int kernel_max = 2;
int kernel;

const int hough_thres_max = 245;
int hough_thres;

const int upper_thres_max = 245;
int upper_thres;

const int center_thres_max = 245;
int center_thres;

const int min_radius_max = 100;
int min_radius;

Mat src, src_gray, gauss_image, canny_image, hough_image;

void on_trackbar( int, void* )
{
 int kernel_size = 2* kernel +3;
 
 //GaussianBlur(src, src, Size(23,23), 1, 0, BORDER_DEFAULT );
 
 Canny(src, canny_image, low_thres+10, high_thres+10, kernel_size);
 
 imshow("src image", src);
 imshow("canny image", canny_image);

}

void on_trackbar2( int, void* )
{
 //int kernel_size = 2* kernel +3;
    //line hough transform
    cvtColor(canny_image, hough_image, CV_GRAY2BGR);
    
 #if 0
  vector<Vec2f> lines;
  HoughLines(canny_image, lines, 1, CV_PI/180, hough_thres, 0, 0 );

  for( size_t i = 0; i < lines.size(); i++ )
  {
     float rho = lines[i][0], theta = lines[i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( hough_image, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
  }
 #else
  vector<Vec4i> lines;
  HoughLinesP(canny_image, lines, 1, CV_PI/180, hough_thres+10, 50, 5 );
  for( size_t i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( hough_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  }
 #endif
 //hough_image=canny_image;
 //imshow("source", src);
 
 //circle hough transform
 cvtColor( src, src_gray, CV_BGR2GRAY );
 GaussianBlur( src_gray, gauss_image, Size(9, 9), 2, 2 );
 vector<Vec3f> circles;
 HoughCircles( gauss_image, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, upper_thres+10, center_thres+10, min_radius, 0 );
 
 for( size_t i = 0; i < circles.size(); i++ )
{
   Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
   int radius = cvRound(circles[i][2]);
   // circle center
   circle( hough_image, center, 3, Scalar(0,255,0), -1, 8, 0 );
   // circle outline
   circle( hough_image, center, radius, Scalar(0,0,255), 3, 8, 0 );
 }
 
 imshow("hough image", hough_image);
 //Canny(src, canny_image, low_thres, high_thres, kernel_size);
 //cvtColor(canny_image, canny_image, CV_GRAY2BGR);
 
 //imshow("src image", src);
 //imshow("canny image", canny_image);

}


int main()
{
    
    src=imread("oko2.jpg");
    src.convertTo(src, -1, 2, 0);   //higher contrast
    erode(src, src, Mat(), Point(-1,-1), 3, BORDER_CONSTANT, morphologyDefaultBorderValue() );
    
    
    
    namedWindow("src image", WINDOW_AUTOSIZE);
    namedWindow("canny image", WINDOW_AUTOSIZE);
    namedWindow("hough image", WINDOW_AUTOSIZE);
    
    char TrackbarName1[15]="low threshold";
    createTrackbar( TrackbarName1, "canny image", &low_thres, low_thres_max, on_trackbar );
    
    char TrackbarName2[15]="high threshold";
    createTrackbar( TrackbarName2, "canny image", &high_thres, high_thres_max, on_trackbar );
    
    char TrackbarName3[15]="kernel";
    createTrackbar( TrackbarName3, "canny image", &kernel, kernel_max, on_trackbar );
    
    char TrackbarName4[25]="hough lines threshold";
    createTrackbar( TrackbarName4, "hough image", &hough_thres, hough_thres_max, on_trackbar2 );
    
    char TrackbarName5[30]="hough circle upper threshold";
    createTrackbar( TrackbarName5, "hough image", &upper_thres, upper_thres_max, on_trackbar2 );
    
    char TrackbarName6[30]="hough circle center threshold";
    createTrackbar( TrackbarName6, "hough image", &center_thres, center_thres_max, on_trackbar2 );
    
    char TrackbarName7[30]="min radius";
    createTrackbar( TrackbarName7, "hough image", &min_radius, min_radius_max, on_trackbar2 );
    
    on_trackbar( low_thres, /*high_thres, kernel,*/ 0 );
    //waitKey(0);
    on_trackbar2( hough_thres, /*high_thres, kernel,*/ 0 );
    
    waitKey(0);
    /*
    while (true)
    {
        
        int end = waitKey(0);
        if (end=='q') break;
    }
    */
    
    return 0;
}