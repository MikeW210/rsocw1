#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

int
main ()
{
	int server_sockfd, client_sockfd;
	socklen_t server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	server_sockfd = socket (AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl (INADDR_ANY);
	server_address.sin_port = htons (9736);
	server_len = sizeof (server_address);
	bind (server_sockfd, (struct sockaddr *) &server_address, server_len);

	/*  Create a connection queue and wait for clients.  */

	listen (server_sockfd, 5);

	while (1)
	{
		double liczba;
		double liczba2;
		

		printf ("server waiting\n");

		/*  Accept connection.  */

		client_len = sizeof (client_address);
		client_sockfd = accept (server_sockfd,
				(struct sockaddr *) &client_address,
				&client_len);

		/*  We can now read/write to the client on client_sockfd.
			The five second delay is just for this demonstration.  */
		time_t timex;
  		struct tm * timeinfo;
  		time ( &timex );
		timeinfo = localtime ( &timex );
		char *h2 = asctime(timeinfo);
        	int length = strlen(h2);
       	 	char h[length];
        	strcpy(h,h2);
		h[length-1] ='\0';

		read (client_sockfd, &liczba,sizeof(double));

		char *czy = (char*)&liczba;
			if(*czy){
			printf("Litlle endian");
			} else{
			printf("Big endian");
			}

		sleep (2);
		liczba2 = sqrt(liczba);
		liczba = liczba2;
		write (client_sockfd, &liczba, sizeof(double));
		write (client_sockfd, &length, sizeof(int));
		write (client_sockfd, &h, length);
		close (client_sockfd);
	
	}
}
