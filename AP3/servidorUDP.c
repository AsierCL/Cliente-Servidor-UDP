#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Uso: %s <puerto>\n", argv[0]);
        exit(1);
    }

    int puerto = atoi(argv[1]);  // Tomar el puerto desde los argumentos
    int servidor_fd, cliente_fd, n;
    struct sockaddr_in servidor_addr, cliente_addr;
    socklen_t cliente_len = sizeof(cliente_addr);
    char buffer[BUFFER_SIZE];

    // Crear el socket
    if((servidor_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Error al crear el socket");
        exit(1);
    }

    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(puerto);
    servidor_addr.sin_addr.s_addr = htons(INADDR_ANY); // Aceptar conexiones de cualquier IP

    // Enlazar el socket al puerto
    if(bind(servidor_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) < 0){
        perror("Error en el bind");
        close(servidor_fd);
        exit(1);
    }

    printf("Servidor escuchando en el puerto %d\n", puerto);

    while(1) {
        // Recibir datos del cliente
        while((n = recvfrom(servidor_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliente_addr, &cliente_len)) > 0) {
            buffer[n] = '\0';  // Asegurar el final del string
            printf("Mensaje recibido: %s", buffer);
            sleep(1);
            // Convertir el mensaje a mayúsculas
            for(int i = 0; buffer[i] != '\0'; i++) {
                buffer[i] = toupper(buffer[i]);
            }

            // Enviar el mensaje de vuelta al cliente
            sendto(servidor_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&cliente_addr, cliente_len);
        }

        if(n < 0) {
            perror("Error al recibir datos");
        }
    }

    close(servidor_fd);
    return 0;
}