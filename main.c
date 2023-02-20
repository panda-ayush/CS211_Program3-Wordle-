// bestWordle.c
// Program 3: Best Wordle Word
// CS 211, UIC, Fall 2022
// System: Replit
// Author:Ayush Panda
//
// Links to wordle dictionary words at: https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/

#include <stdio.h>    // for printf, scanf
#include <stdlib.h>   // for exit( -1)
#include <string.h>   // for strcpy

// Declare globals
#define ANSWERS_FILE_NAME "answersTiny.txt"
#define GUESSES_FILE_NAME "guessesTiny.txt"   

//struct that holds word and score
typedef struct wordCount wordCountStruct;
struct wordCount{
  char word[6];
  int score;     
};

// function to check if the file properly opens
void sanityCheck (char fileName[ ]) {
  FILE *inFilePtr = fopen(fileName, "r"); // open the file
  
  if (inFilePtr == NULL ) {
    printf("Error: could not open %s for reading\n", fileName);
    exit(-1);    
  }
} // end of sanityCheck()

// function to read the file and append all of the words from it into an array
void inputFunction (char fileName[ ], int start, wordCountStruct *words) {
  FILE *inFilePtr = fopen(fileName, "r"); // reread the file 
  char inputString[81]; // set up array
  sanityCheck(fileName); // call the function

  // loop to copy the words into the array
  int i = start;
  while (fscanf(inFilePtr, "%s", inputString) != EOF) {
    strcpy(words[i].word, inputString); // copy the words from inputString into the words array
    words[i].score = 0;
    i++;
  }

  fclose(inFilePtr); // close the file
} // end appendWordsFromFileToArray(..)

// function for getting the score where one point is for each correct letter in incorrect position & three points for each correct letter in correct position
int compareFirst (char wordComparison[], char wordOriginal[]) {
  char COPYwordComparison[6];
  char COPYwordOriginal[6];
  int score = 0;   

  char i;
  for (i = 0; wordOriginal[i] != 0; i++) {
    COPYwordOriginal[i] = wordOriginal[i];
  }

  char j;
  for (j = 0; wordComparison[j] != 0; j++) {
    COPYwordComparison[j] = wordComparison[j];
  }

  // loop to give 3 points
  int a = 0;
  while (a < 5) {
    if (COPYwordOriginal[a] == COPYwordComparison[a]) { // checks if letter are correct in correct positions
      score += 3;   
      COPYwordOriginal[a] = ' '; // replace w/ blank space
      COPYwordComparison[a] = ' ';
    }
    a++;
  }

  // loop to give 1 point
  int b = 0;
  while (b < 5) {
    char d = COPYwordOriginal[b];
    if (d != ' ') {
      for (int e = 0; e < 5; e++) {
        if (d == COPYwordComparison[e]) { // checks if letter is correct but in incorrect position
          score += 1;
          COPYwordOriginal[b] = ' '; // replace w/ blank space
          COPYwordComparison[e] = ' ';
          break; // break out of loop
        }
      } 
    } 
    b++;
  } 
 
  return score;
} //end of compareFirst()

// function for getting the score & calls above function
int functionScoring ( wordCountStruct *answers, int counterAnswer, char wordCompare[]) {
  int score = 0;
  int a = 0;
  while (a < counterAnswer) {
    score += compareFirst(answers[a].word, wordCompare); // call the function
    a++;
  }
  return score;
} // end of functionScoring

// function to compare the scores and the words
int functionComparing (const void * a, const void * b) {
  int firstScore = ((wordCountStruct *) a) -> score;
  int secondScore = ((wordCountStruct *) b) -> score;
  int temp = 0;
  
  if (firstScore != secondScore) {
    return secondScore - firstScore;
  }
  else {
    return strcmp(((wordCountStruct *) a) -> word,  ((wordCountStruct *)b) -> word );
  }
  
} //end compareFunction()

// function to get the score for the words of the answers and guesses by comparing them and also get the # of the top scoring words
void bestWordScore (wordCountStruct *answers, int counterAnswer, wordCountStruct *aAndG, int allCounter, wordCountStruct * *wordsTop, int *topScoringWords) {

  int a = 0;
  while (a < allCounter) {
    aAndG[a].score = functionScoring(answers, counterAnswer, aAndG[a].word);
  a++;
  }

  qsort(aAndG, allCounter, sizeof(wordCountStruct), functionComparing);
  
  int topScore = aAndG[0].score;
  int index = 0;
  while (aAndG[index].score == topScore) {
    index++;
  }
  *topScoringWords = index;

  *wordsTop = (wordCountStruct *) malloc( sizeof( wordCountStruct) * *topScoringWords); // allocate space

  // loop to copy answers & guesses array into the top words array
  int b = 0;
  while (b < *topScoringWords) {
    strcpy((*wordsTop)[b].word, aAndG[b].word); 
    (*wordsTop)[b].score = topScore;
    b++;
  }
  if (0) {
    printf("All words in descending order by score:\n");
    // loop to print the words in descending order based on score
    int c = 0;
    while (c < allCounter) {
      printf("%d %s\n", aAndG[c].score, aAndG[c].word);
      c++;
    }
  }
    
  if (0) {
    printf("Top scoring words:\n");
    // loop to print the top scoring words
    int d = 0;
    while (d < *topScoringWords) {
      printf("%s %d\n", (*wordsTop)[d].word, (*wordsTop)[d].score); 
      d++;
    }
  }
} //end topScoresAndWords()


// function to remove matching letters by replacing them w/ a blank space
void removeFunction (wordCountStruct *COPYwordAnswer, int counterAnswer, char topWord[]) {
  int a = 0;
  while (a < counterAnswer) {
    char COPYbestWord[6];

    char i;
    for (i = 0; topWord[i] != 0; i++) {
      COPYbestWord[i] = topWord[i];
    }
    
    int b = 0;
    while (b < 5) {
      if (COPYbestWord[b] == COPYwordAnswer[a].word[b] ) {
        COPYbestWord[b] = ' ';
        COPYwordAnswer[a].word[b] = ' ';
      }
      b++;
    }
    int c = 0;
    while (c < 5) {
      int d = 0;
      while (d < 5) {
        if (COPYbestWord[c] == COPYwordAnswer[a].word[d] ) {
          COPYwordAnswer[a].word[d] = ' ';
          break; 
        }
        d++;
      } 
      c++;
    } 
    a++;
  } 

} //end removeMatchingLetters()

// function to find the top second words
void bestFunctionSecond (wordCountStruct *answers, int counterAnswer, wordCountStruct *aAndG, int allCounter, wordCountStruct *wordsTop, int topWordIndex) {
  char wordTop[81];
  
  strcpy(wordTop, wordsTop[topWordIndex].word); // copy struct into bestWord
  int scoreTopWord = wordsTop[topWordIndex].score;

  wordCountStruct *COPYwordAnswer = (wordCountStruct *) malloc(sizeof(wordCountStruct) * counterAnswer); // allocate space

  // loop to copy the words into the array
  int a = 0;
  while (a < counterAnswer) {
    strcpy(COPYwordAnswer[a].word, answers[a].word);
    COPYwordAnswer[a].score = 0;
    a++;
  }

  removeFunction(COPYwordAnswer, counterAnswer, wordTop); // call the function

  int numberOfTopScoringSecondWords = 0;
  wordCountStruct *bestSecondWords = NULL;
  bestWordScore(COPYwordAnswer, counterAnswer, aAndG, allCounter, &bestSecondWords, &numberOfTopScoringSecondWords);

  if (0) {
    printf("answerWordsCopy after letters from %s removed:\n", wordTop);
    // loop to print the answers
    int b = 0;
    while (b < counterAnswer) {
      printf("%2d. %s\n", b, COPYwordAnswer[b].word);
      b++;
    }

    printf("    Words and scores:\n");
    // loop to print the words & the scores
    int c = 0;
    while (c < allCounter) {
      printf("    %s %d \n", aAndG[c].word, aAndG[c].score);
      c++;
    }
  } 
  
  printf("%s %d\n", wordTop, scoreTopWord);
  // loop to print the best second words & their scores
  int d = 0;
  while (d < numberOfTopScoringSecondWords) {
    printf("   %s %d", bestSecondWords[d].word, bestSecondWords[d].score);
    d++;
  }
  printf("\n");
  
} //end secondBest()

int main() {
  int answersWordCount = 0;
  int guessesWordCount = 0;
  char answersFileName[81];
  char guessesFileName[81];

  strcpy(answersFileName, ANSWERS_FILE_NAME); // copy name into
  strcpy(guessesFileName, GUESSES_FILE_NAME);
  
  printf("Default file names are %s and %s\n", answersFileName, guessesFileName);

  int menuOption = 0;
  // display the different menu options to user
  do {
    printf("\n");
    printf("Menu Options:\n");
    printf("  1. Display best first words only\n");
    printf("  2. Display best first and best second words\n");
    printf("  3. Change answers and guesses filenames\n");
    printf("  4. Exit\n");
    printf("Your choice: ");
    scanf("%d", &menuOption);
    if (menuOption == 4) { // if option is 4, it'll exit the program
      exit(1); 
    } 
    else if (menuOption == 3) { // if option is 3, it'll change the file names for answers and guesses
      printf("Enter new answers and guesses filenames: ");
      scanf("%s %s", answersFileName, guessesFileName);
    } 
    } while(menuOption == 3);
  
  char inputString[81];
  FILE *inFilePtr  = fopen(answersFileName, "r"); // open the file
  FILE *inFilePtr2 = fopen(guessesFileName, "r");
  sanityCheck(answersFileName); // call the function
  sanityCheck(guessesFileName);
  int counter = 0; // answers counter
  int counter2 = 0; // guesses counter
  while (fscanf(inFilePtr, "%s", inputString) != EOF) {
    counter++;
  }
  printf("%s has %d words\n", answersFileName, counter);
  answersWordCount = counter;
  while (fscanf(inFilePtr2, "%s", inputString) != EOF) {
    counter2++;
  }
  printf("%s has %d words\n", guessesFileName, counter2);
  guessesWordCount = counter2;
    int totalWordCount = answersWordCount + guessesWordCount; // answers + guesses counter

    wordCountStruct *answers = (wordCountStruct *) malloc(sizeof(wordCountStruct) * answersWordCount); // malloc
    wordCountStruct *aAndG = (wordCountStruct *) malloc(sizeof(wordCountStruct) * totalWordCount);

  // displays the count of words for both the files
  char file[81];
  char z;
  for (z = 0; answersFileName[z] != 0; z++) {
    file[z] = answersFileName[z];
  }
  inputFunction(file, 0, answers); // call the function
  inputFunction(file, 0, aAndG);
  char y;
  for (y = 0; guessesFileName[y] != 0; y++) {
    file[y] = guessesFileName[y];
  }
  inputFunction(file, answersWordCount, aAndG); // call the function

    int topScoringWords = 0;
    wordCountStruct *wordsTop = NULL;
    bestWordScore(answers, answersWordCount, aAndG, totalWordCount, &wordsTop, &topScoringWords); // call the function
  
    printf("\n");
    if (menuOption == 1) { // if option 1, display the best 1st words
      printf("Words and scores for top first words:\n");
      // loop to print only the best words & their scores
      int a = 0;
      while (a < topScoringWords) {
        printf("%s %d\n", wordsTop[a].word, wordsTop[a].score); 
        a++;
      }
    }
    else if (menuOption == 2) { // if option 2, it'll print 1st & best words
      printf("Words and scores for top first words and second words:\n");
      // loop to print the values
      int b = 0;
      while (b < topScoringWords) {
        bestFunctionSecond(answers, answersWordCount, aAndG, totalWordCount, wordsTop, b);
        b++;
      }
  }

  printf("Done\n"); 
  return 0;
} // end main()