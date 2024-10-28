#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_CONN 5
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {

    int puerto = 8080;
    int servidor_fd, cliente_fd;
    struct sockaddr_in servidor_addr, cliente_addr;
    socklen_t addr_len = sizeof(cliente_addr);
    char buffer[BUFFER_SIZE];

    // Crear el socket
    if ((servidor_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_addr.s_addr = INADDR_ANY;
    servidor_addr.sin_port = htons(puerto);

    // Enlazar el socket
    if (bind(servidor_fd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) < 0) {
        perror("Error al enlazar el socket");
        close(servidor_fd);
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones
    if (listen(servidor_fd, MAX_CONN) < 0) {
        perror("Error al escuchar");
        close(servidor_fd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d...\n", puerto);

    while (1) {
        // Aceptar conexión de un cliente
        if ((cliente_fd = accept(servidor_fd, (struct sockaddr *)&cliente_addr, &addr_len)) < 0) {
            perror("Error al aceptar conexión");
            continue;
        }

        // Imprimir IP y puerto del cliente
        printf("Conexión aceptada de %s:%d\n", inet_ntoa(cliente_addr.sin_addr), ntohs(cliente_addr.sin_port));

        // Enviar mensaje de saludo al cliente
        char *mensaje = "¡Hola, cliente!\n";
        send(cliente_fd, mensaje, strlen(mensaje), 0);
        
        // Enviar otro mensaje
        mensaje = "Este es un segundo mensaje.\n";
        send(cliente_fd, mensaje, strlen(mensaje), 0);
        
        close(cliente_fd); // Cerrar la conexión con el cliente
    }

    close(servidor_fd); // Cerrar el socket del servidor
    return 0;
}
