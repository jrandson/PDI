#include <iostream>
#include <opencv2/opencv.hpp>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>

using namespace cv;
using namespace std;

void gerar_valores(int vetor[],int tam, int amostras);
void menor(int n, int m, float **mat, int *posic_x,int *posic_y);

int main(int argc, char** argv){
  Mat image, new_image, sample_image,labels;
  Vec3b val;
  int pos_centro_x, pos_centro_y;
  int n_amostras=200;
  int width, height, r_number_x,r_number_y,k;
  srand(time(NULL));
  //Passo 1  --> Carregar imagem colorida
  image = imread(argv[1],CV_LOAD_IMAGE_COLOR);
  image.copyTo(sample_image);
  //Amostra da imagem
  Mat m_sampled(n_amostras, n_amostras, CV_8UC3, Scalar(0, 0, 0));
  //Perguntar ao professor como converter essa imagem
  //imshow("image", m_sampled);
  //imshow("image", image);
  width=image.size().width;
  height=image.size().height;

  val[0] = 0;   //B
  val[1] = 0;   //G
  val[2] = 0; //R
//Define preto na imagem auxiliar
  for(int i=0;i<width;i++){
    for(int j=0;j<height;j++){
      sample_image.at<Vec3b>(i,j)=val;
    }
  }
//amostra do centro da imagem
  sample_image.at<Vec3b>(300,300)=image.at<Vec3b>(300,300);
//Definir pixels de amostra aleatoriamente
  for(int i=0;i<m_sampled.size().width;i++){
    for(int j=0;j<m_sampled.size().height;j++){
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

  //m_sampled.convertTo(m_sampled,CV_32FC3);
//K-Means
//Definindo quantidade de clusters onde k define a quantidade de centroides ou clusters
//Exemplo: se k = 32, Quantidade de centroides = 32*32 = 1024
k = 32;
float aux;
float** dist_p_to_c;
   dist_p_to_c = (float**) malloc (k * sizeof (float *));
   for (int i = 0; i < k; ++i)
      dist_p_to_c[i] = (float *) malloc (k * sizeof (float));

for(int i=0;i<k;i++)
{
    for(int j=0;j<k;j++)
    {
    dist_p_to_c[i][j] = 0.0;
    }
}

//Inicialização da matriz de centroides
Mat centers(k, k, CV_8UC3, Scalar(0, 0, 0));
//Vetor que armazena a posição dos centroides
int posicao_x[k], posicao_y[k];

// gerando posição dos centroides aleatoriamente
gerar_valores(posicao_x,k,n_amostras);
gerar_valores(posicao_y,k,n_amostras);

//Incializando e copiando valores de RGB para os centroides

for(int i=0;i<k;i++)
{
    for(int j=0;j<k;j++)
    {
         centers.at<Vec3b>(i,j) = m_sampled.at<Vec3b>(posicao_x[i],posicao_y[j]);
    }
}

list<int> classificacao(n_amostras*n_amostras,0);
 //Primeiros Centroides já selecionados, então o prximo passo é fazer os calculos para classificação das amostras
for(int i=0;i<n_amostras;i++)
{
    for(int j=0;j<n_amostras;j++)
    {
    //Acesso aos centroides ( Calculo das distancias de um ponto a todos os centroides );
        for(int l=0;l<k;l++)
        {
            for(int m=0;m<k;m++)
            {
                val = m_sampled.at<Vec3b>(i,j) - centers.at<Vec3b>(l,m);
                aux = sqrt(pow((float)val[0],2.0) + pow((float)val[1],2.0) + pow((float)val[2],2.0));
                dist_p_to_c[l][m] = aux;
            }
        }
        //Escolha do centroide usando min e
        //Função retorna a posição do centroide de menor distancia;
        menor(k, k, dist_p_to_c, &pos_centro_x,&pos_centro_y);
        //Cada amostra possui uma classificação
        //classificacao dos pontos armazenadas em uma lista com a posicao do centroide codificado em um inteiro
        //r*nc + c;
        classificacao.push_back(pos_centro_x*k + pos_centro_y);
    }
}
//cout<<classificacao;
for(list<int>::iterator it=classificacao.begin();it != classificacao.end();++it)
{
    cout<<" "<<*it<<endl;
}

/*kmeans(m_sampled, 64, labels,
            TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),
               3, KMEANS_PP_CENTERS, centers);
*/

  //imshow("image_sample",m_sampled);
free(dist_p_to_c);
  waitKey();

  return 0;
}

void gerar_valores(int vetor[],int tam, int amostras)
{
	int i=0, j=0;
	srand(time(NULL));
	for(i=0; i<tam; i++)
	{
		vetor[i] = (rand()%amostras)+1;

		for(j=0; j<i; j++)
		{
			if(vetor[j] == vetor[i])
			{
				vetor[i] = (rand()%amostras)+1;
				j=0;
			}
		}
	}
}
void menor(int n, int m, float **mat, int *posic_x,int *posic_y) {

  int i,j;
  float menor, maior;
  menor = mat[0][0];
  maior = mat[0][0];
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      if (menor > mat[i][j]) {
	menor = mat[i][j];
	*posic_x = i;
	*posic_y = j;
      }

      if (maior < mat[i][j]) {
	maior = mat[i][j];
      }
    }
  }

  return;
}
