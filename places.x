const MAXLEN = 255;
typedef string nametype<MAXLEN>;

/* node to store city informations */
struct locationNode {
    nametype airportName;
    nametype airportCode;
    double cityLat;
    double cityLong;
    double distance;
    locationNode* head;
};

struct stateNode {
    nametype city;
    nametype state;
};

union nearestPlace_ret switch (int err) {
  case 0:
      locationNode* list;
  default:
      void; /*error occured, nothing returned*/
};

/*IDL program*/
program PLACES_PROG {
     version PLACES_VERS {
          nearestPlace_ret FINDPLACE(stateNode*)=1;
     } = 1;
} =  0x31331331;
