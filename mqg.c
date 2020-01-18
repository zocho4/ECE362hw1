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
typedef struct { //struct that holds queue and queue info
    int numElements;
    int queueSum;
    int *intQueue; //this is the actual queue
}queueStruct_t, *queueStructPtr_t;


//////////Prototypes//////////
int getRand(int min, int max);
int compFunc(const void *a, const void *b);
int sumQueue(queueStructPtr_t queueToSum);
void printQueue(int minQueueIndex, queueStructPtr_t queueToPrint);

/////////////////////////////////MAIN//////////////////////////////////////////
int main(int argc, char *argv[]) {

    //Initialize the seed for the rand() function
    srand(time(NULL));

    //declare all the variables for holding command line arguments
    int numQueues = 1; //this variable will hold the argument for the flag -q
    int numReqs = 5; //this one is for -r
    int minInt = 0; //-min
    int maxInt = 20; //-max
    int currentArg = 1; //for tracking which argv we are checking
    int minQueueIndex = 0; //for finding the queue with smallest sum

    //check and parse the command line arguments
    printf("argc is %d\n", argc);
    //better to use a while because we aren't exactly iterating through each argv
    while (currentArg < argc) { //don't include argc because array starts at 0
        printf("argv[%d] is %s\n", currentArg, argv[currentArg]);
	//check for each flag case
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



    int numMarkers = numQueues - 1;
    int *distrMarkers = NULL;
    //only need to distribute the reqs among queues if we have 2+ queues
    if (numQueues>1) {
        //generate a list of numQueues-1 random numbers between 0 and numReqs
        //these are markers for where the number of requests will get split among the queues
        distrMarkers = (int*)malloc( numMarkers*sizeof(int) );
        if (!distrMarkers) {
            printf("ERROR: Could not allocate memory for distrMarkers array.\n");
            return -1;
        }
        //generate the list of markers
        for (int i=0; i<numMarkers; ++i) {
	    distrMarkers[i] = getRand(0, numReqs);
	    printf("distMarkers[%d] = %d\n", i, distrMarkers[i]);
        }
        //sort the list
        qsort(distrMarkers, numMarkers, sizeof(int), compFunc);
        for (int i=0; i<numMarkers; ++i) {
    	printf("sorted distrMarkers[%d] = %d\n", i, distrMarkers[i]);
        }
    } //end if (q>1)
    //if q is 1 then we manually set distMarkers[0]=r
    else if (numQueues == 1) {
        distrMarkers = (int*)malloc( sizeof(int) );
        if (!distrMarkers) {
            printf("ERROR: Could not allocate memory for distrMarkers array.\n");
            return -1;
        }
	distrMarkers[0] = numReqs;
    }
    //case where there are no queues (or somehow negative queues)
    else {
	queueArray = NULL;
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

	//calculate number of integers for this queue using distrMarkers[]
	//check that this is not first or last queue
	if (i>0 && i<lastQueueIndex) {
	    queueArray[i]->numElements = distrMarkers[i] - distrMarkers[i-1];
	}
	//first queue is special case (equivalent to first marker value)
	//if there is only one queue then it will follow this case
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


        //only allocate memory for actual integer queue if this queue has elements
	if (queueArray[i]->numElements > 0) {
            queueArray[i]->intQueue = (int*)malloc( (queueArray[i]->numElements)*sizeof(int) );
            //make sure the allocation worked
	    if (!queueArray[i]->intQueue) {
                printf("ERROR: Could not allocate memory for actual queue of queue %d\n", i);
                return -1;
	    }
	    //now fill in the queue
	    printf("Integers are: ");
	    for (int j=0; j<(queueArray[i]->numElements) ; ++j) {
	        (queueArray[i]->intQueue)[j] = getRand(minInt, maxInt);
	        printf("%d ", (queueArray[i]->intQueue)[j]);
	    }
	    printf("\n");
	} //end if for whether or not queue has elements

	//set intQueue to NULL if the queue has no ints or somehow has a negative ints
	else {
	    queueArray[i]->intQueue = NULL;
	    printf("This queue has no integers to print.\n");
	}

	//calculate the sum and print it
	queueArray[i]->queueSum = sumQueue(queueArray[i]);
	printf("Sum for queue %d is %d\n", i, queueArray[i]->queueSum);

	//change the minQueueIndex to this one if this sum is smaller
	if ( queueArray[i]->queueSum < queueArray[minQueueIndex]->queueSum ) {
	    minQueueIndex = i;
	}

    } //end for loop that iterates through each struct

    //if there are queues to print, print the queue with smallest sum
    if (numQueues>0) {
        printQueue(minQueueIndex, queueArray[minQueueIndex]);
    }
    //if there are no queues print a special message
    else {
	printf("There are no queues to print!\n");
    }

    //free all the allocated memory
    free(distrMarkers);
    for (int i=0; i<numQueues; ++i) {
	//start with the actual integer queue
	free(queueArray[i]->intQueue); //this should still work if r=0
	//then free the struct
	free(queueArray[i]);
    }
    //free the array of struct pointers last
    free(queueArray);

    return 0;
}
///////////////////////////////////END MAIN////////////////////////////////////



//////////Function definitions//////////

//returns random number between min and max
int getRand(int min, int max) {
    int modVal = (max-min) + 1;
    int randNum = (rand() % modVal) + min;
    return randNum;
}


//campare function for qsort, will make array smallest to largest
int compFunc(const void *a, const void *b) {
	return ( *((int*)a) - *((int*)b) );
}


//returns sum of a queue when given a pointer to the queue struct
int sumQueue(queueStructPtr_t queueToSum) {
    int queueSum = 0;
    for (int i=0; i< queueToSum->numElements; ++i) {
	queueSum += (queueToSum->intQueue)[i];
    }
    return queueSum;
}



void printQueue(int minQueueIndex, queueStructPtr_t queueToPrint) {
    printf("Queue %d has the lowest sum.\n", minQueueIndex);
    printf("The values in this queue are:\n");
    for (int i=0; i< queueToPrint->numElements; ++i) {
	printf("%d ", queueToPrint->intQueue[i]);
    }
    if (queueToPrint->numElements == 0) {
	printf("No elements to print!\n");
    }
    printf("\n");
    return;
}

