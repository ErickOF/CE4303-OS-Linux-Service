//
// Created by erick on 5/5/20.
//

#ifndef T1_RAW_FILTERS_H
#define T1_RAW_FILTERS_H

#include "Tools.h"



/*  Takes a sample of an image of size kerner*kernel centered at center and
    calculates the avg of the values. Then returns the result

    center:         Pointer to the center of the convolution window on the image
    pos:            Coordinates to the center
    kernel_size:    Side length of the kernel

*/
uint8_t avg_filter(uint8_t* image, size_t* pos, uint8_t kernel_size){

    // Array containing all the usefull positions in the range
    // size kernel_size*kernel_size
    uint8_t* values = get_values(image, pos, kernel_size);
    // Accumulator and total elements
    uint8_t avg = 0;
    uint8_t total_elements = kernel_size*kernel_size;


    //printf("\t\t\tGot values\n");
    // Iterate over all the numbers and take the image value in each position
    for (size_t i = 0; i < total_elements; ++i)
    {
        // Get the value of the image using the positions we
        // calculated before and divide in each step to prevent
        // overflow remember acc is uint8
        avg += *(values + i) / total_elements;
    }

    // Return the result
    return avg;

}



/*  Takes a sample of an image of size kerner*kernel centered at center and
    calculates the median of the values. Then returns the result

    center:         Pointer to the center of the convolution window on the image
    pos:            Coordinates to the center
    kernel_size:    Side length of the kernel

*/
uint8_t median_filter(uint8_t* image, size_t* pos, uint8_t kernel_size){

    // Array containing all the usefull positions in the range
    // size kernel_size*kernel_size
    uint8_t* values = get_values(image, pos, kernel_size);
    // Total elements
    uint8_t total_elements = kernel_size*kernel_size;

    // Sort the elements and select the middle one
    merge_sort(values, 0, total_elements, total_elements);

    // Check if we have and odd or even amount of numbers
    if(total_elements % 2 == 2){
        total_elements /= 2;
    }
    else{
        total_elements = (total_elements / 2) + 1;
    }

    // Return the result
    return values[total_elements];

}
















#endif //T1_RAW_FILTERS_H
