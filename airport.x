const MAXLEN = 255;
typedef string nametype_air<MAXLEN>;

/* node to store airport informations */
struct airportNode {
     nametype_air airportName;
     nametype_air airportCode;
     double latitude;
     double longitude;  
     double distance;
     airportNode* next;
};

/* node to store city informations */
struct cityNode {
    double cityLat;
    double cityLong;
};

union nearestAiports_ret switch (int err) {
  case 0:
      airportNode* list;
  default:
      void; /*error occured, nothing returned*/
};

/*IDL program*/
program AIRPORT_PROG {
     version AIRPORT_VERS {
          nearestAiports_ret FINDAIRPORT(cityNode*)=1;
     } = 1;
} = 0x31331332;
