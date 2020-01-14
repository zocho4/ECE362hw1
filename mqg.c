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

//////////Prototypes//////////
int getRand(int min, int max);


/////////////////////////////////MAIN//////////////////////////////////////////
int main(int argc, char *argv[]) {

    //this is just to check the command line arguments
    printf("argc is %d\n", argc);
    //int i;
    for (int i=0; i<argc; ++i) {
        printf("argv[%d] is %s\n", i, argv[i]);
    }

    //change the inputs


    //Initialize the seed for the rand() function
    srand(time(NULL));

    //test out the getRand() function
    int min=0, max=20;
    printf("Some random numbers between %d and %d are:\n", min, max);
    for (int i=0; i<5; ++i) {
	printf("%d\n", getRand(min, max));
    }

    return 0;
}
///////////////////////////////////END MAIN////////////////////////////////////



//////////Function definitions//////////

int getRand(int min, int max) {
    int modVal = (max-min) + 1;
    int randNum = (rand() % modVal) + min;
    return randNum;
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
