/*
 * developer:    Jaeger17
 * created:      11/05/2020
 * last changed: 11/07/2020
 * description:  Play Hangman! 6 incorrect guesses and you lose.
*/
#include "hangman.h"

struct stats {
  int games;
  int win;
  int lose;
  int score;
} game;


int main(int argc, char *argv[])
{
        srand(time(NULL));  // seed the rand generator
        char target[MAX];
        char *tp = target;  // target pointer (tp) points to target char array

        get_stats();
        arg_check(argc, argv, tp);
        game_console(tp);
        set_stats();
}


void arg_check(int argc, char *argv[], char *tp)
{
        FILE *fp;

        if(argc == 1) {
                fp = fopen(".words", "r");
                if(!fp) {
                        printf("Error opening %s\n", ".words");
                        exit(1);
                }
                get_target(tp, fp);
        } else if(argc == 2) {
                if((fp = fopen(argv[1], "r")) != NULL) {
                        printf("successfully opened %s\n", argv[1]);
                        get_target(tp, fp);
                } else {
                        printf("unable to open %s\nTry again.\n", argv[1]);
                        exit(1);
                }
        } else {
                printf("Too many arguments.\n");
                exit(1);
        }
}

/*
 * Get stats from the file .hangman. If the file does not exist, initialize the
 * stats struct "game" to all 0's.
 *
 * return - void
 */
void get_stats(void)
{
        FILE *infile;  // infile file pointer

        infile = fopen("./.hangman", "r");

        if(!infile) {
                fprintf(stderr, "Creating stats...\n");
                // initialize game stats
                game.games = 1;
                game.win = game.lose = game.score = 0;
                printf("game %d. %d Win/%d Loss. Average score: %.2f\n",
                       game.games,
                       game.win,
                       game.lose,
                       ((float)game.score/(float)game.games));

        } else {
                while (fread(&game, sizeof(struct stats), 1, infile)) {
                        printf("game %d. %d Win/%d Loss. Average score: %.2f\n",
                               game.games,
                               game.win,
                               game.lose,
                               ((float)game.score/(float)(game.games - 1)));
                }
                fclose(infile);
        }
}

/*
 * Inspiration for the design of this function came from stack overflow.
 * Names of variables were changed.
 * Source: "https://tinyurl.com/y4pa3m5j", Users: DeadCake, JohnH, and paddy
 *
 * Get the target random word from the file passed as a parameter.
 *
 * char *tp - a pointer to the target array.
 * FILE *fp - a pointer to the file that we are opening.
 *
 * return - void
 */
void get_target(char *tp, FILE *fp)
{
        char word[MAX];
        int count = 0;
        int allocate = 4;
        unsigned long alpha_count;
        unsigned long punct_count;
        char **wp;  // words pointer (wp) points to words array

        //fp = fopen("targets.txt", "r");
        wp = (char **) malloc(sizeof(char*) * allocate);

        while(fscanf(fp, "%s", word) != EOF) {
                // reallocates memory based on the amount of words it is taking
                if(count >= allocate) {
                        allocate *= 4;
                        wp = (char **) realloc(wp, sizeof(char*) * allocate);
                }

                alpha_count = 0;  // rest the counter each time
                punct_count = 0;  // rest the counter each time

                for(unsigned long i = 0; i <= strlen(word); i++) {
                        if((word[i] >= 'a' && word[i] <= 'z') ||
                                        (word[i] >= 'A' && word[i] <= 'Z')) {
                                alpha_count++;
                        }
                        if(ispunct(word[i]) != 0) {
                                punct_count++;
                        }
                }

                // this check makes sure that we aren't storing invalid words
                if(alpha_count + punct_count == strlen(word)) {
                        wp[count] = (char *) malloc(sizeof(char) *
                                (strlen(word) + 1));
                        strcpy(wp[count], word);
                        ++count;
                }
        }

        gen_rand(count, wp, tp);

        // free each malloc'd array within wp
        for (int i = 0; i < count; i++) {
                free(wp[i]);
        }

        free(wp);
        fclose(fp);

}


void gen_rand(int size, char **words, char *tp)
{
        int i = rand() % size;
        strcpy(tp, words[i]);
}

/*
 * The console where the user will interact with the hangman game.
 *
 * char *tp - a pointer to the target array.
 *
 * return - void
 */
void game_console(char *tp)
{
        // Set a temporary pointer (tmp) to the target (tp) for comparison ops
        char *tmp = calloc(strlen(tp) + 1, sizeof(*tmp));
        char *hidden = calloc(strlen(tp) + 1, sizeof(*hidden));
        // consumed will eat valid correct answers so that I can count them
        // as incorrect if the user attempts to guess it again
        char *consumed = calloc(strlen(tp) + 1, sizeof(*consumed));
        char tmp_input;
        int counter;  // for counting correct letters
        int counter_r; // checks for repeats;
        char input;
        unsigned tries = 0;  // for counting valid incorrect attempts

        // initialize hidden and consumed arrays
        for (unsigned long i = 0; i <= (strlen(tp)); i++) {
                if(i != strlen(tp)) {
                        if(ispunct(tp[i]) != 0) {
                                hidden[i] = tp[i];
                        } else {
                                hidden[i] = '_';
                                consumed[i] = '_';
                        }
                } else {
                        hidden[i] = '\0';
                }
        }

        // initialize tmp
        for (int i = 0; i <= (int)(strlen(tp)); ++i) {
                tmp[i] = tolower(tp[i]);  // sets the tmp word to all lower
        }
        puts("");
        game.games++;
        while(true) {
                counter = 0;
                counter_r = 0;
                printf("%d ", tries);
                for (unsigned long i = 0; i <= (strlen(tp) - 1); i++) {
                        printf("%c ", hidden[i]);
                }
                printf(":");
                input = input_val(tp);
                // using ascii character '!' 33 as my error check
                if(input == 33) {
                        continue;
                } else if(input == 0) {
                        printf("  ");
                        for (unsigned long i = 0; i <= (strlen(tp) - 1); i++) {
                                printf("%c ", tp[i]);
                        }
                        printf("  \n");
                        printf("You win! You had %d miss(es)\n", tries);
                        game.win++;
                        set_stats();
                        break;
                }
                tmp_input = input;
                // checks to see if a correct guess has already been guessed
                if(strchr(consumed, tolower(tmp_input))) {
                        counter_r++;
                }

                for(unsigned long i = 0; i <= (strlen(tp) - 1); i++) {
                        // checks take letter-case into consideration
                        if (tolower(tmp_input) == tmp[i])  {
                                hidden[i] = tp[i];  // assign the original char
                                consumed[i] = tmp_input;
                                counter++;
                        }
                }

                // if wrong answer or already correctly guessed, add 1 to tries
                if(counter == 0 || counter_r != 0) {
                        tries++;
                        game.score++;
                }

                if(tries == 6) {
                        printf("You lose!\n");
                        game.lose++;
                        break;
                }

                if(strcmp(hidden, tp) == 0) {
                        printf("  ");
                        for (unsigned long i = 0; i <= (strlen(tp) - 1); i++) {
                                printf("%c ", hidden[i]);
                        }
                        printf("  \n");
                        printf("You win! You had %d miss(es)\n", tries);
                        game.win++;
                        break;
                }
        }

	// free up dynamically allocated resources. 
	free(tmp);
	free(hidden);
	free(consumed);
}

/*
 * Validate user input. This is where if the user outright guesses the correct
 * word, they get full credit and win the game.
 *
 * char *tp - a pointer to the target array.
 *
 * return - char - gets assigned to input in user_console().
 */
char input_val(char *tp)
{
        char line[MAX];
        char tmp_line[MAX];  // for case insensitive comparison
        char tmp_target[MAX];  // for case insensitive comparison

        fgets(line, sizeof(line), stdin);

        // convert both temp arrays to lower for comparisons
        for (int i = 0; i <= (int)(strlen(tp)); ++i) {
                // sets the tmp word to all lower
                tmp_target[i] = tolower(tp[i]);
        }

        for (int i = 0; i <= (int)(strlen(line)); ++i) {
                // sets the tmp word to all lower
                tmp_line[i] = tolower(line[i]);
        }

        // account for the newline in the string comparison
        if (strcmp(tmp_line, tmp_target) == 10) {
                return 0;

        } else if ((strlen(line) == 2) && ((line[0] >= 'a' && line[0] <= 'z') ||
                (line[0] >= 'A' && line[0] <= 'Z')) &&
                (line[1] == '\n')){
                return line[0];
        } else {
                printf("Invalid input\n");
                // using ascii character '!' 33 as my error check
                return 33;
        }
}

/*
 * Set stats to the file .hangman. If the file does not exist, or is unable to
 * open, display an error message.
 *
 * return - void
 */
void set_stats(void)
{
        FILE *outfile;  // outfile file pointer (of)

        outfile = fopen("./.hangman", "w+");
        if(!outfile) {
                fprintf(stderr, "Unable to retrieve stats file\n");
        } else {
                fwrite(&game, sizeof(struct stats), 1, outfile);
                fclose(outfile);
        }
}
