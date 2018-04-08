#include "places.h"
#include "airport.h"
#include "errno.h"
#include "CityTrie.h"
#include <iostream>
using namespace std;

/*
 * Receives a stateNode that contains city name and city state from places client.
 * It will check whether city name is ambiguous. Then it sends the data to airport
 * server in order to retrieve the nearest airports.
 *
 * @param statenode: a node containing city name and city state
 * @return locationNode pointing to a linked list of locationNodes (the five nearest airports)
 */
nearestPlace_ret *
findplace_1_svc(stateNode *statenode, struct svc_req *rqstp)
{
	static nearestPlace_ret result;

	// Unpack node
	string cityName = statenode->city;
	string cityState = statenode->state;

	// Check whether city name is ambiguous or not
	CityTrie cityTrie;
	string cityData = cityState + " " + cityName;

	// Free previous result 
	xdr_free((xdrproc_t)xdr_nearestPlace_ret, (char*)(&result));

	if(cityTrie.isValidCity(cityData)) 
	{
		CLIENT *clnt;
		char* host = strdup("localhost");
		nearestAiports_ret  *result_1;
		cityNode  findairport_1_arg;

		double cityLat = cityTrie.getLatitude(cityData);
		double cityLong = cityTrie.getLongitude(cityData);

#ifndef	DEBUG
		clnt = clnt_create (host, AIRPORT_PROG, AIRPORT_VERS, "udp");
		if (clnt == NULL) {
			clnt_pcreateerror (host);
			exit (1);
		}
#endif	/* DEBUG */

		findairport_1_arg.cityLat = cityLat;
		findairport_1_arg.cityLong = cityLong;

		result_1 = findairport_1(&findairport_1_arg, clnt);
		if (result_1 == (nearestAiports_ret *) NULL) {
			clnt_perror (clnt, "call failed");
		}

		// Airport server will return us 5 nearest airport in the form of
		// linked list of airport nodes. Then we need to convert it to location
		// node and send it back to client.

		// However, the first location node will be empty, and it will be connected
		// to the next 5 location nodes that contain the airport data. The first
		// location node will contain the latitude and longitude of the
		// city sent by client so that client can get the latitude and longitude of the
		// city
		airportNode **airportsList;
		airportsList = &result_1->nearestAiports_ret_u.list;


		locationNode **locationsList;
		
		locationsList = &result.nearestPlace_ret_u.list;

		// Storing corrected city name
		(*locationsList) = new locationNode();
		(*locationsList)->airportCode = new char[MAXLEN];
		(*locationsList)->airportName = new char[MAXLEN];
		string fixCityName = cityTrie.autoSuggestions(cityData);
		strcpy((*locationsList)->airportName, fixCityName.c_str());
		(*locationsList)->cityLat = cityLat;
		(*locationsList)->cityLong = cityLong;
		(*locationsList)->distance = 0.0;
		locationsList = &(*locationsList)->head;
		(*locationsList) = NULL;
		
		for (int i = 0; i < 5; i++)
		{
			(*locationsList) = new locationNode();
			(*locationsList)->airportCode = (*airportsList)->airportCode;
			(*locationsList)->airportName = (*airportsList)->airportName;
			(*locationsList)->cityLat = (*airportsList)->latitude;
			(*locationsList)->cityLong = (*airportsList)->longitude;
			(*locationsList)->distance = (*airportsList)->distance;
		
			locationsList = &(*locationsList)->head;
			(*locationsList) = NULL;
			airportsList = &(*airportsList)->next;
			
		}
		// no error
		result.err = 0;
	} 
	else 
	{
		// City name is ambiguous, return error code
		result.err = 1;
	}
	return &result;
}


