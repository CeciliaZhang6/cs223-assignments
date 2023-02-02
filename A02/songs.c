/***************************************************
 * songs.c
 * Author: Cecilia Zhang 
 * Modified: Jan 31, 2023
 * Modified: Feb 1, 2023
 * Implements a program that has a struct called song. Song has 5 attributes: 
 * artist, title, durationMin, durationSec, and danceability. An interface 
 * for the user to change all attributes is also included.
 */

#include <stdio.h>
#include <string.h>

struct song {
  char artist[32];
  char title[32];
  int durationMin;
  int durationSec;
  float danceability;
};

/**
* Prints out all the information about each song from the given list of songs.
* @param songArray An array of songs that will be printed.
* @return None. 
*/
void printSongs(struct song songArray[3]){
  for (int i = 0; i < 3; i++){
    printf("\n%d) %-20s", i, songArray[i].title);
    printf("Artist: %-20s", songArray[i].artist);
    printf("Duration: %d:%-20d", songArray[i].durationMin, songArray[i].durationSec);
    printf("Danceability: %.2f\n", songArray[i].danceability);
  }
  printf("=======================\n");
}

int main() {
  //create and set up song info
  struct song song0, song1, song2; 
  //song 0
  strcpy(song0.artist, "Tears for Fears");
  strcpy(song0.title, "Shout");
  song0.durationMin = 4;
  song0.durationSec = 11;
  song0.danceability = 0.50;
  //song 1
  strcpy(song1.artist, "Harry Styles");
  strcpy(song1.title, "As it was");
  song1.durationMin = 2;
  song1.durationSec = 47;
  song1.danceability = 0.70;
  //song 2
  strcpy(song2.artist, "Pink Floyd");
  strcpy(song2.title, "Wish you were here");
  song2.durationMin = 5;
  song2.durationSec = 34;
  song2.danceability = 0.30;
  //array of all songs
  struct song songArray[3] = {song0, song1, song2};
  //variables that stores user inputs
  int songID = 0;
  char input[32];
  int inputInt = 0;
  float inputFl = 0.0;
  //welcome message
  printf("Welcome to Steven Struct's Song List. \n");
  printSongs(songArray);
  //prompt
  printf("Enter a song id to edit [0,1,2]: ");
  scanf(" %d%*c", &songID);
  if (songID <= 2 && songID >= 0){ //check song validity
    printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
    scanf(" %[^\n]%*c", input);
    if (strcmp(input, "artist") == 0) { //update artist info
      printf("Enter an artist: ");
      scanf(" %[^\n]%*c", input);
      strcpy(songArray[songID].artist, input);
      printSongs(songArray);
    } 
    else if ((strcmp(input, "title") == 0)) { //update title info
      printf("\nEnter a title: ");
      scanf(" %[^\n]%*c", input);
      strcpy(songArray[songID].title, input);
      printSongs(songArray);
    }
    else if ((strcmp(input, "duration") == 0)) { //update duration info
      printf("Enter a duration (min): ");
      scanf(" %d%*c", &inputInt);
      songArray[songID].durationMin = inputInt;
      printf("Enter a duration (sec): ");
      scanf(" %d%*c", &inputInt);
      songArray[songID].durationSec = inputInt;
      printSongs(songArray);
    }
    else if ((strcmp(input, "danceability") == 0)) { //update danceability info
      printf("Enter danceability: ");
      scanf(" %f%*c", &inputFl);
      songArray[songID].danceability = inputFl;
      printSongs(songArray);
    }
    else { //invalid input
      printf("Invalid choice. Please try again. \n");
    }
  } 
  else {
    printf("Invalid song ID. Please try again. Only enter numbers from 0 to 2. \n");
  }
  return 0;
}
