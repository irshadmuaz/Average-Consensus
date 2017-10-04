#include "agent.h"
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
#include <math.h>
#include <unistd.h>
#include <thread>
#include <wiringPi.h>
#include <wiringSerial.h>
#define BUFSIZE 2048
#define BUFLEN 2048
#define SERVICE_PORT 21231
#define RESPONSE_PORT 21232
#define NEIGHBORS 2
#define STATE_FACTOR 10000
#define ALPHA_FACTOR 10
using namespace std;
extern Agent *me = new Agent(string("node1"));
int fd;


int sendData(char* _data, char _size, char _address)
{
	int i = 0;
	char length = _size + 5;
	char checksum = 0;
	serialPutchar(fd,0x7E);
	serialPutchar(fd,0x00);serialPutchar(fd,length); //length
	checksum=1+_address;
	for(int k=0;k<_size;k++)
	{
		checksum+=_data[k];
	}
	checksum = 0xFF - checksum;
	//printf("checksum : %d, length: %d",checksum,length);
	serialPutchar(fd,0x01);serialPutchar(fd,0x00);//type and id;
	serialPutchar(fd,0x00);serialPutchar(fd,_address);//address;
	serialPutchar(fd,0x00);//options
	for(int k=0;k<_size;k++)
	{
		serialPutchar(fd,_data[k]);
	}
	serialPutchar(fd,checksum);
	
}
int SetupServer()
{
	 char buf[BUFSIZE];
	bzero(buf,BUFSIZE);
	int data_size;
	int RSSI;
	int index;
	int _step;
	
	char respAddr;
	if((fd=serialOpen("/dev/ttyACM0",9600)) < 0)
			   {
				   printf("Error! unable to open serial device\n");
				   return 1;
			   }
			if(wiringPiSetup() == -1)
			{
				printf("Error! unable to start wiringPI\n");
				return 1;
			}
    for (;;) {
	   //*********************************starts here*********************************
	 //START:  
	 int i = 0;
	   		while(serialDataAvail(fd))
			   {
				   if(serialGetchar(fd) == 0x7E) //start delimitter
				   {
						data_size = serialGetchar(fd);data_size = serialGetchar(fd); //read data size 2
						//cout<<"Data_size: "<<(data_size-5)<<endl;
						serialGetchar(fd);serialGetchar(fd);// frame type addr_upper byte
						respAddr = serialGetchar(fd); //read respAddr
						RSSI = serialGetchar(fd);serialGetchar(fd);//read option
						for(int k=0;k<(data_size - 5);k++)
						{
							buf[k] = serialGetchar(fd);//printout received data
						}
				   }
							for(int a=0;a<NEIGHBORS;a++)
			{
				
				if(me->_myVector[a].address == respAddr)
				{
					index = a; //what neighbor is this
					
				}
				//printf("resAddr[%d],index[%d]\n",respAddr,index);
				
			}
				paillier_pubkey_t* _pubKey;
				paillier_prvkey_t* _prvKey;
	 			paillier_ciphertext_t* ctxt1;
				 paillier_ciphertext_t* c_res = paillier_create_enc_zero();
	 			char hexPubKey[BUFSIZE]; bzero(hexPubKey,BUFSIZE);
	 			char byteCtxt[BUFSIZE];bzero(byteCtxt,BUFSIZE);
			
	 			int def=0, s_pub, s_ctxt,s_prv;
				

				memcpy(&def,&buf[i],sizeof(int)); i+=sizeof(int); //read type
			
				//cout<<"Def: "<<def<<endl;
				
				if(def == 1)//this is a request
				{
					
					
				memcpy(&_step, &buf[i],sizeof(int));i+=sizeof(int);//read step index;
				//cout<<"received step: "<<_step<<endl;	
				memcpy(&s_pub,&buf[i],sizeof(int)); i+=sizeof(int); //read size of public key
				
			 	memcpy(&s_ctxt,&buf[i],sizeof(int)); i+=sizeof(int); //read size of cypher text
					 
			  	memcpy(&hexPubKey,&buf[i],s_pub); i+=s_pub; //read public key

				
				for(int k=i;k<i+s_ctxt;k++) //read cypher
				{
					byteCtxt[k-i] = buf[k];
				}i+=s_ctxt;
				
			  
				  
			  		_pubKey = paillier_pubkey_from_hex(hexPubKey); //recreate public key
			  	
					  ctxt1 = paillier_ciphertext_from_bytes((void*)byteCtxt, PAILLIER_BITS_TO_BYTES(_pubKey->bits)*2); //recreate cypher text
					
				if(_step <= me->step)
				{		
					me->exchange(_pubKey,ctxt1,c_res,index,_step);
					//cout <<"reached here 2"<<"step: "<<_step<<endl;
					char *resBytes = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(_pubKey->bits)*2, c_res);
					char _addr = me->identity;
					char respBuf[BUFSIZE];bzero(respBuf,BUFSIZE);
					int type = 2; //This is a response
					int size = PAILLIER_BITS_TO_BYTES(_pubKey->bits)*2;
					int j=0;
					//respBuf[j] = _addr;j+=sizeof(char);
					memcpy(&respBuf[j],&type,sizeof(int));j+=sizeof(int);
					memcpy(&respBuf[j],&_step,sizeof(int));j+=sizeof(int);
					memcpy(&respBuf[j],&size,sizeof(int));j+=sizeof(int);
					for(int k=j;k<(j+size);k++)
					{
						respBuf[k] = resBytes[k-j];
					}
					j+=size;
					sendData(respBuf,j,respAddr);
					
			
				cout<<"Sending response"<<endl;
				
			
	  			
					 paillier_freeciphertext(c_res);

					 //increment that I have received a request
			}	 	
				}
				else if(def == 2)//this is a response
				{
					memcpy(&_step, &buf[i],sizeof(int));i+=sizeof(int);//read step index;
					memcpy(&s_ctxt,&buf[i],sizeof(int));i+=sizeof(int);//read size					
					memcpy(&byteCtxt, &buf[i],s_ctxt); //read cypher										
					ctxt1 = paillier_ciphertext_from_bytes((void*)byteCtxt, PAILLIER_BITS_TO_BYTES(me->pubKey->bits)*2); //recreate
					int cont = 1;
					long result =0;
					me->diff_state =0;
					result = me->ciphertext_to_long(ctxt1);
					//cout<<"Index step: "<<me->_myVector[index].step<<endl;
					//cout<<"received step: "<<_step<<endl;
					
					if(me->_myVector[index].step < _step)
					{
						me->_myVector[index].diff =  me->_alphas[_step-1] * result;
						me->_myVector[index].step = _step;
											{
					//compare all neighbors are at the same step
                                        for(int c=0;c<NEIGHBORS;c++)
                                                if(me->_myVector[c].step == _step)
                                                {
                                                        cont *= 1;
                                                }
                                                else
                                                {
                                                        cont *= 0;
                                                }
                                        }

                                //cout<<"contd: "<<cont<<endl;

                                        if(cont)
                                        {
                                                for(int c=0;c<NEIGHBORS;c++)
                                                {
                                                        me->diff_state+=me->_myVector[c].diff;
                                                }
                                                //cout<<"Diff state"<<me->diff_state<<endl;
                                                //cout<<"updating state"<<endl;
                                                  me->updateState();
                                        }

					}		
						
					
												

					  

						//increment that I have received a response
					
				}

			
			
      		bzero(buf,BUFSIZE);
					
			   }

			//goto START;
		
			
		}
	
}
int Communicator()
{
	me->long_state = -(long) lround(me->state * STATE_FACTOR);
  paillier_plaintext_t* m_s = paillier_plaintext_from_ui(me->long_state);
  paillier_ciphertext_t* c_s = NULL;
  c_s = paillier_enc(NULL, me->pubKey, m_s,
		     paillier_get_rand_devurandom);
    char _addr = me->identity;
    char* hexPubKey = paillier_pubkey_to_hex(me->pubKey); //serialize pub key
    char* byteCtxt = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(me->pubKey->bits)*2, c_s);//serialize cypher
    int count = 1;//send is one, this is a request
		int s_pub = strlen(hexPubKey);
		int s_ctxt = PAILLIER_BITS_TO_BYTES(me->pubKey->bits)*2;
		 int i=0;
		char type[sizeof(int)];
		memcpy(&type,&count,sizeof(int));
    int data = sizeof(int) + sizeof(int)+sizeof(int) +strlen(hexPubKey) + strlen(byteCtxt) ;
		char sendBuf[BUFSIZE];
		//sendBuf[i] = _addr; i+=sizeof(char);
		memcpy(&sendBuf[i],&type,sizeof(int)); i+=sizeof(int); //add type
      int step = me->step;
    memcpy(&sendBuf[i],&step,sizeof(int)); i+=sizeof(int); //add step index;
		memcpy(&sendBuf[i],&s_pub,sizeof(int)); i+=sizeof(int); //add size of public key
		memcpy(&sendBuf[i],&s_ctxt,sizeof(int)); i+=sizeof(int); //add size of cypher text
		//cout<<"s_pub: "<<s_pub<<endl;
    	//cout<<"s_ctxt: "<<s_ctxt<<endl;
    strcpy(&sendBuf[i],hexPubKey);i+=s_pub;
     for(int k=i;k<i+s_ctxt;k++)
     {
       sendBuf[k] = byteCtxt[k-i];
     }
     i+=s_ctxt;
    //+++++++++++++++++++++++sending starts here+++++++++++++++++++++++++++++++++++
	//cout<<"Send size: "<<i<<endl;
	//char* test = "ass";
  for(int l=0;l<NEIGHBORS;l++)
  {
    sleep(0.1);
    sendData(sendBuf,i,me->_myVector[l].address);
  }
 paillier_freeplaintext(m_s);
  paillier_freeciphertext(c_s);
 
  return 0;
}

