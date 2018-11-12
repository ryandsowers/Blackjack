// ----------------------------------------------------------------------
// file: card.c
//
// Description: This file implements the CARD module. Its job is to 
//     create an interface for providing cards from a shuffled deck of
//     52 standing playing cards. Each call to card_get() will return
//     the top card in that shuffled deck. If all the cards get used, 
//     then the deck is invisibly (and unknowingly) reshuffled.
//
// Created: 2016-05-03 (P. Clark)
//
// Modifications:
// 2017-10-30 (P. Clark)
//     Added card_init().
// ----------------------------------------------------------------------
#include <stdlib.h>
#include <sys/times.h>
#include "card.h"
#include "common.h"
#include <errno.h>
#include <stdio.h>

#define NUM_SUITS 4
#define CARDS_PER_SUIT 13

static unsigned int Deck_shuffled = FALSE;
static int Random_card;
static int Num_deals;

unsigned char Suit_list[CARDS_PER_DECK];
unsigned char Pattern_list[CARDS_PER_DECK];
unsigned char Card_taken[CARDS_PER_DECK];


// This function must be called before the first call to card_get().
extern void card_init(void)
{
        // initialize the random number generator
        srandom(times(NULL));
}


// Get a card from the current deck.
// suit: This is interpreted as follows:
//     1 = Clubs
//     2 = Hearts
//     3 = Spades
//     4 = Diamonds
// pattern: This is interpreted as the 
//     1 = Ace
//     2..10 as expected
//     11 = Jack
//     12 = Queen
//     13 = King
extern void card_get(unsigned char *suit, unsigned char *pattern)
{
        if (!Deck_shuffled) {
                // shuffle the deck

                Num_deals = 0;

                // initialize suit list to hold 13 cards of each suit
                int i = 0;
                for (int k = 1; k <= NUM_SUITS; ++k) {                        
                        for (int j = 0; j < CARDS_PER_SUIT; ++j) {       
                                Suit_list[i] = k;
                                ++i;
                        }
                }

                // initialize pattern list to hold 4 cards of each pattern
                int n = 0;
                for (int l = 0; l < NUM_SUITS; ++l) {
                        for (int m = 1; m <= CARDS_PER_SUIT; ++m) {
                                Pattern_list[n] = m;
                                ++n;
                        }
                }

                // initialize Card_taken list to false
                for (int h = 0; h < CARDS_PER_DECK; ++h) {
                        Card_taken[h] = 0;
                }

                Deck_shuffled = TRUE;
        }

        // Figure out the next card to give out
        // Make sure you detect when all the cards have been dealt...

        // choose a random card that has not already been chosen
        do {
                Random_card = (random() % CARDS_PER_DECK);
        } while (Card_taken[Random_card] == 1);
        
        // assign the randomly chosen suit and pattern and mark the card as dealt
        if ((suit == NULL) || (pattern == NULL)) {
                exit(-1);
        } else {
                *suit = Suit_list[Random_card];
                *pattern = Pattern_list[Random_card];
                Card_taken[Random_card] = 1;
                Num_deals++;
        }

        // if a full deck has been dealt, flag the deck for shuffling
        if (Num_deals == CARDS_PER_DECK) {                
                Deck_shuffled = FALSE;
        }

} // card_get()


// end of card.c













