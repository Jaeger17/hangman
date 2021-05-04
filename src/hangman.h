#ifndef HANGMAN_H
#define HANGMAN_H
/**
 * @author Jaeger17 (jaeger17@protonmail.com)
 * @brief Header file for hangman.c, contains function prototpyes.
 * @date 2021-05-04
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX 100  // maximum of characters allowed for a line of input

/**
 * @brief Check the command line arguments.
 * 
 * @param argc (int): Number of command-line arguments.
 * @param argv (char *): Pointer to an array of command-line arguments.
 * @param tp (char *): Pointer to target word.
 * 
 * @return (int): Status integer. See hangman.c
 */
void arg_check(int argc, char *argv[], char *tp);

/**
 * @brief Get stats from a .hangman file.
 * 
 */
void get_stats(void);

/**
 * @brief Get the target word. Inspiration for the design of this function came 
 * from stack overflow. Names of variables were changed.
 * Source: "https://tinyurl.com/y4pa3m5j", Users: DeadCake, JohnH, and paddy
 * 
 * @param tp (char *): Pointer to target word.
 * @param fp (FILE *): Pointer to the file that we are opening.
 */
void get_target(char *tp, FILE *fp);

/**
 * @brief Generate a random word from a list of words.
 * 
 * @param size (int): Number of words in the word array.
 * @param words (char **): Words array.
 * @param tp (char *): Pointer to target word.
 * 
 */
void gen_rand(int size, char **words, char *tp);

/**
 * @brief Display player game console.
 * 
 * @param tp (char *): Pointer to target word.
 */
void game_console(char *tp);

/**
 * @brief Validate user input for hangman guesses. If the user correctly guesses
 * an entire word they win the game.
 * 
 * @param tp (char *): Pointer to target word.
 * @return (char): The player's character guess.
 */
char input_val(char *tp);

/**
 * @brief Set the stats to a .hangman file.
 * 
 */
void set_stats(void);

#endif // HANGMAN_H