#include <stdio.h>
#include <stdlib.h>

unsigned char* readBMP(char* filename);

int main(){

    char* name = "BLU.BMP";
    unsigned char* data;
    unsigned char R, G, B;

    data = readBMP(name);

    return 0;
}

unsigned char* readBMP(char* filename){
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); 

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char cacca[size];
    unsigned char* data =  cacca;
    fread(data, sizeof(unsigned char), size, f);
    fclose(f);
    
    for(i = 0; i < size; i += 3)
    {
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;
    }


    printf("pixel(40, 40) %d - %d - %d", data[3 * (40 * width + 40)], data[3 * (40 * width + 40) + 1], data[3 * (40 * width + 40) + 2]);

    return data;
}