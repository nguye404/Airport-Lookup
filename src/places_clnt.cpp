/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "places.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

nearestPlace_ret *
findplace_1(stateNode *argp, CLIENT *clnt)
{
	static nearestPlace_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, FINDPLACE,
		(xdrproc_t) xdr_stateNode, (caddr_t) argp,
		(xdrproc_t) xdr_nearestPlace_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
