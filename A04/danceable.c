/*----------------------------------------------
 * Author: Cecilia Zhang
 * Date: 02/13/2023
 * Description: A program that reads info of songs from another file, 
 *  and stores all info into a linked list. Then, it displays out all songs, 
 *  and allows the user to access the most danceable song in the list. 
 *  When the user asks for the most danceable song, it will show this song's information, 
 *  and then delete it from the linked list.
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

struct node {
  struct song mySong;
  struct node* next;
};

/**
* Prints out all the information about each song from the given list of songs.
* @param myNode A node that contains the song that will be printed.
* @return None. 
*/
void printSongs(struct node* myNode){
  int count = 0;
  while (myNode != NULL){
    printf("\n%d) %-25s", count, myNode->mySong.title);
    printf("Artist: %-15s", myNode->mySong.artist);
    if (myNode->mySong.durationSec < 10) {
      printf("Duration: %d:0%-10d", myNode->mySong.durationMin, myNode->mySong.durationSec);
    } 
    else {
      printf("Duration: %d:%-10d", myNode->mySong.durationMin, myNode->mySong.durationSec);
    }
    
    printf("D: %.3f  ", myNode->mySong.danceability);
    printf("E: %.3f  ", myNode->mySong.energy);
    printf("T: %.3f  ", myNode->mySong.tempo);
    printf("V: %.3f  \n", myNode->mySong.valence);
    count ++;
    myNode = myNode->next;
  }
  printf("\nDataset contains %d song(s) \n", count);
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
* @param lineBuff a char array containing all characters from a line. 
* @return s song struct containing all info.
*/
struct song saveSong(char lineBuff[1024]){
  //create a temp song to store and return
  struct song songTemp;
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

/**
* Insert a new node of song at the front of the linked list.
* @param mySong a song struct that will be included in the new node. 
* @param head the pointer to the new node that will be added.
* @return the pointer to the new head node.
*/
struct node* insertFront(struct song mySong, struct node* head){
  struct node* n = malloc(sizeof(struct node));
  if (n == NULL) {
    printf("ERROR. NO SPACE. \n");
    exit(1);
  }
  n->mySong = mySong;
  n->next = head;
  return n;
}

/**
* Find the song with highest danceability, then print it and delete it.
* @param head the pointer to the head node of the song list.
* @return the pointer to the head of the list after deletion.
*/
struct node* mostDanceable(struct node* head){
  struct node *originalHead = head;
  struct node *mostDan = head;
  struct node *prevNode = NULL;
  //empty linked list
  if(head == NULL){
    return NULL;
  }
  //find most danceable song and print
  while(head != NULL){
    if((mostDan->mySong.danceability) < (head->mySong.danceability)){
      mostDan = head;
    }
    head = head->next;
  }
  printf("\n************* Most Danceable *************\n");
  printf("%-25s", mostDan->mySong.title);
  printf("%-10s", mostDan->mySong.artist);
  printf("%s(%d:%ds)", "", mostDan->mySong.durationMin, mostDan->mySong.durationSec);
  printf("\nD: %.3f", mostDan->mySong.danceability);
  printf(" E: %.3f", mostDan->mySong.energy);
  printf(" T: %.3f", mostDan->mySong.tempo);
  printf(" V: %.3f\n", mostDan->mySong.valence);
  printf("******************************************\n");
  //reset head back to the first song
  head = originalHead;
  //remove the most danceable song
  if (head != NULL && mostDan == head){ //when head is most danceable
    head = head->next;
    free(mostDan);
    return head;
  }
  //when most danceable is not the first song
  while (head != NULL && head != mostDan){
    prevNode = head;
    //update head pointer to next node
    head = head->next;
  }
  //unlink the node for most danceable song and delete
  prevNode->next = mostDan->next;
  head = originalHead;
  free(mostDan);
  return head;
}

int main() {
  //read and check file
  FILE *infile;
  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error! Can't open file: %s\n", "input.txt");
    exit(1);
  }

  char lineBuff[1024];
  char inputBuff[32] = "d";
  struct node* head = NULL;
  //skip title line
  fgets(lineBuff, sizeof(lineBuff), infile);
  //store songs into linked list
  while (fgets(lineBuff, sizeof(lineBuff), infile) != NULL) {
    head = insertFront(saveSong(lineBuff), head);
  }
  //main driver
  while (1){
    printSongs(head);
    printf("Press 'd' to show the most danceable song (any other key to quit): ");
    scanf("%s", inputBuff);
    //if user wants to show the most danceable song
    if (strcmp(inputBuff, "d") == 0) { 
      head = mostDanceable(head);
      printf("\n");
    }
    //if user wants to exit
    else {
      printf("Bye!\n");
      break;
    }
  }
  //if user did not delete all songs, free the rest
  while (head != NULL){
    struct node* toFree = head;
    head = head -> next;
    free(toFree);
  }
  fclose(infile);
  return 0;
}
