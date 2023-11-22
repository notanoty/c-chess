#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Size of the chess board
#define BOARD_SIZE 8

#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
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
    int x;
    int y;
    struct listCoords *next;
};
int getX(struct listCoords *coords){
    return coords->x;
}

int getY(struct listCoords *coords){
    return coords->y;
}

char getSymbol(struct piece piece){
    return piece.symbol;
}

bool getColor(struct piece piece){
    return piece.color;
}
int getMoveAmount(struct piece piece){
    return piece.moveAmount;
}

struct listCoords* getLastCoordElement(struct listCoords* list){
    while(list->next != NULL){
        list = list->next;
    }
    return list;
}

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
    newElement->x = x;
    newElement->y = y;
    newElement->next = previousList;
    return newElement;
}
struct listCoords* connectLists(struct listCoords* list1, struct listCoords* list2) {
    if (list1 == NULL) {
        return list2;
    }

    struct listCoords* current = list1;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = list2;

    return list1;
}

bool inCoordList(int x, int y, struct listCoords *head) {
    struct listCoords *current = head;

    while (current != NULL) {
        if (current->x == x && current->y == y) {
            return true; // Coordinates found in the list
        }
        current = current->next;
    }

    return false; // Coordinates not found in the list
}
void initializeEmptyBoard(struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j].symbol = 0;
            board[i][j].color = false;  // Set the color to false (white) for all squares
        }
    }
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



bool isValidPosition(int col, int row) {
    return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE);
}

struct listCoords* getKnightMoves(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE])
{
    struct listCoords *moveList = NULL; 
    printf("(x = %d)(y = %d)\n", x, y);
    int xOffsets[] = {-1, -1,  1, 1, -2,  2, -2, 2};
    int yOffsets[] = {-2,  2, -2, 2, -1, -1,  1, 1};

    for (int i = 0; i < 8; ++i) {
        int newX = x + xOffsets[i];
        int newY = y + yOffsets[i];
        
        if (isValidPosition(newX, newY) && (getSymbol(board[newY][newX]) == 0 ||  getColor(board[newY][newX]) != getColor(board[y][x]) )) { //(board[newY][newX].symbol == 0 ||  board[newY][newX].color != board[y][x].color )

            moveList = addMove(newX, newY, moveList);
        }
    }
    return moveList;
}

struct listCoords* getPawnMoves(int x, int y,  struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 
   
    int direction = (getColor(board[y][x])) ? -1 : 1; // White pawns move upwards, black pawns move downwards   
    // Check one square forward
    int newY = y + direction;
    int newX = x;
    if (isValidPosition( newX, newY) && board[newY][newX].symbol == ' ') {
        moveList = addMove(newX, newY, moveList);
        // Check two squares forward for the initial move
        if ((y == 1 && board[y][x].color) || (y == 6 && !board[y][x].color)) {
            newY = x + 2 * direction;
            if (isValidPosition(newY, newX) && board[newY][newX].symbol == 0) {
                moveList = addMove(newX, newY, moveList);
            }
        }
    }   
    // Check capturing moves
    int captureCols[] = {x - 1, x + 1};
    for (int i = 0; i < 2; ++i) {
        newX = captureCols[i];
        if (isValidPosition(newX, newY) && board[newY][newX].symbol != 0 && board[newY][newX].color != board[y][x].color) {
            moveList = addMove(newX, newY, moveList);
        }
    }
    return moveList;
}

struct listCoords* getPawnThreats(int x, int y,  struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 
   
    int direction = (getColor(board[y][x])) ? -1 : 1; // White pawns move upwards, black pawns move downwards   
    // Check one square forward
    int newY = y + direction;
    int newX = x;
    // Check capturing moves
    int captureCols[] = {x - 1, x + 1};
    for (int i = 0; i < 2; ++i) {
        newX = captureCols[i];
        if (isValidPosition(newX, newY) && board[newY][newX].symbol != 0 && board[newY][newX].color != board[y][x].color) {
            moveList = addMove(newX, newY, moveList);
        }
    }
    return moveList;
}

// Function to get possible moves for a bishop


struct listCoords* getBishopMoves(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 
    int yOffsets[] = {-1, -1, 1, 1};
    int xOffsets[] = {-1, 1, -1, 1};
    for (int i = 0; i < 4; ++i) {
        int newY = y + yOffsets[i];
        int newX = x + xOffsets[i];
        while (isValidPosition(newX, newY) && (getSymbol(board[newY][newX]) == 0 ||  getColor(board[newY][newX]) != getColor(board[y][x]) )) {
            moveList = addMove(newX, newY, moveList);
            if ( getSymbol(board[newY][newX]) != 0 ) {
                break; 
            }
            newY += yOffsets[i];
            newX += xOffsets[i];
        }
    }
    return moveList;
}

// Function to get possible moves for a rook
struct listCoords* getRookMoves(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 

    int yOffsets[] = {-1, 1, 0, 0};
    int xOffsets[] = {0, 0, -1, 1};    
    for (int i = 0; i < 4; ++i) {
        int newY = y + yOffsets[i];
        int newX = x + xOffsets[i];
        while (isValidPosition(newX, newY) && (getSymbol(board[newY][newX]) == 0 ||  getColor(board[newY][newX]) != getColor(board[y][x]) )) { //&& (getSymbol(board[newY][newX]) == 0 ||  getColor(board[newY][newX]) != getColor(board[y][x]) )
            moveList = addMove(newX, newY, moveList);
            if (getSymbol(board[newY][newX]) != 0 ) {
                break; 
            }
            newY += yOffsets[i];
            newX += xOffsets[i];
        }
    }
    return moveList;
}

// Function to get possible moves for a queen
struct listCoords* getQueenMoves(int startRow, int startCol, struct piece board[BOARD_SIZE][BOARD_SIZE]) {

    return connectLists(getBishopMoves(startRow, startCol, board), getRookMoves(startRow, startCol, board) );
}

// Function to get possible moves for a king
struct listCoords* getKingMoves(int y, int x, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 

    int yOffsets[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int xOffsets[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; ++i) {
        int newY = y + yOffsets[i];
        int newX = x + yOffsets[i];
        if (isValidPosition(newY, newX) && (board[newY][newX].symbol == ' ' || board[newY][newX].color != board[y][x].color)) {
            moveList = addMove(newX, newY, moveList);
        }
    }
    /*
        Somehow i need to find a way to add castling her 
    */
    return moveList;
}

// Function to get moves for a piece based on its type
struct listCoords* getPieceMoves(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    // Get the piece at the specified position
    struct piece currentPiece = board[y][x];

    // Check the piece type and call the appropriate function
    switch (getSymbol(currentPiece)) {
        case 'P':
        //    printf("Pawn moves are not developed yet\n");
           return  getPawnMoves(x, y, board);   
        case 'N':
            return getKnightMoves(x, y, board);
        case 'B':
            return getBishopMoves(x, y, board);
        case 'R':
            return getRookMoves(x, y, board);
        case 'Q':
            return getQueenMoves(x, y, board);
        case 'K':
            return  getKingMoves(x, y, board);   
        default:
            printf("Piece type not recognized.\n");
            return getKnightMoves(x, y, board);   
    }
}
struct listCoords* getPieceThreats(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    // Get the piece at the specified position
    struct piece currentPiece = board[y][x];

    // Check the piece type and call the appropriate function
    switch (getSymbol(currentPiece)) {
        case 'P':
        //    printf("Pawn moves are not developed yet\n");
           return  getPawnThreats(x, y, board);   
        case 'N':
            return getKnightMoves(x, y, board);
        case 'B':
            return getBishopMoves(x, y, board);
        case 'R':
            return getRookMoves(x, y, board);
        case 'Q':
            return getQueenMoves(x, y, board);
        case 'K':
            return  getKingMoves(x, y, board);   
        default:
            printf("Piece type not recognized.\n");
            return getKnightMoves(x, y, board);   
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
            if (getSymbol(board[i][j]) != 0 && getColor(board[i][j]) == color) {
                struct listCoords *movesSave = getPieceThreats(j, i, board);
                struct listCoords *moves = movesSave; 
                //printf("moves\n");
                printMoveList(moves); 
                int movesLen = listLen(moves);
                // printf("moves\n");
                printf("len = %d\n", movesLen);

                //printMoveList(moves);
                displayBoardActions(board, moves);

                for (int iteration = 0; iteration < movesLen; iteration++) {
                    int x = getX(moves);
                    int y = getY(moves);
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
        printf("%s%d%s ",YELLOW, 8 - i, WHITE);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("|");
            if (getSymbol(board[i][j]) == 0){
              printf("%s%c%c%c%s", ( (i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)  ) ? BLACK : WHITE, 178, 178, 178, WHITE);
            }
            else{
              printf(" %s%c%s ", (getColor(board[i][j])) ? BLUE:WHITE, board[i][j].symbol, WHITE);
            }
            
        }
        printf("|\n");
    }
    printf("%s  ", YELLOW);
    for(int i = 0; i  < BOARD_SIZE; ++i){
      printf("  %c ", 'a' + i);
    }
    printf("%s\n",WHITE);
}

void displayBoardActions(struct piece board[BOARD_SIZE][BOARD_SIZE], struct listCoords* moveList)
{

    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%d ", 8 - i);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("|");
            if(inCoordList( j, i, moveList )){
                if ( getSymbol(board[i][j]) == 0){
                  printf("%s%c%c%c%s",  GREEN, 178, 178, 178, WHITE);
                }
                else{
                  printf(" %s%c%s ", GREEN, getSymbol(board[i][j]), WHITE);
                }
            }
            else{
                if ( getSymbol(board[i][j]) == 0){
                  printf("%s%c%c%c%s", ( (i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)  ) ? BLACK : WHITE, 178, 178, 178, WHITE);
                }
                else{
                  printf(" %s%c%s ", (getColor(board[i][j])) ? BLUE:WHITE, getSymbol(board[i][j]), WHITE);
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
            if ( getSymbol(board[i][j]) == 0){
              printf("%s%c%c%c%s", ( (i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)  ) ? BLACK : WHITE, 178, 178, 178, WHITE);
            }
            else{
              printf(" %s%c%s ", (getColor(board[i][j])) ? BLUE:WHITE, getSymbol(board[i][j]), WHITE);
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
            if (getSymbol(board[i][j]) == 0){
              printf("%s%c%c%c%s", ( (i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)  ) ? BLACK : WHITE, 178, 178, 178, WHITE);
            }
            else{
              printf(" %s%d%s ", (getColor(board[i][j])) ? BLUE:WHITE, getMoveAmount(board[i][j]), WHITE);
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

void displayThreatMap(int threatMap[BOARD_SIZE][BOARD_SIZE]){
    for(int i = 0; i < BOARD_SIZE; i++ ){
        printf("%s%d%s |",YELLOW, i, WHITE);
        for(int j = 0; j < BOARD_SIZE; j++){
            printf(" %d ", threatMap[i][j]);
        }
        printf("\n");
    }
    printf("%s   ",YELLOW);
    
    for(int i = 0; i < BOARD_SIZE; i++){
        printf(" %d ", i);
    }

    printf("%s\n",WHITE);

}

void printMoveList(struct listCoords *moveList) {
    struct listCoords *current = moveList;

    printf("List of Moves:\n");
    while (current != NULL) {
        printf("(x = %d, y = %d) ", getX(current), getY(current));
        current = current->next;
    }
    printf("\n");
}

void placePiece(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE], struct piece newPiece) {
    if (y >= 0 && y < BOARD_SIZE && x >= 0 && x < BOARD_SIZE) {
        board[y][x] = newPiece;
    } else {
        printf("Invalid coordinates for placing the piece.\n");
    }
}

int main() {
    // Declare the chess board using the piece structure
    struct piece chessBoard[BOARD_SIZE][BOARD_SIZE];

    // Initialize the chess board
    //initializeEmptyBoard(chessBoard);
    initializeBoard(chessBoard);
    struct piece newPiece = {0, true};
    struct piece newPiece2 = {'Q', true};

    //placePiece( 2, 0, chessBoard, newPiece);
    //placePiece( 1, 0, chessBoard, newPiece);

    //placePiece( 3, 6, chessBoard, newPiece2);

    int threatMap[BOARD_SIZE][BOARD_SIZE];
    initializeThreatMap(threatMap);

    displayBoard(chessBoard);
    //displayThreatMap(threatMap);
    printf("\n");

    createThreatMap(chessBoard, threatMap, 1);
    
    displayThreatMap(threatMap);
    printf("\n");



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
