#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <string>

#define PORT 9734
#define BUFFER_SIZE 100

using namespace std;

int main(){
    // create socket
    int clientSocket= socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket== -1)
    {
        cerr << "Can't create a socket!";
        return -1;
    }

    sockaddr_in between; // it is a bufer that will be used in comunication between client and server
    between.sin_family = AF_INET;
    between.sin_port = htons(PORT);                    //htons will transle it into bytes
    inet_pton(AF_INET, "127.0.0.1", &between.sin_addr); //it's converts a number to an array of integers //0.0.0.0 will give us any existing ip address
    
    // connect to the server on the socket
    int resp = connect(clientSocket, (sockaddr *)&between, sizeof(sockaddr_in));
    if (resp == -1)
    {
        cerr << "Nie mona poczyc sie z serverem";
        return -4;
    }

    char buffer[BUFFER_SIZE];
    string val;
    string val2;
    while (true){

        cout << "Wpisz 0 aby dosta date:"<<endl;
        cout << "Wpisz 1 aby wyslac liczbe zeby odeslano ci pierwiastek: "<<endl;
        cin >> val;

        // send to server
        // we are adding one because we want to have one special place for 0 that will mean end of user buffer
        int resp = send(clientSocket, val.c_str(), val.size() + 1, 0); 
        if (resp <= 0)
        {
            cerr << "Nie mozna wyslac" << endl;
            continue;
        }

        if (val == "1")
        {
            cout << "Podaj numer aby dostac pierwiastek ";
            cin >> val2;

            int resp2 = send(clientSocket, val2.c_str(), val2.size() + 1, 0);
            if (resp2 <= 0)
            {
                cerr << "Nie mozna wysalc" << endl;
                continue;
            }
        }

        // clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        // wait for response
        int bytesRecv = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        cout << "SERVER: " <<buffer << endl;

    } 

    close(clientSocket);
    return 0;
}
