#include "Process.h"
#include <iostream>
#include<sstream>
using namespace std;
Process::Process()
{

}
//ostream& operator<<(ostream& os, const Process& p)
//{
//	os << "PID: " << p.PID << " " << "AT: " << p.AT << " " << "CT: " << p.CT << " " << "N: " << p.N << " ";
//	for (int i = 0; i < p.N; i++)
//		os << "(" << p.IO_R[i] << "," << "" << p.IO_D[i] << ")";
//	return os;
//}

//Process::Process(int at, int pid, int ct, int n, int* io_r, int* io_d)
//{
//	AT = at; PID = pid; CT = ct; N = n; IO_R = io_r; IO_D = io_d;
//}
//Process::Process(int at, int pid, int ct)
//{
//	AT = at; PID = pid; CT = ct; N = 0;
//}
void Process::setNFCFS(int n)
{
	NFCFS = n;
}
int Process::getNCFS()
{
	return NFCFS;
}
void Process::setSJF(int n)
{
	NSJF = n;
}
int Process::getSJF()
{
	return NSJF;
}
void Process::setTRT(int trt)
{
	TRT = trt;
}
void Process::setRR(int n)
{
	NRR = n;
}
int Process::getRR()
{
	return NRR;
}
int Process::getAT()
{
	return AT;
}

void Process::setAT(int newat)
{
	AT = newat;
}

int Process::getCT()
{
	return CT;
}
int Process::getID()
{
	return PID;
}
void Process::setCT(int newct)
{
	CT = newct;
}


void Process::setPID(int newpid)
{
	PID = newpid;
}

void Process::setN(int newn)
{
	N = newn;
}

int Process::getn()
{
	return N;
}

void Process::setIO_R(Queue<int> newior)
{
	IO_R = newior;
}

void Process::setIO_D(Queue<int> newiod)
{
	IO_D = newiod;
}

void Process::getiod(Queue<int>& tempiod)
{
	tempiod = IO_D;
}

void Process::getior(Queue<int>&tempior)
{
	tempior = IO_R;
}

void Process::setState(string newstate)
{
	if (newstate=="BLK")
	{
		State = "BLK";
	}
	if (newstate == "RDY")
	{
		State = "RDY";
	}
	if (newstate == "RUN")
	{
		State = "RUN";
	}
	if (newstate == "TRM")
	{
		State = "TRM";
	}
}
 
int Process::getTRT()
{
	return TRT;
}

void Process::setTerminationt_time(int t)
 {
	 TT = t;
 }

int Process::getTT()
{
	return TT;
}

void Process::setWT(int t)
{
	WT = t;
}
int Process::isblocked()
{
	if (IO_R.isempty())
	{
		return 0;
	}
	return IO_R.getcount();
}
void Process::dequeeIOR()
{
	int s;
	IO_R.dequeue(s);
}

int Process::getWT()
{
	return WT;
}
int Process::getcurrentiod()
{
	return currentiod;
}
void Process::setcurrentiod(int newiod)
{
	currentiod = newiod;
}

void Process::decrementblk()
{
	N--;
	
}
int Process::getcurrentior()
{
	return currentior;
}



void Process::setFP(int f)
{
	FP = f;
}
int Process::getFP()
{
	return FP;
}

//*********************************************************************************\\
//***********************************FORK*******************************************\\

void Process::setchild(Process* p)
{
	child = p;
}

bool Process::isparent()
{
	if (child)
	{
		return true;
	}
	return false;
}

void Process::setforked()
{
	isforked = true;
}

void Process::setwillfork(bool s)
{
	willfork = s;
}

bool Process::willprocessfork()
{
	return willfork;
}

bool Process::isprocessforked()
{
	if (isforked)
	{
		return true;
	}
	else
	{
		return false;
	}
}
Process* Process::getchild()
{
	return child;
}
//*********************************************************************************\\
//*********************************************************************************\\


void Process::settotaliod(int newtotiod)
{
	totaliod = totaliod + newtotiod;
}

int Process::gettotaliod()
{
	return totaliod;
}

int Process::getRT()
{
	return RT;
}

void Process::setRT(int t)
{
	RT = t;
}

void Process::setFinalWT(int newwt)
{
	FinalWT = newwt;
}

int Process::getFinalWT()
{
	return FinalWT;
}

void Process::setctp(int newctp)
{
	ctp=newctp;
}

int Process::getctp()
{
	return ctp;
}
void Process::setwtt(int t)
{
	wtt = t;
}

int Process::getwtt()
{
	return wtt;
}


