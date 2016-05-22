#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

int top_slider = 10;
int top_slider_max = 200;

int step = 1;
int step_max = 10;
int jitter = 2;
int jitter_max = 30;
int raio = 2;
int raio_max = 10;

Mat cannyPoints;

Mat image, border;

void on_trackbar_canny(int, void*){

  int width, height;
  int x, y, gray;

  Vec3b val;

  vector<int> yrange;
  vector<int> xrange;

  width = image.size().width;
  height = image.size().height;

  Canny(image, border, top_slider, 3*top_slider);  

  if(step < 1) step = 1;
  if(jitter < 1) jitter = 1;

  // aqui esntra o pntilhismo

  xrange.resize(height/step);
  yrange.resize(width/step);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*step+step/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*step+step/2;
  }

<<<<<<< HEAD
image.copyTo(border);

 // cannyPoints = Mat(height, width, CV_8U, Scalar(255));
=======
  //cannyPoints = Mat(height, width, CV_8U, Scalar(255));
  cannyPoints = Mat(height,width, CV_8UC3, Scalar(255,255,255));
>>>>>>> 6539df4d02d857c7d9d2001d5f8fb0fa4b9a36d9

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      
      x = i+rand()%(2*jitter)-jitter+1;
      y = j+rand()%(2*jitter)-jitter+1;

<<<<<<< HEAD
      gray = border.at<uchar>(x,y);

      //usando circulo, mas poderia ser outro elemento
=======
      gray = image.at<uchar>(x,y);

      val[0] = (int)(image.at<Vec3b>(x,y)[2]);
      val[1] = (int)(image.at<Vec3b>(x,y)[1]);
      val[2] = (int)(image.at<Vec3b>(x,y)[0]);
 
>>>>>>> 6539df4d02d857c7d9d2001d5f8fb0fa4b9a36d9
      circle(cannyPoints,
             cv::Point(y,x),
             raio,
             CV_RGB(val[0],val[1],val[2]),
             -1,
             CV_AA);
    }
  }

  imshow("cannyPoints", cannyPoints);

}

int main(int argc, char**argv){

<<<<<<< HEAD
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
=======
  image = imread(argv[1],CV_LOAD_IMAGE_COLOR);  
  //image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
>>>>>>> 6539df4d02d857c7d9d2001d5f8fb0fa4b9a36d9

  //sprintf( TrackbarName, "Threshold inferior", top_slider_max );

  namedWindow("cannyPoints",1);

  cvMoveWindow( "cannyPoints", image.cols, 0 );

  createTrackbar("Step", "cannyPoints",
                &top_slider,
                top_slider_max,
                on_trackbar_canny);

  createTrackbar("Threshold inferior", "cannyPoints",
                &step,
                step_max,
                on_trackbar_canny);

  createTrackbar("Jitter", "cannyPoints",
                &jitter,
                jitter_max,
                on_trackbar_canny);

  createTrackbar("Raio", "cannyPoints",
                &raio,
                raio_max,
                on_trackbar_canny);

  on_trackbar_canny(top_slider, 0 );




  imwrite("cannyPoints.jpg", cannyPoints);
  waitKey();
  return 0;
}