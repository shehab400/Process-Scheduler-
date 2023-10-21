#pragma once
#include <iostream>
#include <fstream>
#include"../Data Structures/linkedlist.h"
#include"../Process/Process.h"
#include"FCFS.h"
#include"Processor.h"
#include"SJF.h"
#include"RR.h"
#include"scheduler.h"
#include<Windows.h>
using namespace std;
class UI
{
	Processor* p;
	Process* q;
	processSch s;
	RR R;
	SJF SJF;
	FCFS FCFS;
public:
	
	void ModeChoice(int h)
	{

		if (h == 1)
		{
			interactive_mode();
		}
		else if (h == 2)
		{
			StepByStep_mode();

		}
		else if (h == 3)
		{
			Silent_mode();

		}
	}

	void Running_simulation()
	{
		int n = 1;
		int h = 0;
		cout << "Welcome to Processor simulation program  :D" << endl;
		while (n == 1)
		{
			cout << "for interactive mode enter 1" << endl << "for step by step mode enter 2" << endl << "for silent mode enter 3" << endl;
			cin >> h;
			while (h != 1 && h != 2 && h != 3)
			{
				cout << "please enter a valid choice" << endl;
				cin >> h;
			}
			ModeChoice(h);
			cout << "do you want to run again? " << endl << "1 for YES" << endl << "0 for NO" << endl;
			cin >> n;
 			while (n != 1 && n != 0)
			{
				cout << "please enter a valid choice" << endl;
				cin >> n;
			}
			if (n == 0)
			{
				cout << "Thank you so much for Chosing our program. See You Soon ";
				break;
			}
			s.reset();
		}
	}
	
	void print_BLK()
	{
		cout << s.getblockcount() << "BLK:";
		Node<Process*>* temp = s.ReturnBLKhead();
		while (temp)
		{
			cout << temp->getitem()->getID();
			cout << ", ";
			temp = temp->getnext();
		}
		cout << endl;

	}
	void printRun()
	{
		
		int countofrun = 0;
		for (int i = 0; i < s.getNum_processors(); i++)
		{
			Process*run =s.Processorarr[i]->get_pointer_running();
			if (run)
			{
				countofrun++;
			}
		}
		cout << countofrun << "  RUN: ";
		for (int i = 0; i < s.getNum_processors(); i++)
		{
			Process* run = s.Processorarr[i]->get_pointer_running();
			if (run)
			{
				cout << run->getID();
				cout << "P(" << i+1 << "), ";
				
			}
		}
		cout << endl;
	}

	void print_TRM()
	{
		cout << s.getterm_count() << "TRM:";
		Node<Process*>* temp = s.ReturnTRMhead();
		while (temp)
		{
			cout << temp->getitem()->getID();
			cout << ", ";
			temp = temp->getnext();
		}
		cout << endl;
	}
	void print_ready()
	{

		for (int i = 0; i < s.GetNSJF(); i++)
		{
			Node<Process*>* pointerreadys = s.S11[i]->get_pointer_readylist();
			if (pointerreadys)
			{

				cout << "Processor " << i + 1 << "[SJF]" << s.S11[i]->getcount() << " RDY:";
				while (pointerreadys)
				{
					
					cout << pointerreadys->getitem()->getID();
					cout << ", ";
					pointerreadys = pointerreadys->getnext();
				}
				cout << endl;
			}
			else
			{
				cout << "Processor " << i + 1 << "[SJF] " << "0" << " RDY:" << endl;
			}
		}
		for (int i = 0; i < s.GetNFCFS(); i++)
		{

			Node<Process*>* pointerreadyf = s.F11[i]->get_pointer_readylist();
			if (pointerreadyf)
			{

				cout << "Processor " << i + 1 + s.GetNSJF() << "[FCFS]" << s.F11[i]->getcount() << " RDY:";
				while (pointerreadyf)
				{
					
					cout << pointerreadyf->getitem()->getID();
					cout << ", ";
					pointerreadyf = pointerreadyf->getnext();
				}
				cout << endl;
			}
			else
			{
				cout << "Processor " << i + 1 + s.GetNSJF() << "[FCFS] " << "0" << " RDY:" << endl;
			}

		}
		for (int i = 0; i < s.GetNRR(); i++)
		{

			Node<Process*>* pointerreadyR = s.R11[i]->get_pointer_readylist();
			if (pointerreadyR)
			{
				cout << "Processor " << i + 1 + s.GetNSJF() + s.GetNFCFS() << "[RR]" << s.R11[i]->getcount() << " RDY:";
				while (pointerreadyR)
				{
					
					cout << pointerreadyR->getitem()->getID();
					cout << ", ";
					pointerreadyR = pointerreadyR->getnext();
				}
				cout << endl;
			}
			else
			{
				cout << "Processor " << i + 1 + s.GetNSJF() + s.GetNFCFS() << "[RR] " << "0" << " RDY:" << endl;
			}

		}
	}
	//**************************************************************************************************************************\\
	//**************************************************MODES********************************************************************\\

	void interactive_mode()
	{
		s.SetData();
		while (s.getnumprocess() != s.getterm_count())
		{
			s.simulate();
			cout << "Current Timestep:" << s.gettimestep() << endl;
			cout << "**********RDY*************" << endl;
			print_ready();
			cout << "**********Run*************" << endl;
			printRun();
			cout << "**********BLK*************" << endl;
			print_BLK();
			cout << "**********TRM*************" << endl;
			print_TRM();
			cout << "PRESS ANY KEY TO MOVE TO NEXT STEP!!" << endl;
			cout << "press any key to increment time step" << endl;
       			system("pause");
				s.IncrementTimeStep();
		}
		s.opfile();
		cout << "simulation ends, Outputfile created  ";
	}
	void StepByStep_mode()
	{
		s.SetData();
		while (s.getnumprocess() != s.getterm_count())
		{
			s.simulate();
			cout << "Current Timestep:" << s.gettimestep() << endl;
			cout << "**********RDY*************" << endl;
			print_ready();
			cout << "**********Run*************" << endl;
			printRun();
			cout << "**********BLK*************" << endl;
			print_BLK();
			cout << "**********TRM*************" << endl;
			print_TRM();
			Sleep(1000);
			s.IncrementTimeStep();
		}
		s.opfile();
		cout << "simulation ends, Outputfile created  ";
	}
	void Silent_mode()
	{
		s.SetData();
		cout << "Silent Mode...............Simulation starts..." << endl;
		while (s.getnumprocess() != s.getterm_count())
		{
			s.simulate();
			s.IncrementTimeStep();
		}
		s.opfile();
		cout << "simulation ends, Outputfile created  ";
	}
	//**************************************************************************************************************************\\
	//**************************************************************************************************************************\\

};
