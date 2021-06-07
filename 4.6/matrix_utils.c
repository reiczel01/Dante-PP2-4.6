#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "matrix_utils.h"

void matrix_destroy(struct matrix_t* m) {
    if (m != NULL && m->height > 0) {
        if (m->ptr != NULL) {
            for (int i = 0; m->height > i; i++) {
                if (*(m->ptr + i) != NULL) {
                    free(*(m->ptr + i));
                }
            }
            free(m->ptr);

        }
    }
}
void matrix_destroy_struct(struct matrix_t** m) {
    if (m != NULL) {
        matrix_destroy(*m);
        free(*m);
        *m = NULL;
    }
}
int matrix_create(struct matrix_t* m, int width, int height) { //https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
    if (m == NULL || width <= 0 || height <= 0) {
        return 1;
    }
    m->height = height;
    m->width = width;
    m->ptr = NULL;
    m->ptr = (int**)calloc(height, sizeof(int*)); //rzutownaie na wsak¿nik na wskaŸnik plus alokacaj pamiêi dla kolejnych wierszy * iloœæ pamiêci o wielkoœci wska¿nika int
    if (m->ptr == NULL) {
        return 2;
    }
    for (int i = 0; height > i; i++) {
        *(m->ptr + i) = NULL;
        *(m->ptr + i) = calloc(width, sizeof(int));
        if (*(m->ptr + i) == NULL) {
            matrix_destroy(m);
            return 2;
        }
    }
    return 0;
}
struct matrix_t* matrix_create_struct(int width, int height) {
    struct matrix_t* m = calloc(1, sizeof(struct matrix_t));
    if (m == NULL) {
        return NULL;
    }
    int retVal = matrix_create(m, width, height);
    if (retVal != 0) {
        free(m);
        return NULL;
    }
    else {
        return m;
    }
}
int matrix_read(struct matrix_t* m) {
    if (m == NULL || m->height <= 0 || m->width <= 0 || m->ptr == NULL) {
        return 1;
    }
    printf("Podaj wartoœci: ");
    for (int i = 0; m->height > i; i++) {
        for (int j = 0; m->width > j; j++) {
            int temp;
            int check = scanf("%d", &temp);
            if (check != 1) {
                return 2;
            }
            *(*(m->ptr + i) + j) = temp;
        }
    }
    return 0;
}

struct matrix_t* matrix_transpose(const struct matrix_t* m) {
    if (m == NULL || m->ptr == NULL) {
        return NULL;
    }
    struct matrix_t* transpose;
    transpose = matrix_create_struct(m->height, m->width);
    if (transpose == NULL) {
        matrix_destroy_struct(&transpose);
        return NULL;
    }
    for (int i = 0; i < m->height; ++i) {
        for (int j = 0; j < m->width; ++j) {
          
                *(*(transpose->ptr + j) + i) = *(*(m->ptr + i) + j);
                
           
        }
    }
    return transpose;
}

void matrix_display(const struct matrix_t* m) {
    if (m == NULL || m->height <= 0 || m->width <= 0 || m->ptr == NULL) {
        return;
    }
    for (int i = 0; m->height > i; i++) {
        if (*(m->ptr + i) != NULL) {
            for (int j = 0; m->width > j; j++) {
                int temp = *(*(m->ptr + i) + j);
                printf("%d ", temp);
            }
            printf("\n");
        }
    }
}

int matrix_save_b(const struct matrix_t* m, const char* filename) {
    if (filename == NULL || m == NULL || m->ptr == NULL || m->height <= 0 || m->width <= 0) {
        return 1;
    }
    /*int totChar = 0;
    for (int i = 0; *(filename+i) != '\0'; i++) {
        if (*(filename+i) != ' '){
            totChar++;
        }
    }*/
    /*if (m->height == 1 && m->width == 1) {
        return 1;
    }*/
    /*if (totChar > 30) {
        return 1;
    }*/
    FILE* f = fopen(filename, "wb");

    if (f == NULL) {
        return 2;
    }

    fwrite(&m->width, sizeof(unsigned int), 1, f);
    fwrite(&m->height, sizeof(unsigned int), 1, f);
    
    for (int i = 0; i < m->height; i++) {
       
        fwrite(*(m->ptr + i), sizeof(int), m->width, f);
    }
 
    fclose(f);
    return 0;
}

int matrix_save_t(const struct matrix_t* m, const char* filename) {
    if (filename == NULL || m == NULL || m->ptr == NULL || m->height <= 0 || m->width <= 0) {
        return 1;
    }
    FILE* f = fopen(filename, "w");

    if (f == NULL) {
        return 2;
    }
    fprintf(f, "%d %d\n", m->width, m->height);
    //printf("Saving rectangles to file = %s\n", filename);
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            int temp = *(*(m->ptr + i) + j);
            fprintf(f, "%d ", temp);
       }
        fprintf(f, "\n");
    }

    fclose(f);
    return 0;
}

struct matrix_t* matrix_copy(const struct matrix_t* copy)
{
    if (copy == NULL) {
        return NULL;
    }
    struct matrix_t* deep_copy;
    deep_copy = matrix_create_struct(copy->width, copy->height);
    if (deep_copy == NULL) {
        matrix_destroy_struct(&deep_copy);
        return NULL;
    }
    for (int i = 0; i < copy->height; ++i) {
        for (int j = 0; j < copy->width; ++j) {

            *(*(deep_copy->ptr + i) + j) = *(*(copy->ptr + i) + j);


        }
    }
    return deep_copy;
}



struct matrix_t* matrix_load_t(const char* filename, int* err_code) {
    if (filename == NULL) {
        if (err_code != NULL) {
            *err_code = 1;
        }
        return NULL;
    }

    FILE* f = fopen(filename, "rb");
    if (f == NULL) {
        if (err_code != NULL) {
            *err_code = 2;
        }
        return NULL;
    }
    int width = 0, height = 0;
    if (fscanf(f, "%d", &width) != 1) {
        if (err_code != NULL) {
            *err_code = 3;
        }
        return NULL;
    }
    if (width < 0) {
        if (err_code != NULL) {
            *err_code = 3;
        }
        return NULL;
    }
    if (fscanf(f, "%d", &height) != 1) {
        if (err_code != NULL) {
            *err_code = 3;
        }
        return NULL;
    }
    if (height < 0) {
        if (err_code != NULL) {
            *err_code = 3;
        }
        return NULL;
    }
    struct matrix_t* m;
    m = matrix_create_struct(width, height);
    int retVal = 0;
    if (m != NULL) {
        for (size_t i = 0; i < height; i++)
        {
            for (size_t j = 0; j < width; j++)
            {
                if (fscanf(f, "%d", &retVal) != 1) {
                    matrix_destroy_struct(&m);
                    if (err_code != NULL) {
                        *err_code = 3;
                    }
                    return NULL;
                }
                *(*(m->ptr + i)+j) = retVal;
            }
            
        }
    }
    else {
        if (err_code != NULL) {
            *err_code = 4;
        }
        return NULL;
    }
    return m;
}
struct matrix_t* matrix_add(const struct matrix_t* m1, const struct matrix_t* m2)
{
    if (m1 == NULL || m2 == NULL) {
        return NULL;
    }
    if (m1->height != m2->height || m1->width != m2->width) {
        return NULL;
    }
    int height = m1->height, width = m1->width;
    struct matrix_t* m = matrix_create_struct(m1->width, m2->height);
    if (m == NULL) {
        return NULL;
    }
    else {
        for (size_t i = 0; i < height; i++)
        {
            for (size_t j = 0; j < width; j++)
            {
                *(*(m->ptr + i) + j) = *(*(m1->ptr + i) + j) + *(*(m2->ptr + i) + j);
            }

        }
    }
    return m;
}
struct matrix_t* matrix_subtract(const struct matrix_t* m1, const struct matrix_t* m2) {
    if (m1 == NULL || m2 == NULL) {
        return NULL;
    }
    if (m1->height != m2->height || m1->width != m2->width) {
        return NULL;
    }
    int height = m1->height, width = m1->width;
    struct matrix_t* m = matrix_create_struct(m1->width, m2->height);
    if (m == NULL) {
        return NULL;
    }
    else {
        for (size_t i = 0; i < height; i++)
        {
            for (size_t j = 0; j < width; j++)
            {
                *(*(m->ptr + i) + j) = *(*(m1->ptr + i) + j) - *(*(m2->ptr + i) + j);
            }

        }
    }
    return m;
}
struct matrix_t* matrix_multiply(const struct matrix_t* m1, const struct matrix_t* m2)
{
    if (m1 == NULL || m2 == NULL) {
        return NULL;
    }
    if (m1->height != m2->width || m1->width != m2->height) {
        return NULL;
    }
    int height = m1->height, width = m1->width;
    struct matrix_t* m = matrix_create_struct(m1->width, m2->height);
    if (m == NULL) {
        return NULL;
    }
    for (int i = 0; i < m1->height; ++i) {
        for (int j = 0; j < m2->width; ++j)
            for (int k = 0; k < m1->width; ++k)
            {
                *(*(m->ptr + i) + j) += *(*(m1->ptr + i) + k) * *(*(m2->ptr + k) + j);
            }
    }
    return m;
}
struct matrix_t* matrix_load_b(const char* filename, int* err_code) {
    if (filename == NULL) {
        if (err_code != NULL) {
            *err_code = 1;
        }
        return NULL;
    }

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        if (err_code != NULL) {
            *err_code = 2;
        }
        return NULL;
    }
    int width = 0, height = 0;
    if (fread(&width, sizeof(int), 1, f) != 1) {
        if (err_code != NULL) {
            *err_code = 3;
        }
        return NULL;
    }
    if (width < 0) {
        if (err_code != NULL) {
            *err_code = 3;
        }
        return NULL;
    }
    if (fread(&height, sizeof(int), 1, f) != 1) {
        if (err_code != NULL) {
            *err_code = 3;
        }
        return NULL;
    }
    if (height < 0) {
        if (err_code != NULL) {
            *err_code = 3;
        }
        return NULL;
    }
    struct matrix_t* m;
    m = matrix_create_struct(width, height);
    if (m != NULL) {
        for (size_t i = 0; i < height; i++)
        {
            if (fread(*(m->ptr + i), sizeof(int), width, f) != width) {
                matrix_destroy_struct(&m);
                if (err_code != NULL) {
                    *err_code = 3;
                }
                return NULL;
            }
        }
    }
    else {
        if (err_code != NULL) {
            *err_code = 4;
        }
        return NULL;
    }
    return m;
}
