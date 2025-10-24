// Oriana Mora Lopez
// B75079

#include <stdio.h>

// Imprime una matriz rows x cols usando SOLO punteros
void imprimir_matriz(const int *m, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            // Dirección del elemento (i,j) = base + (i*cols + j)
            const int *p = m + (i * cols) + j;
            printf("%d%s", *p, (j == cols - 1) ? "" : "\t");
        }
        printf("\n");
    }
}


// Copia matriz -> arreglo (aplanar) usando punteros
void matriz_a_arreglo(const int *m, int rows, int cols, int *out) {
    int n = rows * cols;
    int k;
    for (k = 0; k < n; k++) {
        // recorro linealmente la matriz (row-major)
        const int *src = m + k;
        int *dst = out + k;
        *dst = *src;
    }
}

// Copia arreglo -> matriz usando punteros (misma forma lineal)
void arreglo_a_matriz(const int *a, int rows, int cols, int *m) {
    int n = rows * cols;
    int k;
    for (k = 0; k < n; k++) {
        const int *src = a + k;
        int *dst = m + k;
        *dst = *src;
    }
}

// BubbleSort simple usando punteros (ordena de menor a mayor)
void bubble_sort(int *a, int n) {
    int i, j;
    if (n < 2) return;
    for (i = 0; i < n - 1; i++) {
        int hubo_intercambio = 0; // optimización simple: si no se intercambió nada, ya está ordenado
        for (j = 0; j < n - 1 - i; j++) {
            int *p = a + j;     // elemento j
            int *q = a + j + 1; // elemento j+1 
            if (*p > *q) {
                int tmp = *p;
                *p = *q;
                *q = tmp;
                hubo_intercambio = 1;
            }
        }
        if (!hubo_intercambio) {
            // Si en una pasada no hubo cambios, ya está ordenado
            break;
        }
    }
}

// Demostración completa con una matriz cualquiera
void demo(const int *matriz_inicial, int rows, int cols) {
    int n = rows * cols;

    /* Arreglos automáticos del tamaño exacto (VLA) para mantenerlo simple */
    int plano[n];
    int reconstruida[n];

    printf("Matriz original (%dx%d):\n", rows, cols);
    imprimir_matriz(matriz_inicial, rows, cols);

    // 1) Matriz -> arreglo
    matriz_a_arreglo(matriz_inicial, rows, cols, plano);

    // 2) Ordenar (BubbleSort)
    bubble_sort(plano, n);

    // 3) Arreglo -> matriz (reconstruir con las mismas dimensiones)
    arreglo_a_matriz(plano, rows, cols, reconstruida);

    // 4) Imprimir la matriz reconstruida (ya ordenada)
    printf("Matriz reconstruida y ordenada (%dx%d):\n", rows, cols);
    imprimir_matriz(reconstruida, rows, cols);

    printf("--------------------------------------------------\n");
}

int main(void) {
    // Caso 1: 2x3
    const int m1[2][3] = {
        { 5, 1, 9 },
        { 3, 2, 8 }
    };
    demo(&m1[0][0], 2, 3);

    // Caso 2: 3x3
    const int m2[3][3] = {
        { 7, -1, 4 },
        { 0, 12, 3 },
        { 9,  2, 2 }
    };
    demo(&m2[0][0], 3, 3);

    // Caso 3: 4x2
    const int m3[4][2] = {
        { 10, 5 },
        { -3, 7 },
        { 6,  6 },
        { 0, 11 }
    };

    demo(&m3[0][0], 4, 2);

    return 0;
}