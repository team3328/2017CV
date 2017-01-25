#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;

int main(int, char**)
{
  // open video stream
  VideoCapture cap(0); // open the default camera
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
  if(!cap.isOpened())  // check if we succeeded
      return -1;

  Mat edges;
  Mat dest;

  // create window
  namedWindow("test",1);

  for(;;)
  {
    Mat frame;
    cap >> frame; // get a new frame from camera 640x480
    // std::cout << frame.size();
    cvtColor(frame, edges, CV_BGR2HSV);
    // GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);

    // make all green colors white and everything else black
    inRange(edges, Scalar(40, 50, 30), Scalar(80, 255, 255), dest);

    // creates kernels for erode and dilate
    Mat element = getStructuringElement(MORPH_RECT, Size(10,10), Point(0,0));
    Mat element_dilate = getStructuringElement(MORPH_RECT, Size(5,5), Point(0,0));

    erode(dest, dest, element);
    dilate(dest, dest, element_dilate);

    // find contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    imshow("test", dest);



    findContours(dest, contours, hierarchy, CV_RETR_EXTERNAL,
      CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
    {
      approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
      boundRect[i] = boundingRect( Mat(contours_poly[i]) );
      // minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }

    // for( int i = 0; i < boundRect.size(); i++ )
    // {
    //
    // }

    // Mat ++drawing = Mat::zeros( dest.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
      Scalar color = Scalar(255, 0, 0);
      // drawContours( dest, contours, i, color, 2, 8, hierarchy, 0, Point() );
      rectangle( frame, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
    }


    // show original video stream and edited video stream
    // imshow("edges", drawing);
    imshow("color", frame);
    if(waitKey(30) >= 0) break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
