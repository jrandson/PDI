#include <iostream>
#include <opencv2/opencv.hpp>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <unistd.h>

using namespace cv;
using namespace std;

void gerar_valores(int vetor[],int tam, int amostras);
void menor(int n, int m, float **mat, int *posic_x,int *posic_y,float *menorzao);

int main(int argc, char** argv){
  Mat image, new_image, sample_image,labels;
  Vec3b val,val1;
  int pos_centro_x, pos_centro_y;
  float minimo;
  int n_amostras=200;
  int width, height, r_number_x,r_number_y,k;
  srand(time(NULL));
  //Passo 1  --> Carregar imagem colorida
  image = imread(argv[1],CV_LOAD_IMAGE_COLOR);
  image.copyTo(sample_image);
  image.copyTo(new_image);
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
  val1 = val;
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
float teste_x,teste_y,teste_z,erro;
bool flag=false;
int azul, verde,vermelho;
int classificacao[n_amostras][n_amostras];
float aux;
float** dist_p_to_c_local;
float** dist_p_to_c_global;
//Inicialização da matriz de centroides
Mat centers(k, k, CV_8UC3, Scalar(0, 0, 0));
//Vetor que armazena a posição dos centroides
int posicao_x[k], posicao_y[k];
Mat medias = Mat(k,k, CV_64F, double(0));
int contadores[k][k];


   dist_p_to_c_local = (float**) malloc (k * sizeof (float *));
   for (int i = 0; i < k; ++i)
      dist_p_to_c_local[i] = (float *) malloc (k * sizeof (float));

    dist_p_to_c_global = (float**) malloc (n_amostras * sizeof (float *));
    for (int i = 0; i < n_amostras; ++i)
      dist_p_to_c_global[i] = (float *) malloc (n_amostras * sizeof (float));

/*for(int i=0;i<k;i++)
{
    for(int j=0;j<k;j++)
    {
    dist_p_to_c[i][j] = 0.0;
    }
}*/


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
//list<int> classificacao(n_amostras*n_amostras,0);
int z=0;
while(z<100){
flag = true;
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
                val = m_sampled.at<Vec3b>(i,j);
                //cout<<val<<endl;
                val1 = centers.at<Vec3b>(l,m);
                //cout<<val1<<endl;
                azul = (int)val[0] - (int)val1[0];
                verde = (int)val[1] - (int)val1[1];
                vermelho = (int)val[2] - (int)val1[2];
                aux = sqrt(pow((float)azul,2.0) + pow((float)verde,2.0) + pow((float)vermelho,2.0));
                dist_p_to_c_local[l][m] = aux;
                dist_p_to_c_global[i][j] = aux;
                //cout<<aux<<endl;
                //getchar();
            }
        }
        //Escolha do centroide usando min e
        //Função retorna a posição do centroide de menor distancia;
        menor(k, k, dist_p_to_c_local, &pos_centro_x,&pos_centro_y,&minimo);
        //Cada amostra possui uma classificação
        //classificacao dos pontos armazenadas em uma lista com a posicao do centroide codificado em um inteiro
        //r*nc + c;
        classificacao[i][j] = pos_centro_x*k + pos_centro_y;
    }
}


for(int g=0;g<k;g++)
for(int h=0;h<k;h++)
contadores[g][h]= 0;

//Redefinir os centroides de acordo com a media dos pontos que pertencem a eles;
for(int g=0;g<k;g++)
{
    for(int h=0;h<k;h++)
    {
        for(int i=0;i<n_amostras;i++)
        {
            for(int j=0;j<n_amostras;j++)
            {
                aux = classificacao[i][j];
                //cout<<"aqui"<<endl;
                if((g*k + h) == aux)
                {
                    medias.at<double>(g,h)= dist_p_to_c_global[i][j]+ medias.at<double>(g,h);
                    contadores[g][h]+= 1;
                }
            }
        }
    }
}//*/

//CAlculo da média de todos os pontos em cada centroide para selecionar um novo
for(int i=0;i<k;i++)
{
    for(int j=0;j<k;j++)
    {
        if(medias.at<double>(i,j) == 0 || contadores[i][j] == 0)
            continue;
        else
            medias.at<double>(i,j) = medias.at<double>(i,j) / contadores[i][j];
    }
}
//cout<<"aqui"<<endl;
//Encontro de novos RGBs que satisfaçam a equação de distancia entre pontos
for(int i=0;i<k;i++)
{
    for(int j=0;j<k;j++)
    {
        while(flag != true)
        {
            //cout<<flag<<endl;

            if(medias.at<double>(i,j) > 255)
            medias.at<double>(i,j) = 255;
            //cout<<medias.at<double>(i,j)<<endl;
            teste_x = rand() %255;
            teste_y = rand() %255;
            teste_z = rand() %255;
            aux = sqrt(pow(teste_x,2.0) + pow(teste_y,2.0) + pow(teste_z,2.0));
            erro = abs(aux - medias.at<double>(i,j));
            if(erro <= 100)
            {
                val[0] =(int) teste_x;
                val[1] =(int) teste_y;
                val[2] =(int) teste_z;
                //Atualiza novo centroide
                centers.at<Vec3b>(i,j) = val;
                flag = true;
            }
            else flag = false;
        }
    flag = false;
    }
}
//imshow("Atualizar_Centros",centers);
//sleep(2);
z++;
system("clear");
cout<<"Calculando "<<z<<" Iteração"<<endl<<"Aguarde..."<<endl;

//Inicia tudo novamente
}
int classify[width][height];
//cout<<centers;
//Aplicação na imagem
for(int i=0;i<width;i++)
{
    for(int j=0;j<height;j++)
    {
    //Acesso aos centroides ( Calculo das distancias de um ponto a todos os centroides );
        for(int l=0;l<k;l++)
        {
            for(int m=0;m<k;m++)
            {
            //cout<<"aqui"<<endl;
              //  getchar();
                val = new_image.at<Vec3b>(i,j);
                //cout<<val<<endl;
                val1 = centers.at<Vec3b>(l,m);
                //cout<<val1<<endl;
                azul = (int)val[0] - (int)val1[0];
                verde = (int)val[1] - (int)val1[1];
                vermelho = (int)val[2] - (int)val1[2];
                aux = sqrt(pow((float)azul,2.0) + pow((float)verde,2.0) + pow((float)vermelho,2.0));
                dist_p_to_c_local[l][m] = aux;
               // dist_p_to_c_global[i][j] = aux;
                //cout<<aux<<endl;
                //getchar();
            }
        }
        //Escolha do centroide usando min e
        //Função retorna a posição do centroide de menor distancia;
        menor(k, k, dist_p_to_c_local, &pos_centro_x,&pos_centro_y,&minimo);
        //Cada amostra possui uma classificação
        //classificacao dos pontos armazenadas em uma lista com a posicao do centroide codificado em um inteiro
        //r*nc + c;
        classify[i][j] = pos_centro_x*k + pos_centro_y;
    }
}

for(int g=0;g<k;g++)
{
    for(int h=0;h<k;h++)
    {
        for(int i=0;i<width;i++)
        {
            for(int j=0;j<height;j++)
            {
                aux = classify[i][j];

                if((g*k + h) == aux)
                {
                    val = centers.at<Vec3b>(g,h);
                    new_image.at<Vec3b>(i,j) = val;

                }
            }
        }
    }
}

//cout<<contadores<<endl;
  imshow("image_sample",image);
  imshow("image_classify",new_image);
  imshow("centroides",centers);

free(dist_p_to_c_local);
free(dist_p_to_c_global);

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
void menor(int n, int m, float **mat, int *posic_x,int *posic_y, float *menorzao) {

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
  *menorzao = menor;
//cout<<menor<<"< --menor"<<endl;

  return;
}
