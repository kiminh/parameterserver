
/**
 * This File contains the pre defined configurations required for this application.
 */

#define SERVICE_PORT	21234	/* hard-coded port number */
#define BUFLEN 4096 /* buffer size to read from socket */
#define MAX_STRING_SIZE 40 /*We assume this to be the Max Size of IP addresses, its fine since its IPv4*/
#define MAX_PEERS 20 /* We assume a maximum of 20 peers.*/
#define INITIAL_WAIT_TIME 30 /* Time spent waiting for all the proceesses to be up. */ //can we do this better?

