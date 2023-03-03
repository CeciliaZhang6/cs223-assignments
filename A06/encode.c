/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 03/01/2023
 * Description: Takes ppm file as runtime input, asks user for a message to embed 
 *    and convert into binary bits, reads each pixel in ppm file, embeds message by 
 *    changing the least significant bit of pixel's RGB values to the corresponding 
 *    binary bit, and outputs a ppm file with encoded message. 
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  int w, h;
  struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
  //file cannot open
  if (pixels == NULL) {
    exit(0);
  }

  int maxChar = ((w * h * 3) / 8) - 1; 
  printf("Reading %s: w = %d, h = %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d\n", maxChar);
  
  //user input of the message that will be embedded
  char* buff = malloc(sizeof(char) * (maxChar + 1));
  printf("Enter a phrase: ");
  scanf("%s", buff);
  //check for phrase length
  if (strlen(buff) >= maxChar) {
    printf("Invalid phrase length. Too long. \n");
    return 0;
  }

  char* encodedBits = malloc(sizeof(char) * (maxChar + 1) * 8);
  //convert phrase into bits
  for (int i = 0; i < strlen(buff) + 1; i++){
    //mask for checking left most bit
    unsigned char mask = 0x80;
    for (int j = 0; j < 8; j++) {
      encodedBits[i * 8 + j] = (mask & buff[i]) >> (7 - j); 
      //update mask to check the next left most bit
      mask = mask >> 1; 
    }
  }

  int index = 0;
  //embed bits into RGB values
  for (int i = 0; i < h; i++){
    //mask for flipping the right most bit
    unsigned char mask = 0x1;
    for (int j = 0; j < w; j++){
      //red
      if (index < (strlen(buff) + 1) * 8 && 
            pixels[i * w + j].red % 2 != encodedBits[index]) {
        pixels[i * w + j].red = pixels[i * w + j].red ^ mask;
      }
      index ++;
      //green
      if (index < (strlen(buff) + 1) * 8 && 
            pixels[i * w + j].green % 2 != encodedBits[index]) {
        pixels[i * w + j].green = pixels[i * w + j].green ^ mask;
      }
      index ++;
      //blue
      if (index < (strlen(buff) + 1) * 8 && 
            pixels[i * w + j].blue % 2 != encodedBits[index]) {
        pixels[i * w + j].blue = pixels[i * w + j].blue ^ mask;
      }
      index ++;
    }
  }

  //write new pixels into ppm files
  char filename[1024];

  strcpy(filename, argv[1]);
  filename[strlen(filename) - 4] = '\0';
  strcat(filename, "-encoded.ppm");
  write_ppm(filename, pixels, w, h);
  printf("Writing file %s\n", filename);

  free(buff);
  free(pixels);
  free(encodedBits);
  return 0;
}
