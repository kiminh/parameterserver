#include "network.h"
#include <stdlib.h>
#include <fstream>
#include <sys/socket.h>
#include "configurations.h"

int Network::setupMySocket() {
    struct sockaddr_in myaddr; //My address and address to connect to serve

    /* create a socket */
    if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1) {
        printf("socket created\n");
    } else {
        return 0; //FIXME verify this..
    }


    /* bind it to all local addresses and pick any port number */
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(SERVICE_PORT);

    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("socket bind failed");
        return 0;
    }
    return 1;
}

/**
 * Method to get the IP address of the current machine.
 * @return a character pointer to the IPaddress in the IPV4 format.
 */
char* Network::getLocalIP(){
    char hostname[256];
    gethostname(hostname, sizeof(hostname)); //FIXME does it work without this line...

    struct hostent *host_entry;
    host_entry = gethostbyname(hostname);

    char *IPaddress;
    if (host_entry != NULL) {
        IPaddress = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
    }

    //The IP address of this process
    printf("my ip is: %s\n", IPaddress);
    return IPaddress;
}


void Network::connectWithEveryone() {
    readFromHostFile(getLocalIP());

    //Setup serveraddress for everyone else..
    for (int i = 0; i < psize; i++) {
        memset((char *) &servaddr[i], 0, sizeof(servaddr[i]));
        servaddr[i].sin_family = AF_INET;
        servaddr[i].sin_port = htons(SERVICE_PORT);
        servaddr[i].sin_addr.s_addr = inet_addr(pnames[i]);
    }

    //Connection
    for (int pid = 0; pid < psize; pid ++) {
        if (connect(fd, (struct sockaddr *) &servaddr[pid], sizeof(servaddr)) < 0) {
            perror("connect failed");
            return 0;//FIXME
        }
    }


}

/**
 * Method to read from host file, resolve the IP address for the container names and add unique IPs to an array of IP addresses.
 * Takes in the IP address of the current server as a parameter to not add it.
 */
void Network::readFromHostFile( char* myIP) {
    FILE *fp;
    long lSize;
    char* contents;
    char myIPAddress[40];
    strcpy(myIPAddress, myIP);


    std::ifstream inFile;
    inFile.open("hostfile.txt");
    if (inFile.fail()) {
        std::cerr << "Error opening a file" << std::endl;
        inFile.close();
        exit(1);
    }
    std::string line;//Each line represents a different container name.
    char *IPbufferLocal;
    struct hostent *host_entry;
    const char *temp;
    int index = 0;
    while (getline(inFile, line))
    {
        temp = line.c_str();
        host_entry = gethostbyname(temp);
        if(host_entry != NULL) {
            IPbufferLocal = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
        }
        if (strcmp(IPbufferLocal, myIPAddress) == 0) {
            char id = *(temp + 6);
            myId = id - 48;
            printf("my id is: %d \n", myId);
        }
        if((IPbufferLocal != NULL) && strcmp(IPbufferLocal, myIPAddress)!=0) {
            strcpy(pnames[index], IPbufferLocal);

            index++;
        }
    };
    inFile.close();
    psize = index; // Total # of processes.
}

