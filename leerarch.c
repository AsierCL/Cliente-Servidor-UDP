#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main() {
    FILE *inputFile, *outputFile;
    char inputFileName[100], outputFileName[100], line[1024];

    // Pedir el nombre del archivo
    printf("Ingresa el nombre del archivo: ");
    scanf("%s", inputFileName);

    // Abrir el archivo original en modo lectura
    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("No se pudo abrir el archivo %s\n", inputFileName);
        return 1;
    }

    // Crear el nombre del archivo de salida (con el sufijo "_MAYUS")
    snprintf(outputFileName, sizeof(outputFileName), "%s_MAYUS.txt", inputFileName);

    // Abrir el archivo de salida en modo escritura
    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("No se pudo crear el archivo %s\n", outputFileName);
        fclose(inputFile);
        return 1;
    }

    // Leer el archivo original línea por línea, convertir a mayúsculas y escribir en el nuevo archivo
    while (fgets(line, sizeof(line), inputFile)) {
        // Convertir la línea a mayúsculas
        for (int i = 0; line[i] != '\0'; i++) {
            line[i] = toupper(line[i]);
        }
        // Escribir la línea convertida en el archivo de salida
        fputs(line, outputFile);
    }

    // Cerrar los archivos
    fclose(inputFile);
    fclose(outputFile);

    printf("El archivo ha sido convertido a mayúsculas y guardado como %s\n", outputFileName);

    return 0;
}
