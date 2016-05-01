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

#define STEP 3
#define JITTER 3
#define RAIO 5

int top_slider = 10;
int top_slider_max = 200;

int step = 1;
int step_max = 10;
int jitter = 2;
int jitter_max = 10;
int raio = 2;
int raio_max = 10;

Mat cannyPoints;

Mat image, border;

void on_trackbar_canny(int, void*){

  int width, height;
  int x, y, gray;

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
    yrange[i]= yrange[i]*step+step/2;
  }

  cannyPoints = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*jitter)-jitter+1;
      y = j+rand()%(2*jitter)-jitter+1;

      gray = border.at<uchar>(x,y);
      
      //usando circulo, mas poderia ser outro elemento
      circle(cannyPoints,
             cv::Point(y,x),
             raio,
             CV_RGB(gray,gray,gray),
             -1,
             CV_AA);
    }
  } 

  imshow("cannyPoints", cannyPoints);
  
}

int main(int argc, char**argv){  

  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);  

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