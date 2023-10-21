#pragma once
#include "Processor.h"
#include"../Data Structures/linkedlist.h"
#include"../Process/Process.h"
//#include"scheduler.h"
class FCFS : public Processor
{
	Linkedlist<Process*>l1;
	Linkedlist<Process*>Running;
	Process* Running_Process;
	Linkedlist<Process*>TRMlist;
	Linkedlist<Process*>blklist;
	int current_Timestep;
	float processor_utilization;
	float processor_load;
	string processor_state;
	int processor_Busytime;
	int processor_Idealtime;
	int maxw;
	int RTF;
	int blkcount;
	Queue<int> killpid;
	Queue<int> killtime;
	Linkedlist<Process*>Migarate_Process;
	int killcount=0;
	int ntimesteps=10;
public:

	FCFS()
	{
		blkcount = 0;
		processor_utilization = 0;
		processor_load = 0;
		processor_Busytime = 0;
		processor_Idealtime = 0;
		processor_state = "IDEAL";
		Running_Process = nullptr;
	}
	void setkpid(int newp)
	{
		killpid.enqueue(newp);
	}
	void setkt(int newt)
	{
		killtime.enqueue(newt);
	}
	void setkillpid(Queue<int> newkillpid)
	{
		killpid = newkillpid;
	}
	void setkilltime(Queue<int> newkilltime)
	{
		killtime = newkilltime;

	}
	void SchedulingAlgorthim()
	{
		Node<Process*>* front = l1.Returnhead();
		/*move_frm_ready_torun();*/
		
		if (Running.isempty())
		{
			if (l1.isempty())
			{
				processor_state = "IDEAL";
				processor_Idealtime++;
				return;
			}
			l1.deletefirst(Running_Process);
			Running.add(Running_Process);
			Running_Process->setState("RUN");
			Running_Process->setRT(current_Timestep - (Running_Process->getAT()));
		}
		processoroverheat();
		process_migration_condition();
	//	incrementWT();
		if (Running_Process->isblocked()!=0) //running isblocked
		{
			if (Running_Process->getCT() == 0)
			{
				TRMlist.add(Running_Process);

				Running_Process->setTerminationt_time(current_Timestep);
				Running_Process->setFinalWT(current_Timestep - Running_Process->getAT());
				Running_Process->setTRT((Running_Process->getTT()) - (Running_Process->getAT()));
				Running_Process = nullptr;
				Running.deletefirst(Running_Process);
			
				return;
			}
			Queue<int>IO_R;
			Running_Process->getior(IO_R);
			int tempior;
			/*IO_R.dequeue(tempior);
			Running_Process->setIO_R(IO_R);*/
			tempior = IO_R.peek();
			if (tempior != 0)
			{
				Running_Process->setCT((Running_Process->getCT()) - 1);
				if (Running_Process->getCT()==0)
				{
					TRMlist.add(Running_Process);
					Running_Process->setTerminationt_time(current_Timestep);
					Running_Process->setFinalWT(current_Timestep - Running_Process->getAT());
					Running_Process->setTRT((Running_Process->getTT()) - (Running_Process->getAT()));
					Running_Process = nullptr;
					Running.deletefirst(Running_Process);
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
					Running.deletefirst(Running_Process);
					return;
				}
				else
				{
					return;
				}
			}
		}
		int p = generateRN();

		if (Running_Process->getCT() != 0 && !killtime.isempty())
		{
			//p < Running_Process->getFP() &&
			if ( !Running_Process->isparent())
			{
				Running_Process->setwillfork(true);
			}
			Running_Process->setState("RUN");
			processor_state = "BUSY";
			Running_Process->setCT(Running_Process->getCT() - 1);
			processor_Busytime++;
			setprocessorutlization();
			if (Running_Process->getCT() == 0)
			{
				//addtotermnate
				Running_Process->setState("TRM");
				Running_Process->setTerminationt_time(gettimestep());
				Running_Process->setFinalWT(current_Timestep - Running_Process->getAT());
				Running_Process->setTRT((Running_Process->getTT()) - (Running_Process->getAT()));
			
				TRMlist.add(Running_Process);
				Running.deletefirst(Running_Process);
				return;
			}
		}
		else
		{
			TRMlist.add(Running_Process);
			Running.deletefirst(Running_Process);
			return;
		}
		

	}
	void incrementWT()
	{
		Node<Process*>* p = l1.Returnhead();
		while (p)
		{
			p->getitem()->setwtt(p->getitem()->getwtt() + 1);
			p = p->getnext();
		}
	}

	void setntimesteps(int n) // set overheat timesteps
	{
		ntimesteps = n;
	}
	void processoroverheat()
	{
		int r = generateRN();
		if (r<=2)
		{
			if (ntimesteps!=0)
			{
				processor_state = "STOP";
				ntimesteps--;
				if (ntimesteps==0)
				{
					if (l1.isempty())
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
	string getprocessorstate()
	{
		return processor_state;
	}
	void move_frm_ready_torun()
	{
		if (Running.isempty())
		{
			if (l1.isempty())
			{
				processor_state = "IDEAL";
				processor_Idealtime++;
				return;
			}
			l1.deletefirst(Running_Process);
			Running.add(Running_Process);
			Running_Process->setState("RUN");
		}
	}
	//**************************************************************************************************************************
	void deleterunning()
	{
		if (Running.Returnhead()->getitem()->isparent())
		{
			Running.Returnhead()->getitem()->setState("ORPH");
			Running.deletehead();
		}
	}
	void deleteprocess(Process* p)
	{
		Node<Process*>* temp = l1.Returnhead();
		for (int i = 0; i < l1.getcount(); i++)
		{
			if (temp->getitem()->getID() == p->getID())
			{
				l1.deletenode(temp->getitem());
				return;
			}
			else
			{
				temp = temp->getnext();
			}
		}
	}
	//**************************************************************************************************************************
	bool whereprocess(Process* p)
	{
		Node<Process*>* temp = l1.Returnhead();
		for (int i = 0; i < l1.getcount(); i++)
		{
			if (temp->getitem()->getID() == p->getID())
			{
				temp->getitem()->setState("ORPH");
				l1.deletenode(temp->getitem());
				return true;
			}
			else
			{
				temp = temp->getnext();
			}
		}
		return false;
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
	//**************************************************************************************************************************
	void settimestep(int timestep)
	{
		 current_Timestep = timestep;
	}
	//**************************************************************************************************************************
	int gettimestep()
	{
		return current_Timestep;
	}
	//**************************************************************************************************************************
	void add_process(Process* p)
	{
		l1.add(p);
	}
	//**************************************************************************************************************************
	void process_migration_condition()
	{
		if (Running_Process!=nullptr&&Running_Process!=NULL)
		{
			if ((Running_Process->getWT()) <= getMaxw())
			{
				return;
			}
			while (Running.Returnhead()!=nullptr)
			{
		
					if ((Running_Process->getWT()) > getMaxw())
					{
						Migarate_Process.add(Running_Process);
						Running_Process->setWT(gettimestep() - (Running_Process->getAT()));
						Running.deletefirst(Running_Process);
						move_frm_ready_torun();

					}
					else
					{
						return;
					}
			}
		}
	}
	void dequeue_from_migratedlist()
	{
		if (Migarate_Process.Returnhead())
		{
			Migarate_Process.deletehead();
		}
	}
	int get_migrated_count()
	{
		return Migarate_Process.getcount();
	}
	//**************************************************************************************************************************
	int generateRN()
	{
		return rand() % 100 + 1;
	}
	//**************************************************************************************************************************
	void printready()
	{
		l1.PrintList();
	}
	//**************************************************************************************************************************
	void printrunning()
	{
		cout << "aalooo  ";
		cout << Running_Process->getID() << "\n";
	}
	//**************************************************************************************************************************
	Process* get_pointer_running()
	{
		if (Running_Process != nullptr)
		{
			return Running_Process;
		}
		return nullptr;
	}
	//**************************************************************************************************************************
	Node<Process*>* get_pointer_readylist()
	{
		return l1.Returnhead();
	}
	//**************************************************************************************************************************
	Node<Process*>* gettrmlisthead()
	{
		return TRMlist.Returnhead();
	}
	//**************************************************************************************************************************
	Node<Process*>* getblklisthead()
	{
		return blklist.Returnhead();
	}
	//**************************************************************************************************************************
	bool readyisempty()
	{
		return l1.isempty();
	}
	//**************************************************************************************************************************
	int getcount()
	{
		return l1.getcount();
	}
	//**************************************************************************************************************************
	int expected_finishing_time()
	{
		int expected_time = 0;
		Node<Process*>* temp = l1.Returnhead();
		while (temp)
		{
			expected_time = expected_time + temp->getitem()->getCT();
			temp = temp->getnext();
		}
	/*	if (Running_Process)
		{
			expected_time = expected_time + Running_Process->getCT();
		}*/
		return expected_time;
	}
	//**************************************************************************************************************************
	void setprocessorLoad(int d)
	{
		
		processor_load = processor_Busytime / d;
		
	}
	int getblkcount()
	{
		return blkcount;
	}
	//**************************************************************************************************************************
	float getprocessorload()
	{
		return processor_load;
	}
	//**************************************************************************************************************************
	void setprocessorutlization()
	{
		float plus = processor_Busytime + processor_Idealtime;
		processor_utilization = 100 * (processor_Busytime / plus);

	}
	//**************************************************************************************************************************
	float getprocessorutlization()
	{
		return processor_utilization;
	}
	//**************************************************************************************************************************
	void setMAxW(int n)
	{
		maxw = n;
	}
	//**************************************************************************************************************************
	int getMaxw()
	{
		return maxw;
	}
	//**************************************************************************************************************************
	 Node<Process*>* getMigrateprocess()
	{
		return Migarate_Process.Returnhead();
	}
	 //**************************************************************************************************************************
	void dequeue()
	{
		l1.deletehead();
	}
	//**************************************************************************************************************************
	void deleteall()
	{
		l1.DeleteAll();
		cout << "deleting";
	}
	//**************************************************************************************************************************
	void delete_termenation()
	{
		TRMlist.DeleteAll();
	}
	//**************************************************************************************************************************
	void delete_blocked()
	{
		blklist.DeleteAll();
	}
	//**************************************************************************************************************************
	void setRTF(int t)
	{
		RTF = t;
	}
	//**************************************************************************************************************************
	Process* killsignal(double& killcnt, bool& next)
	{
		next = 0; // is true when next killsignal arrives at the same timestep

		if (!Running.isempty() && !killtime.isempty())
		{
			if (current_Timestep == killtime.peek())
			{
				int tempkilltime;
				int tempkillid;

				killtime.dequeue(tempkilltime);
				killpid.dequeue(tempkillid);
				if (Running_Process->getID() == tempkillid)//checks if running process recieved a killsignal
				{
					killcount++;
					killcnt = killcount;
					return Running_Process;

				}
				Node<Process*>* temppp = l1.Returnhead();
				while (temppp) //loops on ready list

				{
					if (temppp->getitem()->getID() == tempkillid)//checks if running process recieved a killsignal
					{
						killcount++;
						killcnt = killcount;
						return temppp->getitem();
					}
					temppp = temppp->getnext();
				}
				if (!killtime.isempty() && temppp)
				{
					if (current_Timestep == killtime.peek())//checks if next killtime equals current timestep
					{
						next = 1;
					}
				}
			}
		}
		return nullptr;
	}
		//**************************************************************************************************************************
	void SetTime_Slice(int T)
	{
		int i = T;
	}

};



