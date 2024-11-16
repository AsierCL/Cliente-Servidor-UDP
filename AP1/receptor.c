#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int puerto = atoi(argv[1]);
    int socket_fd;
    struct sockaddr_in servidor_addr, cliente_addr;
    char buffer[BUFFER_SIZE];
    int n;

    // Crear el socket
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(puerto);
    servidor_addr.sin_addr.s_addr = INADDR_ANY;



    if(bind(socket_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr))<0){
        perror("Error bind\n");
        close(socket_fd);
        exit(1);
    }

    socklen_t cliente_len = sizeof(cliente_addr);

    // Recibir el mensaje del servidor
    printf("Entramos en el bucle\n\n");
    while(1){
        n = recvfrom(socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliente_addr, &cliente_len);
        if (n < 0) {
            perror("Error al recibir");
            break;
        }
        buffer[n] = '\0'; // Asegurar que el buffer sea una cadena
        printf("Mensaje recibido: %s", buffer);
        printf("Número de bytes recibidos: %d\n", n);
        printf("Recibido de %s:%d\n", inet_ntoa(cliente_addr.sin_addr), ntohs(cliente_addr.sin_port));
    }

    close(socket_fd); // Cerrar el socket del cliente
    return 0;
}
