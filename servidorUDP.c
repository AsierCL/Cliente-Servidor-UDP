#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BUFFER 1014

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Uso: %s <puerto>\n", argv[0]);
        exit(1);
    }

    int puerto = atoi(argv[1]);  // Tomar el puerto desde los argumentos
    int servidor_fd, cliente_fd, n;
    struct sockaddr_in servidor_addr, cliente_addr;
    socklen_t cliente_len = sizeof(cliente_addr);
    char buffer[BUFFER];

    // Crear el socket
    if((servidor_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Error al crear el socket");
        exit(1);
    }

    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_addr.s_addr = htons(INADDR_ANY); // Aceptar conexiones de cualquier IP
    servidor_addr.sin_port = htons(puerto);

    // Enlazar el socket al puerto
    if(bind(servidor_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) < 0){
        perror("Error en el bind");
        close(servidor_fd);
        exit(1);
    }

    // Escuchar conexiones entrantes
    if(listen(servidor_fd, 1) < 0){
        perror("Error en el listen");
        close(servidor_fd);
        exit(1);
    }

    printf("Servidor escuchando en el puerto %d\n", puerto);

    while(1) {
        // Aceptar una conexión
        if((cliente_fd = accept(servidor_fd, (struct sockaddr *)&cliente_addr, &cliente_len)) < 0){
            perror("Error al aceptar la conexión");
            close(servidor_fd);
            exit(1);
        }

        printf("Conexión aceptada de %s:%d\n", inet_ntoa(cliente_addr.sin_addr), ntohs(cliente_addr.sin_port));

        // Recibir datos del cliente
        while((n = recv(cliente_fd, buffer, BUFFER, 0)) > 0) {
            buffer[n] = '\0';  // Asegurar el final del string
            printf("Mensaje recibido: %s", buffer);
            sleep(1);
            // Convertir el mensaje a mayúsculas
            for(int i = 0; buffer[i] != '\0'; i++) {
                buffer[i] = toupper(buffer[i]);
            }

            // Enviar el mensaje de vuelta al cliente
            send(cliente_fd, buffer, strlen(buffer), 0);
        }

        if(n < 0) {
            perror("Error al recibir datos");
        }

    }

    close(servidor_fd);
    return 0;
}
