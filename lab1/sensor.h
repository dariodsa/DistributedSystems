#include <string.h>
#include <stdlib.h>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/ws_client.h>                  // WebSocket client
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include <cpprest/producerconsumerstream.h>     // Async streams for producer consumer scenarios
#include <iosfwd>
#include <iostream>
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace std;


class Measurement{
    private:
        std::string username;
        std::string parameter;
        double averageValue;
    public:
    Measurement(std::string username, std::string parameter, double averageValue);
    Measurement(web::json::value value) : Measurement( 
                          value["username"].as_string()
                        , value["parameter"].as_string()
                        , value["averageValue"].as_double()) {}
                    
    std::string getUsername();
    std::string getParameter();
    float getAverageValue();
    
};


class Sensor{
    private:
        std::string username;
        double latitude;
        double longitude;
        std::string ipAddress;
        size_t port;
        std::vector<Measurement*> measurements;
    public:
        Sensor(std::string username, double latitude, double longitude, std::string ipAddress, size_t port);
        Sensor(web::json::value value) : Sensor(
                            value["username"].as_string()
                            , value["latitude"].as_double()
                            , value["longitude"].as_double()
                            , value["ipAddress"].as_string()
                            , value["port"].as_integer()) {}
          
        web::json::value getJSON();
        float getLatitude();
        float getLongitude();
        std::string getUsername();
        size_t getPort();
        std::string getIpAddress();
        std::vector<Measurement*> getMeasurements() {
            return this->measurements;
        }
        std::string to_string();
        void addData(std::string parameter, float data);
};

