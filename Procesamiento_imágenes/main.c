#include <stdio.h>
#include <stdint.h>
// Bibioteca de lectura y escritura de imagenes tomada de
// https://github.com/nothings/stb
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

#include "include/constants.h"
#include "include/Filters.h"



/*  Loads an image from memory
 *
 *  filename:   Path to the image
 *  width:      Width size of the image
 *  height:     Height size of the image
 *  channels:   Number of channels the image has
 *  desired_channels:   Number of channels we want to load from the image
 *
 */
uint8_t* read_image(char* filename, int width, int height, int channels, int desired_channels){


    // Read the image with a given w*h*c, last parameter is to ignore channels
    uint8_t* image = stbi_load(filename, &width, &height, &channels, desired_channels);
    if(image == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }

    // Return the image
    return image;
}

void write_image(char* filename, uint8_t* image){
    // Write the image in png
    stbi_write_png(filename, WIDTH, HEIGHT, CHANNELS, image, WIDTH*CHANNELS);
}

/*  Function that takes an image and applies to it a given filter using the defined
    kernel size. Because of the filter implementation, every image is zero padded.

    target: Image to be filtered
    filter: Filter function to be applied
    kernel_size: Kernel size of the filter
*/
uint8_t* apply_filter(  uint8_t* target,
                        uint8_t (*filter)(uint8_t*, size_t*, uint8_t),
                        uint8_t kernel_size){

    //Prepare variables needed for the convolution process
    int pos[2];
    uint8_t* filtered_image = (uint8_t*) malloc(WIDTH*HEIGHT*CHANNELS * sizeof(uint8_t));


    // Start the convolution
    // HEIGHT and WIDTH limits have to be multiplicated to take into account
    // the channels of the image
    for (size_t h = 0; h < HEIGHT*WIDTH*CHANNELS; h += WIDTH*CHANNELS)
    {
        pos[1] = h;
        for (size_t w = 0; w < WIDTH*CHANNELS; w += CHANNELS)
        {
            // Calc the new kernel center
            pos[0] = w;

            // Apply the filter to each channel
            for (size_t c = 0; c < CHANNELS; ++c)
            {
                pos[0] = w + c;
                // Set the new value in the filtered image
                *(filtered_image + h + w + c) = (*filter)(target, pos, kernel_size);

            }
        }
    }

    // Return result
    return filtered_image;

}


/*  Takes and RGB image and calculates the mean value of each channel, then
 *  selects the channel with the highest value and returns a value between 0-2
 *  representing the selected rgb channel
 *
 *  image: ptr to the image target
 *
 *  return: [0, 3] value representing the rgb channel
 */
uint8_t get_strongest_channel(uint8_t* image){

    // Define accumulator variable
    float* mean_rgb_values = calloc(3, sizeof(float));


    // HEIGHT and WIDTH limits have to be multiplicated to take into account
    // the channels of the image
    for (size_t h = 0; h < HEIGHT*WIDTH*CHANNELS; h += WIDTH*CHANNELS){

        for (size_t w = 0; w < WIDTH*CHANNELS; w += CHANNELS){

            // Add the r value
            mean_rgb_values[0] += (float)*(image + h + w) / (float)(HEIGHT*WIDTH);
            // Add the g value
            mean_rgb_values[1] += (float)*(image + h + w + 1) / (float)(HEIGHT*WIDTH);
            // Add the b value
            mean_rgb_values[2] += (float)*(image + h + w + 2) / (float)(HEIGHT*WIDTH);
        }
    }

    // If the r channel is stronger or equal to the rest, select it
    if((mean_rgb_values[0] >= mean_rgb_values[1]) && (mean_rgb_values[0] >= mean_rgb_values[2])){
        return 0;
    }
    // If the g channel is stronger or equal to the rest, select it
    else if(mean_rgb_values[1] >= mean_rgb_values[2]){
        return 1;
    }
    // If the b channel is stronger or equal to the rest, select it
    else{
        return 2;
    }

}


int main() {

    // Load the image
    uint8_t* image = read_image("../imgs/SnPNoisyLenna.png", WIDTH, HEIGHT, CHANNELS, CHANNELS);
    // Define the kernel size
    uint8_t kernel_size = 3;

    // Start the convolution
    uint8_t* filtered_image = apply_filter(image, &median_filter, kernel_size);


    // Store the filtered image
    write_image("../imgs/FilteredLenna.png", filtered_image);

    // Free the memory of the images
    stbi_image_free(image);
    free(filtered_image);


    uint8_t* r = read_image("../imgs/r.png", WIDTH, HEIGHT, CHANNELS, CHANNELS);
    uint8_t* g = read_image("../imgs/g.png", WIDTH, HEIGHT, CHANNELS, CHANNELS);
    uint8_t* b = read_image("../imgs/b.png", WIDTH, HEIGHT, CHANNELS, CHANNELS);
    printf("On image r the strongest channel is %d\n", get_strongest_channel((r)));
    printf("On image g the strongest channel is %d\n", get_strongest_channel((g)));
    printf("On image b the strongest channel is %d\n", get_strongest_channel((b)));



}