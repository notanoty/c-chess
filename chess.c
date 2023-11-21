#include <stdio.h>
#include <stdlib.h>
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
    bool color;  // true for black, false for white
    int moveAmount; 
};

struct listCoords{
    int coords[2];
    struct listCoords *next;
};

int listLen(struct listCoords *head) {
    int count = 0;
    struct listCoords *current = head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}


void printMoveList(struct listCoords *moveList);
void displayBoardActions(struct piece board[BOARD_SIZE][BOARD_SIZE], struct listCoords* moveList);

// Function to free the memory allocated for the move list
void freeMoveList(struct listCoords *moveList) {
    while (moveList != NULL) {
        struct listCoords *temp = moveList;
        moveList = moveList->next;
        free(temp);
    }
}

struct listCoords* addMove(int x, int y, struct listCoords* previousList) {
    struct listCoords* newElement = malloc(sizeof(struct listCoords)); 
    newElement->coords[0] = x;
    newElement->coords[1] = y;
    newElement->next = previousList;
    return newElement;
}


bool inCoordList(int x, int y, struct listCoords *head) {
    struct listCoords *current = head;

    while (current != NULL) {
        if (current->coords[0] == x && current->coords[1] == y) {
            return true; // Coordinates found in the list
        }
        current = current->next;
    }

    return false; // Coordinates not found in the list
}

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
    for(int i = 0; i < BOARD_SIZE; i++){
        initialBoard[0][i].moveAmount = 0;
        initialBoard[1][i].moveAmount = 0;
        initialBoard[6][i].moveAmount = 0;
        initialBoard[7][i].moveAmount = 0;
    }

    // Copy the initial board to the actual board
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = initialBoard[i][j];
        }
    }
}



bool isValidPosition(int row, int col) {
    return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE);
}

struct listCoords* knightMoves(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE])
{
    struct listCoords *moveList = NULL; 
    
    int xOffsets[] = {-1, -1,  1, 1, -2,  2, -2, 2};
    int yOffsets[] = {-2,  2, -2, 2, -1, -1,  1, 1};

    for (int i = 0; i < 8; ++i) {
        int newRow = y + xOffsets[i];
        int newCol = x + yOffsets[i];
        if (isValidPosition(newRow, newCol) && (board[newRow][newCol].symbol == 0 ||  board[newRow][newCol].color != board[y][x].color ) ) { //
            moveList = addMove(newRow, newCol, moveList);
        }
    }
}


void initializeThreatMap(int threatMap[BOARD_SIZE][BOARD_SIZE]){
    for(int i = 0; i < BOARD_SIZE; i++ ){
        for(int j = 0; j < BOARD_SIZE; j++){
            threatMap[i][j] = 0;
        }
    }
}

void createThreatMap(struct piece board[BOARD_SIZE][BOARD_SIZE], int threatMap[BOARD_SIZE][BOARD_SIZE], bool color) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].symbol != 0 && board[i][j].color == color) {
                struct listCoords *movesSave = knightMoves(j, i, board);
                struct listCoords *moves = movesSave; 
                int movesLen = listLen(moves);
                printf("len = %d\n", movesLen);

                printMoveList(moves);
                displayBoardActions(board, moves);

                for (int iteration = 0; iteration < movesLen; iteration++) {
                    int x = moves->coords[1];
                    int y = moves->coords[0];
                    threatMap[y][x] = 1;
                    moves = moves->next;
                }
                freeMoveList(movesSave);  
            }
        }
    }
}





// Function to display the chess board
void displayBoard(struct piece board[BOARD_SIZE][BOARD_SIZE]) {

    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%d ", 8 - i);
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
    printf("  ");
    for(int i = 0; i  < BOARD_SIZE; ++i){
      printf("  %c ", 'a' + i);
    }
    printf("\n");
}

void displayBoardActions(struct piece board[BOARD_SIZE][BOARD_SIZE], struct listCoords* moveList)
{

    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%d ", 8 - i);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("|");
            if(inCoordList( i, j, moveList )){
                if (board[i][j].symbol == 0){
                  printf("%s%c%c%c%s",  GREEN, 178, 178, 178, WHITE);
                }
                else{
                  printf(" %s%c%s ", GREEN, board[i][j].symbol, WHITE);
                }
            }
            else{
                if (board[i][j].symbol == 0){
                  printf("%s%c%c%c%s", ( (i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)  ) ? BLACK : WHITE, 178, 178, 178, WHITE);
                }
                else{
                  printf(" %s%c%s ", (board[i][j].color) ? BLUE:WHITE, board[i][j].symbol, WHITE);
                }
            }

            
        }
        printf("|\n");
    }
    printf("  ");
    for(int i = 0; i  < BOARD_SIZE; ++i){
      printf("  %c ", 'a' + i);
    }
    printf("\n");
}

void displayBoardDebug(struct piece board[BOARD_SIZE][BOARD_SIZE]) {

    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%d ", i);
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
    printf("  ");
    for(int i = 0; i  < BOARD_SIZE; ++i){
      printf("  %d ", i);
    }
    printf("\n");
}

void displayBoardMoves(struct piece board[BOARD_SIZE][BOARD_SIZE]) {

    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%d ", 8 - i);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("|");
            if (board[i][j].symbol == 0){
              printf("%s%c%c%c%s", ( (i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)  ) ? BLACK : WHITE, 178, 178, 178, WHITE);
            }
            else{
              printf(" %s%d%s ", (board[i][j].color) ? BLUE:WHITE, board[i][j].moveAmount, WHITE);
            }
            
        }
        printf("|\n");
    }
    printf("  ");
    for(int i = 0; i  < BOARD_SIZE; ++i){
      printf("  %c ", 'a' + i);
    }
    printf("\n");
}

void printMoveList(struct listCoords *moveList) {
    struct listCoords *current = moveList;

    printf("List of Moves:\n");
    while (current != NULL) {
        printf("(%d, %d) ", current->coords[0], current->coords[1]);
        current = current->next;
    }
    printf("\n");
}

int main() {
    // Declare the chess board using the piece structure
    struct piece chessBoard[BOARD_SIZE][BOARD_SIZE];

    // Initialize the chess board
    initializeBoard(chessBoard);

    int threatMap[BOARD_SIZE][BOARD_SIZE];
    initializeThreatMap(threatMap);
    
    for(int i = 0; i < BOARD_SIZE; i++ ){
        for(int j = 0; j < BOARD_SIZE; j++){
            printf(" %d ", threatMap[i][j]);
        }
        printf("\n");
    }

    createThreatMap(chessBoard, threatMap, 1);
    
    for(int i = 0; i < BOARD_SIZE; i++ ){
        for(int j = 0; j < BOARD_SIZE; j++){
            printf(" %d ", threatMap[i][j]);
        }
        printf("\n");
    }

    //printf("end\n");


    // Display the initial chess board
    //printf("Chess Board:\n");
    //displayBoardDebug(chessBoard);

    //struct listCoords *moves =  knightMoves( 2, 4, chessBoard);
    //printMoveList(moves);
    //~displayBoardActions(chessBoard, moves);
    // Add your code to make moves and update the board as needed
    printf(WHITE);
    return 0;
}
