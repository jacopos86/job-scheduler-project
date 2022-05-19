#include <iostream>
#include <fstream>
#include <vector>
#include "mystruct.h"
#include <string>
#include <sstream>
using namespace std;

void sort_a(Process temp[], int n) {
	int i, j;
	Process t;
	for (i=1; i<n; i++) {
		for (j=0;j<n-i;j++) {
			if (temp[j].at > temp[j+1].at) {
				t = temp[j];
				temp[j] = temp[j+1];
				temp[j+1] = t;
			}
		}
	}
}

/* First Come First Serve algorithm */

void FCFS(Process Pr[], int n) {
	
	int i;
	Process temp[n];
	int tot_tat, tot_wt;
	float wt_aver, tat_aver;
	ofstream out;
	
	// sort processes in a temp array
	for (i=0; i<n; i++) {
		temp[i] = Pr[i];
	}
	
	// sort array according to arrival time
	sort_a(temp, n);
	
	/* compute waiting time and turn around time */
	/* for each process */
	tot_wt = 0;
	tot_tat = 0;
	
	temp[0].wt = 0;
	temp[0].tat = temp[0].bt - temp[0].at;
	tot_wt = temp[0].wt;
	tot_tat = temp[0].tat;
	
	/* iterate over remaining processes */
	for (i=1; i<n; i++) {
		temp[i].wt = temp[i-1].at + temp[i-1].wt + temp[i-1].bt - temp[i].at;
		temp[i].tat= temp[i].wt + temp[i].bt;
		tot_wt = tot_wt + temp[i].wt;
		tot_tat= tot_tat + temp[i].tat;
	}
	wt_aver = 0.;
	tat_aver = 0.;
	wt_aver = (float)tot_wt/n;
	tat_aver= (float)tot_tat/n;
	
	/* append to file */
	out.open("FCFS", ios::app);
	out << wt_aver << "        " << tat_aver << std::endl;
	out.close();
	
}

/* Shortest Job First   --   non preemptive */
void SJF_NP(Process Pr[], int n) {
	
	Process temp[n], p;
	int i, t;
	float tot_wt, tot_tat, wt_aver, tat_aver;
	PQueue *q = new PQueue;
	ofstream out;
	
	/* first sort processes in a temp array */
	for (i=0; i<n; i++) {
		temp[i] = Pr[i];
	}
	
	// sort array
	sort_a (temp, n);
	
	/* compute waiting time and turn around time */
	/* for each process */
	tot_wt = 0;
	tot_tat = 0;
	/* push to the queue all elements 0 arrival time */
	for (i=0; i<n; i++) {
		if (temp[i].at == 0) {
			q->enqueue_bt(&temp[i]);
		}
		else if (temp[i].at > 0) {
			break;
		}
		else {
			continue;
		}
	}
	//q->display();
	
	/* iterate over processes */
	t = 0;
	while (!q->is_empty() || t < temp[n-1].at) {
		// peek front element
		p = q->peek();
		q->dequeue();
		// check running state
		while (p.rt < p.bt) {
			if (p.rt == 0) {
				Pr[p.id].wt = t - Pr[p.id].at;
			}
			p.rt = p.rt + 1;
			t = t + 1;
			for (i=0; i<n; i++) {
				if (temp[i].at == t) {
					q->enqueue_bt(&temp[i]);
				}
				else if (temp[i].at > t) {
					break;
				}
			}
		}
		// compute turn around time
		Pr[p.id].tat = t - Pr[p.id].at;
		//q->display();
		// if q is empty move directly to next arrival time
		if (q->is_empty()) {
			for (i=0;i<n;i++) {
				if (temp[i].at > t) {
					t = temp[i].at;
					break;
				}
			}
		}
		for (i=0;i<n;i++) {
			if (temp[i].at == t) {
				q->enqueue_bt(&temp[i]);
			}
			else if (temp[i].at > t) {
				break;
			}
		}
	}
	// display Pr list
	//for (i=0; i<n; i++) {
	//	std::cout << Pr[i].id << "  " << Pr[i].wt << "  " << Pr[i].tat << std::endl;
	//}
	/* compute waiting time and turn around time */
	/* for each process */
	for (i=0; i<n; i++) {
		tot_wt = tot_wt + Pr[i].wt;
		tot_tat= tot_tat + Pr[i].tat;
	}
	wt_aver = 0.;
	tat_aver = 0.;
	wt_aver = (float)tot_wt/n;
	tat_aver = (float)tot_tat/n;
	/* append to file */
	out.open("SJF_NP", ios::app);
	out << wt_aver << "        " << tat_aver << std::endl;
	out.close();
}

/* round robin scheduling */
void RR(Process Pr[], int n, int Q) {
	
	int i, t, tot_rmt, tot_wt, tot_tat;
	float wt_aver, tat_aver;
	Process temp[n];
	ofstream out;
	
	/* sort processes in a temp array */
	for (i=0; i<n; i++) {
		temp[i] = Pr[i];
	}
	
	// sort array
	sort_a (temp, n);
	
	// iterate over temp list -> use Q as quantum
	// compute total remaining time
	tot_rmt = 0;
	
	for (i=0; i<n; i++) {
		tot_rmt = tot_rmt + temp[i].rmt;
	}
	
	// round robin algorithm
	i = 0;
	t = 0;
	while (tot_rmt > 0) {
		std::cout << t << std::endl;
		if (temp[i].at <= t) {
			// check if process first time run
			if (temp[i].rmt == temp[i].bt) {
				temp[i].wt = t;
			}
			// check rmt
			if (temp[i].rmt > Q) {
				temp[i].rmt = temp[i].rmt - Q;
				tot_rmt = tot_rmt - Q;
				t = t + Q;
			}
			else if (temp[i].rmt > 0 && temp[i].rmt <= Q) {
				tot_rmt = tot_rmt - temp[i].rmt;
				t = t + temp[i].rmt;
				temp[i].rmt = 0;
				temp[i].tat = t - temp[i].at;
			}
			// if next process at is longer
			if (temp[(i+1)%n].at > t) t = temp[(i+1)%n].at;
		}
		i = (i+1)%n;
	}
	//for (i=0; i<n; i++) {
	//	std::cout << temp[i].id << " " << temp[i].wt << " " << temp[i].tat << std::endl;
	//}
	
	// compute average times
	tot_wt = 0;
	tot_tat= 0;
	for (i=0; i<n; i++) {
		tot_wt = tot_wt + temp[i].wt;
		tot_tat= tot_tat + temp[i].tat;
	}
	wt_aver = 0.;
	tat_aver = 0.;
	wt_aver = (float)tot_wt/n;
	tat_aver = (float)tot_tat/n;
	/* append to file */
	out.open("RR_ALG", ios::app);
	out << wt_aver << "        " << tat_aver << std::endl;
	out.close();
	
}

/* Shortest Job First   --   preemptive */
void SJF(Process Pr[], int n) {
	
	Process temp[n], p;
	int i, t;
	float tot_wt, tot_tat, wt_aver, tat_aver;
	PQueue *q = new PQueue;
	ofstream out;
	
	/* first sort processes in a temp array */
	for (i=0; i<n; i++) {
		temp[i] = Pr[i];
	}
	
	// sort array
	sort_a (temp, n);
	
	/* compute waiting time and turn around time */
	/* for each process */
	tot_wt = 0;
	tot_tat = 0;
	/* push to the queue all elements 0 arrival time */
	for (i=0; i<n; i++) {
		if (temp[i].at == 0) {
			q->enqueue_bt(&temp[i]);
		}
		else if (temp[i].at > 0) {
			break;
		}
		else {
			continue;
		}
	}
	
	/* iterate over processes */
	t = 0;
	while (!q->is_empty() || t < temp[n-1].at) {
		std::cout << t << std::endl;
		// peek front element
		p = q->peek();
		q->dequeue();
		// check running state
		if (p.rt < p.bt) {
			if (p.rt == 0) {
				Pr[p.id].wt = t - Pr[p.id].at;
			}
			p.rt = p.rt + 1;
			p.rmt= p.rmt - 1;
			if (p.rt == p.bt) {
				Pr[p.id].tat = t + 1 - Pr[p.id].at;
			}
			else {
				q->enqueue_rmt(&p);
			}
		}
		t = t + 1;
		for (i=0; i<n; i++) {
			if (temp[i].at == t) {
				q->enqueue_rmt(&temp[i]);
			}
			else if (temp[i].at > t) {
				break;
			}
		}
		// if q is empty move directly to next arrival time
		if (q->is_empty()) {
			for (i=0;i<n;i++) {
				if (temp[i].at > t) {
					t = temp[i].at;
					break;
				}
			}
		}
		for (i=0;i<n;i++) {
			if (temp[i].at == t) {
				q->enqueue_rmt(&temp[i]);
			}
			else if (temp[i].at > t) {
				break;
			}
		}
	}
	// display Pr list
	for (i=0; i<n; i++) {
		std::cout << Pr[i].id << "  " << Pr[i].wt << "  " << Pr[i].tat << std::endl;
	}
	/* compute waiting time and turn around time */
	/* for each process */
	for (i=0; i<n; i++) {
		tot_wt = tot_wt + Pr[i].wt;
		tot_tat= tot_tat + Pr[i].tat;
	}
	wt_aver = 0.;
	tat_aver = 0.;
	wt_aver = (float)tot_wt/n;
	tat_aver = (float)tot_tat/n;
	/* append to file */
	out.open("SJF_P", ios::app);
	out << wt_aver << "        " << tat_aver << std::endl;
	out.close();
}

/* Priority algorithm */
void PR(Process Pr[], int n) {
	
	Process temp[n], p;
	int i, t;
	float tot_wt, tot_tat, wt_aver, tat_aver;
	PQueue *q = new PQueue;
	ofstream out;
	
	/* first sort processes in a temp array */
	for (i=0; i<n; i++) {
		temp[i] = Pr[i];
	}
	
	// sort array
	sort_a (temp, n);
	
	/* compute waiting time and turn around time */
	/* for each process */
	tot_wt = 0;
	tot_tat = 0;
	/* push to the queue all elements 0 arrival time */
	for (i=0; i<n; i++) {
		if (temp[i].at == 0) {
			q->enqueue_pr(&temp[i]);
		}
		else if (temp[i].at > 0) {
			break;
		}
		else {
			continue;
		}
	}
	
	/* iterate over processes */
	t = 0;
	while (!q->is_empty() || t < temp[n-1].at) {
		// peek front element
		p = q->peek();
		q->dequeue();
		// check running state
		while (p.rt < p.bt) {
			if (p.rt == 0) {
				Pr[p.id].wt = t - Pr[p.id].at;
			}
			p.rt = p.rt + 1;
			t = t + 1;
			for (i=0; i<n; i++) {
				if (temp[i].at == t) {
					q->enqueue_pr(&temp[i]);
				}
				else if (temp[i].at > t) {
					break;
				}
			}
		}
		// compute turn around time
		Pr[p.id].tat = t - Pr[p.id].at;
		// if q is empty move directly to next arrival time
		if (q->is_empty()) {
			for (i=0;i<n;i++) {
				if (temp[i].at > t) {
					t = temp[i].at;
					break;
				}
			}
		}
		for (i=0;i<n;i++) {
			if (temp[i].at == t) {
				q->enqueue_pr(&temp[i]);
			}
			else if (temp[i].at > t) {
				break;
			}
		}
	}
	// display Pr list
	//for (i=0; i<n; i++) {
	//	std::cout << Pr[i].id << "  " << Pr[i].wt << "  " << Pr[i].tat << std::endl;
	//}
	/* compute waiting time and turn around time */
	/* for each process */
	for (i=0; i<n; i++) {
		tot_wt = tot_wt + Pr[i].wt;
		tot_tat= tot_tat + Pr[i].tat;
	}
	wt_aver = 0.;
	tat_aver = 0.;
	wt_aver = (float)tot_wt/n;
	tat_aver = (float)tot_tat/n;
	/* append to file */
	out.open("PR_ALG", ios::app);
	out << wt_aver << "        " << tat_aver << std::endl;
	out.close();
}

// main program CPU scheduling

int main (int argc, char *argv[]) {
	
	Process *P = new Process[MAX_SIZE];
	ifstream input;
	std::string buffer;
	int i, temp1, temp2, temp3, temp4, n, it;
	
	/* read data from file */
	/*
	std::cout << std::endl << std::endl;
	std::cout << "SIMULATION OF CPU SCHEDULING ..." << std::endl;
	std::cout << std::endl;
	std::cout << "FIRST READ PROCESSES INFO FROM FILE ..." << std::endl;
	std::cout << std::endl;
	std::cout << "FILE STRUCTURE: ID PROC. - BURST TIME - ARRIVAL TIME - PRIORITY" << std::endl;
	std::cout << std::endl;
	std::cout << "READING INPUT FILE ..." << std::endl;
	std::cout << std::endl;
	*/
	if (argc < 1) return EXIT_FAILURE;
	/* open file */
	input.open(argv[1]);
	if (!input) {
		std::cerr << "Error: file cannot be opened" << std::endl;
		exit(1);
	}
	i = 0;
	while ( !input.eof() ) {
		getline(input, buffer);
		std::istringstream iss(buffer);
		if (iss >> temp1 >> temp2 >> temp3 >> temp4) {
			P[i] = Process(temp1, temp2, temp3, temp4);
			//std::cout << "Process Id: " << P[i].id << " arrival time: " << P[i].at << " burst time: " << P[i].bt << "priority: " << P[i].prt << std::endl;
			i++;
		}
	}
	input.close();
	n = i;
	//std::cout << std::endl;
	//std::cout << "TOTAL NUMBER PROCESSES: " << n << std::endl;
	
	/*
	     compute waiting times and turnaround times
	     different algorithms
	*/
	
	//std::cout << "SIMULATION OF CPU SCHEDULING ALGORITHMS" << std::endl;
	//std::cout << "..." << std::endl;
	
	for (it=0; it<5; it++) {
		std::cout << it << std::endl;
		switch (it) {
			case 0:	
				PR(P, n);
				break;
			case 1:
				RR(P, n, 2);
				break;
			case 2:
				SJF(P, n);
				break;
			case 3:
				SJF_NP(P, n);
				break;
			case 4:
				FCFS(P, n);
				break;
		}
	}
	
	return 0;
}
