#include <math.h>
#include "agent.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#include <unistd.h>
#define KEY_LENGTH 512
#define STATE_FACTOR 10000
#define ALPHA_FACTOR 10
#define SERVICE_PORT 21231
#define BUFSIZE 2048
#define BUFLEN 2048

using namespace std;

class Agent
{
    public:
        double state;
        long alpha;
        long long_state;
        long diff_state;
        paillier_prvt_
        Agent();

};

Agent::Agent(void)
{
    paillier_keygen(KEY_LENGTH,&pubKey,&prvKey,paillier_get_rand_devurandom);
}

