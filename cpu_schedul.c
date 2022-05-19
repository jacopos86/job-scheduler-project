#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

typedef struct process {
	char id[10];
	int bt;    //  burst time
	int at;    //  arrival time
	int wt;    //  waiting time
	int tat;   //  turnaround time
	int prt;   //  priority
}processes;

void b_sort(processes temp[],int n)
{
	processes t;
	int i,j;
	/* order according to arrival time */
	for(i=1;i<n;i++)
		for(j=0;j<n-i;j++){
			if(temp[j].at > temp[j+1].at){
				t = temp[j];
				temp[j] = temp[j+1];
				temp[j+1] = t;
			}
		}
}

/* First Come First Serve algorithm (FCFS)   */

void fcfs(processes P[], int n) {
	
	processes temp[n];
	char line[256];
	int i;
	int tot_tat, tot_wt;
	int fd;
	float wt_aver, tat_aver;
	FILE *fp;
	
	// store processes in a temp array
	for (i=0; i<n; i++) {
		temp[i] = P[i];
	}
	
	// sort array according to arrival time
	b_sort(temp, n);
	
	/* compute waiting time and turn around time */
	/* for each process */
	tot_wt = 0;
	tot_tat = 0;
	
	temp[0].wt = 0;
	temp[0].tat = temp[0].bt - temp[0].at;
	tot_wt = temp[0].wt;
	tot_tat= temp[0].tat;
	
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
	tat_aver = (float)tot_tat / n;
	
	/* write / append data on file  */
	do {
		fd = open("FCFS-DATA", O_APPEND);
		/* retry if open interrupted */
	} while (fd < 0 && errno == EINTR);
	if (fd >= 0) {
		/* file successfully opened */
		fp = fdopen(fd, "a");
	}
	else {
		fp = fdopen(fd, "w");
	}
	/* writing GANTT chart */
	for (i=0; i<n; i++) {
		fprintf(fp, " %s\t%d\t%d\t%d\t%d\n", temp[i].id, temp[i].bt, temp[i].at, temp[i].wt, temp[i].tat);
	}
	fprintf(fp, "%s\t%.7f\t%.7f\n", temp[i].id, wt_aver, tat_aver);
	fprintf(fp, "%s\n", "#");
	fclose(fp);
}

/* shortest job first algorithm -- non preemptive */

void sjf_np (processes P[], int n) {
	int i;
	processes temp[n];
	
	
	
	/* store processes into temp. array */
	for (i=0; i<n; i++) {
		temp[i] = P[i];
	}
	
	/* sort array according to the arrival time */
	b_sort(temp, n);
	
	/* sort array according to burst time -- need to check arrival time */
	/* build queue to add element when these arrive */
	
	
	
	
	
	
	
}

// main program CPU scheduling

int main (int argc, char *argv[]) {
	
	processes p[10];    // list of processes
	char buffer[255];
	FILE *fptr;
	int n, line;
	
	printf("\n\n SIMULATION OF CPU SCHEDULING ...\n");
	printf("\n\n FIRST READ PROCESSES INFO FROM FILE ...\n");
	printf("\n\n FILE STRUCTURE: ID PROC. - BURST TIME - ARRIVAL TIME - PRIORITY\n");
	printf("\n\n ...\n");
	printf("\n\n READING INPUT FILE ...\n");
	
	if (argc < 1) return EXIT_FAILURE;
	/* open file */
	fptr = fopen (argv[1], "r");
	if (fptr == NULL) {
		perror(argv[1]);
		return EXIT_FAILURE;
	}
	/* reading file */
	line = 0;
	while (fgets(buffer, 255, fptr)) {
		printf("%s\n", buffer);
		line++;
	}
	n = line;
	printf("Number of processes: %d\n", n);	
	/* acquire processes data */
	
	
	return 0;
}
