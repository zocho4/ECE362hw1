Zoe Schmertzler
ilanaz@pdx.edu
ECE362 HW1
1/17/20

My program uses a struct for creating the queues. 
The struct contains the number of elements, the 
sum and the actual integer queue.

The code consists of several sections:
- Parsing the inputs argc and argv to determine what 
parameters to use.
- Calculating how many elements to put in each queue. 
The algorithm is to generate a list of numbers (markers)
that split up the number of requests randomly. 
The markers must be ordered smallest to largest with qsort.
- Creating the array of struct pointers to structs that
hold the queues.
- Fill in the queues and calculate the sums.
- Print out the sums and the values of the smallest queue.

The functions used are very self explanatory so I won't 
explain them here.
