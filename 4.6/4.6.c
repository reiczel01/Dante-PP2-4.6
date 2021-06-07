#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "matrix_utils.h"


int main()
{


    //testy:
    /*
    struct matrix_t arr = { .width = 0, .height = -5 };

    int array[1][1] = { { 1} };

    int res = matrix_create(&arr, 1, 1);
    for (int i = 0; i < 1; ++i)
        for (int j = 0; j < 1; ++j)
            arr.ptr[i][j] = array[i][j];
    res = matrix_save_b(&arr, "mostwhereneversteambeautypassseetownmultiplystoodif");
    if (res != 0) {
        printf("Funkcja matrix_save_b() powinna zwrócić kod błędu 0, a zwróciła %d", res);
    }
    */
    //koniec testu
    struct matrix_t* matrix;
    int check = 0;
    int height = 0;
    int width = 0;
    printf("Podaj szerokosc i wysokosc: ");
    check = scanf("%d %d", &width, &height);
    if (check != 2) {
        printf("incorrect input");
        return 1;
    }

    //matrix.width = -3;
    //matrix.height = 7;
    if (width <= 0 || height <= 0) {
        printf("incorrect input data");
        return 2;
    }
    matrix = matrix_create_struct(width, height);
    if (matrix == NULL) {
        printf("Failed to allocate memory");
        return 8;
    }

    check = matrix_read(matrix);
    if (check == 1) {
        printf("incorrect input data");
        matrix_destroy_struct(&matrix);
        return 8;
    }
    else if (check == 2) {
        printf("incorrect input");
        matrix_destroy_struct(&matrix);
        return 1;
    }

    char* filename;
    filename = (char*)calloc(40, sizeof(char));

    if (filename == NULL) {
        printf("Failed to allocate memory");
        matrix_destroy_struct(&matrix);

        return 8;
    }
    matrix_display(matrix);
    printf("Podaj nazwe pliku: ");
    int checkChar = scanf("%39s", filename);

    if (checkChar != 1) {
        printf("incorrect input");

        matrix_destroy_struct(&matrix);

        free(filename);
        return 1;
    }
    if (strlen(filename) < 4) {
        printf("Unsupported file format");
        free(filename);

        matrix_destroy_struct(&matrix);

        return 7;
    }
    char* ext;
    ext = (char*)calloc(5, sizeof(char));
    if (ext == NULL) {
        printf("Failed to allocate memory");
        matrix_destroy_struct(&matrix);
        free(filename);
        return 8;
    }
    struct matrix_t* transpose = matrix_transpose(matrix);
    if (transpose == NULL) {
        printf("Failed to allocate memory");
        matrix_destroy_struct(&matrix);


        free(ext);
        free(filename);
        return 8;
    }

    strcpy(ext, filename + (strlen(filename) - 4));
    if (strcmp(ext, ".bin") == 0) {
        check = matrix_save_b(transpose, filename);
        if (check == 2) {
            printf("Couldn't create file");
            free(filename);
            free(ext);
            matrix_destroy_struct(&matrix);
            matrix_destroy_struct(&transpose);
            return 5;
        }
        else if (check == 1) {
            printf("Incorrect input data");
            free(filename);
            free(ext);
            matrix_destroy_struct(&matrix);
            matrix_destroy_struct(&transpose);
            return 8;
        }
    }
    else if (strcmp(ext, ".txt") == 0) {
        check = matrix_save_t(transpose, filename);
        if (check == 2) {
            printf("Couldn't create file");
            free(filename);
            free(ext);
            matrix_destroy_struct(&matrix);
            matrix_destroy_struct(&transpose);
            return 5;
        }
        else if (check == 1) {
            printf("Incorrect input data");
            free(filename);
            free(ext);
            matrix_destroy_struct(&matrix);
            matrix_destroy_struct(&transpose);
            return 8;
        }
    }
    else {
        printf("Unsupported file format");
        free(filename);
        free(ext);
        matrix_destroy_struct(&matrix);
        matrix_destroy_struct(&transpose);
        return 7;
    }

    matrix_display(matrix_add(matrix_load_t(filename, NULL), matrix_load_t(filename, NULL)));
    printf("File saved");
    matrix_destroy_struct(&transpose);
    matrix_destroy_struct(&matrix);
   
    free(ext);
    free(filename);
    return 0;
}
