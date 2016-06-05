#include <iostream>
#include <opencv2/opencv.hpp>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <values.h>
#include <fstream>


using namespace cv;
using namespace std;
Mat src_gray,src;
int thresh = 100;
int max_thresh = 100;
RNG rng(12345);
vector<vector<vector<Point> >> contornos;
void thresh_callback(int, void*,int iteracoes );

int main(int argc, char** argv){
    Mat image, new_image, sample_image;
    int niter=200,n_amostras=2000,ncenter=8;
    Mat imagem_sep[ncenter];
    ofstream meu_arquivo;

    cout << "Por favor, Digite a quantidade de iterações para treinamento"<<endl;
    cin >> niter;
    cout << "Por favor, Digite a quantidade de amostras"<<endl;
    cin >> n_amostras;
    cout << "Por favor, Digite a quantidade de cores que deseja obter"<<endl;
    cin >> ncenter;



    float dist;
    float aux = 0, aux2 = 0;
    int aux3 = 0;

    Vec3b val,val1;
    vector<int> centers(ncenter,0);
    vector<Vec3b> centroide(ncenter,0);
    vector<Vec3b> samples(n_amostras,0);
    vector<int> indices(n_amostras,0);
    vector<int> contador(ncenter,0);
    vector<float> distancias(ncenter,0);


    val[0] = 0;   //B
    val[1] = 0;   //G
    val[2] = 0;   //R

    bool flag =false;
    int int_aux=0;
    float teste_x, teste_y, teste_z, erro,max_valuea = 442;

    // numeros aleatorios para sorteio da amostra

    int rndx, rndy;
    int width, height;
    srand(time(NULL));

    //Passo 1  --> Carregar imagem colorida

    image = imread(argv[1],CV_LOAD_IMAGE_COLOR);
    image.copyTo(sample_image);
    image.copyTo(new_image);

    //Amostra da imagem
    width=image.size().width;
    height=image.size().height;

     for(int k = 0; k < ncenter; k++)
    {
        imagem_sep[k] = Mat(width, height, CV_8UC3, Scalar(0,0,0));
    }

    val[0] = 0;   //B
    val[1] = 0;   //G
    val[2] = 0;   //R
    val1 = val;

    // realiza a amostragem
    for(int i=0; i<n_amostras; i++){

        rndx = rand()%image.rows;
        rndy = rand()%image.cols;

        val1 = image.at<Vec3b>(rndx, rndy);

        samples[i][0] = val1[0];
        samples[i][1] = val1[1];
        samples[i][2] = val1[2];

        indices[i] = i;
    }

    random_shuffle(indices.begin(), indices.end());

    //indices guardam as posições dos centros.
    //centros são as posições das amostras.
    for(int i=0; i<ncenter; i++){
        centroide[i] = samples[indices[i]];
        centers[i] = indices[i];
    }

    // roda o algoritmo
    for(int i = 0; i < niter; i++){

        for(int j = 0; j < n_amostras; j++){
            indices[j] = 0;
            dist = MAXFLOAT;

            //centroides
            // procura os vizinhos mais proximos

            for(int k=0; k<ncenter; k++){

                int_aux = centers[k];
                //aux armazena a distancia entre o ponto e os centroides

                // calcula distancia da amostra j
                // para o centro k
                // em indices[k] estarao os rotulos

                aux = sqrt(pow(samples[j][0] - centroide[k][0],2)
                        + pow(samples[j][1] - centroide[k][1],2)
                        + pow(samples[j][2] - centroide[k][2],2));

                if(aux < dist){
                    dist = aux;
                    indices[j] = k;
                }
            }

            aux3 = indices[j];
            distancias[aux3] += dist;
            contador[aux3] += 1;
        }

        // recalcula os novos centroides

        for(int k = 0; k < ncenter; k++){
            if(distancias[k] == 0 || contador[k] == 0){
                continue;
            }
            else{
                distancias[k] = distancias[k]/contador[k];
                if(distancias[k] > max_valuea){
                    distancias[k] = (int)distancias[k] % (int)max_valuea;
                }
            }

        }
        for(int k=0;k<ncenter;k++){
            while(flag != true){

                teste_x = rand() % 255;
                teste_y = rand() % 255;
                teste_z = rand() % 255;

                aux = sqrt(pow(teste_x,2.0) + pow(teste_y,2.0) + pow(teste_z,2.0));
                erro = abs(aux - distancias[k]);

                if(erro <= 5){
                    val[0] =(int) teste_x;
                    val[1] =(int) teste_y;
                    val[2] =(int) teste_z;
                    //Atualiza novo centroide

                    centroide[k] = val;

                    flag = true;
                }
                else {
                    flag = false;
                }
            }

            flag = false;
        }

        //centroides atualizados, repetir tudo novamente
        for(int j = 0;j < ncenter; j++){
            distancias[j] = 0;
        }

        system("clear");
        cout << "Calculando iteração: "<<i<<endl<<"Se tiver com pressa pode ir dar uma volta..."<<endl;
    }

    //cria uma estrutura pra guardar cada cor encontrada
    vector<Mat> cores;



    //Aplicacao na imagem
    //for(int k = 0; k < ncenter; k++){

        Mat imageAux(image.rows,image.cols,CV_8UC3, Scalar(255,255,255));

        for(int i=0;i<image.rows;i++){
        for(int j=0;j<image.cols;j++){
            dist =MAXFLOAT;
            for(int k=0; k<ncenter; k++){
                //aux armazena a distancia entre o o ponto e os centroides
                val = new_image.at<Vec3b>(i,j);
                aux = sqrt(pow(val[0] - samples[centers[k]][0],2)
                        + pow(val[1] - samples[centers[k]][1],2)
                        + pow(val[2] - samples[centers[k]][2],2));
                if(aux < dist)
                {
                    dist = aux;
                    val1[0] = samples[centers[k]][0];
                    val1[1] = samples[centers[k]][1];
                    val1[2] = samples[centers[k]][2];
                    new_image.at<Vec3b>(i,j) = val1;
                }
            }
        }
    }
    Vec3i auxiliar,auxiliar2;
    bool testea;
    val[0] = 255;
    val[1] = 255;
    val[2] = 255;

    imshow("image_sample",image);
    imshow("image_classify",new_image);
	//cout<<cores.size()<<endl;
    for(int k = 0; k < ncenter; k++)
    {
        for(int i = 0; i < new_image.rows; i++)
        {
            for(int j = 0; j < new_image.cols; j++)
            {
                auxiliar = samples[centers[k]];
                auxiliar2 = new_image.at<Vec3b>(i,j);
                testea = auxiliar == auxiliar2;
                if(testea)
                {
                    imageAux.at<Vec3b>(i,j) = auxiliar2;
                }
                else {
                    imageAux.at<Vec3b>(i,j) = val;
                }
            }
        }
    imageAux.copyTo(imagem_sep[k]);
        for(int i = 0; i < imageAux.rows; i++)
            for(int j = 0; j < imageAux.cols; j++)
                imageAux.at<Vec3b>(i,j) = val;
    }


//Encontrando contornos nas imagens geradas
//imshow("imggray",src_gray);
for(int i = 0; i < ncenter; i++)
{
    imagem_sep[i].copyTo(src);
    cvtColor( src, src_gray, CV_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );
  //  imshow("Preto e Branco"+to_string(i),src_gray);
    //createTrackbar( " Canny thresh:", "Source", 200, 200, thresh_callback );
    thresh_callback( 0, 0 ,i);
}
//waitKey();

cout<<contornos.size()<<endl;
//Criação do arquivo .fig
meu_arquivo.open("pdifile.fig");
meu_arquivo << "#FIG 3.2"<<endl;
meu_arquivo << "Landscape"<<endl;
meu_arquivo << "Center"<<endl;
meu_arquivo << "Metric"<<endl;
meu_arquivo << "A4" <<endl;
meu_arquivo << "100"<<endl;
meu_arquivo << "Single"<<endl;
meu_arquivo << "-2"<<endl<<"1200 2"<<endl;
for(int i=0;i<contornos.size();i++)
{
    for(int j=0;j<contornos[i].size();j++)
    {
        if(contornos[i][j].size() >= 6)
        for(int k=0;k<contornos[i][j].size();k++)
        {
        rndx =  rand() %16;
            if(k == 0){
            meu_arquivo << "2 3 0 1 "<<rndx<<" 7 50 -1 -1 0.000 0 0 -1 0 0 ";
            meu_arquivo << contornos[i][j].size()<<endl;
            }
            meu_arquivo << contornos[i][j][k].x<<" "<<contornos[i][j][k].y<<" ";
        }
        meu_arquivo << endl;
    }
    meu_arquivo << endl;
}
meu_arquivo.close();
waitKey();
    return 0;
}
void thresh_callback(int, void*,int iteracoes )
{
  Mat canny_output;
  vector<Vec4i> hierarchy;
  vector<vector<Point> > contours;
  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for(int i = 0; i< contours.size(); i++ ){
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
 }
 contornos.push_back(contours);
//cout<<contours[1][1]<<endl;
  /// Show in a window
  //namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  //imshow( "Contours"+to_string(iteracoes), drawing );
}



