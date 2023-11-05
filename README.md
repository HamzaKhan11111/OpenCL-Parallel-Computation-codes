# OpenCL-Parallel-Computation-codes-in-C-plus-plus
Explore OpenCL parallel computing with two examples: local sums and dot product calculations. Optimize performance using GPU acceleration.

## Code 1 [Scatterv]
There are 10 processes that are participating in the communication equivalent to MPI “Scatterv”. 
Out of these 10 processes one process randomly selected as coordinating process (who’s buffer will be scattered). 
The data-buffer to be scattered dynamically created [1000 min-elements to 10000 max-elements] and initialized by each process. 
The sendcount and displacement datastructure is also randomly generated (for 10 processes) in coordinating process. 
After scattering data, every process calculates the sum of the received data values. 
All the processes communicate their local sums to main program (host program in this case) and the main program then find a global sum and display the result on the screen.

## Code 2 [Gatherv]
There are 10 processes that are participating in the communication equivalent to MPI “Gatherv”. 
Out of these 10 processes one process is randomly selected as coordinating process (who’s buffer will be prepared by gathering data sent by others). 
The data-size to be gathered is dynamically created by each process [20 min-elements to 200 max-elements] and initialized. 
The sendcount and displacement data-structure is also be dynamically/randomly prepared (for 10 processes) and used by the coordinating process. 
After gathering data, the coordinating process calculates a dot product using another randomly created and prepared sparse data-array (sparse because the gathered data have been prepared using displacements thus could result in vacant data values).
The coordinating process conveys the dot product data to main program (i.e., host). 
The host program will sum all values and display the result. 
