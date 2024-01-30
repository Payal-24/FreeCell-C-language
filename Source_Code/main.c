#include "function.h"
int main() {
    // Set background color to green
    Zone *Deck = (Zone *)malloc(sizeof(Zone));
    Zone *Temp = (Zone *)malloc(sizeof(Zone));
    Zone *Goal = (Zone *)malloc(sizeof(Zone));
    srand(time(NULL));
    initializeGame(Deck,Temp,Goal);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(CP_UTF8);
    int choice;
    while (true) {
        // Clear the console screen
        system("cls");


        Print_Goal(Goal);
        printf("\n");
        Print_Temp(Temp);
        printf("\n");
        // Print the updated deck
        Print_Deck(Deck);

        printf("1-MoveCard_Deck_To_Deck || 2-MoveCard_Deck_To_Temp || 3-MoveCard_Deck_To_Goal || 4-Move_Temp_To_Goal ||");
        printf("5-Move_Temp_To_Deck || 6-Reset_Game || 7-Exit\n");
        // Use the getValidChoice function to get a valid input
        choice = getValidChoice(1, 7);

        switch (choice) {
            case 1:
                moveCardInDeck(Deck);
                break;
            case 2:
                Move_Deck_To_Temp(Deck, Temp);
                break;
            case 3:
                move_Deck_To_Goal(Deck, Goal);
                break;
            case 4:
                Move_Temp_To_Goal(Temp, Goal);
                break;
            case 5:
                Move_Temp_To_Deck(Temp, Deck);
                break;
            case 6:
                initializeGame(Deck, Temp, Goal);
                printf("Game reset successfully.\n");
                break;
            case 7:
                printf("Exiting the game.\n");
                exit(EXIT_SUCCESS);
        }
        if(checkwinner(Goal)){
            system("cls");
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | BACKGROUND_WHITE);
            printf("\n\n\n\n\n\n-----------------------------------Congratulation!!!! U won -----------------------------------------");
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            exit(EXIT_SUCCESS);
        }
        if(checkBlocksituation(Deck,Temp,Goal)){
            system("cls");
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | BACKGROUND_WHITE);
            printf("\n\n\n\n\n\n-----------------------------------Game Over!!!!-----------------------------------------");
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            exit(EXIT_SUCCESS);
        }
        // Add a delay to better visualize changes
        Sleep(2000);
    }

    return 0;
}



