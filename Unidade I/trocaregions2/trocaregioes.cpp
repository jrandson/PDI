#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

// Para compilar use make pixels

int main(int argc, char** argv){
  Mat image,A;
  Vec3b intensity;

  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu biel.png" << endl;

  int cols = image.cols;
  int rows = image.rows;

  namedWindow("janela",WINDOW_AUTOSIZE);

  imshow("janela", image);


  image.copyTo(A);

  image(Rect(0, 0, rows/2, cols/2)).copyTo(A(Rect(rows/2, cols/2, rows/2, cols/2)));
  image(Rect(rows/2, cols/2, rows/2, cols/2)).copyTo(A(Rect(0, 0, rows/2, cols/2)));
  image(Rect(rows/2, 0, rows/2, cols/2)).copyTo(A(Rect(0, cols/2, rows/2, cols/2)));
  image(Rect(0, cols/2, rows/2, cols/2)).copyTo(A(Rect(rows/2, 0, rows/2, cols/2)));

  imshow("image A", A);

  waitKey();
  return 0;
}
