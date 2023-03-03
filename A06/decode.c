/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 02/28/2023
 * Description: Takes ppm file as runtime input, reads each pixel in ppm file, 
 *    extracts the least significant bit of pixel's RGB values into binary, and 
 *    decodes the binary into ASCII string. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  int w, h;
  struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
  //file cannot open
  if (pixels == NULL) {
    return 0;
  }

  int maxChar = (w * h * 3) / 8; 
  char* decodedBits = malloc(sizeof(char) * w * h * 3);
  int index = 0;
  //read and save the bits
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      //read RGB vals and store bits
      decodedBits[index] = pixels[i * w + j].red % 2;
      decodedBits[index + 1] = pixels[i * w + j].green % 2;
      decodedBits[index + 2] = pixels[i * w + j].blue % 2;
      index += 3;
    }
  }

  printf("Reading %s: w = %d, h = %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d\n", maxChar);

  for (int i = 0; i < maxChar; i ++){
    //mask for extracting left most bit
    unsigned char myChar = 0x0;
    for (int j = 0; j < 8; j++){
      myChar = myChar | decodedBits[i * 8 + j] << (7 - j);
    }
    //stop extraction when read null char
    if (myChar == '\0'){
      break;
    }
    printf("%c", myChar);
  }
  printf("\n");

  free(decodedBits);
  free(pixels);
  return 0;
}

