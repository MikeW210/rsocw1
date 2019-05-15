#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

#define PORT 9734
#define bufSize 100
// AF_INET - Ipv4 format
//SOCK_STREAM - TCP
// 0 - IP
// SOMAXCONN - max number of connections
using namespace std;

double ifBig(double value)
{
    char *b = (char *)&value;
    if (b[0])
    {
        cout << "little endian " << endl;
        value = htonl(value);
    }
    else
    {
        cout << "big endian " << endl;
    }
    return value;
}

int main(){

    // create a socekt [int socket(int domain, int type, int protocol)];
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "Nie mozna stworzyc socketu";
        return -1;
    }
    // bind the socket to a IP/PORT
    sockaddr_in between; // it is a bufer that will be used in comunication between client and server
    between.sin_family = AF_INET;
    between.sin_port = htons(PORT);                   //htons converts the unsigned short integer hostshort from host byte order to network byte order.
    socklen_t size = sizeof(between);
    inet_pton(AF_INET, "127.0.0.1", &between.sin_addr); //it's converts a number to an array of integers

    int binded = bind(serverSocket, (sockaddr *)&between, size);
    if (binded == -1){
        cerr << "Nie mozna zbindowac sie";
        return -2;
    }

    int listened = listen(serverSocket, SOMAXCONN); // mark the socket for listening in
    

    if (listened == -1){
        cerr << "Blad";
        return -3;
    }

    // accept 
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    int client_soc = accept(serverSocket, (sockaddr *)&client, &clientSize);
    if (client_soc == -1){
        cerr << "Nie mozna polaczyc sie z klientem!";
        return -4;
    }
    close(serverSocket);
    cout << "Server dziala!" << endl;

    // while receving display echo message
    char buf[bufSize];
    char buf2[bufSize];
    while (true)
    {
        // clear the buf
        memset(buf, 0, bufSize);
        // wait for message
        int bytesRecv = recv(client_soc, buf, bufSize, 0); // receives data from a connected socket or a bound connectionless socket.

        if (bytesRecv <= 0)
        {
            cerr << "Something went wrong" << endl;
            break;
        }

        // get time of request
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        stringstream s;
        s << put_time(localtime(&in_time_t), "%Y-%m-%d %X");
        string date_time= s.str();
        
        string msg;    

        if (buf[0] == '0')
        {
            msg = "Data: " + date_time;
        }
        else if (buf[0] == '1')
        {
            msg = " Zle podane dane nie mozna spierwiastkowac "; /// initial value to send displays wrong input message

            int bytesRecv2 = recv(client_soc, buf2, bufSize, 0);
            if (bytesRecv2 <= 0)
            {
                cerr << "Cos nie dziala" << endl;
                break;
            }

            double value;
            string input = "";
            bool is_dot = false;
            bool correct = true;

            //check if number in correct order
            for (int i = 0; i < bytesRecv2 - 1; i++)
            {
                if (buf2[i] >= 48 && buf2[i] <= 57)
                {
                    input += buf2[i];
                }

                else if (buf2[i] == '.' && !is_dot)
                {
                    input += buf2[i];
                    is_dot = true;
                }

                else
                {
                    correct = false;
                    break;
                }
            }

            if (correct)
            {
                value = atof(input.c_str());
                value = ifBig(value);
                //user_value = ntohl(user_value);
                cout << "Dane od klienta: " << value << endl;

                value = sqrt(value);
                msg = "Liczba odesana: "+to_string(value);
            }
        }
        else
        {
            msg = "Zle dane";
        }

        int message_len = msg.length();

        memset(buf, 0, bufSize);
        for (int x = 0; x < message_len; x++)
        {
            buf[x] = msg[x];
        }
        //send rsponse
        send(client_soc, buf, message_len + 1, 0);
    }
    // close socket
    close(client_soc);
    return 0;
}
/*
double reverseValue(const char *data)
{
    double result;

    char *dest = (char *)&result;

    for (int i = 0; i < sizeof(double); i++)
    {
        dest[i] = data[sizeof(double) - i - 1];
    }
    return result;
}

int32_t swap_int32(int32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | ((val >> 16) & 0xFFFF);
}*/
