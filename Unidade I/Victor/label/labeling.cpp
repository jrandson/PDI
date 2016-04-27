#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int main(int argc, char** argv){
  Mat image, mask,image1;
  int width, height;
  int nobjects;

  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;
  int obj_with_hole=0,obj_without_hole=0;
// Retirada de bordas
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
	if(image.at<uchar>(i,j) == 255 && i == 1 || i == height - 1 || j == 1 || j == width - 1){
		p.x=j;
		p.y=i;
		floodFill(image,p,0);
	   }
	}
  }
  //Mudando a cor do fundo da imagem
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
	if(image.at<uchar>(i,j) == 0 && i == 1 || i == height - 1 || j == 1 || j == width - 1){
		p.x=j;
		p.y=i;
		floodFill(image,p,20);
	   }
	}
  }
  //Contando imagens com buracos e sem buracos
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
	if(image.at<uchar>(i,j) == 0){
		p.x=j;
		p.y=i;
		floodFill(image,p,255);
		obj_with_hole++;
	   }
	}
  }

    image1 = image.clone();
    imshow("image", image);
    imwrite("labeling.png", image);
    waitKey();
// Verificando buracos
  imshow("image1",image1);
  waitKey();
// busca objetos com buracos presentes
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
	if(image.at<uchar>(i,j) == 255){
		// achou um objeto
		nobjects++;
		p.x=j;
		p.y=i;
		floodFill(image,p,nobjects);
	   }
	}
  }
  cout<<nobjects<<endl;
  cout<<"Objetos sem buracos: "<<nobjects-obj_with_hole<<endl;
  cout<<"Objetos com buracos: "<<obj_with_hole<<endl;
  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
