/*
Zoe Schmertzler
ilanaz@pdx.edu
ECE362 HW1
1/10/20

This is the main program for the MultiQueue Generator assignment.
It takes input arguments from the command line which are used to 
determine how it will execute the rest of the program.
The final result is to generate an array of integer arrays.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//////////Typedef//////////
typedef struct {
    int numElements;
    int queueSum;
    int *intQueue;
}queueStruct_t, *queueStructPtr_t;


//////////Prototypes//////////
int getRand(int min, int max);
int compFunc(const void *a, const void *b);

/////////////////////////////////MAIN//////////////////////////////////////////
int main(int argc, char *argv[]) {

    //declare all the variables for holding command line arguments
    int numQueues = 1; //this variable will hold the argument for the flag -q
    int numReqs = 5; //this one is for -r
    int minInt = 0; //-min
    int maxInt = 20; //-max
    int currentArg = 1; //for tracking which argv we are checking

    //check and parse the command line arguments
    printf("argc is %d\n", argc);
    //better to use a while because we aren't exactly iterating through each argv
    while (currentArg < argc) { //don't include argc because array starts at 0
        printf("argv[%d] is %s\n", currentArg, argv[currentArg]);
	if (strcmp(argv[currentArg], "-q") == 0) {
	    //check that next argv is a number and print as string
	    printf("The number of queues is %s as a string.\n", argv[currentArg+1]);
	    //ok to assume that the next argv is an ascii number
	    numQueues = atoi(argv[++currentArg]);
	    printf("The number of queues is %d as an integer.\n", numQueues);
	    //increment currentArg again to check next flag
	    ++currentArg;
	}
	else if (strcmp(argv[currentArg], "-r") == 0) {
            //same deal for the number of requests
            printf("The number of requests is %s as a string.\n", argv[currentArg+1]);
            numReqs = atoi(argv[++currentArg]);
            printf("The number of requests is %d as an integer.\n", numReqs);
            ++currentArg;
	}
	else if (strcmp(argv[currentArg], "-min") == 0) {
            printf("The minimum integer is %s as a string.\n", argv[currentArg+1]);
            minInt = atoi(argv[++currentArg]);
            printf("The minimum integer is %d as an integer.\n", minInt);
            ++currentArg;
	}
        else if (strcmp(argv[currentArg], "-max") == 0) {
            printf("The maximum integer is %s as a string.\n", argv[currentArg+1]);
            maxInt = atoi(argv[++currentArg]);
            printf("The maximum integer is %d as an integer.\n", maxInt);
            ++currentArg;

        }
	else {
	    printf("Not a valid command line argument!\n");
	    //skip to checking the next argv
	    ++currentArg;
	}
    } //close while loop

    //check the variable values to be sure they went to default when unspecified
    printf("queues=%d\nreqs=%d\nmin=%d\nmax=%d\n", numQueues, numReqs, minInt, maxInt);


    //allocate array of pointers to structs (so queueArray is a ** type)
    queueStructPtr_t *queueArray = (queueStructPtr_t*)malloc(numQueues*sizeof(queueStructPtr_t));
    if (!queueArray) {
        printf("ERROR: Could not allocate memory for array of structs.\n");
        return -1;
    }

    //generate a list of numQueues-1 random numbers between 0 and numReqs
    //these are markers for where the number of requests will get split among the queues
    int numMarkers = numQueues - 1;
    int *distrMarkers = (int*)malloc( numMarkers*sizeof(int) );
    if (!distrMarkers) {
        printf("ERROR: Could not allocate memory for distrMarkers array.\n");
        return -1;
    }
    for (int i=0; i<numMarkers; ++i) {
	distrMarkers[i] = getRand(0, numReqs);
	printf("distMarkers[%d] = %d\n", i, distrMarkers[i]);
    }

    //sort the list
    qsort(distrMarkers, numMarkers, sizeof(int), compFunc);
    for (int i=0; i<numMarkers; ++i) {
	printf("sorted distrMarkers[%d] = %d\n", i, distrMarkers[i]);
    }

    //allocate memory for each queue
    //use the sorted list of markers to calculate the number of integers per queue
    int lastQueueIndex = numQueues-1;
    for (int i=0; i<numQueues; ++i) {
	//allocate memory for the queue struct
	queueArray[i] = (queueStructPtr_t)malloc(sizeof(queueStruct_t));
	if (!queueArray[i]) {
	    printf("ERROR: Could not allocate memory for queue struct %d\n", i);
	    return -1;
	}

	//calculate number of integers for this queue
	//check that this is not first or last queue
	if (i>0 && i<lastQueueIndex) {
	    queueArray[i]->numElements = distrMarkers[i] - distrMarkers[i-1];
	}
	//first queue is special case (equivalent to first marker value)
	else if (i==0) {
	    queueArray[i]->numElements = distrMarkers[0];
	}
	//last queue is also special case (remainder of reqs)
	else if (i == lastQueueIndex) {
	    queueArray[i]->numElements = numReqs - distrMarkers[numMarkers-1];
	}
	else {
	    printf("ERROR: somehow i is outside the proper range!");
	}
	printf("Queue %d has %d integers\n", i, queueArray[i]->numElements);

        //allocate memory for intQueue member
        //queueArray[i]->intQueue = (int*)malloc(sizeof());


    }

/*
    for (int i=0; i<2; ++i) {
	//allocate space for the queue struct and set to pointer in array
	queueArray[i] = (queueStructPtr_t)malloc(sizeof(queueStruct_t));
	printf("queueArray[%d] is the pointer %p.\n", i, queueArray[i]);

	//for this element of the array set the numElements struct member to 3
	queueArray[i]->numElements = 3;
	printf("It points to a queue struct with %d elements.\n", queueArray[i]->numElements);

	//allocate space for the actual queue (in the struct) and initialize first element
	(queueArray[i]->intQueue) = (int*)malloc((queueArray[i]->numElements)*sizeof(int));
	(queueArray[i]->intQueue)[0] = 5;
	printf("The first element in this queue is %d.\n", (queueArray[i]->intQueue)[0] );
    }
*/

/*
    //Initialize the seed for the rand() function
    srand(time(NULL));

    //test out the getRand() function
    int min=0, max=20;
    printf("Some random numbers between %d and %d are:\n", min, max);
    for (int i=0; i<5; ++i) {
	printf("%d\n", getRand(min, max));
    }
*/
/*
    //free all the allocated memory
    for (int i=0; i<2; ++i) {
	//start with the actual integer queue
	free(queueArray[i]->intQueue);
	//then free the struct
	free(queueArray[i]);
    }
    //free the array of struct pointers last
    free(queueArray);
*/
    return 0;
}
///////////////////////////////////END MAIN////////////////////////////////////



//////////Function definitions//////////

int getRand(int min, int max) {
    int modVal = (max-min) + 1;
    int randNum = (rand() % modVal) + min;
    return randNum;
}



int compFunc(const void *a, const void *b) {
	return ( *((int*)a) - *((int*)b) );
}



int sumQueue() {

}



void printQueue() {

}


/*
Assignment checklist
(x) Test out the use of argc and argv
( ) Test out how to generate a random number with certain parameters
*/
