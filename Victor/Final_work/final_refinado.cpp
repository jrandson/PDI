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

using namespace cv;
using namespace std;

int main(int argc, char** argv){
    Mat image, new_image, sample_image;
    int niter=200,n_amostras=2000,ncenter=8;
    cout<<"Por favor, Digite a quantidade de iterações para treinamento"<<endl;
    cin>>niter;
    cout<<"Por favor, Digite a quantidade de amostras"<<endl;
    cin>>n_amostras;
    cout<<"Por favor, Digite a quantidade de cores que deseja obter"<<endl;
    cin>>ncenter;

    float dist;
    float aux=0,aux2=0;
    int aux3=0;
    Vec3b val,val1;
    vector<int> centers(ncenter,0);
    vector<Vec3b> centroide(ncenter,0);
    vector<Vec3b> samples(n_amostras,0);
    vector<int> indices(n_amostras,0);
    vector<int> contador(ncenter,0);
    vector<float> distancias(ncenter,0);
    bool flag =false;
    int int_aux=0;
    float teste_x,teste_y,teste_z,erro,max_valuea = 442;
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

    val[0] = 0;   //B
    val[1] = 0;   //G
    val[2] = 0; //R
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
    for(int i=0; i<ncenter; i++){
        centroide[i] = samples[indices[i]];
        centers[i] = indices[i];
    }

    //indices guardam as posições dos centros.
    //centros são as posições das amostras.
//cout<<"aqui"<<endl;
    // roda o algoritmo
    for(int i=0; i<niter; i++){
        //Amostras
        //for(int m=0;m<ncenter;m++ ){
          //      cout<<distancias[m]<<endl;
            //    distancias[m] = 0;
              //  cout<<m<<endl;
            //}
        //cout<<"aqui"<<endl;
        for(int j=0; j<n_amostras; j++){
            indices[j] = 0;
            dist = MAXFLOAT;
            //centroides
            // procura os vizinhos mais proximos
            for(int k=0; k<ncenter; k++){
                int_aux = centers[k];
                //aux armazena a distancia entre o o ponto e os centroides
                aux = sqrt(pow(samples[j][0] - centroide[k][0],2)
                        + pow(samples[j][1] - centroide[k][1],2)
                        + pow(samples[j][2] - centroide[k][2],2));
                if(aux < dist)
                {
                    dist = aux;
                    //distancias[k] += dist;
                    indices[j] = k;
                }
                // calcula distancia da amostra j
                // para o centro k
                // em indices[k] estarao os rotulos
            }
            aux3 = indices[j];
            distancias[aux3]+=dist;
            contador[aux3] += 1;
            //cout<<"amostra "<<j<<" acabou e seu centroide escolhido foi "<<aux3<<" "<<samples[aux3]<<endl;
        }
        // recalcula os novos centroides

    for(int k=0; k<ncenter;k++){
        if(distancias[k] == 0 || contador[k] == 0)
        continue;
        else{
            distancias[k] = distancias[k]/contador[k];
            if(distancias[k] > max_valuea)
                distancias[k] = (int)distancias[k] % (int)max_valuea;
        }
    }
    for(int k=0;k<ncenter;k++){
        while(flag != true){
            teste_x = rand() % 255;
            teste_y = rand() % 255;
            teste_z = rand() % 255;
            aux = sqrt(pow(teste_x,2.0) + pow(teste_y,2.0) + pow(teste_z,2.0));
            erro = abs(aux - distancias[k]);
            if(erro <= 100)
            {
                val[0] =(int) teste_x;
                val[1] =(int) teste_y;
                val[2] =(int) teste_z;
                //Atualiza novo centroide
                centroide[k] = val;
                flag = true;
            }
            else flag = false;
        }
        flag = false;
    }
    //centroides atualizados, repetir tudo novamente
    for(int j=0;j<ncenter;j++)
            distancias[j] = 0;
    system("clear");
    cout<<"Calculando iteração: "<<i<<endl<<"Por favor Aguarde..."<<endl;
}

    //Aplicacao na imagem
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
    imshow("image_sample",image);
    imshow("image_classify",new_image);
    //imshow("centroides",);
    waitKey();

    return 0;
}


