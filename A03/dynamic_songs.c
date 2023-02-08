/*----------------------------------------------
 * File Name: dynamic_songs.c
 * Author: Cecilia Zhang
 * Date: 02/07/2023
 * Description: A program that reads each song's info from a CSV file into an array. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct song {
  char artist[32];
  char title[32];
  int durationMin;
  int durationSec;
  float danceability;
  float energy;
  float tempo;
  float valence;
};

/**
* Prints out all the information about each song from the given list of songs.
* @param songArray An array of songs that will be printed.
* @param songNum Total number of songs in the array. 
* @return None. 
*/
void printSongs(struct song songArray[], int songNum){
  printf("Welcome to Dynamic Donna's Danceability Directory. \n");
  for (int i = 0; i < songNum; i++){
    printf("\n%d) %-20s", i, songArray[i].title);
    printf("Artist: %-20s", songArray[i].artist);
    printf("Duration: %d:%-20d", songArray[i].durationMin, songArray[i].durationSec);
    printf("Danceability: %.3f  ", songArray[i].danceability);
    printf("Energy: %.3f  ", songArray[i].energy);
    printf("Tempo: %.3f  ", songArray[i].tempo);
    printf("Valence: %.3f  \n", songArray[i].valence);
  }
  printf("=======================\n");
}

/**
* Calculate the duration (minute) from the given milisecond. 
* @param ms Milisecond that will be converted. 
* @return int representing the minute. 
*/
int getMin(int ms){
  return (ms / 1000) / 60;
}

/**
* Calculate the duration (second) from the given milisecond. 
* @param ms Milisecond that will be converted. 
* @return int representing the second. 
*/
int getSec(int ms){
  return (ms / 1000) % 60;
}

/**
* Read a line of song info and stores info to a song struct. 
* @param songTemp a temp struct of song that will hold all song info. 
* @param lineBuff a char array containing all characters from a line. 
* @return song struct containing all info.
*/
struct song saveSong(struct song songTemp, char lineBuff[1024]){
  //split the line into words
  char * word = strtok(lineBuff, ",");
  strcpy(songTemp.title, word);
  word = strtok(NULL, ",");
  strcpy(songTemp.artist, word);
  word = strtok(NULL, ",");
  songTemp.durationMin = getMin(atoi(word));
  songTemp.durationSec = getSec(atoi(word));
  word = strtok(NULL, ",");
  songTemp.danceability = atof(word);
  word = strtok(NULL, ",");
  songTemp.energy = atof(word);
  word = strtok(NULL, ",");
  songTemp.tempo = atof(word);
  word = strtok(NULL, ",");
  songTemp.valence = atof(word);
  return songTemp;
}

int main() {
  //read and check file
  FILE *infile;
  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error! Can't open file: %s\n", "input.txt");
    exit(1);
  }

  int songNum = 0;
  char lineBuff[1024];
  
  //read number of songs
  fscanf(infile, "%d", &songNum);

  //skip title lines
  fgets(lineBuff, sizeof(lineBuff), infile);
  fgets(lineBuff, sizeof(lineBuff), infile);

  //array of all songs
  struct song* songList = NULL; 
  songList = malloc(sizeof(struct song) * songNum);

  //read and store
  for (int i = 0; i < songNum; i ++) {
    fgets(lineBuff, sizeof(lineBuff), infile);
    songList[i] = saveSong(songList[i], lineBuff);
  }
  
  printSongs(songList, songNum);

  free(songList);
  fclose(infile);
  return 0;
}
