// INF110 - Trabalho pratico 3
//
// programa para ler, modificar e gravar uma imagem no formato PNM
//
// Autores: Andre Gustavo dos Santos			(criado em 16/06/14)
//          Andre Gustavo dos Santos			(modificado em 22/05/18)
//					Andre Gustavo dos Santos			(modificado em 13/09/21)
//					Andre Gustavo dos Santos			(modificado em 25/06/22)

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <cmath>

const int MAXALTURA = 1500; // tamanho maximo aceito (pode ser alterado)
const int MAXLARGURA = 1500;

using namespace std;
// Função cabeçalho
int cabecalho()
{
	int opc;
	cout << "################################################\n";
	cout << "Clarear[0]\n"
		 << "Escurecer[1]\n"
		 << "Negativo[2]\n"
		 << "Espelhar[3]\n"
		 << "Filtro de Sobel[4]\n"
		 << "Detecção de bordas de Laplace[5]\n"
		 << "Transformar em tons de cinza[6]\n"
		 << "Operação criativa[7]\n"
		 << "Parar de editar[8]\n";

	cout << "################################################\n";
	cout << "Informe o número da operação que deseja fazer: ";
	cin >> opc;

	return opc;
}

// Declaração das matrizes principais e auxiliares
unsigned char imagem[MAXALTURA][MAXLARGURA];
unsigned char copia1[MAXALTURA][MAXLARGURA];
unsigned char copia2[MAXALTURA][MAXLARGURA];
unsigned char matriz_color[MAXALTURA][MAXLARGURA][3];
unsigned char matriz_color_copia1[MAXALTURA][MAXLARGURA][3];
unsigned char matriz_color_copia2[MAXALTURA][MAXLARGURA][3];

int main()
{
	int largura, altura;					   // dimensoes da imagem
	char tipo[4];							   // tipo da imagem
	ifstream arqentrada;					   // arquivo que contem a imagem original
	ofstream arqsaida;						   // arquivo que contera a imagem modificada
	char comentario[200], c;				   // auxiliares
	int i, j, valor, ident, vet_aux[3], media; // auxiliares
	char nomearq[100];						   // nome do arquivo

	//*** LEITURA DA IMAGEM ***//
	// inicialmente nao sera necessario entender nem mudar nada nesta parte

	//*** Abertura do arquivo ***//
	cout << "Nome do arquivo com a imagem PNM: ";
	cin >> nomearq;
	arqentrada.open(nomearq, ios::in); // Abre arquivo para leitura
	if (!arqentrada)
	{
		cout << "Nao consegui abrir arquivo " << nomearq << endl;
		return 0;
	}
	//***************************//

	//*** Leitura do cabecalho ***//
	arqentrada >> tipo; // Le o tipo de arquivo
	arqentrada.get();	// Le e descarta o \n do final da 1a. linha

	if (strcmp(tipo, "P2") == 0)
	{
		cout << "Imagem em tons de cinza\n";
	}
	else if (strcmp(tipo, "P3") == 0)
	{
		cout << "Imagem colorida\n";
		ident = 3;
	}
	else if (strcmp(tipo, "P1") == 0)
	{
		cout << "Imagem preto e branco\n";
		cout << "Desculpe, nao trabalho com esse tipo de imagem.\n";
		arqentrada.close();
		return 0;
	}
	else if (strcmp(tipo, "P4") == 0 || strcmp(tipo, "P5") == 0 || strcmp(tipo, "P6") == 0)
	{
		cout << "Imagem no formato RAW\n";
		cout << "Desculpe, nao trabalho com esse tipo de imagem.\n";
		arqentrada.close();
		return 0;
	}

	while ((c = arqentrada.get()) == '#')	 // Enquanto for comentario
		arqentrada.getline(comentario, 200); // Le e descarta a linha "inteira"

	arqentrada.putback(c); // Devolve o caractere lido para a entrada, pois como
						   // nao era comentario, era o primeiro digito da largura

	arqentrada >> largura >> altura; // Le as dimensoes da imagem, numero de pixels da horizontal e da vertical
	cout << "Tamanho: " << largura << " x " << altura << endl;

	if (largura > MAXLARGURA)
	{
		cout << "Desculpe, ainda nao trabalho com imagens com mais de " << MAXLARGURA << " pixels de largura.\n";
		arqentrada.close();
		return 0;
	}
	if (altura > MAXALTURA)
	{
		cout << "Desculpe, ainda nao trabalho com imagens com mais de " << MAXALTURA << " pixels de altura.\n";
		arqentrada.close();
		return 0;
	}

	arqentrada >> valor; // Valor maximo do pixel (temos que ler, mas nao sera usado)
	//****************************//

	//*** Leitura dos pixels da imagem em preto e branco***//
	if (ident != 3)
	{
		for (i = 0; i < altura; i++)
			for (j = 0; j < largura; j++)
			{
				arqentrada >> valor;
				imagem[i][j] = (unsigned char)valor;
			}
	}
	//*** Leitura dos pixels da imagem colorida***//
	else
	{
		for (i = 0; i < altura; i++)
			for (j = 0; j < largura; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					arqentrada >> valor;
					matriz_color[i][j][k] = (unsigned char)valor;
				}
			}
	}

	//************************************//

	arqentrada.close(); // Fecha arquivo apos a leitura

	//*** FIM DA LEITURA DA IMAGEM ***//

	//*** TRATAMENTO DA IMAGEM ***//
	// inicialmente sera nesta parte do codigo que voce vai trabalhar
	int fator;
	int opc = 0, aux;
	while (true)
	{
		for (int i = 0; i < altura; i++)
			for (int j = 0; j < largura; j++)
			{
				// Faz duas copias da matriz original que guarda a imagem em tons de cinza
				copia1[i][j] = imagem[i][j];
				copia2[i][j] = imagem[i][j];
			}
		opc = cabecalho();
		if (opc == 8)
		{
			break;
		}
		if (opc == 0)
		{
			cout << "Qual o fator de clareamento (1-100)? ";
			cin >> fator;
			if (ident != 3)
			{

				//*** Clareia a imagem em tons de cinza***//
				for (i = 0; i < altura; i++)
					for (j = 0; j < largura; j++)
					{
						valor = (int)imagem[i][j];			 // pega o valor do pixel
						valor += fator;						 // clareia o pixel
						if (valor > 255)					 // se der acima de 255
							valor = 255;					 //  deixa branco
						imagem[i][j] = (unsigned char)valor; // modifica o pixel
					}
			}
			else
			{
				for (i = 0; i < altura; i++)
					for (j = 0; j < largura; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							//*** Clareia a imagem colorida***//
							valor = (int)matriz_color[i][j][k];
							valor += fator;
							if (valor > 255)
								valor = 255;

							matriz_color[i][j][k] = (unsigned char)valor;
						}
					}
			}
		}

		if (opc == 1)
		{
			cout << "Qual o fator de escurecimento (1-100)? ";
			cin >> fator;

			if (ident != 3)
			{

				//*** Escurece a imagem em tons de cinza***//
				for (i = 0; i < altura; i++)
					for (j = 0; j < largura; j++)
					{
						valor = (int)imagem[i][j];			 // pega o valor do pixel
						valor -= fator;						 // escurece o pixel
						if (valor < 0)						 // se der abaixo de 0
							valor = 0;						 //  deixa preto
						imagem[i][j] = (unsigned char)valor; // modifica o pixel
					}
			}
			else
			{
				for (i = 0; i < altura; i++)
					for (j = 0; j < largura; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							//*** Escurece a imagem colorida***//
							valor = (int)matriz_color[i][j][k];
							valor -= fator;
							if (valor < 0)
								valor = 0;

							matriz_color[i][j][k] = (unsigned char)valor;
						}
					}
			}
		}

		if (opc == 2)
		{
			if (ident != 3)
			{
				//*** Negativa a imagem em tons e cinza***//
				for (i = 0; i < altura; i++)
					for (j = 0; j < largura; j++)
					{
						valor = (int)imagem[i][j];
						valor = 255 - valor;
						imagem[i][j] = (unsigned char)valor;
					}
			}
			else
			{
				for (i = 0; i < altura; i++)
					for (j = 0; j < largura; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							//*** Negativa a imagem colorida***//
							valor = (int)matriz_color[i][j][k];
							valor = 255 - valor;
							matriz_color[i][j][k] = (unsigned char)valor;
						}
					}
			}
		}

		if (opc == 3)
		{
			//*** Espelha a imagem em tons de cinza***//
			if (ident != 3)
				for (i = 0; i < altura; i++)
					for (j = 0; j < largura / 2; j++)
					{
						aux = (int)imagem[i][j]; // Variável aux guarda o valor do pixel	a ser trocado
						valor = (int)imagem[i][largura - 1 - j];
						imagem[i][j] = (unsigned char)valor;			 // Troca o valor do pixel inicial
						imagem[i][largura - 1 - j] = (unsigned char)aux; // Troca o valor do pixel final
					}

			else
			{
				for (i = 0; i < altura; i++)
					for (j = 0; j < largura / 2; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							//*** Espelha a imagem colorida***//
							aux = (int)matriz_color[i][j][k];
							matriz_color[i][j][k] = (unsigned char)matriz_color[i][largura - 1 - j][k]; // Troca o valor do pixel inicial
							matriz_color[i][largura - 1 - j][k] = (unsigned char)aux;					// Troca o valor do pixel final
						}
					}
			}
		}
		if (opc == 4)
		{
			if (ident != 3)
			{
				// Cálculo da matriz Gy do Filtro de Sobel
				for (i = 1; i < altura - 1; i++)
					for (j = 1; j < largura - 1; j++)
					{
						// Calcula o valor de cada índice da matriz copia1 pela matriz Gy do filtro de Sobel
						valor = (int)imagem[i - 1][j - 1] + 2 * (int)imagem[i - 1][j] + (int)imagem[i - 1][j + 1] - ((int)imagem[i + 1][j - 1] + 2 * (int)imagem[i + 1][j] + (int)imagem[i + 1][j + 1]);
						if (valor < 0)
							copia1[i][j] = 0; // Deixa o valor do pixel em 0, caso menor que isso
						else if (valor > 255)
							copia1[i][j] = 255; // Deixa o valor do pixel em 255, caso maior que isso
						else
							copia1[i][j] = (unsigned char)valor; // Guarda o valor do pixel
					}
				// Calcula o valor de cada índice da matriz copia2 pela matriz Gx do filtro de Sobel
				for (i = 1; i < altura - 1; i++)
					for (j = 1; j < largura - 1; j++)
					{
						valor = (int)imagem[i - 1][j - 1] + 2 * (int)imagem[i][j - 1] + (int)imagem[i + 1][j - 1] - ((int)imagem[i - 1][j + 1] + 2 * (int)imagem[i][j + 1] + (int)imagem[i + 1][j + 1]);
						if (valor < 0)
							copia2[i][j] = 0;
						else if (valor > 255)
							copia2[i][j] = 255;
						else
							copia2[i][j] = (unsigned char)valor;
					}

				for (i = 0; i < altura; i++)
					for (j = 0; j < largura; j++)
					{
						copia1[i][j] = (int)copia1[i][j];
						copia2[i][j] = (int)copia2[i][j];
						// Calcula os valores da matriz resultante pela magnitude do gradiente
						valor = sqrt(pow(copia1[i][j], 2) + pow(copia2[i][j], 2));
						if (valor < 0)
							imagem[i][j] = 0;
						else if (valor > 255)
							imagem[i][j] = 255;
						else
							imagem[i][j] = (unsigned char)valor;
					}
			}
			else
			{
				// O código abaixo refere-se ao Filtro de Sobel para matrizes de imagens coloridas
				// Logo, tem as mesmas características do código do Filtro de Sobel para
				// imagens em tons de cinza

				for (i = 1; i < altura - 1; i++)
				{
					for (j = 1; j < largura - 1; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							// Cálculo pela matriz Gy do Filtro de Sobel
							valor = (int)matriz_color[i - 1][j - 1][k] + 2 * (int)matriz_color[i - 1][j][k] + (int)matriz_color[i - 1][j + 1][k] - ((int)matriz_color[i + 1][j - 1][k] + 2 * (int)matriz_color[i + 1][j][k] + (int)matriz_color[i + 1][j + 1][k]);
							if (valor < 0)
								matriz_color_copia1[i][j][k] = 0;
							else if (valor > 255)
								matriz_color_copia1[i][j][k] = 255;
							else
								matriz_color_copia1[i][j][k] = (unsigned char)valor;
						}
					}
				}
				// Cálculo pela matriz Gx do Filtro de Sobel
				for (i = 1; i < altura - 1; i++)
				{
					for (j = 1; j < largura - 1; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							valor = (int)matriz_color[i - 1][j - 1][k] + 2 * (int)matriz_color[i][j - 1][k] + (int)matriz_color[i + 1][j - 1][k] - ((int)matriz_color[i - 1][j + 1][k] + 2 * (int)matriz_color[i][j + 1][k] + (int)matriz_color[i + 1][j + 1][k]);
							if (valor < 0)
								matriz_color_copia2[i][j][k] = 0;
							else if (valor > 255)
								matriz_color_copia2[i][j][k] = 255;
							else
								matriz_color_copia2[i][j][k] = (unsigned char)valor;
						}
					}
				}
				// Cálculo da matriz resultante pela magnitude de gradiente
				for (i = 0; i < altura; i++)
				{
					for (j = 0; j < largura; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							matriz_color_copia1[i][j][k] = (int)matriz_color_copia1[i][j][k];
							matriz_color_copia2[i][j][k] = (int)matriz_color_copia2[i][j][k];
							// Magnitude de gradiente
							valor = sqrt(pow(matriz_color_copia1[i][j][k], 2) + pow(matriz_color_copia2[i][j][k], 2));
							if (valor < 0)
								matriz_color[i][j][k] = 0;
							else if (valor > 255)
								matriz_color[i][j][k] = 255;
							else
								matriz_color[i][j][k] = (unsigned char)valor;
						}
					}
				}
			}
		}
		if (opc == 5)
		{
			// Filtro de Laplace para imagens em tons de cinza
			if (ident != 3)
			{
				for (i = 1; i < altura - 1; i++)
					for (j = 1; j < largura - 1; j++)
					{
						// Apenas aplica a matriz convolucional do filtro de Laplace
						valor = (int)imagem[i][j] * 4 - ((int)imagem[i - 1][j] + (int)imagem[i + 1][j] + (int)imagem[i][j + 1] + (int)imagem[i][j - 1]);
						if (valor < 0)
							copia1[i][j] = 0;
						else if (valor > 255)
							copia1[i][j] = 255;
						else
							copia1[i][j] = (unsigned char)valor;
					}

				for (i = 0; i < altura; i++)
					for (j = 0; j < largura; j++)
					{
						// Muda o valor dos pixels da matriz principal
						imagem[i][j] = copia1[i][j];
					}
			}
			else
			{
				// Filtro de Laplace para imagens coloridas
				for (i = 1; i < altura - 1; i++)
					for (j = 1; j < largura - 1; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							// Aplicação da matriz convolucional de Laplace
							valor = 4 * (int)matriz_color[i][j][k] - ((int)matriz_color[i - 1][j][k] + (int)matriz_color[i + 1][j][k] + (int)matriz_color[i][j + 1][k] + (int)matriz_color[i][j - 1][k]);
							if (valor < 0)
								matriz_color_copia1[i][j][k] = 0;
							else if (valor > 255)
								matriz_color_copia1[i][j][k] = 255;
							else
								matriz_color_copia1[i][j][k] = (unsigned char)valor;
						}
					}

				for (i = 0; i < altura; i++)
					for (j = 0; j < largura; j++)
					{
						for (int k = 0; k < 3; k++)
							// Salva os pixels modificados na matriz principal
							matriz_color[i][j][k] = matriz_color_copia1[i][j][k];
					}
			}
		}
		if (opc == 6)
		{
			// Transformação de imagens coloridas em tons de cinza
			if (ident != 3)
			{
				cout << "\n Funcionalidade referente apenas às imagens coloridas\n";
				continue;
			}
			for (i = 0; i < altura; i++)
				for (j = 0; j < largura; j++)
				{
					// Calcula a média dos valores rgb de cada pixel
					media = (matriz_color[i][j][0] + matriz_color[i][j][1] + matriz_color[i][j][2]) / 3;
					for (int k = 0; k < 3; k++)
					{
						// Modifica os valores rgb para receberem o valor da media
						matriz_color[i][j][k] = (unsigned char)media;
					}
				}
		}
		if (opc == 7)
		{
			// Operação extra
			if (ident != 3)
			{
				for (i = 0; i < altura; i++)
					for (j = i; j < largura; j++)
					{
						// Percorre apenas uma parte da imagem cinza e muda
						// os valores de seus pixels para preto ou branco
						if (imagem[i][j] > 128)
							imagem[i][j] = 255;
						else
							imagem[i][j] = 0;
					}
			}
			else
			{
				for (i = 0; i < altura; i++)
					for (j = i; j < largura; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							// Percorre apenas uma parte da imagem colorida
							// e mantém apenas os valores rgb mais vibrantes de cada pixel
							if (matriz_color[i][j][k] < 150)
							{
								matriz_color[i][j][k] = 0;
							}
						}
					}
			}
		}
	}

	//*************************//

	//*** FIM DO TRATAMENTO DA IMAGEM ***//

	//*** GRAVACAO DA IMAGEM ***//
	// inicialmente nao sera necessario entender nem mudar nada nesta parte

	//*** Grava a nova imagem ***//
	cout << "Digite o nome do arquivo de saída\n";
	cin >> nomearq;
	arqsaida.open(nomearq, ios::out); // Abre arquivo para escrita
	if (!arqsaida)
	{
		cout << "Nao consegui criar novaimagem.pnm\n";
		return 0;
	}

	arqsaida << tipo << endl;			  // tipo
	arqsaida << "# TP3-INF110, by AGS\n"; // comentario
	arqsaida << largura << " " << altura; // dimensoes
	arqsaida << " " << 255 << endl;		  // maior valor
	for (i = 0; i < altura; i++)
		for (j = 0; j < largura; j++)
			if (ident != 3)
				arqsaida << (int)imagem[i][j] << endl;
			else
				for (int k = 0; k < 3; k++)
					arqsaida << (int)matriz_color[i][j][k] << endl; // pixels

	arqsaida.close(); // fecha o arquivo
	//***************************//

	//*** FIM DA GRAVACAO DA IMAGEM ***//

	return 0;
}
