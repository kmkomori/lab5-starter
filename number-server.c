#include "http-server.h"
#include <string.h>

int num = 0;

char const HTTP_404_NOT_FOUND[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
char const HTTP_200_FOUND[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";

void handle_404(int client_sock, char *path)  {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);

    char response_buff[BUFFER_SIZE];
    snprintf(response_buff, BUFFER_SIZE, "Error 404:\r\nUnrecognized path \"%s\"\r\n", path);
    // snprintf includes a null-terminator

    // TODO: send response back to client?
    write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
    write(client_sock, "404 message to display to user", strlen("404 message to display to user"));

    // write(404, "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n", strlen("HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n"));
    // write(404, "... 404 message to display to user ...", strlen("... 404 message to display to user ..."));
}


void handle_response(char *request, int client_sock) {
    char path[256];

    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line (limit buffer size; sscanf null-terminates)
    if (sscanf(request, "GET %255s", path) != 1) {
        printf("Invalid request line\n");
        return;
    }


    if (strcmp(path,"/shownum") == 0)
    {
	write(client_sock, HTTP_200_FOUND, strlen(HTTP_200_FOUND));

	char str[50] = "Your number is ";
	char num_str[10];

	sprintf(num_str, "%d", num);
	strcat(str, num_str);

	write(client_sock, str, strlen(str));
    }
    else if (strcmp(path, "/increment") == 0)
    {

	write(client_sock, HTTP_200_FOUND, strlen(HTTP_200_FOUND));
	num += 1;

	char str[50] = "Incremented! Your new number is ";
	char num_str[10];

	sprintf(num_str, "%d", num);
	strcat(str, num_str);

	write(client_sock, str, strlen(str));
    }
    else
    {
	handle_404(client_sock, path);
    }
}

int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) { // if called with a port number, use that
        port = atoi(argv[1]);
    }

    start_server(&handle_response, port);
}
