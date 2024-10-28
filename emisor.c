#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BUFFER 1024

int main(int argc, char** argv) {

    if (argc != 4) {
        fprintf(stderr, "Uso: %s <puerto_local> <IP_servidor> <puerto_servidor>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int puerto_cliente = atoi(argv[1]);
    char *ip = argv[2];
    int puerto = atoi(argv[3]);
    
    int cliente_fd;
    struct sockaddr_in servidor_addr, local_addr;
    char buffer[BUFFER];
    
    if((cliente_fd = socket(AF_INET, SOCK_DGRAM, 0))<0){
        perror("Error socket");
        exit(1);
    }
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_addr.s_addr = inet_addr(ip);
    servidor_addr.sin_port = htons(puerto);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(puerto_cliente); // Puerto de envío
    local_addr.sin_addr.s_addr = INADDR_ANY; // Dirección IP del cliente

    if (bind(cliente_fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("Error en bind del cliente");
        close(cliente_fd);
        exit(EXIT_FAILURE);
    }
    
    while(1){
        char *mensaje = "Primer mensaje\n";
        sendto(cliente_fd, mensaje, strlen(mensaje), 0, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr));
        
        mensaje = "Segundo mensaje.\n";
        sendto(cliente_fd, mensaje, strlen(mensaje), 0, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr));
        sleep(2);
    }

    close(cliente_fd);

    return 0;
}