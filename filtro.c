#include "stdio.h"
#include "stdlib.h"
#include "energia.h" 

#define INPUT_FILENAME "input.ppm"
#define OUTPUT_FILENAME "output.ppm"
#define OUTPUT_FILENAME2 "output2.ppm"

int main() {
    rapl_init();

    FILE* inputFile = NULL;
    FILE* outputFile = NULL;
    FILE* outputFile2 = NULL;
    int heigth, width, maximun, nothing, countH, countW = 0;
    unsigned char grayscaleColor, auxR, auxG, auxB, pixel, pixelR, pixelG, pixelB = 0;
    unsigned char** image;

    start_rapl_sysfs();
    clock_t t = clock();

    /* Read input file */
    inputFile = fopen(INPUT_FILENAME, "rb");

    if (inputFile == NULL) {
        fprintf(stderr, "Falha ao abrir arquivo '%s'\n", INPUT_FILENAME);
        return 1;
    }

    fseek(inputFile, 2, SEEK_SET);
    fscanf(inputFile, "%d", &width);
    fscanf(inputFile, "%d", &heigth);
    fscanf(inputFile, "%d", &maximun);

    if (maximun != 255) {
        fprintf(stderr, "Valor maximo de pixel não suportado\n");
        return 1;
    }

    /* Write outputfile */
    outputFile = fopen(OUTPUT_FILENAME, "wb");
    outputFile2 = fopen(OUTPUT_FILENAME2, "wb");

    if (outputFile == NULL || outputFile2 == NULL) {
        fprintf(stderr, "Falha ao criar arquivo de saida\n");
        return 1;
    }
 
    image = malloc (heigth * sizeof (unsigned char*));
    for (int i=0; i < heigth; i++) {
        image[i] = malloc (width * sizeof (int)) ;
    }

    fprintf(outputFile, "P6\n");
    fprintf(outputFile, "%d ", width);
    fprintf(outputFile, "%d\n", heigth);
    fprintf(outputFile, "%d\n", maximun);

    fprintf(outputFile2, "P6\n");
    fprintf(outputFile2, "%d ", width);
    fprintf(outputFile2, "%d\n", heigth);
    fprintf(outputFile2, "%d\n", maximun);

    for (countH=0 ; countH<heigth ; countH++) {
        for (countW=0; countW<width ; countW++) {
            if (!feof (inputFile))
                fread(&auxR, sizeof(unsigned char), 1, inputFile);
            if (!feof (inputFile))
                fread(&auxG, sizeof(unsigned char), 1, inputFile);
            if (!feof (inputFile))
                fread(&auxB, sizeof(unsigned char), 1, inputFile);
            grayscaleColor = (unsigned char)((0.3*auxR)+(0.6*auxG)+(0.1*auxB));
            image[countH][countW] = grayscaleColor;
            for (int i=0 ; i<3 ; i++) {
                fwrite(&grayscaleColor, 1, sizeof(grayscaleColor), outputFile);
            }
        }
    }

    for (countH=0 ; countH<heigth ; countH++) {
        for (countW=0; countW<width ; countW++) {
            pixel = image[countH][countW];
            pixelR = pixel*1.3;
            pixelG = pixel*1.6;
            pixelB = pixel*1.1;
            fwrite(&pixelR, 1, sizeof(pixelR), outputFile2);
            fwrite(&pixelG, 1, sizeof(pixelG), outputFile2);
            fwrite(&pixelB, 1, sizeof(pixelB), outputFile2);
        }
    }

    /* free memory */
    for (int i=0; i < heigth; i++)
        free (image[i]) ;
    free (image) ;
    fclose (inputFile);
    fclose (outputFile);
    free(inputFile);
    free(outputFile);

    t = clock() - t;
    double energy = end_rapl_sysfs();
    double tempo = ((double)t)/CLOCKS_PER_SEC;
    printf("Tempo de execucao em segundos: %.5f\n", tempo);
    printf("Energia consumida em Joules:   %.5f\n", energy);

    return 0;
}