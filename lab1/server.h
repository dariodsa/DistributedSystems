#include "sensor.h"
#include <vector>


using namespace web;
using namespace http;
using namespace utility;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

char PORT[]= "8080";
char IP_ADDRESS[]= "http://localhost:";

class Server{
    private:
        map<std::string, Sensor*> sensors;
        http_listener m_listener;
        
        void handle_get(http_request request);
        void handle_post(http_request request);

    public:
        Server(utility::string_t url);
        pplx::task<void> open() { return m_listener.open();}
        pplx::task<void> close() { return m_listener.close();}
        bool registerNewSensor(Sensor *s);
        float dist(Sensor *s1, Sensor *s2);
        Sensor* closest(Sensor* s1);
        void printSensors();
        map<std::string, Sensor*> getSensors() { return this->sensors;}
};
