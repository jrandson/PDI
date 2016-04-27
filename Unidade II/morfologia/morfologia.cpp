#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

int morph_element = 0;
int morph_size = 0;
int kernel_size = 0;
char* windowName = "Operações de morfologia";
int morph_operator = 0;

int const morph_element_max = 2;
int const morph_operator_max = 3;
int const kernel_size_max = 21;

Mat src, dst;

void morphology_operations( int, void*);

int main(int arg, char** argv){
	VideoCapture cap;

		
		/*
		cap.open(0);

		if(!cap.isOpened()){
			return -1;
		}
		cap >> src; */
		src = imread(argv[1]);
		if(!src.data){
			return -1;
		}

		namedWindow(windowName, CV_WINDOW_AUTOSIZE);			

		//image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

		createTrackbar("Operações",windowName, &morph_operator,morph_operator_max, morphology_operations);

		createTrackbar("Elemento",windowName,&morph_element,morph_element_max, morphology_operations);

		createTrackbar("Kernel size", windowName, &morph_size, kernel_size_max, morphology_operations);

		morphology_operations(0, 0);
		
		waitKey(0);
		return 0;
}

 /**
  * @function Morphology_Operations
  */
void morphology_operations( int, void* )
{
	Mat element;
	int operation; 
	//operation = morph_operator + 2;

	/*
	Opening: MORPH_OPEN : 2
	Closing: MORPH_CLOSE: 3
	Gradient: MORPH_GRADIENT: 4
	Top Hat: MORPH_TOPHAT: 5
	Black Hat: MORPH_BLACKHAT: 6
	*/

	switch(morph_operator){
		case 0: // close, erosão
			operation = 2;
			element = getStructuringElement( morph_element, Size( 2*morph_size + 1, 2*morph_size+1 ), 
					  Point( morph_size, morph_size ) );

			morphologyEx( src, dst, operation, element );
		break;
		case 1:  //open, dilatação
			operation = 3;
			element = getStructuringElement( morph_element, Size( 2*morph_size + 1, 2*morph_size+1 ), 
						Point( morph_size, morph_size ) );

			morphologyEx( src, dst, operation, element );
		break;			
		case 2: // erosão -> dilatação
			operation = 2;
			element = getStructuringElement( morph_element, Size( 2*morph_size + 1, 2*morph_size+1 ), 
						Point( morph_size, morph_size ) );
			morphologyEx( src, dst, operation, element );
			
			operation = 3;
			element = getStructuringElement( morph_element, Size( 2*morph_size + 1, 2*morph_size+1 ), 
						Point( morph_size, morph_size ) );
			morphologyEx( src, dst, operation, element );
		break;			
		case 3: //dilatação -> erosão
			operation = 3;
			element = getStructuringElement( morph_element, Size( 2*morph_size + 1, 2*morph_size+1 ), 
						Point( morph_size, morph_size ) );
			morphologyEx( src, dst, operation, element );
			
			operation = 2;
			element = getStructuringElement( morph_element, Size( 2*morph_size + 1, 2*morph_size+1 ), 
						Point( morph_size, morph_size ) );
			morphologyEx( src, dst, operation, element );
		break;

	}	

	imshow( windowName, dst);

  }
