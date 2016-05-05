#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;

  image= imread("bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu bolhas.png" << endl;
  int x1,y1,x2,y2;
  bool trava = true;
  typedef Size height, width;
  while(trava)
  {
  cout<<"Digite os pontos na seguinte ordem: x1 --> y1!"<<endl;
  
  cin >> x1;
  cin >> y1;

  }
  namedWindow("janela",WINDOW_AUTOSIZE);

  for(int i=200;i<210;i++){
    for(int j=10;j<200;j++){
      image.at<uchar>(i,j)=0;
    }
  }  
  imshow("janela", image);  
  waitKey();

  image= imread("bolhas.png",CV_LOAD_IMAGE_COLOR);

  val[0] = 0;   //B
  val[1] = 0;   //G
  val[2] = 255; //R
  
  for(int i=200;i<210;i++){
    for(int j=10;j<200;j++){
      image.at<Vec3b>(i,j)=val;
    }
  }

  imshow("janela", image);  
  waitKey();
  return 0;
}

