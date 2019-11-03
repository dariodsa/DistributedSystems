#include "server.h"
#include <math.h>
#include <stdexcept>

#define EARTH_RADUIS 6371

unique_ptr<Server> g_http;

std::string getLast(std::string s) {
    int br = 0;
    for(int i=0, len = s.length(); i < len; ++i) {
        if(s[i] == '/') {
            br = i;
        }
    }
    std::string ans = "";
    for(int i=br + 1, len = s.length(); i < len; ++i) {
        ans += s[i];
    }
    return ans;
}

Server::Server(string_t url) : m_listener(url) {
    m_listener.support(methods::GET, std::bind(&Server::handle_get, this, placeholders::_1));
    m_listener.support(methods::POST, std::bind(&Server::handle_post, this, placeholders::_1));
}

void Server::printSensors() {
    cout << this->sensors.size()<< endl;
    /*for(int i = 0, len = this->sensors.size(); i < len; ++i) {
        cout << this->sensors[i]->getJSON().serialize() << endl;
    }*/
    return;
}

void Server::handle_post(http_request request) {
    cout<<"POST URL:"<<request.request_uri().to_string()<<endl;

    if(std::string("/register").compare(request.request_uri().to_string()) == 0) {
        request.extract_json().then([=](web::json::value value) {
            //cout << value.serialize() << endl;
            Sensor* s = new Sensor(value);
            this->registerNewSensor(s);
            this->printSensors();
            cout << value.serialize() << endl;
            request.reply(status_codes::OK, value);
        });
    } else if(std::string("/store").compare(request.request_uri().to_string()) == 0) {
        request.extract_json().then([=](web::json::value value) {
            //cout << value.serialize() << endl;
            Measurement* measurment = new Measurement(value);
            this->getSensors()[measurment->getUsername()]->addData(measurment->getParameter(), measurment->getAverageValue());
            
            cout << value.serialize() << endl;
            request.reply(status_codes::OK, value);
        });
    } else {
        request.reply(status_codes::BadRequest);
    }
    
}

void Server::handle_get(http_request request) {
    cout<<"URL:"<<request.request_uri().to_string()<<endl;
    cout << request.request_uri().to_string().find(std::string("/closest/")) << endl;
    if(request.request_uri().to_string().find(std::string("/closest/")) == 0) {
        std::string username = getLast(request.request_uri().to_string());
        cout <<"username " << username <<"."<< endl;
        
        if(sensors.count(username) > 0) {
            cout << sensors[username]->getPort() << endl;
            Sensor* s1 = this->closest(this->sensors[username]);
                cout << sensors[username]->getPort() << endl;
            request.reply(status_codes::OK, s1->getJSON());
            
        } else {
            
            for(auto it = sensors.begin(); it != sensors.end(); ++it) {
                cout << it -> first << "=="<<username<<"."<<endl;
            }
            request.reply(status_codes::NotFound);
        }
        
    } else {
        /*auto answer = json::value::array();
        answer[0] = json::value(false);
        answer[1] = json::value(U("Hello"));
        ucout << request.to_string() << endl;*/
        request.reply(status_codes::BadRequest);
    }
}

bool Server::registerNewSensor(Sensor* s1) {
    auto it = sensors.find(s1->getUsername());
    if(it != sensors.end()) return false;
    else {
        sensors[s1->getUsername()] = s1;
        return true;
    }
}

float Server::dist(Sensor* s1, Sensor* s2) {
    float deltaLon = abs(s1->getLongitude() - s2->getLongitude());
    float deltaLat = abs(s1->getLatitude() - s2->getLatitude());
    float a = (sin(deltaLat/2) * sin(deltaLat/2.0) + cos(s1->getLatitude()) * cos(s2->getLatitude()) * sin(deltaLon/2.0)*sin(deltaLon/2.0));
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADUIS * c;
}
Sensor* Server::closest(Sensor* s1) {
    
    if(sensors.size() == 0) {
        throw invalid_argument( "list of sensors is empty");
    }
    float dul = 0x3f3f3f3f;
    std::string name = "";
    for(auto it = sensors.begin(); it != sensors.end(); ++it) {
        if(s1->getUsername().compare(it->second->getUsername()) == 0) continue;
        float new_dul = dist(s1, it->second);
        if(new_dul < dul) {
            dul = new_dul;
            name = it->second->getUsername();
        }
    }
    if(name.compare("") == 0) return s1;
    else {
        return sensors[name];
    }
}

void init(const string_t& address) {
    
    uri_builder uri(address);
    uri.append("rassus/");
    auto addr = uri.to_uri().to_string();
    g_http = unique_ptr<Server>(new Server(address));
    g_http->open().wait();
    ucout << utility::string_t(U("Slusam na: ")) << addr << endl;
}

int main(int argc, char* argv[]) {
    utility::string_t address = U(IP_ADDRESS);
    utility::string_t port    = U(PORT);
    address.append(port);
    init(address);
    string line;
    getline(cin, line);
}
