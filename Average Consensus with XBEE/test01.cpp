#include "agent.h"
#include "communication.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <thread>
using namespace std;

#define BUFSIZE 2048
#define BUFLEN 2048
#define SERVICE_PORT 21231

const int numAgents = 2;
const int maxSteps = 40;
  // The agents are stored in a vector
  // The graph (neighbors) is stored in a vector of list
 // vector<Agent*> nodes(numAgents);
 // vector<AgentList> edges(numAgents);


myVector a,b,c,d,e;

int main()
{
a.address = 1;
b.address = 2;
me->identity = 3;
  
  srand(2);
 me->_myVector[0] = a;
 me->_myVector[1] = b;
  //printf("Initialize nodes long=%lu\n", sizeof(long));
  
  char id[32];
  double state;
  
  int myIndex = 0;
  thread t1(SetupServer);
  
       
      state = (rand() % 1000);
      me->setState(state);
      me->updateAlpha();
      me->old_alpha = me->alpha;
     
  
  printf("Initialize edges\n");
   printf("Main loop\n");
   for(int i=0; i< 1; ++i)
     {
       printf("Step %2d: ", i);
  //     // NOTE: The following steps must be done in seperate steps
  //     // TODO: consider pair-wise update
  //     // First everyone collect info from neighbors
	//
	sleep(5); 
  for(int i=0;i<maxSteps;i++)
  {
       // cin>>id;
        Communicator();
        cout<<"My state is: " <<me->state<<endl;
        sleep(1);
  }
      fclose(me->fp);
        t1.detach();
        
  return 0;
}
}
