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
#include <windows.h>
#include <winsock2.h>
#include <process.h>    // Necessário para função _beginthread()
#include <time.h>       // Necessário para função time()

#define BUFFER_SIZE 128
#define EXIT_CALL_STRING "sair"

int message_length = 0, monitora_length = 0;

unsigned short remote_port = 0, randomNum;

char remote_ip[32];
char mensagem[BUFFER_SIZE], monitora[BUFFER_SIZE];

SOCKADDR_IN remote_address;

SOCKET remote_socket = 0;

WSADATA wsa_data;

void loop(void*);

void erro(char *msg)
{
    fprintf(stderr, msg);
    system("PAUSE");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        erro("Erro Winsock\n");
    }

    //! Solicita o IP e a Porta do servidor
    printf("IP do servidor: ");
    scanf("%s", remote_ip);
    fflush(stdin);

    printf("Porta do servidor: ");
    scanf("%d", &remote_port);
    fflush(stdin);

    remote_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (remote_socket == INVALID_SOCKET)
    {
        WSACleanup();
        erro("Erro Socket\n");
    }

    memset(&remote_address, 0, sizeof(remote_address));
    remote_address.sin_family = AF_INET;
    remote_address.sin_addr.s_addr = inet_addr(remote_ip);
    remote_address.sin_port = htons(remote_port);

    printf("Conectando-se ao servidor %s...\n", remote_ip);
    if (connect(remote_socket, (SOCKADDR *)&remote_address, sizeof(remote_address)) == SOCKET_ERROR)
    {
        WSACleanup();
        erro("Falha\n");
    }

    printf("Digite as mensagens:\n");

    //! Inicia a função paralela à função main
    _beginthread(loop, 0, NULL);

    do  //! Envia o que é digitado no teclado
    {
        //!Pega o que foi digitado no teclado
        printf("PC-01: ");
        gets(mensagem);
        fflush(stdin); //! Limpa o buffer do teclado

        //! Envia o que foi digitado para o servidor
        message_length = strlen(mensagem);
        send(remote_socket, mensagem, message_length, 0);
    }
    while(strcmp(mensagem, EXIT_CALL_STRING));  //! Até que o usuário digite "sair" (sem aspas)

    printf("\n\nEncerrando...\n\n");
    WSACleanup();
    closesocket(remote_socket);

    system("PAUSE");
    return 0;
}

void loop(void*)    // Função paralela à função main
{
    //!Random
    srand(time(NULL));

    //!Loop para envio da variável monitora
    for(;;)
    {
        randomNum = rand() % ((130 + 1 - 40) + 40); //rand() % (max_number + 1 - minimum_number) + minimum_number

        //! Printa a velocidade a ser enviada
        printf("Velocidade: %dkm/h\n", randomNum);
        printf("PC-01: ");

        //! Envia para o servidor a última velocidade detectada pelo "sensor" (são valores simulados, e o nosso simula um sensor de velocidade que um carro passou pelo sensor)
        sprintf(monitora, "Velocidade: %dkm/h", randomNum);
        monitora_length = strlen(monitora);
        send(remote_socket, monitora, monitora_length, 0);

        //! Limpa o buffer
        memset(&monitora, 0, BUFFER_SIZE);

        Sleep(15000);   // Aguarda 15s para enviar novamente os dados
    }
}
