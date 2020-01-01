/*
 * url_parser.h
 *
 *  Created on: Jun 4, 2019
 *      Author: osboxes
 */

#ifndef URL_PARSER_H_
#define URL_PARSER_H_

#define PATH_LEN 100
#define HOSTNAME 200
#include <netinet/in.h>

typedef struct
{
	struct in_addr ip;
	char h_name[HOSTNAME];//Host name. Can be domain name or IP address
	uint16_t port;
	char path[PATH_LEN + 1];//Resource length
}url_t;

/*
 * The function parses the URL and save the result in result output variable.
 * Allowed formats:
 * 1.  <protocol>://<hostname>/<path>
 * 2. <protocol>://<hostname>:<port>/<path>
 */
void parse_url(char * url, url_t * result);


#endif /* URL_PARSER_H_ */
