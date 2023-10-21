#pragma once
#include"Processor.h"
#include"Queue.h"
#include"../Data Structures/linkedlist.h"
#include"../Process/Process.h"
class RR : public Processor
{
	Queue<Process*>Q1;
	Process* Running_Process;
	Linkedlist<Process*>Runninglist;
	Linkedlist<Process*>TRMlist;
	Linkedlist<Process*>blklist;
	int TC;
	int realTC;
	int current_Timestep;
	float processor_utilization;
	float processor_load;
	string processor_state;
	int processor_Busytime;
	int processor_Idealtime;
	int maxw;
	int RTf;
	int blkcount;
	Linkedlist<Process*>migratedlist;
	int ntimesteps=10;
public:
	//**************************************************************************************************************************\\

	RR()
	{
		blkcount = 0;
		processor_utilization = 0;
		processor_load = 0;
		processor_Busytime = 0;
		processor_Idealtime = 0;
		processor_state = "IDEAL";
		Running_Process = nullptr;
	}
	void SetTime_Slice(int T)
	{
		TC = T;
		save_TC();
	}
	int GETREAL_TC()
	{
		return realTC;
	}
	int getblkcount()
	{
		return blkcount;
	}
	void save_TC()
	{
		realTC = TC;
	}
	//**************************************************************************************************************************\\
	//****************************************MAIN FUNC*************************************************************************\\

	void add_process(Process* p)
	{
		Q1.enqueue(p);
	}
	void SchedulingAlgorthim()
	{
		Node<Process*>* front = Q1.returnfront();
		if (Runninglist.isempty())
		{
			if (Q1.isempty())
			{
				processor_state = "IDEAL";
				processor_Idealtime++;
				return;
			}
			Q1.deletefirst(Running_Process);
			Runninglist.add(Running_Process);
			Running_Process->setState("RUN");
			Running_Process->setRT(current_Timestep - (Running_Process->getAT()));
		}
		process_migration_condition();
		if (Running_Process->isblocked() != 0) //running isblocked
		{
			if (Running_Process->getCT() == 0)
			{
				TRMlist.add(Running_Process);
				Running_Process->setTerminationt_time(current_Timestep);
				Running_Process->setFinalWT(current_Timestep - Running_Process->getAT());
				Running_Process->setTRT((Running_Process->getTT()) - (Running_Process->getAT()));
				Running_Process = nullptr;
				Runninglist.deletefirst(Running_Process);
				return;
			}
			Queue<int>IO_R;
			Running_Process->getior(IO_R);
			int tempior;
			tempior = IO_R.peek();
			if (tempior != 0)
			{
				Running_Process->setCT((Running_Process->getCT()) - 1);
				if (Running_Process->getCT() == 0)
				{
					TRMlist.add(Running_Process);
					Running_Process->setTerminationt_time(current_Timestep);
					Running_Process->setFinalWT(current_Timestep - Running_Process->getAT());
					Running_Process->setTRT((Running_Process->getTT()) - (Running_Process->getAT()));
					Running_Process = nullptr;
					Runninglist.deletefirst(Running_Process);
					return;
				}
				tempior--;
				IO_R.sethead(tempior);
				Running_Process->setIO_R(IO_R);
				processor_Busytime++;
				setprocessorutlization();
				processor_state = "BUSY";
				if (tempior == 0)
				{
					IO_R.dequeue(tempior);
					Running_Process->setIO_R(IO_R);
					blklist.add(Running_Process);
					Running_Process->setState("BLK");
					blkcount++;
					Runninglist.deletefirst(Running_Process);
					return;
				}
				else
				{
					return;
				}
			}
		}
		Running_Process->setWT(gettimestep() - (Running_Process->getAT()));
		if (Running_Process->getCT() + TC != Running_Process->getCT())
		{
			Running_Process->setState("RUN");
			processor_state = "BUSY";
			Running_Process->setCT(Running_Process->getCT() - 1);
			TC--;
			processor_Busytime++;
			setprocessorutlization();
			
			
			if (Running_Process->getCT() == 0)
			{
				//add to termnate
				Running_Process->setState("TRM");
				Running_Process->setTerminationt_time(gettimestep());
				Running_Process->setFinalWT(current_Timestep - Running_Process->getAT());
				Running_Process->setTRT((Running_Process->getTT()) - (Running_Process->getAT()));
				
				TRMlist.add(Running_Process);
				Runninglist.deletefirst(Running_Process);
				TC = GETREAL_TC();
				return;
			}
		}
		else
		{
			Q1.enqueue(Running_Process);
			Runninglist.deletefirst(Running_Process);
			TC = GETREAL_TC();
		}
	}
	//**************************************************************************************************************************\\
	
	void move_frm_ready_torun()
	{
		if (Runninglist.isempty())
		{
			if (Q1.isempty())
			{
				processor_state = "IDEAL";
				processor_Idealtime++;
				return;
			}
			Q1.deletefirst(Running_Process);
			Runninglist.add(Running_Process);
		}
	}
	void process_migration_condition()
	{

		if ((Running_Process->getCT() >= RTf))
		{
			return;
		}
		while (Runninglist.Returnhead())
		{
			if ((Running_Process->getCT() < RTf))
			{
				migratedlist.add(Running_Process);
				Runninglist.deletefirst(Running_Process);
				Running_Process->setWT(gettimestep() - (Running_Process->getAT()));
				move_frm_ready_torun();

			}
			else
			{
				return;
			}
		}
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

	void deleteprocess(Process* p)
	{
		return;
	}
	void deleterunning()
	{
		return;
	}
	bool whereprocess(Process* p)
	{
		return false;
	}
	//**************************************************************************************************************************\\

	void settimestep(int timestep)
	{
		current_Timestep = timestep;
	}
	string getprocessorstate()
	{
		return processor_state;
	}
	//**************************************************************************************************************************\\

	int generateRN()
	{
		return rand() % 100 + 1;
	}
	//**************************************************************************************************************************\\

	int gettimestep()
	{
		return current_Timestep;
	}
	//**************************************************************************************************************************\\
	//****************************************GET POINTER FUNC******************************************************************\\

	Node<Process*>* get_pointer_readylist()
	{
		return Q1.returnfront();
	}
	Process* get_pointer_running()
	{
		return Running_Process;
	}
	Node<Process*>* get_preadylist()
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
	//**************************************************************************************************************************\\

	bool readyisempty()
	{
		return Q1.isempty();
	}
	bool isrunningnull()
	{
		if (Running_Process == NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//**************************************************************************************************************************\\

	int getcount()
	{
		return Q1.getcount();
	}
	//**************************************************************************************************************************\\

	int expected_finishing_time()
	{
		int expected_time = 0;
		Node<Process*>* temp = Q1.returnfront();
		while (temp)
		{
			expected_time = expected_time + temp->getitem()->getCT();
			temp = temp->getnext();
		}
		//	if (Running_Process)
		   //   {
			//	expected_time = expected_time + Running_Process->getCT();
		   //   }
		return expected_time;
	}
	//**************************************************************************************************************************\\

	void setprocessorLoad(int d)
	{

		 processor_load= processor_Busytime / d;

	}
	//**************************************************************************************************************************\\

	float getprocessorload()
	{
		return processor_load;
	}
	//**************************************************************************************************************************\\

	void setprocessorutlization()
	{
		float plus = processor_Busytime + processor_Idealtime;
		processor_utilization = 100 * (processor_Busytime / plus);
	}
	//**************************************************************************************************************************\\

	float getprocessorutlization()
	{
		return processor_utilization;
	}
	//**************************************************************************************************************************\\

	void setMAxW(int n)
	{
		maxw = n;
	}
	//**************************************************************************************************************************\\

	int getMaxw()
	{
		return maxw;
	}
	//**************************************************************************************************************************\\

	void setRTF(int t)
	{
		RTf = t;
	}
	//**************************************************************************************************************************\\
	//*****************************************************MIGRATION*************************************************************\\

	Node<Process*>* getMigrateprocess()
	{
		return migratedlist.Returnhead();
	}
	void dequeue_from_migratedlist()
	{
		if (migratedlist.Returnhead())
		{
			migratedlist.deletehead();
		}
	}
	int get_migrated_count()
	{
		return migratedlist.getcount();
	}
	//**************************************************************************************************************************\\

	void dequeue()
	{
		Process* p;
		Q1.dequeue(p);
	}
	//**************************************************************************************************************************\\

	void delete_termenation()
	{
		TRMlist.DeleteAll();
	}
	//**************************************************************************************************************************\\
	
	void delete_blocked()
	{
		blklist.DeleteAll();
	}
	void setntimesteps(int n)
	{
		ntimesteps = n;
	}

};