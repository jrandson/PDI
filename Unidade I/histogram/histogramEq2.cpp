#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;


int main(int argc, char** argv){
  Mat imageEq;
  int width, height;
  VideoCapture cap;
  vector<Mat> planesEq;
  Mat histReq, histGeq, histBeq;
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

  Mat histImgR_eq(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG_eq(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB_eq(histh, histw, CV_8UC3, Scalar(0,0,0));

  int aux = 0;
  while(1){
    cap >> imageEq;
    split (imageEq, planesEq);

    //------- equalização da imagem -------

    equalizeHist(planesEq[0],planesEq[0]);
    equalizeHist(planesEq[1],planesEq[1]);
    equalizeHist(planesEq[2],planesEq[2]);
    //------- fim da equalização da imagem -------

    //Calcula histograma equalizado

    merge(planesEq,imageEq);
    calcHist(&planesEq[0], 1, 0, Mat(), histReq, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planesEq[1], 1, 0, Mat(), histGeq, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planesEq[2], 1, 0, Mat(), histBeq, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histReq, histReq, 0, histImgR_eq.rows, NORM_MINMAX, -1, Mat());
    normalize(histGeq, histBeq, 0, histImgR_eq.rows, NORM_MINMAX, -1, Mat());
    normalize(histBeq, histBeq, 0, histImgR_eq.rows, NORM_MINMAX, -1, Mat());

    histImgR_eq.setTo(Scalar(0));
    histImgG_eq.setTo(Scalar(0));
    histImgB_eq.setTo(Scalar(0));
  
    for(int i=0; i<nbins; i++){
      line(histImgR_eq, Point(i, histh),
           Point(i, cvRound(histReq.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG_eq, Point(i, histh),
           Point(i, cvRound(histGeq.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB_eq, Point(i, histh),
           Point(i, cvRound(histBeq.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }

    histImgR_eq.copyTo(imageEq(Rect(0, 0       ,nbins, histh)));
    histImgG_eq.copyTo(imageEq(Rect(0, histh   ,nbins, histh)));
    histImgB_eq.copyTo(imageEq(Rect(0, 2*histh ,nbins, histh)));

    imwrite("histogramEquilized.png",imageEq);
    imshow("image equalized", imageEq);
    if(waitKey(30) >= 0) break;
  }
  return 0;

}

