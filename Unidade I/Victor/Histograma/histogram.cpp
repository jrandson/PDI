#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histR, histG, histB, histAux, histMotion;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgAux(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgMotion(histh, histw, CV_8UC3, Scalar(0,0,0));

  while(1){
    cap >> image;
    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImgR, Point(i, histh),
           Point(i, cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG, Point(i, histh),
           Point(i, cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB, Point(i, histh),
           Point(i, cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh   ,nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh ,nbins, histh)));
    imshow("image", image);

    cap >> image;
    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histAux, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[0], 1, 0, Mat(), histMotion, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histAux, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histMotion, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    for(int i=0;i<nbins;i++){
    histMotion.at<float>(i) = histAux.at<float>(i) - histR.at<float>(i);
    cout<<i<<"-->"<<histMotion.at<float>(i)<<endl;
    }

    float auxiliar=0,aux2=0;
    for(int i=0;i<nbins;i++)
    {
        auxiliar+=histR.at<float>(i);
        aux2 +=histAux.at<float>(i);
    }
    cout<<"Soma Red: "<<auxiliar<<endl;
    cout<<"Soma Aux: "<<aux2<<endl;
    cout<<"Diferença: "<<aux2 - auxiliar<<endl;
    //if((aux2) <)
    //sleep(1);
//    for(int i=0;i<nbins;i++)
  //  if(histMotion.at<float>(i) > 5000)
   // cout<<"ALARME ALARME ALARME"<<endl;
   // sleep(0.5);

    //histMotion  = histAux - histR;

    //waitKey();
    //sleep(1);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
