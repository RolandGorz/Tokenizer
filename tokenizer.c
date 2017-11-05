/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
	int sep[128];
	char *str;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts) {
	TokenizerT *group = malloc(sizeof(int) * 128 + sizeof(char) * strlen(ts));
	group->str = malloc(sizeof(char) * strlen(ts) + 1);
	int separ = 0;
	int buh = strlen(separators);
	while (separ < buh-1) {
		if (separators[separ] == '\\') {
			if (separators[separ+1] == 'n') {
				separators[separ] = '\n';
				separators[separ+1] = '\n';
			}
			else if (separators[separ+1] == 't') {
				separators[separ] = '\t';
				separators[separ+1] = '\t';	
			}
			else if (separators[separ+1] == 'v') {
				separators[separ] = '\v';
				separators[separ+1] = '\v';
			}
			else if (separators[separ+1] == 'b') {
				separators[separ] = '\b';
				separators[separ+1] = '\b';	
			}
			else if (separators[separ+1] == 'r') {
				separators[separ] = '\r';
				separators[separ+1] = '\r';	
			}
			else if (separators[separ+1] == 'f') {
				separators[separ] = '\f';
				separators[separ+1] = '\f';
			}
			else if (separators[separ+1] == 'a') {
				separators[separ] = '\a';
				separators[separ+1] = '\a';	
			}
			else if (separators[separ+1] == '"') {
				separators[separ] = '\"';
				separators[separ+1] = '\"';
			}
		}
		separ++;
	}
	int cmp = strlen(separators);
	cmp--;
	int fill = 0;
	while (fill != 128) { //intitialize the table
		group->sep[fill] = 0;
		fill++;
	}
	while (cmp >= 0) { //mark as 1 for characters that are in the table
		int ascii = separators[cmp];
		group->sep[ascii] = 1;
		cmp--;
	}
	int put = 0;
	cmp = 0; //make escape characters in string
	buh = strlen(ts);
	while (cmp < buh) {
		if (ts[0] == '\\') {
			if (ts[1] == '\\') {
				group->str[put] = ts[0];
				ts = ts + 2;
				cmp = cmp + 2;
			}
			else if (ts[1] == 'n') {
				group->str[put] = '\n';
				if (strlen(ts) > 2) {
					ts = ts + 2;
				}
				cmp = cmp + 2;
			}
			else if (ts[1] == 't') {
				group->str[put] = '\t';
				if (strlen(ts) > 2) {
					ts = ts + 2;
				}
				cmp = cmp + 2;
			}
			else if (ts[1] == 'v') {
				group->str[put] = '\v';
				if (strlen(ts) > 2) {
					ts = ts + 2;
				}
				cmp = cmp + 2;
			}
			else if (ts[1] == 'b') {
				group->str[put] = '\b';
				if (strlen(ts) > 2) {
					ts = ts + 2;
				}
				cmp = cmp + 2;
			}
			else if (ts[1] == 'r') {
				group->str[put] = '\r';
				if (strlen(ts) > 2) {
					ts = ts + 2;
				}
				cmp = cmp + 2;
			}
			else if (ts[1] == 'f') {
				group->str[put] = '\f';
				if (strlen(ts) > 2) {
					ts = ts + 2;
				}
				cmp = cmp + 2;
			}
			else if (ts[1] == 'a') {
				group->str[put] = '\a';
				if (strlen(ts) > 2) {
					ts = ts + 2;
				}
				cmp = cmp + 2;
			}
			else if (ts[1] == '"') {
				group->str[put] = '\"';
				if (strlen(ts) > 2) {
					ts = ts + 2;
				}
				cmp = cmp + 2;
			}
			else {
				group->str[put] = ts[0];
				cmp++;
				ts++;
			}
		}
		else {
			group->str[put] = ts[0];
			cmp++;
			ts++;
		}
		put++;
	}
  	return group;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {/*FREE THE MEMORY*/
	free(tk->str);
	free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {
	int amt = 0; //How much we went through the array
	char *copy = malloc(sizeof(char) * (strlen(tk->str)+20)); //Fixed when it was +1 check it again.
	strcpy(copy, tk->str); /*gotta make the escape character seperators into single characters*/
	int cmp = tk->str[amt];
	while (amt < strlen(tk->str)) {
		if (tk->sep[cmp] == 1)	{
			copy[amt] = '\0';
			if (amt != 0) {
				tk->str = tk->str + amt;
			}
			else {
				tk->str = tk->str + 1;
			}
			return copy;
		}
		else {
			amt++;
			cmp = tk->str[amt]; 
		}
	}
	tk->str[0] = '\0';
  	return copy;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.amt < strlen(tk->str)
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("ERROR");
		printf("\n");
		return 0;
	}
	TokenizerT *group = TKCreate(argv[1], argv[2]);
	char* cpy = group->str;
	while(strlen(group->str) != 0) {
		char *out = TKGetNextToken(group);
		if (strlen(out) != 0) {
			int go = strlen(out);
			int up = 0;
			while (up < go) {
				if (out[up] == '\n') {
					printf("[0x0a]");
					up++;
				}
				else if (out[up] == '\t') {
					printf("[0x09]");
					up++;
				}
				else if (out[up] == '\v') {
					printf("[0x0b]");
					up++;
				}
				else if (out[up] == '\b') {
					printf("[0x08]");
					up++;
				}
				else if (out[up] == '\r') {
					printf("[0x0d]");
					up++;
				}
				else if (out[up] == '\f') {
					printf("[0x0c]");
					up++;
				}
				else if (out[up] == '\a') {
					printf("[0x07]");
					up++;
				}
				else if (out[up] == '\\' && up != go - 1) {
					printf("[0x5c]");
					up++;
				}
				else if (out[up] == '\"') {
					printf("[0x22]");
					up++;
				}
				else if (out[up] == '\\' && up == go - 1) {
					up++;
				}
				else {
					printf("%c", out[up]);
					up++;
				}
			}
			printf("\n");
		}
		free(out);
	}
	group->str = cpy;
	TKDestroy(group);
  	return 0;
}
