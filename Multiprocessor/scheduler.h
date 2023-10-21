#pragma once
#include <iostream>
#include <fstream>

#include"../Data Structures/linkedlist.h"
#include"../Process/Process.h"
#include"FCFS.h"
#include"Processor.h"
#include"SJF.h"
#include"RR.h"
#include <random>
#include <iomanip>
using namespace std;
class processSch
{
private:
	Queue<Process*> TRMlist;
	int trmnLCount;
	Linkedlist <Process*> BLK;
	int TimeStep;
	Linkedlist<Process*> Processlist;
	
	double num_WorkSteal;
	double numprocess;
	int blockcount;
	int NFCFS, NSJF, NRR; //Processor types
	double maxTime;
	double minTime;
	int numprocessors;
	double numforked;
	Process p;
	int RTF, MAXW, STL, FP;
	Queue<int>  kill_PID;
	Queue<int>  kill_time;
	double cntkillprocess = 0;
public:
	FCFS** F11 = new FCFS * [NFCFS];
	RR** R11 = new RR * [NRR];
	SJF** S11 = new SJF * [NSJF];
	Processor** Processorarr;
	processSch()
	{
		num_WorkSteal = 0;
		numprocessors = 0;
		NFCFS = 0;
		NSJF = 0;
		NRR = 0;
		MAXW = 0;
		RTF = 0;
		blockcount = 0;
		TimeStep = 1;
		trmnLCount = 0;
		numprocess = 0;
		maxTime = -1;
		minTime = 999999999999999;
		numforked = 0;
		STL = 0;
		FP = 0;
		cntkillprocess = 0;
		load();
		
	}
	void simulate()
	{
		addtoBLK();
		killingprocess();
		moveTOready();
		MoveFromBLKtoReady();
		Migrate_process_toRR();
		Migrate_process_toSJF();
		WorkStealing();
		CallingSchedulingAlgorthim();
		Migrate_process_toRR();
		Migrate_process_toSJF();
		processorOverheat();
		addtoTRMN();
		//setpload();
		//forking
		//kill orphan
	}
	void setpload()
	{
		int sumtrt=0;
		Node<Process*>* templ = TRMlist.returnfront();
		while (templ)
		{
			sumtrt = sumtrt + templ->getitem()->getTRT();
			templ = templ->getnext();
		}
		if (sumtrt != 0)
		{
			for (int i = 0; i < numprocessors; i++)
			{
				Processorarr[i]->setprocessorLoad(sumtrt);
			}
		}
	}
	void killingprocess()// loop on FCFS processors and call Kill signal on them
	{
		Process* pkill = nullptr;

		for (int i = 0; i < NFCFS; i++)
		{
			bool next = 1;// equals 1 if next item in killtime is the same as current timestep
			while (next)
			{
				pkill = F11[i]->killsignal(cntkillprocess, next); //call kilsignal on all FCFS and return process*
				if (pkill)
				{
					kill(pkill);
				}
			}

		}
	}
	int getnumprocess()
	{
		return numprocess;
	}

	Node<Process*>* ReturnTRMhead()
	{
		return TRMlist.returnfront();
	}
	Node<Process*>* ReturnBLKhead()
	{
		return BLK.Returnhead();
	}
	int GetNFCFS()
	{
		return NFCFS;
	}
	int GetNSJF()
	{
		return NSJF;
	}
	int GetNRR()
	{
		return NRR;
	}
	void SetData()
	{
		for (int i = 0; i < numprocessors; i++)
		{
			Processorarr[i]->setMAxW(MAXW);
			Processorarr[i]->setRTF(RTF);
		}
	}
	void addtoBLK()
	{
		for (int i = 0; i < numprocessors; i++)
		{
			if (Processorarr[i]->getblkcount() != 0)
			{
				Node<Process*>* tempz = Processorarr[i]->getblklisthead();
				while (tempz)
				{
					BLK.add(tempz->getitem());
					blockcount++;
					tempz = tempz->getnext();
				

				}
				Processorarr[i]->delete_blocked();
			}
		}

	}
	double getkilledpercentage()
	{
		    double denom = numprocess;
			return	(cntkillprocess / denom) * 100;
	}
	int getavgwt()
	{
		int tempwt = 0;
		Node<Process*>* temppp = Processlist.Returnhead();
		while (temppp)
		{
			tempwt = tempwt + temppp->getitem()->getFinalWT();
			temppp = temppp->getnext();
		}
		return tempwt;
	}

	int getavgrt()
	{
		int temprt = 0;
		Node<Process*>* temppp = Processlist.Returnhead();
		while (temppp)
		{
			temprt = temprt + temppp->getitem()->getRT();
			temppp = temppp->getnext();
		}
		return temprt;
	}

	int getavgtrt()
	{
		int temptrt = 0;
		Node<Process*>* temppp = Processlist.Returnhead();
		while (temppp)
		{
			temptrt = temptrt + temppp->getitem()->getTRT();
			temppp = temppp->getnext();
		}
		return temptrt;
	}

	int getblockcount()
	{
		return blockcount;
	}

	void MoveFromBLKtoReady() //handles blocked processes until they are done receiving I/O and sends them back to processors

	{
		Queue<int>tempq;

		if (blockcount != 0)
		{
			Node<Process*>* tempB = BLK.Returnhead();
			if (tempB)
			{
				int currentiod;
				currentiod = tempB->getitem()->getcurrentiod();
				if (currentiod <= 0)  //no process is currently getting I/O
				{
					tempB->getitem()->getiod(tempq); //get IO_D Queue from process

					tempq.dequeue(currentiod);
					tempB->getitem()->setIO_D(tempq);
					tempB->getitem()->setcurrentiod(currentiod); //sets current IO_D;
					tempB->getitem()->settotaliod(currentiod); //adds current IO_D to total
				}

				else //process was previously getting I/O
				{
					currentiod = currentiod - 1; //Decrement current I/O required time;
					tempB->getitem()->setcurrentiod(currentiod);
					if (currentiod == 0) //if process finished getting I/O
					{
						Processor* p = GetPwithShortestReady(); 

						tempB->getitem()->setState("RDY");
						p->add_process(tempB->getitem()); //Adds the process to the processor with shortest ready list
						blockcount--;
						Process* hd = tempB->getitem();
						tempB = tempB->getnext();
						BLK.deletenode(hd); //dequeue head


					}
					
				}


			}
		}

	}
	void moveTOready()//bta5od process mn el new w twdiha ll ready w btreturn a5er process 
	{
		for (int i = 0; i < numprocessors; i++)
		{
			Processorarr[i]->settimestep(TimeStep);
		}
		Node<Process*>* temp = Processlist.Returnhead();
		while (temp)
		{
			
			if (temp->getitem()->getAT() == gettimestep())
			{
				
				GetPwithShortestReady()->add_process(temp->getitem());
				temp->getitem()->setState("RDY");
				temp = temp->getnext();
			}
			else
			{
				temp = temp->getnext();
			}

		}
	}
	Processor* GetPwithLongestReady()
	{
		Processor* PwLongestReady = NULL;
		for (int i = 0; i < numprocessors; i++)
		{
			if (Processorarr[i]->expected_finishing_time() == GetLongestReady())
			{
				PwLongestReady = Processorarr[i];
				
			}

		}
		return PwLongestReady;
	}
	Processor* GetPwithShortestReady()
	{
		Processor* PwShortestReady = NULL;

		for (int i = 0; i < numprocessors; i++)
		{
			if (Processorarr[i]->expected_finishing_time() == GetShortestReady())
			{
				PwShortestReady = Processorarr[i];
				
			}

		}
		return PwShortestReady;
	}
	double GetShortestReady()
	{
		minTime = 10909;
		for (int i = 0; i < numprocessors; i++)
		{
			if (Processorarr[i]->expected_finishing_time() < minTime)
			{
				minTime = Processorarr[i]->expected_finishing_time();

			}

		}
		return minTime;
	}
	double GetLongestReady()
	{
		maxTime = -1;

		for (int i = 0; i < numprocessors; i++)
		{
				if (Processorarr[i]->expected_finishing_time()>maxTime)
				{
					maxTime = Processorarr[i]->expected_finishing_time();
				}

		}
		if (maxTime != 0)
			return maxTime;
		else
			return -1;
	}
	void WorkStealing()
	{
		

		if (GetLongestReady() != -1 && GetShortestReady()>0  )
		{
			double LT1 = GetLongestReady();
			double ST1 = GetShortestReady();
			Linkedlist<double> LT;
			Linkedlist<double> ST;
			while ((GetLongestReady() - GetShortestReady()) / GetLongestReady() > 0.4)
			{
;
				Node<Process*>* tempW = GetPwithLongestReady()->get_pointer_readylist();
				Processor* tempP = GetPwithLongestReady();
				while (tempW->getitem()->isprocessforked())
				{
					if (tempW->getnext() != nullptr)
					{
						tempW = tempW->getnext();
					}
					else
					{
						tempW = nullptr;
						break;
					}

				}
				if (tempW != nullptr)
				{
					GetPwithShortestReady()->add_process(tempW->getitem());
					tempP->dequeue();
					num_WorkSteal++;
					
				}
				else
				{
					break;
				}

				if (!LT.isempty() && !ST.isempty())
				{
					Node<double>* tempL = LT.Returnhead();
					Node<double>* tempS = ST.Returnhead();
					bool Exit = false;
					while (tempL && tempS)
					{
						
							if (tempL->getitem() == GetLongestReady() && tempS->getitem() == GetShortestReady())
							{
								Exit = true;
								break;
							}
							else 
							{
								tempL = tempL->getnext();
								tempS = tempS->getnext();
							}
					}
					if (Exit)
					{
						num_WorkSteal--;
						LT.DeleteAll();
						ST.DeleteAll();
						break;
					}
				}
				
				if (GetLongestReady()==LT1 && GetShortestReady()==ST1)
				{
					num_WorkSteal--;
					break;
				}

				LT.add(GetLongestReady());
				ST.add(GetShortestReady());
			}
		}
	}

	double worksteal_percentacge()
	{
		double WPercent = 0;
		WPercent = num_WorkSteal / numprocess;
		return WPercent * 100;
	}
	void IncrementTimeStep()
	{
		TimeStep++;
	}
	void addtoTRMN() //hta5od process w t7otha f el trmn list(l argument list bt3tha process w bta5od current size bta3 trmn list)
	{
		for (int i = 0; i < numprocessors; i++)
		{
			Node<Process*>* tempz = Processorarr[i]->gettrmlisthead();
			while (tempz)
			{
				TRMlist.enqueue(tempz->getitem());
				trmnLCount++;
				tempz = tempz->getnext();

			}
			Processorarr[i]->delete_termenation();
		}

	}
	void reset()
	{

		while (!TRMlist.isempty())
		{
			Process* temp0 = TRMlist.peek();
			TRMlist.dequeue(temp0);
			
		}
		num_WorkSteal = 0;
		BLK.DeleteAll();
		numprocessors = 0;
		NFCFS = 0;
		NSJF = 0;
		NRR = 0;
		MAXW = 0;
		RTF = 0;
		blockcount = 0;
		TimeStep = 1;
		trmnLCount = 0;
		numprocess = 0;
		maxTime = -1;
		minTime = 999999999999999;
		numforked = 0;
		STL = 0;
		FP = 0;
		cntkillprocess = 0;
		load();
	}
	int generateRN()
	{
		return rand() % 100 + 1;
	}
	void load() // complexity O(N^2)
	{
		string Filename = "input.txt"; //string that contains imput file name

		ifstream input;
		input.open(Filename, ios::in); //open file to read data

		char b; //character to get rid of brackets and commas
		//int NFCFS, NSJF, NRR ; //Processor types
		int RRTS; //Round Robin Timeslice
		//
		//Number of processes
		int AT, PID, CT, N; //Process attributes
		int sgkl = 0; //Sigkill counter
		string p;
		if (input.is_open())
		{
			input >> NFCFS >> NSJF >> NRR >> RRTS >> RTF >> MAXW >> STL >> FP >> numprocess; //read all data before processes

			///INITIATE PROCESSORS/////////////////////////////////
			numprocessors = NSJF + NRR + NFCFS;
			Processorarr = new Processor * [numprocessors];
			SJF* SJFARR = new SJF[NSJF];
			FCFS* FCFSARR = new FCFS[NFCFS];
			RR* RRARR = new RR[NRR];
			SJF** S1 = new SJF * [NSJF];
			FCFS** F1 = new FCFS * [NFCFS];
			RR** R1 = new RR * [NRR];
			int c = 0;
			for (int i = 0; i < NSJF; i++)
			{
				S1[i] = &SJFARR[i];
				Processorarr[c] = &SJFARR[i];
				c++;
			}
			for (int i = 0; i < NFCFS; i++)
			{
				F1[i] = &FCFSARR[i];
				Processorarr[c] = &FCFSARR[i];
				c++;
			}
			for (int i = 0; i < NRR; i++)
			{
				R1[i] = &RRARR[i];
				Processorarr[c] = &RRARR[i];
				Processorarr[c]->SetTime_Slice(RRTS);
				c++;

			}
			F11 = F1;
			R11 = R1;
			S11 = S1;
			/////////////////////////////////////////////////

			 //sigkill
			
			int tempkillpid;
			int tempkilltime;
			Linkedlist<Process*> P;



			for (int i = 0; i < numprocess; i++)
			{

				input >> AT >> PID >> CT >> N;
				if (N)
				{
					//int* IO_D = new int[N]; // initialize array of IO_D for each process
					//int* IO_R = new int[N]; // initialize array of IO_R for each process
					Queue<int> IOD;
					Queue<int> IOR;
					int tempior;
					int tempiod;
					for (int j = 0; j < N; j++)
					{

						input >> b >> tempior >> b >> tempiod >> b; // Read IO_R and IO_D and insert in array
						IOR.enqueue(tempior);
						IOD.enqueue(tempiod);

						if (j != N - 1) // read extra character as long as last values are not read yet as there is a comma before opening bracket
							input >> b;
					}


					Process* Processs = new Process();
					Processs->setAT(AT);
					Processs->setPID(PID);
					Processs->setCT(CT);
					Processs->setN(N);
					Processs->setIO_D(IOD);
					Processs->setIO_R(IOR);
					Processs->setctp(CT);
					P.add(Processs); //call processor constructor for any N!=0
				}
				else
				{
					Process* Processs = new Process();
					Processs->setAT(AT);
					Processs->setPID(PID);
					Processs->setCT(CT);
					Processs->setN(N);
					P.add(Processs); //call processor constructor for any N == 0
				}
			}
			while (input >> tempkilltime)//read sigkill data
			{
				kill_time.enqueue(tempkilltime);
				input >> tempkillpid;
				kill_PID.enqueue(tempkillpid);
				sgkl++;
			}

			
			Processlist = P;
			int kpid;
			int kt;
			Queue<int> killpid2;
			Queue<int>  killtime2;
			for (int i = 0; i < NFCFS; i++)
			{
				while (kill_PID.dequeue(kpid))
				{
					kill_time.dequeue(kt);
					F11[i]->setkpid(kpid);
					F11[i]->setkt(kt);
					killpid2.enqueue(kpid);
					killtime2.enqueue(kt);
				}
				while (killpid2.dequeue(kpid))
				{
					killtime2.dequeue(kt);
					kill_PID.enqueue(kpid);
					kill_time.enqueue(kt);
				}

			}
		}

	}
	
	int gettimestep()
	{
		return TimeStep;
	}
	int getterm_count()
	{
		return trmnLCount;
	}
	int getNum_processors()
	{
		return numprocessors;
	}

	//**************************************************************************************************************************\\
	//**********************************************MIGRATE**********************************************************************\\

	Processor* Get_Shortest_SJFprocessor()
	{
		Processor* minprocessor = nullptr;
		int min = 100000;
		for (int i = 0; i < NSJF; i++)
		{
			if (S11[i]->expected_finishing_time() < min)
			{
				minprocessor = S11[i];

			}
			return minprocessor;

		}
	}
	Processor* Get_Shortest_RRprocessor()
	{
		Processor* minprocessor = nullptr;
		int min = 100000;
		for (int i = 0; i < NSJF; i++)
		{
			if (R11[i]->expected_finishing_time() < min)
			{
				minprocessor = R11[i];

			}
			return minprocessor;

		}
	}
	int get_migrated_RTFpercentage()
	{
		int migratedcount = 0;
		for (int i = 0; i < NRR; i++)
		{
			migratedcount = R11[i]->get_migrated_count() + migratedcount;
		}
		if (numprocess != 0)
		{
			int migratedpercentage = (migratedcount / numprocess) * 100;
			return migratedpercentage;
		}
	}
	int get_migrated_MAXWPercentge()
	{
		int migratedcount = 0;
		for (int i = 0; i < NFCFS; i++)
		{
			migratedcount = F11[i]->get_migrated_count() + migratedcount;
		}
		if (numprocess != 0)
		{
			int migratedpercentage = (migratedcount / numprocess) * 100;
			return migratedpercentage;
		}
	}
	int Get_average_processorutlization()
	{
		int total_processor_utlization = 0;
		int average_proceccesor_utlization = 0;
		for (int i = 0; i < numprocessors; i++)
		{
			total_processor_utlization = Processorarr[i]->getprocessorutlization() + total_processor_utlization;
		}
		if (numprocess != 0)
		{


			average_proceccesor_utlization = total_processor_utlization / numprocessors;
			return average_proceccesor_utlization;
		}
	}

	void Migrate_process_toRR()
	{

			for (int i = 0; i < NFCFS; i++)
			{
				Node<Process*>* migrateprocess = F11[i]->getMigrateprocess();
				
				
					while (migrateprocess)
					{
						if (!(migrateprocess->getitem()->isprocessforked()))
						{
							Get_Shortest_RRprocessor()->add_process(migrateprocess->getitem());
							migrateprocess = migrateprocess->getnext();
							F11[i]->dequeue_from_migratedlist();
							
						}
						else
						{
							migrateprocess = migrateprocess->getnext();
							F11[i]->dequeue_from_migratedlist();
						}
					}
				
			}
	}
	void Migrate_process_toSJF()
	{
			for (int i = 0; i < NRR; i++)
			{
				Node<Process*>* migrateprocess = R11[i]->getMigrateprocess();
				while (migrateprocess)
				{
					if (!(migrateprocess->getitem()->isprocessforked()))
					{
					
					Get_Shortest_SJFprocessor()->add_process(migrateprocess->getitem());
					migrateprocess = migrateprocess->getnext();
					R11[i]->dequeue_from_migratedlist();
				    }
					else
					{

						R11[i]->dequeue_from_migratedlist();
					}
				}
			}
	}
	//**************************************************************************************************************************\\
	//***************************************************************************************************************************\\

	void CallingSchedulingAlgorthim()
	{
		p.setFP(FP);
		for (int i = 0; i < numprocessors; i++)
		{
			Processorarr[i]->SchedulingAlgorthim();
			if (!Processorarr[i]->isrunningnull())
			{
				if (Processorarr[i]->get_pointer_running()->willprocessfork())
				{
					Fork(Processorarr[i]->get_pointer_running());
					Processorarr[i]->get_pointer_running()->setwillfork(false);
				}
			}
		}
	}
	//**************************************************************************************************************************\\
	//**********************************************FORK*************************************************************************\\

	double getforkprocesspercent()
	{
		double ff = 0;
		ff = (numforked / numprocess);
		return ff * 100;
		
	}
	void Fork(Process* pp)
	{
		Process* p = new Process();
		p->setPID(pp->getID() + 1000);
		p->setCT(pp->getCT());
		p->setAT(gettimestep());
		pp->setchild(p);
		pp->setforked();
		Processor* min = Processorarr[GetNSJF()];
		for (int i = GetNSJF()+1; i < (GetNSJF() + GetNFCFS()); i++)
		{
			if (min->expected_finishing_time()> Processorarr[i]->expected_finishing_time())
			{
				min = Processorarr[i];
			}
		}
		min->add_process(p);
		numprocess++;
		numforked++;
	}
	//**************************************************************************************************************************\\

	void kill(Process* m)
	{
		for (int i = GetNSJF(); i < (GetNSJF() + GetNFCFS()); i++) //loop to search in running
		{
			if (!Processorarr[i]->isrunningnull())
			{
				if (Processorarr[i]->get_pointer_running()->getID() == m->getID())
				{
					TRMlist.enqueue(Processorarr[i]->get_pointer_running());
					Processorarr[i]->get_pointer_running()->setState("TRM");
					Processorarr[i]->get_pointer_running()->setTerminationt_time(gettimestep());
					Processorarr[i]->get_pointer_running()->setTRT((Processorarr[i]->get_pointer_running()->getTT()) - (Processorarr[i]->get_pointer_running()->getAT()));
					Processorarr[i]->deleterunning();
					cntkillprocess++;
					return;
				}
			}
		}
		for (int i = GetNSJF(); i < (GetNSJF() + GetNFCFS()); i++) //loop to search in running
		{
			if (Processorarr[i]->whereprocess(m)) //where search for the process in the ready list
			{
				Processorarr[i]->deleteprocess(m);
				TRMlist.enqueue(Processorarr[i]->get_pointer_running());
				Processorarr[i]->get_pointer_running()->setTerminationt_time(gettimestep());
				Processorarr[i]->get_pointer_running()->setTRT((Processorarr[i]->get_pointer_running()->getTT()) - (Processorarr[i]->get_pointer_running()->getAT()));
				cntkillprocess++;
				return;
			}
		}
		if (m != NULL)
		{
			if (m->isparent()) //recursive call to get the last child (if exist)
			{
				return kill(m->getchild());
			}
		}
	}
	void setprocessorstimestep()
	{
		for (int i = 0; i < numprocessors; i++)
		{
			Processorarr[i]->settimestep(TimeStep);
		}
	}
	void processorOverheat()
	{
		for (int i = 0; i <numprocessors;i++)
		{
			if (Processorarr[i]->getprocessorstate()=="STOP")
			{
				if (Processorarr[i]->get_pointer_running()!=nullptr)
				{
					Process* r = Processorarr[i]->get_pointer_running();
					GetPwithShortestReady()->add_process(r);
				}
				if (Processorarr[i]->get_pointer_readylist()!=nullptr)
				{
					Node<Process*>* RDY = Processorarr[i]->get_pointer_readylist();
					while (RDY)
					{
						GetPwithShortestReady()->add_process(RDY->getitem());
						RDY = RDY->getnext();
						Processorarr[i]->dequeue();
					}
					
				}	
			}	
		}
	}
	//**************************************************************************************************************************
	void opfile()
	{
		string Filename = "output.txt"; //string that contains output file name
		ofstream output;
		output.open(Filename, ios::out);
		output << "TT" << setw(6) << "PID" << setw(6) << "AT" << setw(6) << "CT" << setw(6) << "IO_D" << setw(6) << "WT" << setw(6) << "RT" << setw(6) << " TRT" << endl;
		Node <Process*>* temp = ReturnTRMhead();
		while (temp) //loop om processes and print their members
		{
			output << left << setw(6) << temp->getitem()->getTT() << setw(6) << temp->getitem()->getID() << setw(6) << temp->getitem()->getAT() << setw(6) << temp->getitem()->getctp() << setw(6) << temp->getitem()->gettotaliod() << setw(6) << temp->getitem()->getFinalWT() << setw(6) << temp->getitem()->getRT() << setw(6) << temp->getitem()->getTRT() << endl;
			temp = temp->getnext();
		}
		output << "Processes: " << getnumprocess() << endl;
		output << "Avg WT = " << getavgwt() / numprocess << ",     Avg RT = " << getavgrt() / numprocess << ",     Avg TRT =" << getavgtrt()/numprocess << endl;
		output << "Migration %: " << "  RTF = " << get_migrated_RTFpercentage() << "%,     MaxW = " << get_migrated_MAXWPercentge() << " % " << endl;
		output << "Work Steal %:"<< worksteal_percentacge() <<"%" << endl;
		output << "Forked Process:" << getforkprocesspercent() << "%" << endl;
		output << "Killed Process:" << getkilledpercentage() << "%" << endl;
		output << "Processors:" << GetNFCFS() + GetNRR() + GetNSJF() << "[" << GetNFCFS() << " FCFS, " << GetNSJF() << " SJF, " << GetNRR() << " RR]"<<endl;
		output << "Processors Load" << endl;
		for (int i = 0; i < numprocessors; i++) //loop on processors and print their load percentage
		{
			int j = i + 1;
			output << "p" << j << "=" << Processorarr[i]->getprocessorload() << "%    ";
		}
		output << endl;
		output << "Processor Utiliz" << endl;
		for (int i = 0; i < numprocessors; i++)//loop on processors and print their utilization
		{
			int j = i + 1;
			output << "p" << j << "=" << Processorarr[i]->getprocessorutlization() << "%    ";
		}
		output << endl;
		output << "Avg utilization = " << Get_average_processorutlization() << "%";
	}
	int getnumfork()
	{
		return numforked;
	}
};