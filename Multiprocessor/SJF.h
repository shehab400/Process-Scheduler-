#pragma once
#include"LinkedQueue.h"
#include"Processor.h"
#include"../Data Structures/linkedlist.h"
#include"../Process/Process.h"
class SJF : public Processor
{
	Linkedqueue<Process*>Q1;
	Linkedlist<Process*>Running;
	Linkedlist<Process*>TRMlist;
	Linkedlist<Process*>blklist;
	Process *Running_process;
	int current_Timestep;
	string processor_state;
	float processor_utilization ;
	float processor_load;
	int processor_Busytime;
	int processor_Idealtime;
	int maxw;
	int RTF;
	int blkcount;
	int ntimesteps=10;
	
public:
	
	SJF()
	{
		blkcount = 0;
		processor_utilization = 0;
		processor_load = 0;
		processor_Busytime = 0;
		processor_Idealtime = 0;
		processor_state = "IDEAL";
		int i = 1;
		Running_process =nullptr;
	}
	//**************************************************************************************************************************\\
	
	void SchedulingAlgorthim()
	{		
			if (Running.isempty())
			{
				if (Q1.isempty())
				{
					processor_Idealtime++;
					processor_state = "IDEAL";
					return;
				}
				Running.add(Running_process);
				Q1.dequeue(Running_process);
				Running_process->setState("RUN)");
				Running_process->setRT(current_Timestep - (Running_process->getAT()));

			}
			//if (Running_process->isblocked()) //running isblocked
			//{
			//	Queue<int>IO_R;
			//	Running_process->getior(IO_R);
			//	int tempior;
			//	IO_R.dequeue(tempior);
			//	Running_process->setIO_R(IO_R);
			//	if (tempior!=0)
			//	{
			//		Running_process->setCT((Running_process->getCT()) - 1);
			//		tempior--;
			//		processor_Busytime++;
			//		setprocessorutlization();
			//		processor_state = "BUSY";
			//		if (tempior==0)
			//		{
			//			blklist.add(Running_process);
			//			Running_process->setState("BLK");
			//			blkcount++;
			//			Running.deletefirst(Running_process);					
			//			return;
			//		}
			//	}
			//}
			if (Running_process->isblocked() != 0) //running isblocked
			{
				if (Running_process->getCT() == 0)
				{
					TRMlist.add(Running_process);
					Running_process->setTerminationt_time(current_Timestep);
					Running_process->setFinalWT(current_Timestep - Running_process->getAT());
					Running_process->setTRT((Running_process->getTT()) - (Running_process->getAT()));
					Running_process = nullptr;
					Running.deletefirst(Running_process);
					return;
				}
				Queue<int>IO_R;
				Running_process->getior(IO_R);
				int tempior;
				tempior = IO_R.peek();
				if (tempior != 0)
				{
					Running_process->setCT((Running_process->getCT()) - 1);
					if (Running_process->getCT() == 0)
					{
						TRMlist.add(Running_process);
						Running_process->setTerminationt_time(current_Timestep);
						Running_process->setFinalWT(current_Timestep - Running_process->getAT());
						Running_process->setTRT((Running_process->getTT()) - (Running_process->getAT()));
						Running_process = nullptr;
						Running.deletefirst(Running_process);
						return;
					}
					tempior--;
					IO_R.sethead(tempior);
					Running_process->setIO_R(IO_R);
					processor_Busytime++;
					setprocessorutlization();
					processor_state = "BUSY";
					if (tempior == 0)
					{
						IO_R.dequeue(tempior);
						Running_process->setIO_R(IO_R);
						blklist.add(Running_process);
						Running_process->setState("BLK");
						blkcount++;
						Running.deletefirst(Running_process);
						return;
					}
					else
					{
						return;
					}
				}
			}
					
			if (Running_process->getCT() != 0)
			{
				processor_state = "BUSY";
				Running_process->setCT(Running_process->getCT() - 1);
				processor_Busytime++;
				setprocessorutlization();
				if (Running_process->getCT() == 0)
				{
					Running_process->setState("TRM");
					Running_process->setTerminationt_time(gettimestep());
					Running_process->setFinalWT(current_Timestep - Running_process->getAT());
					Running_process->setTRT((Running_process->getTT()) - (Running_process->getAT()));
			
					TRMlist.add(Running_process);
					Running.deletefirst(Running_process);
					return;
				}

			}	
			else
			{
				TRMlist.add(Running_process);
				Running.deletefirst(Running_process);
				return;
			}
	}
	void setntimesteps(int n)
	{
		ntimesteps = n;
	}
	string getprocessorstate()
	{
		return processor_state;
	}
	void processoroverheat()
	{
		int r = generateRN();
		if (r <= 2)
		{
			if (ntimesteps != 0)
			{
				processor_state = "STOP";
				ntimesteps--;
				if (ntimesteps == 0)
				{
					if (Q1.isempty())
					{
						processor_state = "IDEAL";
					}
					else
					{
						processor_state = "BUSY";
					}
					return;
				}
				else
				{
					return;
				}
			}


		}
	}
	//**************************************************************************************************************************
	Node<Process*>* Get_Pready()
	{
		return Q1.returnfront();
	}

	void add_process(Process* p)
	{
		Q1.enqueue(p, p->getCT());
	}
	//**************************************************************************************************************************
	void removeprocess()
	{
		Process* r;
		Q1.dequeue(r);
	}
	//**************************************************************************************************************************
	void deleterunning()
	{
		return;
	}
	bool whereprocess(Process* p)
	{
		return false;
	}
	void deleteprocess(Process* p)
	{
		return;
	}
	//**************************************************************************************************************************
	void printlist()
	{
		while (!Q1.isempty())
		{
			Process* p;
			Q1.dequeue(p);
			cout << p->getCT() << " ";
		}
	}
	//**************************************************************************************************************************
	int Getcount()
	{
		 return Q1.getcount();
	}
	//**************************************************************************************************************************
	int generateRN()
	{
		return rand() % 100 + 1;
	}
	//**************************************************************************************************************************
	void settimestep(int timestep)
	{
		 current_Timestep= timestep;
	}
	//**************************************************************************************************************************
	bool isrunningnull()
	{
		if (Running_process == NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	int gettimestep()
	{
		return current_Timestep;
	}
	Process* get_pointer_running()
	{
		if (Running_process!=nullptr)
		{
			return Running_process;
		}
		return nullptr;
    }
	Node<Process*>* get_pointer_readylist()
	{
		 return Q1.returnfront();
	 }
	Node<Process*>* gettrmlisthead()
	{
		return TRMlist.Returnhead();
	}
	Node<Process*>* getblklisthead()
	{
		return blklist.Returnhead();
	}
	bool readyisempty()
	{
		return Q1.isempty();
	}
	int getcount()
	{
		return Q1.getcount();
	}
	int expected_finishing_time()
	{
		int expected_time = 0;
		Node<Process*>*temp = Q1.returnfront();
		while (temp)
		{
			expected_time = expected_time + temp->getitem()->getCT();
			temp = temp->getnext();
		}
		/*expected_time = expected_time + Running_process->getCT();*/
		return expected_time;
	}
	int getblkcount()
	{
		return blkcount;
	}
	void setprocessorLoad(int d)
	{

		processor_load = processor_Busytime / d;

	}
	float getprocessorload()
	{
		return processor_load;
	}
	void setprocessorutlization()
	{
		float plus = processor_Busytime + processor_Idealtime;
		processor_utilization = 100*(processor_Busytime / plus);
	}
	float getprocessorutlization()
	{
		return processor_utilization;
	}
	Node<Process*>* getMigrateprocess()
	{
		return nullptr;
	}
	void dequeue()
	{
		Process* p;
		Q1.dequeue(p);
	}
	void delete_termenation()
	{
		TRMlist.DeleteAll();
	}
	void delete_blocked()
	{
		blklist.DeleteAll();
	}
	void setMAxW(int n)
	{
		maxw = n;
	}
	void setRTF(int t)
	{
		RTF = t;
	}
	void SetTime_Slice(int T)
	{
		int i = T;
	}
};

