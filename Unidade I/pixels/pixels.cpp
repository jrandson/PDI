#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

// Para compilar use make pixels

int main(int argc, char** argv){
  Mat image;

  Vec3b intensity;

  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu imagem" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  for(int i=200;i<210;i++){
    for(int j=10;j<200;j++){
      image.at<uchar>(i,j)=0;
    }
  }
  
  imshow("janela", image);  
  waitKey();

  image= imread("bolhas.png",CV_LOAD_IMAGE_COLOR);

  intensity.val[0] = 100;   //B
  intensity.val[1] = 94;   //G
  intensity.val[2] = 180; //R
  
  for(int i=200;i<210;i++){
    for(int j=10;j<200;j++){
      image.at<Vec3b>(i,j)= intensity;
    }
  }

  imshow("janela", image);  
  waitKey();
  return 0;
}