// Author: Mingyi Su
// This is a program that plays a word-guessing game similar to hangman
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "io-string.h"
#include "player.h"

// all_out(arr, n) returns true if all the players are out, 
//    i.e. chance == 0
// requires: arr is valid, n > 0
// time: O(n)
bool all_out(struct player **arr, int n) {
  assert(arr);
  assert(n > 0);
  for (int i = 0; i < n; i++) {
    if (arr[i]->chance > 0) {
      return false;
    }
  }
  return true;
}

// swap(p1, p2) swaps p1 and p2
// requires: p1, p2 are valid pointers
// effects: modifies p1, p2
// time: O(1)
void swap(struct player *p1, struct player *p2) {
  assert(p1);
  assert(p2);
  struct player temp = {0};
  temp.id = p1->id;
  temp.chance = p1->chance;
  temp.score = p1->score;
  p1->id = p2->id;
  p1->chance = p2->chance;
  p1->score = p2->score;
  p2->id = temp.id;
  p2->chance = temp.chance;
  p2->score = temp.score;
}

// sort(arr, len) sorts arr
// effects: modifies arr
// time: O(n^2)
void sort(struct player **arr, int len) {
  for (int i = 0; i < len - 1; i++) {
    for (int j = 0; j < len - i - 1; j++) {
      if (arr[j]->score < arr[j + 1]->score) {
        swap(arr[j], arr[j + 1]);
      }
    }
  }
}

// is_equal(c1, c2) returns true if a and b are 
//    equivalent and false otherwise
// requires: c1 and c2 are non-whitespace printable characters
bool is_equal(char c1, char c2) {
  assert(32 <= c1 && c1 <= '~');
  assert(32 <= c2 && c2 <= '~');
  if ('a' <= c1 && c1 <= 'z') {
    c1 -= 32;
  }
  if ('a' <= c2 && c2 <= 'z') {
    c2 -= 32;
  }
  return (c1 == c2);
}

// print_board(arr, len) prints arr
// requires: len >= 0
// 	         arr is a valid pointer
// effects: produces output
// time: Sum[i=0 .. i=len-1] O(1) = O(n)
void print_board(const char arr[], int len) {
  assert(len >= 0);
  assert(arr); 
  printf("\n");
  for (int i = 0; i < len; ++i) {
    printf("%c", arr[i]);
  }
  printf("\n");
}

// print_guessed(arr, len) prints guessed letters
// requires: len >= 0
// 	         arr is a valid pointer
// effects: produces output
// time: Sum[i=0 .. i=len-1] O(1) = O(n)
void print_guessed(const char *arr, int len) {
  assert(len >= 0);
  assert(arr); 
  printf("Guessed Letters: ");
  for (int i = 0; i < len; ++i) {
    printf(" %c", arr[i]);
  }
  printf("\n");
}

// is_over(arr, len) returns true if arr is a 
//    finished game and false otherwise
// requires: arr is a valid pointer
// 	         len >= 0
// time: Sum[i=0 .. i=len-1] O(1) = O(n)
bool is_over(const char arr[], int len) {
  assert(len >= 0);
  assert(arr);
  for (int i = 0; i < len; ++i) {
    if (is_equal(arr[i], '*')) {
      return false;
    }
  }
  return true;
}

// is_in(arr, len, c) produces true if c is in arr and false otherwise
// requires: arr is a valid pointer
// 	         len >= 0;
// time: O(n)
bool is_in(const char arr[], int len, char c) {
    assert(len >= 0);
    assert(arr);
    for (int i = 0; i < len; ++i) {
    if (is_equal(arr[i], c)) {
      return true;
    }
  }
  return false;
}

// check_guessed(arr, n, c) inserts c into arr if c is not in it and does nothing otherwise
// requires: arr is a valid pointer
//           *n >= 0;
// effects: modifies arr, *n
// time: O(1)
char *check_guessed(char arr[], int *n, const char c) {
  if (!is_in(arr, *n, c)) {
    arr[*n] = c;
    *n += 1;                                  // if input is not guessed, mark it as guessed
    arr = realloc(arr, *n + 1);               // extend the size by 1
  }
  return arr;
}

// print_scoreboard(arr, len) prints arr
void print_scoreboard(struct player **arr, int len) {
  printf("\n\tRANKINGS");
  for (int i = 0; i < len; ++i) {
    printf("\n");
    if (i == 0) {
      printf("CHAMPION: ");
      printf(player_info, arr[i]->id, arr[i]->score, arr[i]->chance);
    } else if (arr[i]->score > 0) {
      printf("NO.%d: ", i + 1);
      printf(player_info, arr[i]->id, arr[i]->score, arr[i]->chance);
    } else {
      printf("PLAYER %d is OUT!\n", i + 1);
    }
  }
}

int main(void) {
  while (1) {
    system("clear"); // change to "cls" on Windows
    int result = 0;
    int nop = 0;
    int len = 0;
    int cur = -1;
    int num_guessed = 0;
    char *guessed = malloc(sizeof(char));
    char guess = 0;
    char again = 0;
    char word[51] = {0};

    printf("%s", num_of_player);
    scanf(" %d", &nop);
    getchar();

    struct player **players = malloc(nop * sizeof(struct player *));
    for (int i = 0; i < nop; ++i) {
      players[i] = malloc(sizeof(struct player));
      players[i]->id = i + 1;
      players[i]->chance = MAX_CHANCE;
      players[i]->score = 0;
    }

    printf("%s", enter_word);
    fgets(word, 51, stdin);
    len = strlen(word) - 1;
    word[len] = '\0';
    for (int i = 0; i < len; ++i) {
      assert(32 <= word[i] && word[i] <= '~');
      assert(word[i] != '*');
      if ('a' <= word[i] && word[i] <= 'z') {
        word[i] -= 32; // transfer to upper cases
      }
    }

    char *board = malloc(sizeof(char) * len);
    for (int i = 0; i < len; ++i) {                           // set up board
      if (word[i] == ' ') {
        board[i] = ' ';                                    // spaces are spaces
      } else {
        board[i] = '*';                                   // capital letters are printed as "*"
      }
    }

    while (1) {                 // game loop
      system("clear"); // change to "cls" on Windows 
      if (is_over(board, len)) {
        break;
      }

      if (all_out(players, nop)) {
        result = LOSE;
        break;
      }

      cur++;
      if (cur >= nop) {
        cur = 0;
      }

      if (players[cur]->chance <= 0) {
        printf(out, players[cur]->id);
        continue;
      }

      print_board(board, len);
      print_guessed(guessed, num_guessed);
      printf(whose_turn, players[cur]->id);
      printf("Score: %d\n", players[cur]->score);
      printf("Chances: %d\n", players[cur]->chance);

      printf("%s", enter_letter);
      scanf(" %c", &guess);

      if ((!(32 <= guess && guess <= '~') || 
           guess == '*')) {              // case 4 not valid input, start a new round
        printf(not_valid, guess);
        players[cur]->chance--;
        continue;
      }

      if (guess >= 'a' && guess <= 'z') {                   // if lower case, transfer it to upper case
        guess -= 32;
      }

      if (is_in(guessed, num_guessed, guess)) {             // case 3 is a guessed letter
        printf(already_guessed, guess);
        continue;
      }

      if (!is_in(word, len, guess)) {                       // case 2 not in the word
        guessed = check_guessed(guessed, &num_guessed, guess);
        printf(wrong, guess);
        players[cur]->chance--;

        if (players[cur]->chance > 0) {
          printf(remain_chance, players[cur]->chance);       // print # of guesses left
        }
        continue;                                            // if unsuccessful guess but still alive, start a new round
      }

      for (int i = 0; i < len; ++i) {                       // case 1 unguessed letter and contained in the phrase
        if (guess == word[i] && board[i] == '*') {
          board[i] = word[i];                               // if first time, show it on the board
          guessed = check_guessed(guessed, &num_guessed, guess);
          players[cur]->score += BONUS;
        }
      }

      if (is_over(board, len)) {                            // if game finished, print congratulations and forward to "want-again" part
        result = WIN;
        printf(phrase, word);
        printf("%s", congra);
        break;
      }
    }

    system("clear"); // change to "cls" on Windows
    if (result == LOSE) {
      printf("ALL LOSERS!\n");
    } else if (result == WIN) {
      for (int i = 0; i < nop; ++i) {
        if (players[i]->chance <= 0) {
          players[i]->score = 0; // ELIMINATED, lost all scores
        }
      }
      sort(players, nop);
      print_scoreboard(players, nop);
    }

    for (int i = 0; i < nop; ++i) {
      free(players[i]);
    }
    free(players);
    free(guessed);
    free(board);

    while (1) {                 // recursively asking for yes or not until a valid input
      printf("%s", try_again);
      scanf(" %c", &again);
      if (again != 'Y' && again != 'y' && again != 'N' && again != 'n') {   // invalid input, do again
        printf("%s", invalid);
        continue;
      } else {              // valid, break
        break;
      }
    }

    if (again == 'y' || again == 'Y') {               // yes, play again
      continue;
    } else if (again == 'n' || again == 'N') {        // no, quit
      break;
    }
  }
  
  return 0;
}
