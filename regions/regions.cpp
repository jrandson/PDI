#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
using namespace std;
using namespace cv;

int main(int argc, char** argv){
  Mat image;
  Size s;

  cout << "Bem vindo ao programa do negativo\n"; 
  unsigned int p1x,p1y, p2x, p2y;
  cout << "Informe um ponto: ";
  cin >> p1x;
  cin >> p1y;
  cout << "informe outro ponto: ";
  cin >> p2x;
  cin >> p2y;
  cout << "p1: x =" << p1x << "; y =" << p2y << "\n";
  cout << "p2: x =" << p2x << "; y =" << p2y << "\n";


  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  s = image.size();

  cout << "A imagem possui dimenção " << s.height << " por " << s.width << "\n";

  // encontra negatindo da região selecionada
  for(int i  = p1x; i < p2x; i++){
  	for(int j = p1y; j < p2y; j++){
		image.at<uchar>(i,j) = 256 - image.at<uchar>(i,j);		
	}
  }
  imshow("image", image);
  waitKey();


  return 0;

  
}

void negativo(int x, int y, int r){
	/*float d;
	d = sqrt(pow((px - x),2) + pow((py - y),2)));
	if( d < r){
		image.at<uchar>(x,y) = 256 - image.at<uchar>(x,y);
	}*/
}