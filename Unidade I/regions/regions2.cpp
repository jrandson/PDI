#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
using namespace std;
using namespace cv;

int main(int argc, char** argv){
  Mat image;
  Size s;

  int d;

  cout << "Bem vindo ao programa do negativo\n"; 
  unsigned int x,y,r;
  cout << "Informe x, y e o raio: ";
  cin >> x;
  cin >> y;
  cin >> r;


  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  s = image.size();

  cout << "A imagem possui dimenção " << s.height << " por " << s.width << "\n";

  // encontra negatindo da região selecionada
  for(int i  = 0; i < s.width; i++){
    for(int j = 0; j < s.height; j++){
        d = sqrt(pow((x - i),2) + pow((y - j),2));
        if(d <= r){
           image.at<uchar>(i,j) = 255 - image.at<uchar>(i,j);
        }
    }
  }
		
	imshow("image", image);
  waitKey();


  return 0;
}
