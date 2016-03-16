#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image, c_image;
  Vec3b val, aux;
  int largura, altura;
  int x1=0,y1=0,x2,y2;
  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu bolhas.png" << endl;
  largura = image.size().width;
  altura  = image.size().height; //clone;copyto;
  //c_image = image.clone();
  int id1,id2,id3,id4;
  x2 = altura/2;
  y2 = largura/2;

  //Rect(x1,y1,x2,y2);
  image.copyTo(c_image);
  namedWindow("janela",WINDOW_AUTOSIZE);
  imshow("janela", image);
  cout<<"Digite uma tecla para embaralhar a imagem"<<endl;
  waitKey();
  for(int i=x1;i<x2;i++){
    for(int j=y1;j<y2;j++){
      c_image.at<uchar>(i,j) = image.at<uchar>(i+x2,j+y2);
    }
  }
  //1
  for(int i=x1;i<x2;i++){
    for(int j=y2;j<largura;j++){
      c_image.at<uchar>(i,j) = image.at<uchar>(i+x2,j-y2);
    }
  }
  //id2 = 2;
  for(int i=x2;i<altura;i++){
    for(int j=y1;j<y2;j++){
      c_image.at<uchar>(i,j) = image.at<uchar>(i-x2,j+y2);
    }
  }
  //id3 = 3;
  for(int i=x2;i<altura;i++){
    for(int j=y2;j<largura;j++){
      c_image.at<uchar>(i,j) = image.at<uchar>(i-x2,j-y2);
    }
  }
  //id4 = 4;

  imshow("janela1", c_image);
  waitKey();
  return 0;
}

