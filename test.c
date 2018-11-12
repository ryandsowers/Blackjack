// ----------------------------------------------------------------------
// file: test.c
//
// Description: This is a simple program to test the card.c module.
//
// Created: 2017-04-18 (P. Clark)
//
// Modifications:
// 2017-10-31 (P. Clark)
//     Added a call to card_init.
//     Added some analysis of the information returned by card_get.
// 2017-11-03 (P. Clark)
//     Fixed a problem where it wasn't catching too many patterns of a
//     particular suit. Added a lot of additional calls to card_get to
//     catch problems that only show after a couple of shuffles.
// ----------------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "card.h"
#include "common.h"

#define NUM_SUITS 4
#define CARDS_PER_SUIT 13
#define NUM_OTHER_CALLS 200

unsigned int Seen_suit[NUM_SUITS+1];
unsigned int Seen_pattern[CARDS_PER_SUIT+1];
unsigned int Num_bad_patterns = 0;
unsigned int Num_bad_suits = 0;
unsigned int First_suit = 0;
unsigned int First_pattern = 0;
unsigned int Num_cards = 0;



void display_card(unsigned char suit, unsigned char pattern)
{
        // Print human-readable version of the pattern
        if (pattern == ACE) {
                printf("Ace   of ");
        } else if (pattern > ACE && pattern < JACK) {
                printf("%-5d of ", pattern);
        } else if (pattern == JACK) {
                printf("Jack  of ");
        } else if (pattern == QUEEN) {
                printf("Queen of ");
        } else if (pattern == KING) {
                printf("King  of ");
        } else {
                printf("INVALID pattern: %d, ", pattern);
                ++Num_bad_patterns;
        }

        // Print human-readable version of the suit
        switch (suit) {
                case CLUBS:
                        printf("Clubs\n");
                        break;
                case HEARTS:
                        printf("Hearts\n");
                        break;
                case SPADES:
                        printf("Spades\n");
                        break;
                case DIAMONDS:
                        printf("Diamonds\n");
                        break;
                default:
                        printf("INVALID suit: %d\n", suit);
                        ++Num_bad_suits;
                        break;
        }
}



int main(int argc, const char *argv[])
{
        unsigned int i;
        unsigned char suit;
        unsigned char pattern;
        bool all_good = true;

        // initialize the card module
        card_init();

        // initialize stat's
        for (i=1; i < NUM_SUITS+1; ++i) {
                Seen_suit[i] = 0;
        }
        for (i=1; i < CARDS_PER_SUIT+1; ++i) {
                Seen_pattern[i] = 0;
        }

        // Call card_get 52 times to see what we get
        for (i=0; i < CARDS_PER_DECK; ++i) {
                // pick a card...any card
                card_get(&suit, &pattern);
                Num_cards++;
                if (i==0) {
                        // remember the first card
                        First_suit = suit;
                        First_pattern = pattern;
                }

                // show the card
                display_card(suit, pattern);

                // Update stats
                Seen_suit[suit]++;
                Seen_pattern[pattern]++;
        }
        printf("--------------------------------\n");


        // pick a 53rd card and compare with the first card
        card_get(&suit, &pattern);
        Num_cards++;
        if ((suit == First_suit) && (pattern == First_pattern)) {
                printf("-Bad: your first card is the same as your 53rd card: ");
                display_card(suit, pattern);
        } else {
                printf("-Good: 1st and 53rd cards are different\n");
        }

        // Did we get a pattern out of the proper range?
        if (Num_bad_patterns > 0) {
                printf("-Bad: %d bad patterns returned\n", Num_bad_patterns);
        } else {
                printf("-Good: No bad patterns returned\n");
        }

        // Did we get a suit out of the proper range?
        if (Num_bad_suits > 0 ) {
                printf("-Bad: %d bad suits returned\n", Num_bad_suits);
        } else {
                printf("-Good: No bad suits returned\n");
        }

        // Did we see 13 of each suit?
        for (i=1; i < NUM_SUITS+1; ++i) {
                if (Seen_suit[i] != CARDS_PER_SUIT) {
                        printf("-Bad: suit %d had %d cards instead of %d\n",
                                i, Seen_suit[i], CARDS_PER_SUIT);
                        all_good = false;
                }
        }
        if (all_good) {
                printf("-Good: %d of every suit seen\n", CARDS_PER_SUIT);
        }

        // Did we see 4 of each pattern?
        all_good = TRUE;
        for (i=1; i < CARDS_PER_SUIT+1; ++i) {
                if (Seen_pattern[i] != NUM_SUITS) {
                        printf("-Bad: pattern %d had %d cards instead of %d\n",
                                i, Seen_pattern[i], NUM_SUITS);
                        all_good = false;
                }
        }
        if (all_good) {
                printf("-Good: %d of every pattern seen\n", NUM_SUITS);
        }

        // Try calling card_get a bunch more times to verify all is well
        all_good = true;
        for (i=0; i<NUM_OTHER_CALLS; ++i) {
                card_get(&suit, &pattern);
                Num_cards++;
                if ((suit==0) || (suit>NUM_SUITS)) {
                        printf("-Bad suit seen on deal %d\n", Num_cards);
                        all_good = false;
                }
                if ((pattern==0) || (pattern>CARDS_PER_SUIT)) {
                        printf("-Bad pattern seen on deal %d\n", Num_cards);
                        all_good = false;
                }
        }
        if (all_good) {
                printf("-Good: no bad cards after %d more cards\n", NUM_OTHER_CALLS);
        }

        return 0;
}

// end of test.c
