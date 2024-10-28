#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <IP> <puerto>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip = argv[1];
    int puerto = atoi(argv[2]);
    int socket_fd;
    struct sockaddr_in servidor_addr;
    char buffer[BUFFER_SIZE];
    int n;

    // Crear el socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(puerto);
    inet_pton(AF_INET, ip, &servidor_addr.sin_addr);

    // Conectar al servidor
    if (connect(socket_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) < 0) {
        perror("Error al conectar");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }


    // Recibir el mensaje del servidor
    while((n = recv(socket_fd, buffer, BUFFER_SIZE,0))>0){
        //n = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0);
        if (n < 0) {
            perror("Error al recibir");
        } else {
            buffer[n] = '\0'; // Asegurar que el buffer sea una cadena
            printf("Mensaje recibido: %s", buffer);
            printf("Número de bytes recibidos: %d\n", n);
        }
    }

    close(socket_fd); // Cerrar el socket del cliente
    return 0;
}
