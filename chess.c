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


enum moveType {
    NORMAL,
    DOUBLE_PAWN_MOVE,
    EN_PASSANT,
    CASTLING
};
char* returnMoveType(enum moveType moveType){
    if(moveType == NORMAL){
        return "NORMAL";
    }
    else if(moveType == DOUBLE_PAWN_MOVE){
        return "DOUBLE_PAWN_MOVE";
    }    
    else if(moveType == EN_PASSANT){
        return "EN_PASSANT";
    }   
    else if(moveType == CASTLING){
        return "CASTLING";
    }
    else{
        return "Type is not defined";
    }
}
 // Structure to represent a chess piece
struct piece {
    char symbol;
    bool color;  // true for black, false for white
    int moveAmount; 
};

struct listCoords{
    int x;
    int y;
    enum moveType type;
    struct listCoords *next;
};

struct chessMoveList{
    int oldX, oldY, newX, newY;
    struct piece piece;
    bool turn;
    enum moveType type;
    struct chessMoveList *next;
};

void getOldCoords(struct chessMoveList *move, int* oldX, int* oldY){
    *oldX = move->oldX;
    *oldY = move->oldY;
}

void getNewCoords(struct chessMoveList *move, int* newX, int* newY){
    *newX = move->newX;
    *newY = move->newY;
}
int getNewX(struct chessMoveList *move){
    return move->newX;
}

int getNewY(struct chessMoveList *move){
    return move->newY;
}

struct piece getPiece(struct chessMoveList* move){
    return move->piece;
}


struct chessMoveList* addChessMove(int oldX, int oldY, int newX, int newY, bool turn, struct piece piece, enum moveType type,struct chessMoveList* previousList) {
    struct chessMoveList* newElement = malloc(sizeof(struct chessMoveList)); 
    newElement->oldX = oldX;
    newElement->oldY = oldY;
    newElement->newX = newX;
    newElement->newY = newY;
    newElement->piece = piece;
    newElement->turn = turn;
    newElement->type = type;
    newElement->next = previousList;
    return newElement;
}

int chessMoveListLen(struct chessMoveList *head) {
    int count = 0;
    struct chessMoveList *current = head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}

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

enum moveType getMoveType(struct listCoords *move){
    return move->type;
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
void displayBoardActionsDebug(struct piece board[BOARD_SIZE][BOARD_SIZE], struct listCoords* moveList);
void printChessMoveListDebug(struct chessMoveList *moveList);

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
    newElement->type = NORMAL;
    newElement->next = previousList;
    return newElement;
}

struct listCoords* addMoveType(int x, int y,  enum moveType type, struct listCoords* previousList) {
    struct listCoords* newElement = malloc(sizeof(struct listCoords)); 
    newElement->x = x;
    newElement->y = y;
    newElement->type = type;
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
struct listCoords * returnInCoordList(int x, int y, struct listCoords *head) {
    struct listCoords *current = head;

    while (current != NULL) {
        if (current->x == x && current->y == y) {
            return current; // Coordinates found in the list
        }
        current = current->next;
    }

    return NULL; // Coordinates not found in the list
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
    //printf("(x = %d)(y = %d)\n", x, y);
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
struct listCoords* getKnightThreats(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE])
{
    struct listCoords *moveList = NULL; 
    //printf("(x = %d)(y = %d)\n", x, y);
    int xOffsets[] = {-1, -1,  1, 1, -2,  2, -2, 2};
    int yOffsets[] = {-2,  2, -2, 2, -1, -1,  1, 1};

    for (int i = 0; i < 8; ++i) {
        int newX = x + xOffsets[i];
        int newY = y + yOffsets[i];
        
        if (isValidPosition(newX, newY) ) { 

            moveList = addMove(newX, newY, moveList);
        }
    }
    return moveList;
}


struct listCoords* getPawnMoves(int x, int y, struct chessMoveList *chessMoveList, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 
   
    int direction = (getColor(board[y][x])) ? 1 : -1; // White pawns move upwards, black pawns move downwards   
    // Check one square forward
    int newY = y + direction;
    int newX = x;
    if (isValidPosition( newX, newY) && board[newY][newX].symbol == 0) {
        moveList = addMove(newX, newY, moveList);
        // Check two squares forward for the initial move
    }

    if ((y == 6) || (y == 1 )) {
        int startMoveY = y + 2 * direction;
        if (isValidPosition(newX, newY) && board[newY][newX].symbol == 0) {
            moveList = addMoveType(newX, startMoveY, DOUBLE_PAWN_MOVE, moveList);
        }
    }
    
    //printChessMoveListDebug(moveList);
    if (((y == 3) || (y == 4)) && chessMoveList !=NULL && chessMoveList->type == DOUBLE_PAWN_MOVE) { //   !!!!!!!!!!!!!!! please future me change this, please, please, please 
        int captureCols[] = {x - 1, x + 1};
        int doublePawnX, doublePawnY;

        getNewCoords(chessMoveList, &doublePawnX, &doublePawnY);

        for(int i = 0; i < 2; i++){
            printf("my x = %d my y = %d new x = %d new y = %d\n", captureCols[i], y , doublePawnX, doublePawnY);
            if (isValidPosition(captureCols[i], y) && doublePawnX == captureCols[i] && doublePawnY == y && isValidPosition(captureCols[i], y + direction) && board[y][captureCols[i]].symbol != 0) {
                moveList = addMoveType(captureCols[i], y + direction, EN_PASSANT, moveList);
            }
        }
    }

    // Check capturing moves
    int captureCols[] = {x - 1, x + 1};
    for (int i = 0; i < 2; ++i) {
        newX = captureCols[i];
        if (isValidPosition(newX, newY) && getSymbol(board[newY][newX]) != 0  && getColor(board[newY][newX]) != getColor(board[y][x] ) ){// && getSymbol(board[newY][newX]) != 0 && getColor(board[newY][newX]) != getColor(board[y][x])
            moveList = addMove(newX, newY, moveList);
        }
    }
    return moveList;
}

struct listCoords* getPawnThreats(int x, int y,  struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 
   
    int direction = (getColor(board[y][x])) ? 1 : -1; // White pawns move upwards, black pawns move downwards   
    // Check one square forward
    int newY = y + direction;
    //printf("newY = %d\n", newY);
    int newX = x;
    // Check capturing moves
    int captureCols[] = {x - 1, x + 1};
    for (int i = 0; i < 2; ++i) {
        newX = captureCols[i];
        //printf("newX = %d\n", newX);
        if (isValidPosition(newX, newY) )  {// && getSymbol(board[newY][newX]) != 0 && getColor(board[newY][newX]) != getColor(board[y][x]))
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
struct listCoords* getBishopThreats(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 
    int yOffsets[] = {-1, -1, 1, 1};
    int xOffsets[] = {-1, 1, -1, 1};
    for (int i = 0; i < 4; ++i) {
        int newY = y + yOffsets[i];
        int newX = x + xOffsets[i];
        while (isValidPosition(newX, newY) && (getSymbol(board[newY][newX]) == 0 ||  getColor(board[newY][newX]) != getColor(board[y][x]) )) {
            moveList = addMove(newX, newY, moveList);
            newY += yOffsets[i];
            newX += xOffsets[i];
            if (getSymbol(board[newY][newX]) != 0 ) {
                moveList = addMove(newX, newY, moveList);
                break; 
            }
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

struct listCoords* getRookThreats(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
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
        if(isValidPosition(newX,newY)){
            moveList = addMove(newX, newY, moveList);
        }
    }
    return moveList;
}

// Function to get possible moves for a queen
struct listCoords* getQueenMoves(int startRow, int startCol, struct piece board[BOARD_SIZE][BOARD_SIZE]) {

    return connectLists(getBishopMoves(startRow, startCol, board), getRookMoves(startRow, startCol, board) );
}

struct listCoords* getQueenThreats(int startRow, int startCol, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    return connectLists(getBishopThreats(startRow, startCol, board), getRookThreats(startRow, startCol, board) );
}

// Function to get possible moves for a king
struct listCoords* getKingMoves(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE], int threatMap[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 

    int yOffsets[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int xOffsets[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int i = 0; i < 8; ++i) {
        int newY = y + yOffsets[i];
        int newX = x + xOffsets[i];
        if (isValidPosition(newY, newX) && threatMap[newY][newX] == 0 && (board[newY][newX].symbol == 0 || board[newY][newX].color != board[y][x].color)) {
            moveList = addMove(newX, newY, moveList);
        }
    }
    /*
        Somehow i need to find a way to add castling her 
    */
    return moveList;
}
// Function to get possible moves for a king
struct listCoords* getKingThreats(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 

    int yOffsets[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int xOffsets[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; ++i) {
        int newY = y + yOffsets[i];
        int newX = x + yOffsets[i];
        if (isValidPosition(newY, newX) && (board[newY][newX].symbol == 0 || board[newY][newX].color != board[y][x].color)) {
            moveList = addMove(newX, newY, moveList);
        }
    }
    /*
        Somehow i need to find a way to add castling her 
    */
    return moveList;
}

// Function to get moves for a piece based on its type
struct listCoords* getPieceMoves(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE], struct chessMoveList * chessMoveList , int threatMap[BOARD_SIZE][BOARD_SIZE]) {
    // Get the piece at the specified position
    struct piece currentPiece = board[y][x];

    // Check the piece type and call the appropriate function
    switch (getSymbol(currentPiece)) {
        case 'P':
        //    printf("Pawn moves are not developed yet\n");
           return  getPawnMoves(x, y, chessMoveList, board);   
        case 'N':
            return getKnightMoves(x, y, board);
        case 'B':
            return getBishopMoves(x, y, board);
        case 'R':
            return getRookMoves(x, y, board);
        case 'Q':
            return getQueenMoves(x, y, board);
        case 'K':
            return  getKingMoves(x, y, board, threatMap);   
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
           return  getPawnThreats(x, y, board);   
        case 'N':
            return getKnightThreats(x, y, board);
        case 'B':
            return getBishopThreats(x, y, board);
        case 'R':
            return getRookThreats(x, y, board);
        case 'Q':
            return getQueenThreats(x, y, board);
        case 'K':
            return  getKingThreats(x, y, board);   
        default:
            printf("Piece type is not recognized.\n");
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

                int movesLen = listLen(moves);

                for (int iteration = 0; iteration < movesLen; iteration++) {
                    int x = getX(moves);
                    int y = getY(moves);
                    threatMap[y][x] += 1;
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
        printf("%s%d%s ",YELLOW, 8 - i, WHITE);
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
    printf("%s  ", YELLOW);
    for(int i = 0; i  < BOARD_SIZE; ++i){
      printf("  %c ", 'a' + i);
    }
    printf("%s\n",WHITE);
}
void displayBoardActionsDebug(struct piece board[BOARD_SIZE][BOARD_SIZE], struct listCoords* moveList)
{

    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%s%d%s ",YELLOW, i, WHITE);
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
    printf("%s  ", YELLOW);
    for(int i = 0; i  < BOARD_SIZE; ++i){
      printf("  %d ",i);
    }
    printf("%s\n",WHITE);
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


void printChessMoveListDebug(struct chessMoveList *moveList) {
    struct chessMoveList *current = moveList;

    printf("List of Moves:\n");
    while (current != NULL) {
        int oldX, oldY, newX, newY;
        getOldCoords(current, &oldX, &oldY);
        getNewCoords(current, &newX, &newY);

        printf("%s %c move from [oldX = %d, oldY  = %d] to [newX = %d, newY  = %d] type %s \n", current->turn? "black" : "white",
        getSymbol(getPiece(current)), oldX, oldY, newX, newY, returnMoveType(current->type));
        current = current->next;
    }
    printf("\n");
}

char coordsToSymbol(int x){
    return 'a' + x;
}

void printChessMoveList(struct chessMoveList *moveList) {
    struct chessMoveList *current = moveList;

    printf("List of Moves:\n");
    while (current != NULL) {
        int oldX, oldY, newX, newY;
        getOldCoords(current, &oldX, &oldY);
        getNewCoords(current, &newX, &newY);

        printf("%c%c%d to %c%d\n", 
        getSymbol(getPiece(current)), coordsToSymbol(oldX), oldY, coordsToSymbol(newX), newY );
        
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

void handelInput(int *x, int *y){
    while (true){
        printf("Please input Letter: ");
        char newX;
        scanf(" %c", &newX);
        //printf("x = %c(%d)\n", newX, newX);
        if(newX >= 'A' && newX <= 'H'){
            *x = newX - 'A';
            //printf("\n(%d)\n", *x);
            break;
        }
        else if(newX >= 'a' && newX <= 'h'){
            *x = newX - 'a';
            break;
        }
        else{
            printf("Wrong Letter please try again(x = %d)\n", newX);
        }  
    }
    while (true){
        printf("Please input Number: ");
        char newY;
        scanf(" %c", &newY);
        printf("\n");

        if( newY >= '1' && newY <= '8'){
            *y = 7 - (newY - '1');
            break;
        }
        else{
            printf("Wrong Number please try again(y = %d)\n", newY);
        }
    }
}

bool findKing(struct piece board[BOARD_SIZE][BOARD_SIZE] , bool color, int *x, int *y){

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(getSymbol( board[i][j]) == 'K' &&  getColor(board[i][j]) == color){
                *x = j;
                *y = i;
                return true;
            }
        }
    }
    printf("King was not found\n");
    return false;
    
}

bool kingHasThreats(int threatMap[BOARD_SIZE][BOARD_SIZE], int x , int y){
    return threatMap[y][x] >= 1; 
}
bool kingHasMoves(struct piece board[BOARD_SIZE][BOARD_SIZE] ,int threatMap[BOARD_SIZE][BOARD_SIZE], int x , int y){
    struct listCoords* kingMoves = getKingMoves( x, y, board, threatMap);
    int kingMovesAmount = listLen(kingMoves);
    freeMoveList(kingMoves);
    return kingMovesAmount > 0; 
}

bool checkMate(struct piece board[BOARD_SIZE][BOARD_SIZE], bool color, int threatMap[BOARD_SIZE][BOARD_SIZE]){
    int x, y; 
    findKing(board, color, &x, &y);
    //printf("kingHasThreats = %d\n!kingHasMoves = %d\n",kingHasThreats(threatMap, x, y), !kingHasMoves(board, threatMap, x, y));
    return kingHasThreats(threatMap, x, y) && !kingHasMoves(board, threatMap, x, y);
}


void chess(){
    struct piece chessBoard[BOARD_SIZE][BOARD_SIZE];
    initializeBoard(chessBoard);
    struct piece testPiece2 = {'K', false};
    struct piece testPiece = {'P', false};
    struct piece emptyPiece = {0 , false};

    struct chessMoveList * chessMoveList = NULL;
    //placePiece( 4, 2, chessBoard, testPiece);
    //placePiece( 4, 1, chessBoard, emptyPiece);
    //placePiece( 4, 6, chessBoard, emptyPiece);
    placePiece( 3, 4, chessBoard, testPiece);
    //placePiece( 4, 4, chessBoard, emptyPiece);
    displayBoard(chessBoard);
    bool turn = false;
    while(true){
        printChessMoveListDebug(chessMoveList);
        int threatMapOppositeTeam [BOARD_SIZE][BOARD_SIZE];
        initializeThreatMap(threatMapOppositeTeam);

        int threatMapCurentTeam[BOARD_SIZE][BOARD_SIZE];
        initializeThreatMap(threatMapCurentTeam);
        
        createThreatMap(chessBoard, threatMapOppositeTeam, !turn);

        //displayThreatMap(threatMapOppositeTeam);
        createThreatMap(chessBoard, threatMapCurentTeam, turn);
        if(checkMate(chessBoard, turn, threatMapOppositeTeam)){
            break;
        }
        int x, y;
        printf("---Please choose a piece---\n");
        while (true){
            handelInput(&x, &y); 
            if(getSymbol(chessBoard[y][x]) != 0 && getColor(chessBoard[y][x]) == turn){
                //printf("x = %d, y = %d, piece = %c\n",x,y, getSymbol(chessBoard[y][x]) );
                struct listCoords* moves = getPieceMoves(x, y, chessBoard, chessMoveList, threatMapOppositeTeam);
                if(listLen(moves) != 0){
                    displayBoardActions(chessBoard, moves);
                    printf("___Please choose a move___\n");
                    int newX, newY;
                    while (true){
                        //printMoveList(moves);

                        handelInput(&newX, &newY);
                        //printf("(x = %d, y = %d, piece = %c)\n",newX,newY, getSymbol(chessBoard[newY][newX]) );

                        if(inCoordList(newX, newY, moves)){
                            struct listCoords* finalMove = returnInCoordList(newX, newY, moves);
                            chessBoard[newY][newX] = chessBoard[y][x];
                            struct piece newPiece = {0, true};
                            placePiece( x, y, chessBoard, newPiece);
                            displayBoard(chessBoard);
                            chessMoveList = addChessMove(x, y, newX, newY, turn, chessBoard[newY][newX], getMoveType(finalMove), chessMoveList);
                            if(chessMoveList->type == EN_PASSANT){
                                printf("this happened\n");
                                placePiece( getNewX(chessMoveList), getNewY(chessMoveList) + (chessMoveList->turn? -1 : 1), chessBoard, emptyPiece);
                            }
                            displayBoard(chessBoard);
                            //printf("moveLen = %d\n", chessMoveListLen(chessMoveList));
                            break;
                        }
                        else{
                            printf("Wrong square please try again\n");
                        }
                    }

                    break;
                }
                else{
                    printf("This piece does not have moves\n");
                }
                freeMoveList(moves);
            }   
            else{
                printf("Wrong square please try again\n");
            }
        }
        turn = !turn;
    }
    printf("%s - win\n", turn ? "White":"Black");

}


int main() {
    // struct piece chessBoard[BOARD_SIZE][BOARD_SIZE];
    // initializeBoard(chessBoard);
    // struct piece testPiece2 = {'P', false};
    // struct piece testPiece = {'P', true};
    // struct piece emptyPiece = {0, false};
    // placePiece( 2 , 5, chessBoard, testPiece2);
    // placePiece( 3 , 4, chessBoard, testPiece);

    // struct listCoords* moves = getPawnMoves(2, 5, chessBoard);
    // displayBoardActionsDebug(chessBoard, moves); 

    chess();
    printf(WHITE);
    return 0;
}
