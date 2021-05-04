#ifndef HANGMAN_H
#define HANGMAN_H
/**
 * @author Jaeger17 (jaeger17@protonmail.com)
 * @brief Header file for hangman.c, contains function prototpyes.
 * @date 2021-05-04
 * 
 * @copyright Copyright (c) 2021
 * 
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
 * @brief Get the target word
 * 
 * @param tp 
 * @param fp 
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
 * @brief 
 * 
 * @param tp 
 */
void game_console(char *tp);

/**
 * @brief 
 * 
 * @param tp 
 * @return char 
 */
char input_val(char *tp);

/**
 * @brief Set the stats to a .hangman file.
 * 
 */
void set_stats(void);

#endif // HANGMAN_H