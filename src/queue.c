/*
 * queue.c
 * Attribute - https://github.com/shni9045/PES-Assignment-2/blob/main/cbfifo.c
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "queue.h"

#define size  16     //Macro to store size


/*structure to hold Circular Buffer*/

typedef struct cbfifo{
    indication buffer[size];
    uint8_t write;
    uint8_t read;

}cbfifo;


cbfifo cb={.write=0,.read=0};            //Initialise read & write index


size_t cbfifo_enqueue(indication temp, size_t nbyte){


    size_t current_length,current_space;
    current_length=cbfifo_length();
    current_space=size-current_length-1;    //To distinguish between empty & full one byte is kept as it is
    size_t bytes_written=0;

    if(cbfifo_capacity() == current_length)
      return 0;

       for(int i=0;i<nbyte;i++){

           cb.buffer[cb.write].charHandle = temp.charHandle;
           for(int i = 0; i < temp.bufferlength;i++ ){
           cb.buffer[cb.write].buffer[i] = temp.buffer[i];
           }

           bytes_written++;
           cb.write=(cb.write+1) & (size-1);      //Increament write index with wraparound

        }



    return bytes_written;
}

size_t cbfifo_dequeue(indication *temp, size_t nbyte){

    uint8_t bytes_read;

    size_t current_length;
    current_length=cbfifo_length();
    if (current_length==0 ){
        return 0;
    }


    if(nbyte>current_length)
      return 0;


     uint8_t i;

     for ( i=0;i<nbyte;i++){

         temp->bufferlength = cb.buffer[cb.read].bufferlength;

         for(int i = 0; i < temp->bufferlength;i++ ){

             temp->buffer[i] = cb.buffer[cb.read].buffer[i] ;

         }
         temp->charHandle = cb.buffer[cb.read].charHandle;
         cb.read=(cb.read+1)&(size-1);             //Increament read index with wraparound

     }
     bytes_read=i;

     return bytes_read;
}


size_t cbfifo_length(){

    return ((cb.write-cb.read)&(size-1));
}

size_t cbfifo_capacity(){
    return (size);
}
