/**
 * @author Jaeger17 (jaeger17@protonmail.com)
 * @brief Play Hangman! 6 incorrect guesses and you lose.
 * @date 2021-05-04
 */
#include "hangman.h"

struct stats {
  int games;
  int win;
  int lose;
  int score;
} game;

void clear_screen()
{
	#ifdef WINDOWS
    		system("cls");
	#else
    		system("clear");
#endif
}

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

void get_stats(void)
{
        FILE *infile;
        infile = fopen("./.hangman", "r");

        if(!infile) {
                puts("No '.hangman file', Creating stats...");
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

void get_target(char *tp, FILE *fp)
{
        char word[MAX];
        int count = 0;
        int allocate = 4;
        unsigned long alpha_count;
        unsigned long punct_count;
        char **wp;
        wp = (char **) malloc(sizeof(char*) * allocate);

        while(fscanf(fp, "%s", word) != EOF) {
                // reallocates memory based on the amount of words it is taking
                if(count >= allocate) {
                        allocate *= 4;
                        wp = (char **) realloc(wp, sizeof(char*) * allocate);
                }

                alpha_count = 0;
                punct_count = 0;

                for(unsigned long i = 0; i <= strlen(word); i++) {
                        if((word[i] >= 'a' && word[i] <= 'z') ||
                                        (word[i] >= 'A' && word[i] <= 'Z')) {
                                alpha_count++;
                        }
                        if(ispunct(word[i]) != 0) {
                                punct_count++;
                        }
                }

                // make sure that we aren't storing invalid words
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

void game_console(char *tp)
{
        /* 
	 * Set a temporary pointer (tmp) to the target (tp) for comparison ops.
	 * Consumed will "eat" valid correct answers so that I can count them
         * as incorrect if the user attempts to guess it again.
	 */
        char *tmp = malloc(strlen(tp) * sizeof(*tmp));
        char *hidden = malloc(strlen(tp) * sizeof(*hidden));
        char *consumed = malloc(strlen(tp) * sizeof(*consumed));
        char tmp_input;
        int counter;		// count correctly guessed characters
        int counter_r;  	// checks for repeats guesses
        char input;
        unsigned tries = 0;  	// for counting valid incorrect attempts
	char image[7][10] = {
		{' ', '_', '_', '_', '_', ' ', ' ', ' ', ' ', '\n' },
		{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n' },
		{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n' },
		{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n' },
		{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n' },
		{'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\n' },
		{'|', '_', '_', '_', '_', '_', '_', ' ', ' ', '\n' }
	};

        // initialize hidden and consumed arrays to an '_'
	memset(hidden,'_', strlen(tp));
	memset(consumed,'_', strlen(tp));

        // initialize tmp to a lower-case variant.
        for (int i = 0; i <= (int)(strlen(tp)); ++i) {
                tmp[i] = tolower(tp[i]);  // sets the tmp word to all lower
        }
        puts("");
        while(true) {
                counter = 0;
                counter_r = 0;
		clear_screen();
		printf("game %d. %d Win/%d Loss. Average score: %.2f\n",
                       game.games,
                       game.win,
                       game.lose,
                       ((float)game.score/(float)game.games));
		for (int row = 0; row < 7; row++)
		{
			for (int col = 0; col < 10; col++)
			{
				printf("%c", image[row][col]);
			}
			
		}

		if(tries == 6) {
                        printf("You lose!\n");
			printf("The word was: %s\n", tp);
                        game.lose++;
                        break;
                }

                printf("%d ", tries);

                for (size_t i = 0; i <= (strlen(tp) - 1); i++) {
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

                // check to see if a correct guess has already been guessed
                if(strchr(consumed, tolower(tmp_input))) {
                        counter_r++;
                }

                for(unsigned long i = 0; i <= (strlen(tp) - 1); i++) {
                        // check take letter-case into consideration
                        if (tolower(tmp_input) == tmp[i])  {
                                hidden[i] = tp[i];  // assign the original char
                                consumed[i] = tmp_input;
                                counter++;
                        }
                }

                // if wrong answer or already correctly guessed, add 1 to tries
                if(counter == 0 || counter_r != 0) {
                        tries++;
			switch (tries) {
			case 1:
				image[1][4] = 'O';
				break;
			case 2:
				image[2][4] = '|';
				break;
			case 3:
				image[2][3] = '/';
				break;
			case 4:
				image[2][5] = '\\';
				break;
			case 5:
				image[3][3] = '/';
				break;
			case 6:
				image[3][5] = '\\';
				break;
			default:
				break;
			}
                        game.score++;
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
	game.games++;
}

char input_val(char *tp)
{
        char line[MAX];
        char tmp_line[MAX];
        char tmp_target[MAX];

        fgets(line, sizeof(line), stdin);

        // convert both temp arrays to lower for comparisons
        for (int i = 0; i <= (int)(strlen(tp)); ++i) {
                tmp_target[i] = tolower(tp[i]);
        }

        for (int i = 0; i <= (int)(strlen(line)); ++i) {
                tmp_line[i] = tolower(line[i]);
        }

	//set last char to NULL instead of newline for better comparisons
	tmp_line[strlen(tmp_line) - 1] = '\0';

        if (strcmp(tmp_line, tmp_target) == 0) {
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

void set_stats(void)
{
        FILE *outfile;

        outfile = fopen("./.hangman", "w+");
        if(!outfile) {
                fprintf(stderr, "Unable to retrieve stats file\n");
        } else {
                fwrite(&game, sizeof(struct stats), 1, outfile);
                fclose(outfile);
        }
}


