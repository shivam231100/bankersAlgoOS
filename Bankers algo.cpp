#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

int nResrc,nProcs;
int *resrc,**alloc,**maxReq,**need,*safeSeq;
int nProcessRan = 0;

pthread_mutex_t lockResources;
pthread_cond_t condition;

// finding safe sequence if there is one else return false
bool findSafeSeq();
// process function
void* display(void* );
void welcome();
void input();
void calculate();
void threads();
int main(int argc, char** argv) {
	
   welcome();
   input();
   calculate();
   threads();

}

void welcome(){
	
	
	    	sleep(1);
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			sleep(1);
			printf("!         ******    WELCOME TO Bankers Algortihm  *******             !\n");
			sleep(1);
			printf("!                   Developed By SHIVAM PANDEY                        !\n");
			sleep(1);
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
			sleep(1);
}
void input(){
	
        printf("\nNumber of Processes? ");
        scanf("%d", &nProcs);

        printf("\nNumber of Resources? ");
        scanf("%d", &nResrc);

        resrc = (int *)malloc(nResrc * sizeof(*resrc));
        printf("\nCurrently Available Resources (R1 R2 ...)? ");
        for(int i=0; i<nResrc; i++)
                scanf("%d", &resrc[i]);

        alloc = (int **)malloc(nProcs * sizeof(*alloc));
        for(int i=0; i<nProcs; i++)
                alloc[i] = (int *)malloc(nResrc * sizeof(**alloc));

        maxReq = (int **)malloc(nProcs * sizeof(*maxReq));
        for(int i=0; i<nProcs; i++)
                maxReq[i] = (int *)malloc(nResrc * sizeof(**maxReq));

        // alloc
        printf("\n");
        for(int i=0; i<nProcs; i++) {
                printf("\nResource allocated to Process %d (R1 R2 ...)? ", i+1);
                for(int j=0; j<nResrc; j++)
                        scanf("%d", &alloc[i][j]);
        }
        printf("\n");

	// maximum required resrc
        for(int i=0; i<nProcs; i++) {
                printf("\nMaximum Resources required by process %d (R1 R2 ...)? ", i+1);
                for(int j=0; j<nResrc; j++)
                        scanf("%d", &maxReq[i][j]);
        }
        printf("\n");
    }
        void calculate(){
		   
	// calculate need matrix
        need = (int **)malloc(nProcs * sizeof(*need));
        for(int i=0; i<nProcs; i++)
                need[i] = (int *)malloc(nResrc * sizeof(**need));

        for(int i=0; i<nProcs; i++)
                for(int j=0; j<nResrc; j++)
                        need[i][j] = maxReq[i][j] - alloc[i][j];

	// getting safe sequence
	safeSeq = (int *)malloc(nProcs * sizeof(*safeSeq));
        for(int i=0; i<nProcs; i++) safeSeq[i] = -1;

        if(!findSafeSeq()) {
                printf("\n!!WRANING!!Unsafe State! The processes leads the system to a unsafe state.\n\n");
                sleep(1);
			printf("     +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			sleep(1);
			printf("     ++++     !!Thank You!! For using Bankers Algortihm     ++++\n");
			sleep(1);
			printf("     ++++++          Developed By SHIVAM PANDEY           ++++++\n");
			sleep(1);
			printf("     +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			sleep(1);
                exit(-1);
        }

        printf("\n\nSafe Sequence Found : ");
        for(int i=0; i<nProcs; i++) {
                printf("%-3d", safeSeq[i]+1);
        }

        printf("\nExecuting Processes...\n\n");
        sleep(1);
}


bool findSafeSeq() {
	// finding safe sequence if any
        int tempRes[nResrc];
        for(int i=0; i<nResrc; i++) tempRes[i] = resrc[i];

        bool finished[nProcs];
        for(int i=0; i<nProcs; i++) finished[i] = false;
        int nfinished=0;
        while(nfinished < nProcs) {
                bool safe = false;

                for(int i=0; i<nProcs; i++) {
                        if(!finished[i]) {
                                bool possible = true;

                                for(int j=0; j<nResrc; j++)
                                        if(need[i][j] > tempRes[j]) {
                                                possible = false;
                                                break;
                                        }

                                if(possible) {
                                        for(int j=0; j<nResrc; j++)
                                                tempRes[j] += alloc[i][j];
                                        safeSeq[nfinished] = i;
                                        finished[i] = true;
                                        ++nfinished;
                                        safe = true;
                                }
                        }
                }

                if(!safe) {
                        for(int k=0; k<nProcs; k++) safeSeq[k] = -1;
                        return false; // no safe sequence found
                }
        }
        return true; // safe sequence found
}
  void threads(){
  
  	// run threads
	pthread_t processes[nProcs];
        pthread_attr_t attr;
        pthread_attr_init(&attr);

	int processNumber[nProcs];
	for(int i=0; i<nProcs; i++) processNumber[i] = i;

        for(int i=0; i<nProcs; i++)
                pthread_create(&processes[i], &attr, display, (void *)(&processNumber[i]));

        for(int i=0; i<nProcs; i++)
                pthread_join(processes[i], NULL);

        printf("\nAll Processes Finished with Execution\n");
		sleep(1);
			printf("     +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			sleep(1);
			printf("     ++++     !!Thank You!! For using Bankers Algortihm     ++++\n");
			sleep(1);
			printf("     ++++++          Developed By SHIVAM PANDEY           ++++++\n");
			sleep(1);
			printf("     +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			sleep(1);	
	
	// free resrc
        free(resrc);
        for(int i=0; i<nProcs; i++) {
                free(alloc[i]);
                free(maxReq[i]);
		free(need[i]);
        }
        free(alloc);
        free(maxReq);
	free(need);
        free(safeSeq);}

void* display(void *arg) {
        int p = *((int *) arg);

	// lock resrc
        pthread_mutex_lock(&lockResources);

        // condition check
        while(p != safeSeq[nProcessRan])
                pthread_cond_wait(&condition, &lockResources);

	// process
        printf("\n--> Process %d", p+1);
        printf("\n\tAllocated : ");
        for(int i=0; i<nResrc; i++)
                printf("%3d", alloc[p][i]);

        printf("\n\tNeeded    : ");
        for(int i=0; i<nResrc; i++)
                printf("%3d", need[p][i]);

        printf("\n\tAvailable : ");
        for(int i=0; i<nResrc; i++)
                printf("%3d", resrc[i]);

        printf("\n"); sleep(1);

        printf("\tResource Allocated!");
        printf("\n"); sleep(1);
        printf("\tProcess Code Running...");
        printf("\n"); sleep(rand()%3 + 2); // process code
        printf("\tProcess Code Completed...");
        printf("\n"); sleep(1);
        printf("\tProcess Releasing Resource...");
        printf("\n"); sleep(1);
        printf("\tResource Released!");

	for(int i=0; i<nResrc; i++)
                resrc[i] += alloc[p][i];

        printf("\n\tNow Available : ");
        for(int i=0; i<nResrc; i++)
                printf("%3d", resrc[i]);
        printf("\n\n");

        sleep(1);

	// condition broadcast
        nProcessRan++;
        pthread_cond_broadcast(&condition);
        pthread_mutex_unlock(&lockResources);
	pthread_exit(NULL);
}
