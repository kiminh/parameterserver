//
// Created by Rahul Ravindran on 2019-11-10.
//

#ifndef FINAL_PROJECT_NETWORK_H
#define FINAL_PROJECT_NETWORK_H
class Network {
private:
    char* getLocalIP();
    void readFromHostFile( char* myIP);
public:
    int setupMySocket();
    void connectWithEveryone();

};

#endif //FINAL_PROJECT_NETWORK_H
