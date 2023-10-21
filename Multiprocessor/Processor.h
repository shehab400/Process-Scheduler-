#pragma once
#include"../Data Structures/linkedlist.h"
#include"../Data Structures/Node.h"
class Processor
{
	bool isempty;
public:
	Processor()
	{
		isempty = 1;
	}
	virtual void SchedulingAlgorthim() = 0;
	virtual void add_process(Process* p) = 0;
	virtual Node<Process*>* get_pointer_readylist() = 0;
	virtual Process* get_pointer_running() = 0;
	virtual Node<Process*>* gettrmlisthead() = 0;
	virtual Node<Process*>* getblklisthead() = 0;
	virtual bool readyisempty() = 0;
	virtual int getcount() = 0;
	virtual void dequeue() = 0;
	virtual void delete_termenation() = 0;
	virtual void delete_blocked() = 0;
	virtual Node<Process*>* getMigrateprocess() = 0;
	virtual int expected_finishing_time()=0;
	virtual void settimestep(int timestep)  = 0;
	virtual void setMAxW(int n) = 0;
	virtual void setRTF(int t) = 0;
	virtual int getblkcount() = 0;
	virtual void deleterunning() = 0;
	virtual bool whereprocess(Process* p) = 0;
	virtual void deleteprocess(Process* p) = 0;
	virtual float getprocessorload() = 0;
	virtual float getprocessorutlization() = 0;
	virtual bool isrunningnull() = 0;
	virtual string getprocessorstate() = 0;
	virtual void SetTime_Slice(int t) = 0;
	virtual void setprocessorLoad(int d) = 0;
	void setwtt(int t);
	int getwtt();
};

