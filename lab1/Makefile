
PROJECT2 = client
SOURCE2 = client.cpp
PROJECT1 = sensor
SOURCE1 = sensor.cpp
PROJECT3 = server
SOURCE3 = server.cpp

CXX = g++
CXXFLAGS = -Wall -g -std=c++17
OBJECTS1 = ${SOURCE1:.cpp=.o}
OBJECTS2 = ${SOURCE2:.cpp=.o}
OBJECTS3 = ${SOURCE3:.cpp=.o}

all: $(PROJECT3) $(PROJECT2)


#$(PROJECT1): $(OBJECTS1)
#	$(CC) $(OBJECTS1) -o $(PROJECT1) -lboost_system -lcrypto -lssl -lcpprest -lpthread -std=c17

$(PROJECT2): $(OBJECTS2) $(OBJECTS1)
	$(CXX) $(OBJECTS2)  $(OBJECTS1) -o $(PROJECT2) -lboost_system -lcrypto -lssl -lcpprest -lpthread -std=c17

$(PROJECT3): $(OBJECTS3) $(OBJECTS1)
	$(CXX) $(OBJECTS3) $(OBJECTS1) -o $(PROJECT3) -lboost_system -lcrypto -lssl -lcpprest -lpthread -std=c17



clean:
	-rm -f $(PROJECT1) $(OBJECTS1) *.core
	-rm -f $(PROJECT1) $(OBJECTS1) *.o
	-rm -f $(PROJECT2) $(OBJECTS2) *.o
	-rm -f $(PROJECT2) $(OBJECTS2) *.core
	-rm -f $(PROJECT3) $(OBJECTS3) *.o
	-rm -f $(PROJECT3) $(OBJECT33) *.core
