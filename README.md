# FIND NEAREST AIRPORT

An rpc application where client will contact server and give it two arguments:
city name and state. The server will return 5 nearest airports to that location.

For example:

```
./client localhost “Princeton borough” NJ
Princeton borough, NJ: 40.352206, -74.657071
code=TTN, name=Trenton, state=NJ distance: 10 miles
code=WRI, name=Mcguire AFB, state=NJ distance: 23 miles
code=PNE, name=North Philadelphia, state=PA distance: 27 miles
code=NXX, name=Willow Grove, state=PA distance: 28 miles
code=NEL, name=Lakehurst, state=NJ distance: 28 miles
```

## Architecture

The application is built by following the three-tier architecture. In this case 
it will be: client - places - airport. 

**Client or Places Client**
Client will send a city name and state to places server.

**Places Server**
Places server will also act as a client for the airport server. It bridges the places client with
the airport server. In places server, the city name will be validated in order to determined whether there is an ambiguity or not. For instance, let's say that New York and New Jersey are in the same state (although they are not), but when client sends 'New' to the places server, it will return an error code saying that it should refine the query to make it less ambiguous.

**Airport Server**

