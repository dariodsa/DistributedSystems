#include "client.h"
#include <vector>
using namespace std;

class Server{
    private:
        vector<Sensor*> sensors;

    public:
    bool registerNewSensor(Sensor &s);
    float dist(Sensor &s1, Sensor &s2);
    Sensor closest(Sensor &s1);
};