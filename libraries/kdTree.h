#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <vector>
#include <map> 
#include "airport.h"
using namespace std;
 
#define MAX_DIM 3
#define pi 3.14159265358979323846
struct kd_node_t
{
	// gps[0] is latitude and gps[1] is longitude
    double gps[MAX_DIM];
	string airportCode;
	string airportName;
    struct kd_node_t *left, *right;
};
 
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function converts decimal degrees to radians :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg) 
{
	return (deg * pi / 180);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function converts radians to decimal degrees :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad) 
{
	return (rad * 180 / pi);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function calculates the distance between 2 coordinates on earth :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
inline double dist(struct kd_node_t *a, struct kd_node_t *b, int dim)
{
	double theta, dist;
	theta =  a->gps[1] - b->gps[1];
	
	dist = sin(deg2rad(a->gps[0])) * sin(deg2rad(b->gps[0])) + cos(deg2rad(a->gps[0])) 
		   * cos(deg2rad(b->gps[0])) * cos(deg2rad(theta));
	dist = acos(dist);
	dist = rad2deg(dist);
	dist = dist * 60 * 1.1515;
	
	return (dist);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function swaps the latitude and longitude between 2 kd_node_t :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
inline void swap(struct kd_node_t *x, struct kd_node_t *y) 
{
    double tmp[MAX_DIM];
    memcpy(tmp,  x->gps, sizeof(tmp));
    memcpy(x->gps, y->gps, sizeof(tmp));
    memcpy(y->gps, tmp,  sizeof(tmp));
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: see quickselect method :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
struct kd_node_t* find_median(struct kd_node_t *start, struct kd_node_t *end, int idx)
{
    if (end <= start) return NULL;
    if (end == start + 1)
        return start;
 
    struct kd_node_t *p, *store, *md = start + (end - start) / 2;
    double pivot;
    while (1) 
	{
        pivot = md->gps[idx];
		
		// store the airportCode and airportName of kd_node_t md and (end - 1)
		string mdAirportCode = md->airportCode;
		string endAirportCode = (end - 1)->airportCode;
		string mdAirportName = md->airportName;
		string endAirportName = (end - 1)->airportName;
        
		swap(md, end - 1);
		
		// swap the airportCode and airportName of kd_node_t md and (end - 1)
		md->airportCode = endAirportCode;
		(end - 1)->airportCode = mdAirportCode;
		md->airportName = endAirportName;
		(end - 1)->airportName = mdAirportName;
		
        for (store = p = start; p < end; p++) 
		{
            if (p->gps[idx] < pivot) 
			{
                if (p != store)
				{
					// store the airportCode and airportName of kd_node_t p and store
					string pAirportCode = p->airportCode;
					string storeAirportCode = store->airportCode;
					string pAirportName = p->airportName;
					string storeAirportName = store->airportName;
					 
					swap(p, store);
					
					// swap the airportCode and airportName of kd_node_t p and store
					p->airportCode = storeAirportCode;
					store->airportCode = pAirportCode;	
					p->airportName = storeAirportName;
					store->airportName = pAirportName;	
				}
                store++;
            }
        }
		// store the airportCode and airportName of kd_node_t store and (end - 1)
		string storeAirportCode = store->airportCode;
		string endAirportCode2 = (end - 1)->airportCode;
		string storeAirportName = store->airportName;
		string endAirportName2 = (end - 1)->airportName;
		
        swap(store, end - 1);
		
		// swap the airportCode and airportName of kd_node_t store and (end - 1)
		store->airportCode = endAirportCode2;
		(end - 1)->airportCode = storeAirportCode;
		store->airportName = endAirportName2;
		(end - 1)->airportName = storeAirportName;
 
        /* median has duplicate values */
        if (store->gps[idx] == md->gps[idx])
        {	
			return md;
		}
 
        if (store > md) 
			end = store;
        else        
			start = store;
    }
}
 
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function makes the KD tree :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
struct kd_node_t* make_tree(struct kd_node_t *t, int len, int i, int dim)
{
    struct kd_node_t *n;
 
    if (!len) return 0;
 
    if ((n = find_median(t, t + len, i))) 
	{
        i = (i + 1) % dim;
        n->left  = make_tree(t, n - t, i, dim);
        n->right = make_tree(n + 1, t + len - (n + 1), i, dim);
    }
    return n;
}
 
map<double, airportNode> visited;

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function finds the nearest node :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void nearest(struct kd_node_t *root, struct kd_node_t *nd, int i, int dim,
        struct kd_node_t **best, double *best_dist)
{
    double d, dx, dx2;
 
    if (!root) return;
    d = dist(root, nd, dim);
	
	struct airportNode a;  
	
	char* tempCode = strdup(root->airportCode.c_str());
	char* tempName = strdup(root->airportName.c_str());
	
	a.airportCode = tempCode; 
	a.airportName = tempName;
	
	a.latitude = root->gps[0];
	a.longitude = root->gps[1];
	a.distance = d; 
	
	// potential 5 nearest airports
	visited.insert(pair<double, airportNode>(d, a));
	
    dx = root->gps[i] - nd->gps[i];
	dx2 = dx * dx;
	
    if (!*best || d < *best_dist) 
	{
        *best_dist = d;
        *best = root;
    }
	
    /* if chance of exact match is high */
    if (!*best_dist) return;
 
    if (++i >= dim) i = 0;
 
    nearest(dx > 0 ? root->left : root->right, nd, i, dim, best, best_dist);
	
    if (dx2 >= *best_dist) return;
    
	nearest(dx > 0 ? root->right : root->left, nd, i, dim, best, best_dist);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function would be called in places_server.cpp to find the 5 nearest airports :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
vector<airportNode> fiveNearestAirports(struct kd_node_t *root, double latitude, double longitude)
{
	struct kd_node_t testNode = {{latitude, longitude}, "a"};
    struct kd_node_t *found; 
    double best_dist = -1.0;
	int count = 0;
	
	visited.clear();
	
    found = 0;
	nearest(root, &testNode, 0, 3, &found, &best_dist);
	
	vector<airportNode> fiveNearest;
	for (std::map<double, airportNode>::iterator i = visited.begin(); i != visited.end(); i++)
	{
		if(count++ == 5)
			break;
		fiveNearest.push_back(i->second);
	}
	return fiveNearest;
}