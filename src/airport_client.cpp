/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "airport.h"

/*
 initialize client and create connection
 check the client to ensure it is valid if not return error
 check result from nearest airport, if it is NULL return error
 */
void
airport_prog_1(char *host)
{
	CLIENT *clnt;
	nearestAiports_ret  *result_1;
	cityNode  findairport_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, AIRPORT_PROG, AIRPORT_VERS, "udp");
    /*
     check to make sure there is a valid client
     */
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = findairport_1(&findairport_1_arg, clnt);
    /*
     check to make sure there are results from nearest airports function
     */
	if (result_1 == (nearestAiports_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
    /*
     if there isn't 2 arguments return statement and exit
     */
	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	airport_prog_1 (host);
exit (0);
}