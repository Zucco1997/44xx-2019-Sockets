/*!
                SERVIDOR UDP
        Integrantes:    Cássio F. Braga,
                        Hiago Silva,
                        Nathan Nogara,
                        Vítor Becker,
                        Maksine Baldin, 
						Thalia Schwaab

        Turma: 4422
        Prof.: Marcos Zuccolotto

                 FONTES:
    https://docs.microsoft.com/pt-br/cpp/c-runtime-library/reference/beginthread-beginthreadex?view=vs-2019
    https://www.hardware.com.br/comunidade/tutorial-simples/1252370/
    http://www.professordiovani.com.br/sd/Sockets_cplus.htm
    https://armantutorial.wordpress.com/2014/07/23/winsock-tutorial-socket-programming-in-c-on-windows-using-codeblock/
    http://etutorials.org/Programming/Pocket+pc+network+programming/Chapter+1.+Winsock/Connectionless+UDP+Sockets/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "pomoc.cpp"

#define BACKLOG_MAX 5
#define BUFFER_SIZE 128
#define EXIT_CALL_STRING "#quit"

int remote_length = 0;
int message_length = 0;

int sizeMessage = sizeof(SOCKADDR_IN);

unsigned short local_port = 0;
unsigned short remote_port = 0;

char message[BUFFER_SIZE];

WSADATA wsa_data;

SOCKET remote_socket, local_socket;
SOCKADDR_IN local_address, remote_address;

void cores(char cor);

//! Exibe uma mensagem de erro e termina o programa
void msg_err_exit(char *msg)
{
    fprintf(stderr, msg);
    system("PAUSE");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    //!Verifica se há algum erro no Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        msg_err_exit("Houve falha no WSAStartup().\n");
    }

    //!Declara e aloca espaço para SockAddr
    memset(&local_address, 0, sizeof(SOCKADDR_IN));
    memset(&remote_address, 0, sizeof(SOCKADDR_IN));

    //!Conecta o Socket
    local_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(local_socket == INVALID_SOCKET)
    {
        int iSocketError = WSAGetLastError();
        msg_err_exit("Houve falha no socket().\n");
        return FALSE;
    }

    //!Define a porta local
    printf("Porta local: ");
    scanf("%d", &local_port);

    //!Define parâmetros para SockAddr
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(local_port);
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(local_socket, (SOCKADDR *)&local_address, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
        int iSocketError = WSAGetLastError();
        closesocket(local_socket);
        msg_err_exit("Houve falha no bind().\n");
        return FALSE;
    }

    printf("\nSERVER ON!\n");

    printf("Esperando dados...\n");

    while(1)
    {
        memset(message, 0, BUFFER_SIZE);

        //!Tenta receber algum arquivo, com uma chamada de retenção
		if ((remote_length = recvfrom(local_socket, message, BUFFER_SIZE, 0, (SOCKADDR *)&remote_address, &sizeMessage)) == SOCKET_ERROR)
		{
			printf("Houve falha no recvfrom(). Codigo de erro: %d\n", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		//!Printa detalhes do cliente and the data received
		printf("Pacote recebido de: %s:%d\n", inet_ntoa(remote_address.sin_addr), ntohs(remote_address.sin_port));

        if(remote_length == 1)
        {
            cores(message[0]);
        }

		printf("Dado: %s\n", message);
    }

    //!Fecha o socket
    closesocket(local_socket);
    WSACleanup();
}

void cores(char cor)
{
    switch(cor)
    {
        case 'r':
            textcolor(LIGHT_RED);
        break;

        case 'g':
            textcolor(LIGHT_GREEN);
        break;

        case 'b':
            textcolor(LIGHT_BLUE);
        break;

        case 'R':
            textcolor(VERMELHO);
        break;

        case 'G':
            textcolor(VERDE);
        break;

        case 'B':
            textcolor(AZUL);
        break;

        case 'y':
            textcolor(YELLOW);
        break;

        case 'w':
            textcolor(WHITE);
        break;

        default:
            return;
    }
}
