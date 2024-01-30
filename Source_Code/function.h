
#ifndef functions.H
#define functions.H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define FOREGROUND_BLACK 0
#define BACKGROUND_WHITE BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
#include <stdbool.h>

typedef struct Card {
    char *suit;
    char *rank;
    struct Card *next;
    struct Card *previous;
} Card;

typedef struct Pile {
    Card *top;
    int size;
} Pile;

typedef struct Zone {
    Pile **pile;
    int num_columns;
} Zone;

void initializePile(Pile *pile);

void initializeZone(Zone *zone, int numColumns);

Card *createCard(int SuitIndex, int RankIndex);

void pushcard(Pile **P, Card *C);

Card *Peek(Pile *P);

void Pop(Pile *P);

void swapCards(Card **deck, int i, int j);

void shuffleDeck(Card **deck, int deckSize);

Card **generateFullDeck();

void FillDeck(Zone *Deck);

void initializeGame(Zone *Deck, Zone *Temp, Zone *Goal);

void SetCardColorsBySuit(const char *suit);

void Print_Card(Card *C);

void printPile(Pile *P);

void Print_Deck(Zone *Deck);

void Print_Temp(Zone *Temp);

void Print_Goal(Zone *Goal);

bool isMovetoTableValid(Pile *sourcePile, Pile *destinationPile);

bool isMovetoGoalValid(Pile *sourcePile, Pile *destinationPile);

int getValidChoice(int min, int max);

void moveCardInDeck(Zone *Deck);

void Move_Deck_To_Temp(Zone *Deck, Zone *Temp);

void move_Deck_To_Goal(Zone *Deck, Zone *Goal);

void Move_Temp_To_Deck(Zone *Temp, Zone *Deck);

void Move_Temp_To_Goal(Zone *Temp, Zone *Goal);

bool checkwinner(Zone *Goal);

bool checkBlocksituation(Zone *Deck, Zone *Temp, Zone *Goal);

#endif /* FREECELL-FUNCTIONS-H */

