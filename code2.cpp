#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <time.h>
#include<iostream>
#include <cstdlib> 
using namespace std;

int main()
{
    srand(time(0)); 
    int N=1000;
    // initializing  OpenCL environment
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_int err;

    
    //creating sendout and displacement data structures
    int sendcout[10];
    int displacements[10];
    for(int i=0;i<10;i++)
    {
        sendcout[i]=rand()%10+1;
        displacements[i]=rand()%980+1;
    }

    //write +read buffer
    int *b=new int[10*N];
    for (int i = 0; i < 10; i++)
    {
        for(int j=0;j<1000;j++)
        {
            if(i==0)
                b[i*1000+j]=1;
            else
            b[i*1000+j]=rand()%1000+1;
        }
    }

    //buffer for storing local dotProducts
    int dotProduct[10]; 
    for (int i = 0; i < 10; i++)
    {
        dotProduct[i]=0;
    }

    int *randomArray=new int[1000];
    for(int i=0;i<1000;i++)
        randomArray[i]=rand()%1000+1;

    clGetPlatformIDs(1, &platform_id, NULL); //get first platform
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL); //get first device
    context=clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    queue=clCreateCommandQueue(context, device_id, 0, &err);

    //DEFINING KERNEL FUNCTION
    const char *source = "__kernel void sum( __global int* b, __global int* c, __global int* d, __global int* e, __global int* f) {"
                         "    int k = get_global_id(0);"
                        "        for(int init=0;init<c[k];init++)"
                        "        {"
                        "           b[0*1000+init+d[k]]=b[k*1000+init];"
                        "        }"
                        "        for(int i=0;i<1000;i++)"
                        "        {"
                        "           e[k]+=b[0*1000+i]*f[i];"
                        "        }"
                        " }";
    program = clCreateProgramWithSource(context,1,&source,NULL,&err); //create program using source
    err=clBuildProgram(program,1,&device_id,NULL,NULL,NULL); //build the program

    kernel = clCreateKernel(program, "sum", &err); //create the kernel

    // CREATE BUFFERS
    cl_mem c_mem = clCreateBuffer(context, CL_MEM_READ_ONLY, 10 * sizeof(int),NULL, &err);
    cl_mem d_mem = clCreateBuffer(context, CL_MEM_READ_ONLY, 10 * sizeof(int),NULL, &err);
    cl_mem b_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, 10 * N * sizeof(int),NULL, &err);
    cl_mem e_mem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 10 * sizeof(int),NULL, &err);
    cl_mem f_mem = clCreateBuffer(context, CL_MEM_READ_ONLY, N * sizeof(int),NULL, &err);



    // Copy the arrays to their respective memory buffers (enqueue them)
    clEnqueueWriteBuffer(queue, b_mem, CL_TRUE, 0, 10 * N * sizeof(int), b, 0, NULL, NULL); 
    clEnqueueWriteBuffer(queue, c_mem, CL_TRUE, 0, 10 * sizeof(int), sendcout, 0, NULL, NULL); 
    clEnqueueWriteBuffer(queue, d_mem, CL_TRUE, 0, 10 * sizeof(int), displacements, 0, NULL, NULL); 
    clEnqueueWriteBuffer(queue, e_mem, CL_TRUE, 0, 10 * sizeof(int), dotProduct, 0, NULL, NULL); 
    clEnqueueWriteBuffer(queue, f_mem, CL_TRUE, 0, N * sizeof(int), randomArray, 0, NULL, NULL); 

    // // Set the arguments of the kernel
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &b_mem);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &c_mem);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_mem);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &e_mem);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &f_mem);

    size_t global_size = 10;
    size_t local_size= 1;

    clEnqueueNDRangeKernel(queue,kernel,1,NULL,&global_size,&local_size,0,NULL,NULL);

    clEnqueueReadBuffer(queue, b_mem, CL_TRUE, 0, 10 * N * sizeof(int), b, 0, NULL, NULL); // Read the memory buffer b_mem on the device to the local variable b
    clEnqueueReadBuffer(queue, e_mem, CL_TRUE, 0, 10 *  sizeof(int), dotProduct, 0, NULL, NULL); // Read the memory buffer b_mem on the device to the local variable b



    // Display the results to the screen
    int global=0;
    cout<<"Dot Products: "<<endl;
    for(int i=0;i<10;i++)
    {
        global=global+dotProduct[i];
        cout<<dotProduct[i]<<" ";
    }
    cout<<endl<<"Global Sum of Dot Product:"<<endl;
    cout<<global<<endl;

    //CLEANING
    clReleaseMemObject(b_mem);
    clReleaseMemObject(c_mem);
    clReleaseMemObject(d_mem);
    clReleaseMemObject(e_mem);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
