#include <stdio.h>
#include <stdlib.h>

unsigned char* readBMP(char* filename);
unsigned char* vectFusion(unsigned char vec1[], int n, unsigned char* vec2, int m);

int main()
{
    char* name = "MARBLES.BMP";
    unsigned char* data;
    unsigned char R, G, B;
    data = readBMP(name);
    free(data);
    return 0;
}

unsigned char* readBMP(char* filename)
{
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    printf("%d", size);
    //unsigned char base[size];
    unsigned char* data = calloc(sizeof(unsigned char), size);
    unsigned char* prova;

    fread(data, sizeof(unsigned char), size, f);
    /*
    for (int i = 0; i < size; i += 3)
    {
        if (i > size)
        {
            break;
        }
        *(data+i) = 250;
    }
    */

    prova = vectFusion(info, 54, data, size);

    for (int i = 0; i < 54+size; i++){

        printf("i = %d - %d\n", i, *(prova+i));

    }

    printf("\n---------\n");

    FILE* r = fopen("PROVA.BMP", "wb");
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
