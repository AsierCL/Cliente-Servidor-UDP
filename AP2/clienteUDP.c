#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <archivo> <puerto propio> <IP servidor> <puerto servidor>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int puerto_cliente = atoi(argv[2]);
    char *ip = argv[3];
    int puerto_servidor = atoi(argv[4]);
    int cliente_fd;
    struct sockaddr_in servidor_addr, local_addr;
    char buffer[BUFFER_SIZE];
    int n;
    FILE *inputFile, *outputFile;
    char inputFileName[100], outputFileName[100], line[BUFFER_SIZE];
    strcpy(inputFileName, argv[1]);
    strcpy(outputFileName, argv[1]); // Copiar la cadena a outputFileName
    for (int i = 0; outputFileName[i] != '\0'; i++) {
        outputFileName[i] = toupper(outputFileName[i]); // Convertir cada caracter a mayúscula
    }

    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("No se pudo abrir el archivo %s\n", inputFileName);
        return 1;
    }

    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("No se pudo crear el archivo %s\n", outputFileName);
        fclose(inputFile);
        return 1;
    }

    // Crear el socket
    if ((cliente_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(puerto_servidor);
    servidor_addr.sin_addr.s_addr = inet_addr(ip);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(puerto_cliente); // Puerto de envío
    local_addr.sin_addr.s_addr = INADDR_ANY; // Dirección IP del cliente

    if (bind(cliente_fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("Error en bind del cliente");
        close(cliente_fd);
        exit(EXIT_FAILURE);
    }

    socklen_t servidor_len = sizeof(servidor_addr);

    // Leer el archivo original línea por línea, convertir a mayúsculas y escribir en el nuevo archivo
    while (fgets(line, sizeof(line), inputFile)) {
        // Escribir la línea convertida en el archivo de salida
        sendto(cliente_fd, line, strlen(line), 0, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr));
        n = recvfrom(cliente_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&servidor_addr, &servidor_len);
            if (n < 0) {
                perror("Error al recibir");
            } else {
                buffer[n] = '\0'; // Asegurar que el buffer sea una cadena
                printf("Mensaje recibido: %s", buffer);
                printf("Número de bytes recibidos: %d\n", n);
                fputs(buffer, outputFile);
            }
        }

    close(cliente_fd); // Cerrar el socket del cliente
    return 0;
}
