#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int* decimalToBinary(int);
int fromBinaryToDecimal(int[]);
void fromArraytoWord(char* ,int[],int n);
int* fromWordToArray(char[], int n);
void imEncode();
void imDecode();
void wavEncode();
void wavDecode();
int getInx(unsigned char word[], int s);

int getInfo(FILE* fpo);
int* comparison(FILE* f1, FILE* f2, int size1, int size2, int* size);
unsigned char* readBMP(char* filename, int* sentence, int dim);
unsigned char* readWav(char* filename, int* sentence, int dim);
unsigned char* vectFusion(unsigned char vec1[], int n, unsigned char* vec2, int m);

int main()
{
    int scelta;

    do
    {
        printf("0)Exit\n");
        printf("1)Encode text from bmp image\n");
        printf("2)Decode text from bmp image\n");
        printf("3)Encode text from wav audio\n");
        printf("4)Decode text from wav audio\n");
        printf("Select an action --> ");
        scanf("%d", &scelta);
        switch (scelta)
        {
            case 0:
                exit(0);
            case 1:
                imEncode();
                break;
            case 2:
                imDecode();
                break;
            case 3:
                wavEncode();
                break;
            case 4:
                wavDecode();
            default:
                printf("Choose a valid action!\n");
                break;
        }
    } while (scelta != 0);
    return 0;
}

void imEncode()
{
    char name[50];
    char word[1000];
    int size = 0;
    int* vect;
    char ch = getchar();
    unsigned char* data;
    printf("Insert the image you want to use --> ");
    scanf("%s", name);
    char stocazzo = getchar();
    printf("Insert the sentence you want to hide -> ");
    fgets(word, sizeof(word), stdin);
    size = strlen(word)-1;
    printf("Size --> %d", size);
    getchar();


    vect = fromWordToArray(word, size);
    printf("-----------------\n");
    /*
    for (int i = 0; i < size*8; i++){
        printf("%d <-- 0x%x\n", *(vect+i), (vect+i));
    }
    */
    data = readBMP(name, vect, size*8);
    free(data);
    free(vect);
}

void imDecode()
{
    int size1, size2, dim;
    int* differences;
    char nome1[20], nome2[20];
    printf("Inserisci foto da decodificare --> ");
    scanf("%s", nome1);
    printf("Inserisci foto originale --> ");
    scanf("%s", nome2);
	FILE* f1 = fopen(nome1, "rb");
	FILE* f2 = fopen(nome2, "rb");
	size1 = getInfo(f1);
	size2 = getInfo(f2);
	fclose(f1);
	fclose(f2);

	FILE* f10 = fopen(nome1, "rb");
	FILE* f20 = fopen(nome2, "rb");
    dim = 0;
	differences = comparison(f10, f20, size1, size2, &dim);
	fclose(f10);
	fclose(f20);
    char* frase = calloc(sizeof(char), dim);
    fromArraytoWord(frase, differences, dim);
    printf("\n\nThe decoded sentence is --> %s\n\n", frase);
    free(differences);
}

unsigned char* readBMP(char* filename, int* sentence, int dim)
{
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    //printf("%d", size);
    unsigned char* data = calloc(sizeof(unsigned char), size);
    unsigned char* prova;

    fread(data, sizeof(unsigned char), size, f);

    for (int i = 0; i < dim; i++)
    {
        if (i > size)
        {
            break;
        }

        *(data+i) += (*(sentence+i))+1;
    }

    prova = vectFusion(info, 54, data, size);

    /*
    for (int i = 0; i < 54+size; i++){

        printf("i = %d - %d\n", i, *(prova+i));

    }
    */

    //printf("\n---------\n");
    char nome[50];
    printf("Insert name of the encoded image --> ");
    scanf("%s", nome);
    FILE* r = fopen(nome, "wb");
    fwrite(prova, sizeof(unsigned char), size+54, r);
    fclose(r);
    fclose(f);

    free(data);
    return prova;
}

unsigned char* vectFusion(unsigned char vec1[], int n, unsigned char* vec2, int m)
{
    int i = 0;
    unsigned char* vec = calloc(sizeof(unsigned char), n+m);

    for (int s = 0; s < n; s++)
    {
        *(vec+i) = vec1[s];
        i++;
    }

    for (int r = 0; r < m; r++)
    {
        *(vec+i) = *(vec2+r);
        i++;
    }
    return vec;
}

/*
    Utility functions
*/

int fromBinaryToDecimal(int binario[])
{

	int risultato = 0;
	int j=0;
	for (int i=7;i>0;i--)
	{
		if(binario[i]==1)
		{
			risultato+= pow(2,7-i);

		}

	}



	return risultato;

}

void fromArraytoWord(char *res,int a[],int sizeOfA)
{


		int byte[8];
		int j=0;
		int c=0;


		for(int i=0;i<sizeOfA;i++)
		{
			byte[j]=a[i];

			j++;

			if(j==8)
			{
				j=0;
					res[c]=fromBinaryToDecimal(byte);

				c++;

			}
		}
	return;
}

int* decimalToBinary (int n)
{
	int a[8];
	int* r = a;
	for(int i = 7; i >= 0; i--)
	{
        if (n>=0)
        {
    		a[i]=n%2;
    		n=n/2;
        } else {
            a[i] = 0;
        }
	}

	return r;
}


int* fromWordToArray(char parola[] ,int size_parola)
{
	int a[8];
    int* s = a;

	int* vett = calloc(sizeof(int), size_parola*8);
    int r = 0;

	while (r < size_parola*8){
		s = decimalToBinary(parola[(int)(r/8)]);
        for (int i = 0; i < 8; i++)
        {
            *(vett+r) = *(s+i);
            r++;
        }
	}
    r = 0;
    /*
    while (r < size_parola*8){
        printf("%d <-- 0x%x\n", *(vett+r), (vett+r));
        r++;
    }
    */
    return vett;
}

int getInfo(FILE* fpo)
{
	int int_var;
	int size;
	char char_2[2];
	fseek(fpo,0,0);
	fread(char_2,2, 1, fpo);
	//printf(" - Firma: %c%c\n",char_2[0],char_2[1]);
	fseek(fpo,2,0);
	fread(&size, 1, sizeof(int), fpo);
	//printf(" - Dimensione File: %d\n",size);
	fseek(fpo,10,0);
	fread(&int_var, 1, sizeof(int), fpo);
	//printf(" - Offset: %d\n",int_var);
	//printf("\n-----------------------\n");

	return size;
}

int* comparison(FILE* f1, FILE* f2, int size1, int size2, int* size)
{
	int ciao, c;
    c = 0;
	unsigned char* l1 = calloc(sizeof(unsigned char), size1);
	unsigned char* l2 = calloc(sizeof(unsigned char), size2);

	fread(l1, sizeof(unsigned char), size1, f1);
	fread(l2, sizeof(unsigned char), size2, f2);

	for (int i = 0; i < size1; i++)
	{
		if (*(l1+i) != *(l2+i))
		{
            printf("%d) # %d - %d #\n", i, *(l1+i), *(l2+i));
			c++;
		} else
		{

		}
	}

    *size = c;
    int* differences = calloc(sizeof(int), (*size));
    int cont = 0;
    for (int i = 0; i < size1; i++)
	{
		if (*(l1+i) != *(l2+i))
		{
			*(differences+cont) = (*(l1+i))-(*(l2+i))-1;
            cont++;
		}
        else
		{
			//printf("%d) # %d - %d #\n", i, *(l1+i), *(l2+i));
		}
	}

	free(l1);
	free(l2);
	return differences;
}

void wavEncode()
{
    char name[50];
    char word[1000];
    int size = 0;
    int* vect;
    char ch = getchar();
    unsigned char* data;
    printf("Insert the audio you want to use --> ");
    scanf("%s", name);
    char stocazzo = getchar();
    printf("Insert the sentence you want to hide -> ");
    fgets(word, sizeof(word), stdin);
    size = strlen(word)-1;
    printf("Size --> %d", size);
    getchar();


    vect = fromWordToArray(word, size);
    printf("-----------------\n");
    /*
    for (int i = 0; i < size*8; i++){
        printf("%d <-- 0x%x\n", *(vect+i), (vect+i));
    }
    */
    data = readWav(name, vect, size*8);
    free(data);
    free(vect);
}

unsigned char* readWav(char* filename, int* sentence, int dim)
{

    FILE* f1 = fopen(filename, "rb");
    unsigned char info[100];
    fread(info, sizeof(unsigned char), 100, f1);
    int dataStart;
    dataStart = getInx(info, 100);
    int dataSize = *(int*)&info[dataStart];
    fclose(f1);

    FILE* f = fopen(filename, "rb");
    fread(info, sizeof(unsigned char), (dataStart+4), f);
    unsigned char* data = calloc(sizeof(unsigned char), dataSize);
    unsigned char* prova;
    printf("%d\n", dataSize);
    fread(data, sizeof(unsigned char), dataSize, f);
    for (int i = 0; i < dim; i++)
    {
        if (i > dataSize)
        {
            break;
        }

        *(data+i) += (*(sentence+i))+1;
    }

    prova = vectFusion(info, dataStart+4, data, dataSize);

    char nome[50];
    printf("Insert name of the encoded audio --> ");
    scanf("%s", nome);
    FILE* r = fopen(nome, "wb");
    fwrite(prova, sizeof(unsigned char), dataSize+dataStart, r);
    fclose(r);
    fclose(f);

    free(data);
    return prova;
}

int getInx(unsigned char word[], int s)
{
    int i;

    for (i = 0; i < s; i++)
    {
        if (word[i] == 'd')
        {
            if (word[i+1] == 'a')
            {
                break;
            }
        }
    }

    return i+4;
}

void wavDecode()
{
    int size1, size2, dim;
    int* differences;
    char nome1[20], nome2[20];
    printf("Insert audio to decode --> ");
    scanf("%s", nome1);
    printf("Insert original audio --> ");
    scanf("%s", nome2);
	FILE* f1 = fopen(nome1, "rb");
	FILE* f2 = fopen(nome2, "rb");
    fseek(f1, 0, SEEK_END);
    size1 = ftell(f1);
    //printf("%d\n", size1);
    fseek(f2, 0, SEEK_END);
	size2 = ftell(f2);
    //printf("%d\n", size2);
	fclose(f1);
	fclose(f2);

	FILE* f10 = fopen(nome1, "rb");
	FILE* f20 = fopen(nome2, "rb");
    dim = 0;
	differences = comparison(f10, f20, size1, size2, &dim);
    printf("%d\n", dim);
	fclose(f10);
	fclose(f20);
    char* frase = calloc(sizeof(char), dim);
    fromArraytoWord(frase, differences, dim);
    printf("\n\nThe decoded sentence is --> %s\n\n", frase);
    free(differences);
}
