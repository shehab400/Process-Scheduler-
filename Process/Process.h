#pragma once
using namespace std;
#include"../Data Structures/linkedlist.h"
#include"../Process/Process.h"
#include"../Multiprocessor/Queue.h"
class Process
{
private:
	string State = "NEW";
	int AT, PID, CT, N;
	Queue<int> IO_R;
	Queue<int> IO_D;
	int RT;
	int TT;
	int TRT;
	int WT;
	int NFCFS;
	int NSJF;
	int NRR;
	int totaliod;
	int currentior = N;
	int currentiod=0;
	Process* child = NULL;
	bool isforked = false;
	bool willfork = false;
	int FP;
	int FinalWT;
	int ctp;
	int wtt = 0;
public:
	Process();
	Process(int at, int pid, int ct, int n, int* io_r, int* io_d);
	Process(int at, int pid, int ct);

	int getAT();
	void setAT(int newat);
	int getCT();
	int getID();
	void setCT(int newct);
	void setPID(int newpid);
	void setN(int newn);
	int getn();
	void setIO_R(Queue<int> newior);
	void setIO_D(Queue<int> neiod);
	void getiod( Queue<int> &tempiod);
	void getior(Queue<int>&empior);
	void setState(string h);
	void setNFCFS(int N);
	int getNCFS();
	void setRR(int N);
	int getRR();
	void setSJF(int N);
	int getSJF();
	void setTRT(int trt);
	int getTRT();
	void setTerminationt_time(int t);
	int getTT();
	void setWT(int t);
	int isblocked();
	void dequeeIOR();
	
	int getWT();
	int getcurrentiod();
	void setcurrentiod(int newiod);
	void decrementblk();
	int getcurrentior();
	


	//******************************\\   
    //***********FORK****************\\   

	void setFP(int f);
	int getFP();
	void setchild(Process* p);
	bool isparent();         //check if process in parent
	void setforked();
	void setwillfork(bool s);      //check the condition in the FCFS algo to call fork in scheduler
	bool willprocessfork(); //check after each timestep if process in running will fork
	bool isprocessforked();  //check if process in forked
	Process* getchild();
	//******************************\\

	void settotaliod(int newtotiod);
	int gettotaliod();
	int getRT();
	void setRT(int t);
	void setFinalWT(int newwt);
	int getFinalWT();
	void setctp(int newctp);
	int getctp();
	void setwtt(int t);
	int getwtt();
};