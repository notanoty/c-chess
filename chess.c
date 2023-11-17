#include <stdio.h>
#include <stdbool.h>

// Size of the chess board
#define BOARD_SIZE 8
#define GREEN "\033[0;32m"

#define BLUE "\033[0;34m"
#define BLACK "\033[0;30m"
#define WHITE "\033[0;37m"


// Structure to represent a chess piece
struct piece {
    char symbol;
    bool color;  // true for white, false for black
};

// Function to initialize the chess board
void initializeBoard(struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    // Set up the initial position of pieces on the board
    struct piece initialBoard[BOARD_SIZE][BOARD_SIZE] = {
        {{'R', true}, {'N', true}, {'B', true}, {'Q', true}, {'K', true}, {'B', true}, {'N', true}, {'R', true}},
        {{'P', true}, {'P', true}, {'P', true}, {'P', true}, {'P', true}, {'P', true}, {'P', true}, {'P', true}},
        {{ 0, false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}},
        {{ 0, false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}},
        {{ 0, false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}},
        {{ 0, false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}, { 0 , false}},
        {{'P', false}, {'P', false}, {'P', false}, {'P', false}, {'P', false}, {'P', false}, {'P', false}, {'P', false}},
        {{'R', false}, {'N', false}, {'B', false}, {'Q', false}, {'K', false}, {'B', false}, {'N', false}, {'R', false}}
    };

    // Copy the initial board to the actual board
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = initialBoard[i][j];
        }
    }
}

// Function to display the chess board
void displayBoard(struct piece board[BOARD_SIZE][BOARD_SIZE]) {

    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf(" %d ", 8 - i);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("|");
            if (board[i][j].symbol == 0){
              printf("%s%c%c%c%s", ( (i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)  ) ? BLACK : WHITE, 178, 178, 178, WHITE);
            }
            else{
              printf(" %s%c%s ", (board[i][j].color) ? BLUE:WHITE, board[i][j].symbol, WHITE);
            }
            
        }
        printf("|\n");
    }
    char *letters = "abcdefgh";
    printf("   ");
    for(int i = 0; i  < BOARD_SIZE; ++i){
      printf("  %c ", letters[i]);
    }
    printf("\n");
}

int main() {
    // Declare the chess board using the piece structure
    struct piece chessBoard[BOARD_SIZE][BOARD_SIZE];

    // Initialize the chess board
    initializeBoard(chessBoard);

    // Display the initial chess board
    printf("Chess Board:\n");
    displayBoard(chessBoard);

    // Add your code to make moves and update the board as needed
    printf(WHITE);
    return 0;
}
