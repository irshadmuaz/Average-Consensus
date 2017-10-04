/*
	Agent class
*/

#ifndef AGENT_H
#define AGENT_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <list>
#include <vector>
#include <gmp.h>

extern "C" 
{
  #include <paillier.h>
}

typedef unsigned long ulong;
 struct myVector
{
  char address=0;
  int step = 0;
  double diff = 0.0;
};

class Agent
{
 public:
  Agent(std::string id);
  ~Agent();

  double setState(const double st);
  double getState() { return state;}
  double getAlpha() {return alpha;}
  double setDiff(const double diff);
  /*
    Send inquiries to all neighbors and do computation 
    without updating its states
   */
  int communicate();
	
  myVector _myVector[10];
  /*
    Update the internal states
  */
  int updateState();
  
  int logState();
  
  double old_alpha = 0.0;
  double old_state = 0.0;
 
  
  /*
    Process another agent's inquiry
   */
  int exchange(paillier_pubkey_t* pub,
	       paillier_ciphertext_t* msg_in,
	       paillier_ciphertext_t* msg_out, int a, int c);

  long ciphertext_to_long(paillier_ciphertext_t* c);

  // Generate a new random weight alpha
  long updateAlpha();

  std::string id;
  // For illustrative purpose, state is a scalar
  double state;
  long alpha;
  long long_state;
  long diff_state;
  std::vector<double> _states;
  std::vector<double> _alphas;
  int step=0;
  char identity=0;
  FILE* logfile = NULL;
  FILE* fp = NULL;
  paillier_pubkey_t* pubKey = NULL;
  paillier_prvkey_t* prvKey = NULL;
};


#endif
