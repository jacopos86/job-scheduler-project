#ifndef PROCESS_H
#define PROCESS_H

#define MAX_SIZE 100000

class Process {
	public:
	int id;
	int bt;    // burst time
	int at;    // arrival time
	int wt;    // waiting time
	int tat;   // turnaround time
	int prt;   // priority
	int rt;    // running time
	int rmt;   // remaining time
	Process() = default;
	Process(int a, int b, int c, int d) : id(a), at(b), bt(c), rt(0), rmt(c), wt(0), tat(0), prt(d) {}
};

class PQueue {
	private:
	typedef struct node {
		Process pr;
		struct node* link;
	}Node, *Nodeptr;
	
	node *front, *rear;
	public:
	// methods
	PQueue(void);                 // initialization
	bool is_empty(void);          // check if empty
	int size(void);               // size
	void enqueue_bt(Process *P);  // enqueue process -> front with lowest bt
	void enqueue_pr(Process *P);  // enqueue process -> front highest priority
	void enqueue_rmt(Process *P); // enqueue process -> front lowest rmt
	void dequeue(void);           // dequeue process
	Process peek(void);           // peek process
	void display(void);           // display queue
};

PQueue::PQueue () {
	front = NULL;
	rear = NULL;
}

bool PQueue::is_empty () {
	return front == NULL;
}

int PQueue::size () {
	// traverse linked list
	Node* n;
	if (front == NULL) {
		return 0;
	}
	else {
		int length = 0;
		n = front;
		while (n != NULL) {
			n = n->link;
			length++;
		}
		return length;
	}
}

void PQueue::enqueue_bt(Process *P) {
	Nodeptr np = (Nodeptr) malloc(sizeof(Node));
	np->pr = *P;
	np->link = NULL;
	if (is_empty()) {
		front = np;
		rear = np;
	}
	else {
		Node* temp = front;
		if (np->pr.bt < temp->pr.bt) {
			front = np;
			front->link = temp;
		}
		else {
			// search where we place the node
			while(temp->pr.bt <= np->pr.bt) {
				if (temp->link == NULL) break;
				temp = temp->link;
			}
			if (temp->link == NULL && np->pr.bt >= temp->pr.bt) {
				temp->link = np;
				rear = np;
			}
			else {
				np->link = temp;
				Node* temp2 = front;
				while(temp2->link != NULL) {
					if (temp2->link == temp) {
						temp2->link = np;
						break;
					}
					temp2 = temp2->link;
				}
			}
		}
	}
}

void PQueue::enqueue_rmt(Process *P) {
	Nodeptr np = (Nodeptr) malloc(sizeof(Node));
	np->pr = *P;
	np->link = NULL;
	if (is_empty()) {
		front = np;
		rear = np;
	}
	else {
		Node* temp = front;
		if (np->pr.rmt < temp->pr.rmt) {
			front = np;
			front->link = temp;
		}
		else {
			// search where we place the node
			while(temp->pr.rmt <= np->pr.rmt) {
				if (temp->link == NULL) break;
				temp = temp->link;
			}
			if (temp->link == NULL && np->pr.rmt >= temp->pr.rmt) {
				temp->link = np;
				rear = np;
			}
			else {
				np->link = temp;
				Node* temp2 = front;
				while(temp2->link != NULL) {
					if (temp2->link == temp) {
						temp2->link = np;
						break;
					}
					temp2 = temp2->link;
				}
			}
		}
	}
}

void PQueue::enqueue_pr (Process *P) {
	Nodeptr np = (Nodeptr) malloc (sizeof(Node));
	np->pr = *P;
	np->link = NULL;
	if (is_empty()) {
		front = np;
		rear = np;
	}
	else {
		Node* temp = front;
		if (np->pr.prt > temp->pr.prt) {
			front = np;
			front->link = temp;
		}
		else {
			// search where we place the node
			while (temp->pr.prt >= np->pr.prt) {
				if (temp->link == NULL) break;
				temp = temp->link;
			}
			if (temp->link==NULL && np->pr.prt <= temp->pr.prt) {
				temp->link = np;
				rear = np;
			}
			else {
				np->link = temp;
				Node* temp2 = front;
				while(temp2->link != NULL) {
					if (temp2->link == temp) {
						temp2->link = np;
						break;
					}
					temp2 = temp2->link;
				}
			}
		}
	}
}

void PQueue::display () {
	Node* temp;
	if (front == NULL) {
		std::cout << "queue is empty" << std::endl;
	}
	else {
		temp = front;
		while (temp != NULL) {
			if (temp != front) std::cout << "->";
			std::cout << temp->pr.id;
			temp = temp->link;
		}
		std::cout << std::endl;
	}
}

void PQueue::dequeue () {
	Node* temp;
	if (is_empty()) {
		std::cout << "Error: attempt to pop data from empty queue..." << std::endl;
		exit(1);
	}
	temp = front;
	front = front->link;
	if (front == NULL) rear = NULL;
	free(temp);
}

Process PQueue::peek () {
	return front->pr;
}

#endif
