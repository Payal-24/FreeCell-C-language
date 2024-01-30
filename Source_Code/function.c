
#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define FOREGROUND_BLACK 0
#define BACKGROUND_WHITE BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
char *suits[] = {"\u2665", "\u2666", "\u2663", "\u2660"};
char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
/*"\u2665" représente le symbole du cœur (Heart),
"\u2666" représente le symbole du carreau (Diamond),
"\u2663" représente le symbole du trèfle (Club),
"\u2660" représente le symbole du pique (Spade).*/

void initializePile(Pile *pile) {
    pile->top = NULL;
    pile->size = 0;
}

void initializeZone(Zone *zone, int numColumns) {
    zone->num_columns = numColumns;
    zone->pile = (Pile **)malloc(numColumns * sizeof(Pile *));

    for (int i = 0; i < numColumns; i++) {
        zone->pile[i] = (Pile *)malloc(sizeof(Pile));
        initializePile(zone->pile[i]);
    }
}
Card *createCard(int SuitIndex, int RankIndex) {
    Card *RCard = (Card *)malloc(sizeof(Card));
    RCard->rank = ranks[RankIndex];
    RCard->suit = suits[SuitIndex];
    RCard->next=NULL;
    RCard->previous=NULL;
    return RCard;
}

void pushcard(Pile **P, Card *C) {
    if ((*P)->top != NULL) {
        C->next = (*P)->top;
        (*P)->top->previous = C;
        (*P)->top = C;
    } else {
        C->next = (*P)->top;
        C->previous = NULL;
        (*P)->top = C;
    }
    // la mise a jour de size
    (*P)->size++;
}
Card *Peek(Pile *P) {
    return P->top;
}

void Pop(Pile *P) {
    if (P->top != NULL) {
        P->top = P->top->next;

        // Update the previous pointer of the new top card
        if (P->top != NULL) {
            P->top->previous = NULL;
        }

        P->size--;
    }
}

void swapCards(Card **deck, int i, int j) {
    Card *temp = deck[i];
    deck[i] = deck[j];
    deck[j] = temp;
}

void shuffleDeck(Card **deck, int deckSize) {
    for (int i = deckSize - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swapCards(deck, i, j);
    }
}

Card **generateFullDeck() {
    Card **allCards = (Card **)malloc(52 * sizeof(Card *));
    int i = 0;

    // Generate 52 cards (13 of each suit)
    for (int suitIndex = 0; suitIndex < 4; suitIndex++) {
        for (int rankIndex = 0; rankIndex < 13; rankIndex++) {
            Card *newCard = createCard(suitIndex, rankIndex);
            allCards[i++] = newCard;
        }
    }

    // Shuffle the deck
    shuffleDeck(allCards, 52);

    return allCards;
}

void FillDeck(Zone *Deck) {
    Card **myDeck = generateFullDeck();
    int i = 0;

    // Fill columns col1 to col4 with 7 cards each
    for (int col = 0; col < 4; col++) {
        for (int j = 0; j < 7; j++) {
            pushcard(&(Deck->pile[col]), myDeck[i++]);
        }
    }

    // Fill columns col5 to col8 with 6 cards each
    for (int col = 4; col < 8; col++) {
        for (int j = 0; j < 6; j++) {
            pushcard(&(Deck->pile[col]), myDeck[i++]);
        }
    }
}
//function to initialize a new game
void initializeGame(Zone *Deck, Zone *Temp, Zone *Goal) {
    // Clear Temp and Goal zones
    initializeZone(Temp, 4);
    initializeZone(Goal, 4);
    initializeZone(Deck,8);
    // fill the Deck
    FillDeck(Deck);
}

void SetCardColorsBySuit(const char *suit) {// Set the card color based on its suit
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set background color to white
    SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);

    // Set font color based on suit
    if (strcmp(suit, "\u2665") == 0 || strcmp(suit, "\u2666") == 0) {
        // Hearts or Diamonds, set font color to red
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | BACKGROUND_WHITE);
    } else {
        // Clubs or Spades, set font color to black
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLACK | BACKGROUND_WHITE);
    }
}

void Print_Card(Card *C) {
    if (C != NULL) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Set colors based on card suit
        SetCardColorsBySuit(C->suit);

        // Print the card
        if(strcmp(C->rank,"10")==0)
            printf("%s     %s", C->rank, C->suit);
        else
            printf("%s      %s", C->rank, C->suit);

        // Reset colors to default
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}
void printPile(Pile *P) {
    if(P->top==NULL){
        return;
    }
    Card *current = P->top;
    // traverser to the last element
    while(current->next!=NULL) current=current->next;

    while (current != NULL) {
        Print_Card(current);
        printf(" ");
        current = current->previous;
    }
    printf("\n");
}
//Functions to display each zone
void Print_Deck(Zone *Deck) {
    int i = 0;
    printf("--------------------------------------------------Deck--------------------------------------------------\n\n");
    while (i < 8) {
            printf("Column %d : ",i+1);
            printPile(Deck->pile[i]);
            printf("\n");
            i++;
    }
}

void Print_Temp(Zone *Temp){
    int i=0;
    printf("--------------------------------------------------Temp--------------------------------------------------\n\n");
    while(i<4){
        if(Temp->pile[i]->top==NULL){
            printf("Empty\t\t\t\t");
        }else{
            Card *C=Temp->pile[i]->top;
            Print_Card(C);
            printf("\t\t");
        }
        i++;
    }
}

void Print_Goal(Zone *Goal){
    int i=0;
    printf("--------------------------------------------------Goal--------------------------------------------------\n\n");
    while(i<4){
        if(Goal->pile[i]->size==0){
            printf("Empty\t\t\t\t");
        }else if(Goal->pile[i]->size==12){
            printf("//FULL//t\t\t\t");
        }
        else{
            Print_Card(Goal->pile[i]->top);
            printf("\t\t");
        }
        i++;
    }
}
// Function to get valid user input within a range
int getValidChoice(int min, int max) {
    int choice;
    while (1) {
        printf("Enter a number between %d and %d: ", min, max);
        if (scanf("%d", &choice) == 1) {
            // Input is an integer

            if (choice >= min && choice <= max) {
                // Input is within the valid range
                return choice;
            } else {
                printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
            }
        } else {
            // Input is not an integer, clear the input buffer
            while (getchar() != '\n');
            printf("Invalid input. Please enter a valid integer.\n");
        }
    }
}
//function to check if a move to Deck(Table) Zone is valid
bool isMovetoTableValid(Pile *sourcePile, Pile *destinationPile) {
    // Check if the source pile is not empty
    if (sourcePile->top == NULL) {
        return false; // Source pile is empty, invalid move
    }

    // Check if the destination pile is empty
    if (destinationPile->top == NULL) {
        return true; // Valid move to an empty pile
    }

    // Get the cards from the top of both piles
    Card *sourceCard = sourcePile->top;
    Card *destinationCard = destinationPile->top;

    // Check if the ranks are in descending order and the colors are opposite
    if (
        ((strcmp(sourceCard->rank, "Q") == 0 && strcmp(destinationCard->rank, "K") == 0) ||
        (strcmp(sourceCard->rank, "J") == 0 && strcmp(destinationCard->rank, "Q") == 0) ||
        (strcmp(sourceCard->rank, "10") == 0 && strcmp(destinationCard->rank, "J") == 0) ||
        (strcmp(sourceCard->rank, "A") == 0 && strcmp(destinationCard->rank, "2") == 0) ||
        (atoi(sourceCard->rank) == atoi(destinationCard->rank) - 1 )) && (// Check for consecutive ranks
         ((strcmp(sourceCard->suit, "\u2665") == 0 || strcmp(sourceCard->suit, "\u2666") == 0) &&
          (strcmp(destinationCard->suit, "\u2663") == 0 || strcmp(destinationCard->suit, "\u2660") == 0)) ||
         ((strcmp(sourceCard->suit, "\u2663") == 0 || strcmp(sourceCard->suit, "\u2660") == 0) &&
          (strcmp(destinationCard->suit, "\u2665") == 0 || strcmp(destinationCard->suit, "\u2666") == 0)))) {
        return true; // Valid move
    } else {
        return false; // Invalid move
    }
}
//function to check if a move to Goal Zone is valid
bool isMovetoGoalValid(Pile *sourcePile, Pile *destinationPile) {
    // Check if the source pile is not empty
    if (sourcePile->top == NULL) {
        return false; // Source pile is empty, invalid move
    }

    // Check if the destination pile is empty
    if (destinationPile->top == NULL) {
        return true; // Valid move to an empty pile
    }

    // Get the cards from the top of both piles
    Card *sourceCard = sourcePile->top;
    Card *destinationCard = destinationPile->top;
    // Check if the ranks are in descending order and the colors are opposite
    if (
    (strcmp(sourceCard->rank, "K") == 0 && strcmp(destinationCard->rank, "Q" ) == 0 &&strcmp(sourceCard->suit, destinationCard->suit) == 0) ||
    (strcmp(sourceCard->rank, "Q") == 0 && strcmp(destinationCard->rank, "J") == 0 &&strcmp(sourceCard->suit, destinationCard->suit) == 0) ||
    (strcmp(sourceCard->rank, "J") == 0 && strcmp(destinationCard->rank, "10") == 0 &&strcmp(sourceCard->suit, destinationCard->suit) == 0) ||
    (strcmp(sourceCard->rank, "2") == 0 && strcmp(destinationCard->rank, "A") == 0 &&strcmp(sourceCard->suit, destinationCard->suit) == 0) ||
    (atoi(sourceCard->rank) == atoi(destinationCard->rank) + 1 && strcmp(sourceCard->suit, destinationCard->suit) == 0)
     ) {
    return true; // Valid move
} else {
    return false; // Invalid move
}


}

// Function to move a card from one column to another in the deck
void moveCardInDeck(Zone *Deck) {
    int sourceColumn, destinationColumn;
    printf("Deck");
    sourceColumn=getValidChoice(1,8);
    printf("Deck");
    destinationColumn=getValidChoice(1,8);
    // Check if the move is valid
    if (!isMovetoTableValid(Deck->pile[sourceColumn - 1], Deck->pile[destinationColumn - 1])) {
        printf("Invalid move\n");
        return;
    }
    Pile *sourcePile = Deck->pile[sourceColumn - 1];
    Pile *destinationPile = Deck->pile[destinationColumn - 1];
    Card *card=Peek(sourcePile);
    Pop(sourcePile);
    // Move the card from the source column to the destination column
    pushcard(&destinationPile,card);
    printf("Move successful\n");
}

void Move_Deck_To_Temp(Zone *Deck,Zone *Temp){

    int sourceColumn, destinationColumn;
    printf("Deck :");
    sourceColumn=getValidChoice(1,8);
    printf("Temp :");
    destinationColumn=getValidChoice(1,4);
    if(Temp->pile[destinationColumn-1]->top!=NULL){
        printf("Invalid Move \n");
        return;
    }
    Pile *sourcePile = Deck->pile[sourceColumn - 1];
    Pile *destinationPile = Temp->pile[destinationColumn - 1];
    Card *card=Peek(sourcePile);
    Pop(sourcePile);
    // Move the card from the source column to the destination column
    pushcard(&destinationPile,card);
    printf("Move successful\n");
}

void move_Deck_To_Goal(Zone *Deck,Zone *Goal){
  int sourceColumn, destinationColumn;
    printf("Deck :");
    sourceColumn=getValidChoice(1,8);
    printf("Goal :");
    destinationColumn=getValidChoice(1,4);
    if(!isMovetoGoalValid(Deck->pile[sourceColumn - 1],Goal->pile[destinationColumn - 1])){
        printf("Invalid Move \n");
        return;
    }
    if(Goal->pile[destinationColumn-1]->size==13){
        printf("already Full");
        return;
    }
    if ((Goal->pile[destinationColumn - 1]->top == NULL && Peek(Deck->pile[sourceColumn - 1])->rank[0] != 'A')) {
        printf("Invalid move. Ensure source pile is not empty, and only an 'A' card can be moved to an empty pile in the Goal.\n");
        return;
    }
    Pile *sourcePile = Deck->pile[sourceColumn - 1];
    Pile *destinationPile = Goal->pile[destinationColumn - 1];
    Card *card=Peek(sourcePile);
    Pop(sourcePile);
    // Move the card from the source column to the destination column
    pushcard(&destinationPile,card);
    printf("Move successful\n");
}
void Move_Temp_To_Deck(Zone *Temp, Zone *Deck) {
    int sourceColumn,destinationColumn;
    printf("Temp :");
    sourceColumn=getValidChoice(1,4);
    printf("Deck :");
    destinationColumn=getValidChoice(1,8);
    // Check if the move is valid
    if (!isMovetoTableValid(Temp->pile[sourceColumn - 1], Deck->pile[destinationColumn - 1])) {
        printf("Invalid move\n");
        return;
    }
    Pile *sourcePile = Temp->pile[sourceColumn - 1];
    Pile *destinationPile = Deck->pile[destinationColumn - 1];
    Card *card = Peek(sourcePile);
    Temp->pile[sourceColumn - 1]->top=NULL;//Function pop won't work properly in this case
    // Move the card from the source column to the destination column
    pushcard(&destinationPile, card);
    printf("Move successful\n");
}
void Move_Temp_To_Goal(Zone *Temp, Zone *Goal) {
    int sourceColumn, destinationColumn;
    printf("Temp");
    sourceColumn=getValidChoice(1,4);
    printf("Goal");
    destinationColumn=getValidChoice(1,4);
    if(Goal->pile[destinationColumn-1]->size==13){
        printf("already Full");
        return;
    }
    if ((Goal->pile[destinationColumn - 1]->top == NULL && Peek(Temp->pile[sourceColumn - 1])->rank[0] != 'A')) {
        printf("Invalid move. Ensure source pile is not empty, and only an 'A' card can be moved to an empty pile in the Goal.\n");
        return;
    }
    // Check if the move is valid
    if (!isMovetoGoalValid(Temp->pile[sourceColumn - 1], Goal->pile[destinationColumn - 1])) {
        printf("Invalid move\n");
        return;
    }
    Pile *sourcePile = Temp->pile[sourceColumn - 1];
    Pile *destinationPile = Goal->pile[destinationColumn - 1];
    Card *card = Peek(sourcePile);
    Temp->pile[sourceColumn - 1]->top=NULL;//Function pop won't work properly in this case
    // Move the card from the source column to the destination column
    pushcard(&destinationPile, card);
    printf("Move successful\n");
}
// Function to check if the player have won
bool checkwinner(Zone *Goal){
    int i=0;
    while(i<4){
        if(Goal->pile[i]->size < 12){//if a column in goal is not full thats means the game is not completed
            return false;
        }
        i++;
    }
    return true;
}
bool checkBlocksituation(Zone *Deck, Zone *Temp, Zone *Goal) {
    int i, j;

    // check if Temp is Full
    for (i = 0; i < 4; i++) {
        if (Temp->pile[i]->top == NULL) {
            return false;
        }
    }

    // check if there is a valid move between Temp To deck
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
            if (isMovetoTableValid(Temp->pile[i], Deck->pile[j])) {
                return false;
            }
        }
    }

    // check if there is a valid move between Temp To Goal
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (isMovetoGoalValid(Temp->pile[i], Goal->pile[j])) {
                return false;
            }
        }
    }

    // check if there is a valid move between Deck to Goal
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 4; j++) {
            if (isMovetoGoalValid(Deck->pile[i], Goal->pile[j])) {
                return false;
            }
        }
    }

    // check if there is a valid move between Temp To Goal
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (isMovetoGoalValid(Temp->pile[i], Goal->pile[j])) {
                return false;
            }
        }
    }

    // check if there is a valid move in the deck's columns
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (i != j && isMovetoTableValid(Deck->pile[i], Deck->pile[j])) {
                return false;
            }
        }
    }

    // If none of the conditions were met, the situation is considered valid
    return true;
}

