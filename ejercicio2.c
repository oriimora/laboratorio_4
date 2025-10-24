// Oriana Mora Lopez
// B75079

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WORD_BUF 1024     // tamaño máximo de palabra
#define OUTNAME_BUF 1024  // tamaño del nombre de archivo de salida

// Función auxiliar: decide si un char pertenece a una "palabra"
// Aquí consideramos letras, dígitos y guion bajo como parte de palabra.
int es_parte_de_palabra(int c) {
    return isalpha((unsigned char)c) || isdigit((unsigned char)c) || c == '_';
}

int main(int argc, char *argv[]) {
    // Verificar argumentos: programa + 3 args
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <archivo_entrada> <palabra_buscar> <reemplazo>\n", argv[0]);
        return 1;
    }

    const char *in_name = argv[1];
    const char *target  = argv[2];
    const char *repl    = argv[3];

    // Construir nombre de salida: "<entrada>_modificado.txt"
    char out_name[OUTNAME_BUF];
    if (snprintf(out_name, sizeof(out_name), "%s.mod.txt", in_name) >= (int)sizeof(out_name)) {
        fprintf(stderr, "Error: nombre de salida demasiado largo.\n");
        return 1;
    }

    // Abrir archivos
    FILE *fin = fopen(in_name, "r");
    if (!fin) {
        perror("No se pudo abrir archivo de entrada");
        return 1;
    }

    FILE *fout = fopen(out_name, "w");
    if (!fout) {
        perror("No se pudo crear archivo de salida");
        fclose(fin);
        return 1;
    }

    // Buffer para ir armando la palabra actual
    char word[WORD_BUF];
    int idx = 0;  // cuántos chars hay acumulados en 'word'

    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        if (es_parte_de_palabra(ch)) {
            // Estamos dentro de una palabra: acumulamos el char
            if (idx < WORD_BUF - 1) {
                word[idx++] = (char)ch;
            } else {
                // Si la palabra supera el buffer, para mantenerlo simple
                // se vacia lo acumulado y reiniciamos.
                word[idx] = '\0';
                fputs(word, fout);
                idx = 0;
                // Seguimos ignorando el resto de la palabra larga hasta un delimitador.
            }
        } else {
            // Llegó un delimitador (espacio, puntuación, salto de línea)
            if (idx > 0) {
                // Cerramos palabra y decidimos si reemplazar
                word[idx] = '\0';
                if (strcmp(word, target) == 0) {
                    fputs(repl, fout);   // reemplazo
                } else {
                    fputs(word, fout);   // palabra original
                }
                idx = 0; // reiniciar para la próxima palabra
            }
            // Escribir delimitador tal cual para conservar el formato
            fputc(ch, fout);
        }
    }

    // Si el archivo termina justo después de una palabra, procesarla
    if (idx > 0) {
        word[idx] = '\0';
        if (strcmp(word, target) == 0) {
            fputs(repl, fout);
        } else {
            fputs(word, fout);
        }
        idx = 0;
    }

    // Cerrar archivos (muy importante para no dejar descriptores abiertos)
    fclose(fin);
    fclose(fout);

    // Mensaje simple al terminar
    printf("Listo. Salida escrita en: %s\n", out_name);
    return 0;
}