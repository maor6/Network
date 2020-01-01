/*
 * url_parser.c
 *
 *  Created on: Jun 4, 2019
 *      Author: osboxes
 */

#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <ctype.h>
#include <arpa/inet.h>

#include "url_parser.h"
#define PORT 80
/*
 * The function parses the host name of the URL and returns true if the host name has
 * IP representation.
 */
static bool parse_hostname(char * hostname, url_t * result)
{
	bool is_hostname_ip = false;

	if(hostname == NULL)
	{
		perror("Error parsing hostname.\n");
		exit(1);//Just terminate gracefully the program.
	}

	if(!isdigit(hostname[0]))//Means i'ts a domain name. Keep parsing by that.
	{
		struct addrinfo* addr_info;

		//This function retrieves the IP address of the host name given in the function.
		if (0 != getaddrinfo(hostname, NULL, NULL, &addr_info))
		{
			fprintf(stderr, "Error in resolving hostname %s\n", hostname);//Print to error standard(to the screen)
			exit(1);
		}

		struct sockaddr_in * host_addr = (struct sockaddr_in *)addr_info->ai_addr;
		result->ip.s_addr = host_addr->sin_addr.s_addr;//Copy the resolved IP address.
		strncpy(result->h_name, hostname, HOSTNAME);
		//Port will be the default
	}
	else//The host name should be IP address v4
	{
		//Let's check if it's a valid IP address, and if it is set it.
		if(inet_pton(AF_INET, hostname, &result->ip) != 1)
		{
			printf("IP address isn't valid\n");
			exit(1);
		}

		strncpy(result->h_name, hostname, strlen(hostname) + 1);//Host name field can also be an IP.
		is_hostname_ip = true;
	}

	return is_hostname_ip;
}

/*
 * The function parses the protocol of the URL and returns true if it's http.
 */
static bool parse_protocol(char * protocol)
{
	bool is_http = true;

	if(protocol == NULL)
	{
		is_http = false;
	}
	else if(strcmp(protocol, "http"))
	{
		is_http = false;
	}

	return is_http;
}

/*
 * The function parses the path of the URL and put it in output parameter.
 */
static void parse_path(char * path, char * output)
{
	if(path != NULL)//If path no exists, default will be '/'
	{
		strncpy(output, path, strlen(path) + 1);//For null terminator
	}
}

/*
 * The function parses a port string representation and converts it to it's
 * numeric representation.
 */
static uint16_t parse_port(char * port_str)
{

	if(port_str != NULL)
	{
		return strtol(port_str, NULL, 10);//Convert the string port into number
	}
	else
	{
		perror("Error parsing port.\n");
		exit(1);//Just terminate gracefully the program.
	}
}

void parse_url(char * url, url_t * result)
{
	result->port = PORT;//Default port(in case host name is domain name)
	strncpy(result-> path, "/", 2);//Copy default path

	//For now, supporting only HTTP protocol. Protocol must be "http".
	if(!parse_protocol(strtok(url, ":/")))//First token should be http.
	{
		perror("Protocol must be http.\n");
		exit(1);//Just terminate gracefully the program.
	}

	//Next token should be host name - can be IP or domain name.
	if(parse_hostname(strtok(NULL, ":/"), result))
	{
		//Now we need to parse the port.
		result->port = parse_port(strtok(NULL, ":/"));
	}

	//Now let's parse the path.
	parse_path(strtok(NULL, ""), result->path + 1);
}
