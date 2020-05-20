#include "include/ImageProcessing.h"


int main() {

	char originalName [256] = "./imgs/SnPNoisyLenna.png";
	char filteredName[256] =  "./imgs/FilteredLenna2.png";


    int strongestColor = processImage(  originalName ,  filteredName );
    printf("The strongest channel is %d\n", strongestColor );

}
