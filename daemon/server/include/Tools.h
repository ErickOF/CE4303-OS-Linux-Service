//
// Created by erick on 5/5/20.
//

#ifndef T1_RAW_TOOLS_H
#define T1_RAW_TOOLS_H

#include <string.h>

/*  Takes the current position of the center of the convolution filter and using the
    kernel size calculates the values inside the range of the filter

    center:         ptr to the center of the convolution window on the image
    current_pos:    2D array containing the center position of the convolution filter {h,w}
    kernel_size:    Size of the kernel used

*/
uint8_t* get_values(uint8_t* values, uint8_t* image, size_t* current_pos, uint8_t kernel_size){

    // Calc the amount of steps to move in each direction
    // Multiply by the CHANNELS to take them into account when
    // moving through the image
    uint8_t half_kernel = (kernel_size / 2);
    //uint8_t* values = calloc(kernel_size*kernel_size, sizeof(uint8_t));
    
    memset(values, 255, kernel_size * kernel_size);
    size_t v = 0;

    // Define the limits of the kernel
    uint8_t steps_back = half_kernel;
    uint8_t steps_fwd = half_kernel;
    // If the kernel size if even
    if(kernel_size % 2 == 0){
        steps_back = half_kernel - 1;
    }

    // Get the values
    for (size_t h = current_pos[1] - steps_back*WIDTH*CHANNELS; h <= current_pos[1] + steps_fwd*WIDTH*CHANNELS; h += WIDTH*CHANNELS)
    {
        // If we are in a valid position search for the value
        if((h >= 0) && (h < HEIGHT*WIDTH*CHANNELS)){
            for (size_t w = current_pos[0] - steps_back*CHANNELS; w <= current_pos[0] + steps_fwd*CHANNELS; w += CHANNELS)
            {
                // If we are in a valid position search for the value
                if((w >= 0) && (w < WIDTH*CHANNELS)){
                    *(values + v++) =  *(image + h + w);
                }
            }

        }
    }

    return values;

}


void print_array(uint8_t* array , int len){

    printf("[ " );

    for (int i = 0; i < len; ++i) {
        printf("%i, ", array[i]);
    }

    printf("]\n" );

}

void merge_sort(uint8_t* array,int lower_limit, int upper_limit, size_t arr_len){

    // Calc the lenght of the array
    int len = upper_limit - lower_limit;

    // If we have just two elements left then sort them and return
    if(len <= 2){
        if(array[lower_limit] > array[upper_limit - 1]){
            uint8_t temp = array[upper_limit - 1];
            array[upper_limit - 1] = array[lower_limit];
            array[lower_limit] = temp;
        }
    }
    // If we have a bigger array then cut it and sort
    else{
        // Calc the middle point in the array using absolute indexes
        // Absolute against array
        int mid_limit = lower_limit + (len / 2);
        uint8_t copy[arr_len];

        // Call merge sort again for each part
        merge_sort(array, lower_limit, mid_limit, arr_len);
        merge_sort(array, mid_limit, upper_limit, arr_len);

        // Copy the almost sorted array
        memcpy(copy, array, arr_len*sizeof(uint8_t));

        // Create indexes for resorting
        int lower_half_index = lower_limit;
        int upper_half_index = mid_limit;
        int array_index = lower_limit;

        // Make sure each half is sorted correctly
        while((lower_half_index < mid_limit) && (upper_half_index < upper_limit)){

            // If the current lower half value is smaller then the current upper half
            if(copy[lower_half_index] < copy[upper_half_index]){
                int a = copy[lower_half_index];
                // Then set the new value to the array and increment the lower half index
                array[array_index] = copy[lower_half_index++];
            }
            // Else add the upper one
            else{
                int a = copy[upper_half_index];
                array[array_index] = copy[upper_half_index++];
            }

            // Increment the array current index
            ++array_index;
        }


        // Check if we have any half unfinished
        while(lower_half_index < mid_limit){
            array[array_index++] = copy[lower_half_index++];
        }
        while(upper_half_index < upper_limit){
            array[array_index++] = copy[upper_half_index++];
        }
    }

}









#endif //T1_RAW_TOOLS_H
