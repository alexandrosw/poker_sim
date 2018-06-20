/* Poker Simulator v0.1
	Created by Alexandros Williams
	First release (0.1): 20.6.2018
	Last release (0.1): 20.6.2018

	Current abilities:
	Generates a texas hold-em poker board state

	Future abilities:
	Recognition of particular hands, winners
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/****************************CONSTANTS & STRUCTURES****************************/

// Basic game constants
#define NUM_PLAYERS 4
#define HAND_SIZE 2
#define BOARD_SIZE 5

// Related to suit/number recognition in random card generation
#define H 0
#define D 1
#define S 2
#define C 3

#define A 1
#define J 11
#define Q 12
#define K 13

// Holds card information
typedef struct{
	char number[3];									   // Eg. 2-10 or A, J, K, Q
	char suit;						  // H(earts), D(iamonds), C(lubs), S(pades)
} card;
typedef card hand[2];
typedef card board[5];


/****************************FUNCTION PROTOTYPES*******************************/

void hand_gen(card *used_cards, int *used_cards_len, hand ret_hand);
void board_gen(card *used_cards, int *used_cards_len, board ret_board);
void card_gen(card *ret);
int card_in_arr(card curr, card *arr, int arr_len);
int card_is_card(card card1, card card2);
void itoa(int num, char* ret_str, int base);
void printcard(card subj, int newline, int space);
void printcardarr(card *arr, int arrlen);

// Currently prints a board state
int main(int argc, char **argv){
	int seed = time(NULL);
	srand(seed);

	// Generate hands
	card used_cards[52];
	int used_cards_len = 0, player;
	hand hands[NUM_PLAYERS];
	for(player = 0; player < NUM_PLAYERS; player++){
		hand_gen(used_cards, &used_cards_len, hands[player]);
	}


	// Generate board
	card board[5];
	board_gen(used_cards, &used_cards_len, board);

	// Print hand contents for each player
	int card_num;
	for(player = 0; player < NUM_PLAYERS; player++){
		printf("Player %d hand: ", player);
		for(card_num = 0; card_num < HAND_SIZE; card_num++){
			printcard(hands[player][card_num], 0, 1);
		}
		printf("\n");
	}

	// Print board contents
	printf("Board: ");
	for(card_num = 0; card_num < BOARD_SIZE; card_num++){
		printcard(board[card_num], 0, 1);
	}
	printf("\n");
}

// Generates a random hand from the remaining cards
void hand_gen(card *used_cards, int *used_cards_len, hand ret_hand){
	int num_card;
	card curr;
	strcpy(curr.number, "  ");

	/* For each player, generate random cards until a card not already used is 
		generated, then add to the hand and the used cards list */
	for(num_card = 0; num_card < HAND_SIZE; num_card++){
		while(1){
			card_gen(&curr);
			if (!(card_in_arr(curr, used_cards, *used_cards_len))){
				memcpy(&ret_hand[num_card], &curr, sizeof(curr));
				memcpy(&used_cards[*used_cards_len], &curr, sizeof(curr));
				(*used_cards_len)++;
				break;
			}
		}
		
	}
}

// Generates a random board from the remaining cards
void board_gen(card *used_cards, int *used_cards_len, board ret_board){
	int num_card;
	card curr;
	strcpy(curr.number, "  ");

	/* Similar to hand gen, generate random cards until a card not already used
		is generated, then add to the board and the used cards list */
	for(num_card = 0; num_card < BOARD_SIZE; num_card++){
		while(1){
			card_gen(&curr);
			if (!(card_in_arr(curr, used_cards, *used_cards_len))){
				memcpy(&ret_board[num_card], &curr, sizeof(curr));
				memcpy(&used_cards[*used_cards_len], &curr, sizeof(curr));
				(*used_cards_len)++;
				break;
			}
		}
	}
}

// Generates a random card
/* This whole function would be much better with an associative array but not
   sure how to do that yet */
void card_gen(card *ret){
	int suit_num, card_num;

	// Generate a random number for the suit and the number of the card
	suit_num = rand() % 4;
	card_num = 1 + rand() % 13;

	/* Intepret the two generated numbers into a card descriptor 
		(eg AS - ace of spades). */
	if(suit_num == H)
		ret->suit = 'H';
	else if(suit_num == D)
		ret->suit = 'D';
	else if(suit_num == S)
		ret->suit = 'S';
	else if(suit_num == C)
		ret->suit = 'C';

	if(card_num  == A)
		strcpy(ret->number, "A");
	else if(card_num == J)
		strcpy(ret->number, "J");
	else if(card_num == Q)
		strcpy(ret->number, "Q");
	else if(card_num == K)
		strcpy(ret->number, "K");
	else{
		char scard_num[3] = "";
		itoa(card_num, scard_num, 10);
		strcpy(ret->number, scard_num);
	}
}

// Figures out if a particular card is in an array of cards
int card_in_arr(card curr, card *arr, int arr_len){
	int pos;

	for(pos = 0; pos < arr_len; pos++){
		if (card_is_card(curr, arr[pos])){
			return 1;
		}
	}

	return 0;
}

// Compares two cards for equality
int card_is_card(card card1, card card2){
	return (!(strcmp(card1.number, card2.number)) && (card1.suit == card2.suit));
}

// Converts an unsigned integer into a string
void itoa(int num, char* ret_str, int base){
	int div = 1, pos = 0;
	while(num >= div)
		div *= base;

	div /= base;

	while (div){
		ret_str[pos] = '0' + num / div;
		num -= num / div * div;
		div /= base;
		pos++;
	}
}

// Prints a card
void printcard(card subj, int newline, int space){
	printf("%s%c", subj.number, subj.suit);
	if (space)
		printf(" ");
	if (newline)
		printf("\n");
}

// Prints an array of cards
void printcardarr(card *arr, int arrlen){
	int element;
	printf("array: ");
	for(element = 0; element < arrlen; element++){
		printcard(arr[element], 0, 1);
	}
	printf("\n");
}