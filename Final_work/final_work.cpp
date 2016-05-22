#include <iostream>
#include <opencv2/opencv.hpp>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;
int main(int argc, char** argv){
  Mat image, new_image, sample_image,labels,centers;

  Vec3b val;
  int width, height, r_number_x,r_number_y ;
  srand(time(NULL));
  //Passo 1  --> Carregar imagem colorida
  image = imread(argv[1],CV_LOAD_IMAGE_COLOR);
  image.copyTo(sample_image);
  cout<<sample_image.at<Vec3b>(300,300);
  Mat m_sampled(200, 200, CV_8UC3, Scalar(0, 0, 0));
  //Perguntar ao professor como converter essa imagem
  m_sampled.convertTo(m_sampled,CV_32FC3);
  //imshow("image", m_sampled);
  waitKey();
  //imshow("image", image);
  width=image.size().width;
  height=image.size().height;
      cout<<"Largura: "<< width<<endl;
    cout<<"Altura: " << height<<endl;
  val[0] = 0;   //B
  val[1] = 0;   //G
  val[2] = 0; //R
cout<<"aqui";
  for(int i=0;i<width;i++){
    for(int j=0;j<height;j++){
      sample_image.at<Vec3b>(i,j)=val;
    }
  }

  sample_image.at<Vec3b>(300,300)=image.at<Vec3b>(300,300);
  for(int i=0;i<200;i++){
    for(int j=0;j<200;j++){
        r_number_x = rand() % width;
        r_number_y = rand() % height;
        if(sample_image.at<Vec3b>(r_number_x,r_number_y) == val){
            sample_image.at<Vec3b>(r_number_x,r_number_y) = image.at<Vec3b>(r_number_x,r_number_y);
            m_sampled.at<Vec3b>(i,j) = sample_image.at<Vec3b>(r_number_x,r_number_y);
            }
            else
                j--;
         }
  }

kmeans(m_sampled, 64, labels,
            TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),
               3, KMEANS_PP_CENTERS, centers);

  //imshow("image_sample",m_sampled);
  waitKey();

  return 0;
}
