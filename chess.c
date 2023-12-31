#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Size of the chess board
#define BOARD_SIZE 8

#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define BLACK "\033[0;30m"
#define WHITE "\033[0;37m"

// Piece values
#define PAWN_VALUE 1
#define KNIGHT_VALUE 3
#define BISHOP_VALUE 3
#define ROOK_VALUE 5
#define QUEEN_VALUE 9



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
    int x, y;
    enum moveType type;
    struct listCoords *next;
};

struct protectorPieceList{
    int x, y;
    struct listCoords *moves;
    struct protectorPieceList *next;
};

struct moveScoreList{
    int oldX, oldY, newX, newY;
    int score;
    struct moveScoreList *next;
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
void freeMoveScoreList(struct moveScoreList *moveList) {
    while (moveList != NULL) {
        struct moveScoreList *temp = moveList;
        moveList = moveList->next;
        free(temp);
    }
}

void freeProtectorPieceList(struct protectorPieceList *protectorPiece) {
    while (protectorPiece != NULL) {
        struct protectorPieceList  *temp = protectorPiece;
        freeMoveList( protectorPiece->moves);
        protectorPiece = protectorPiece->next;
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


struct protectorPieceList* addProtectorPiece(int x, int y,  struct listCoords *moves, struct protectorPieceList* previousList){
    struct protectorPieceList*  newElement = malloc(sizeof(struct protectorPieceList)); 
    newElement->x = x;
    newElement->y = y;
    newElement->moves = moves;
    newElement->next = previousList;
    return newElement;
}
struct moveScoreList* addMoveScoreList(int oldX, int oldY, int newX, int newY, int score,struct moveScoreList* previousList){
    struct moveScoreList*  newElement = malloc(sizeof(struct moveScoreList)); 
    newElement->oldX = oldX;
    newElement->oldY = oldY;
    newElement->newX = newX;
    newElement->newY = newY;
    newElement->score = score;
    newElement->next = previousList;
    return newElement;
}

struct moveScoreList* findMaxScore(struct moveScoreList *head) {
    if (head == NULL) {
        return NULL; // Return NULL if the list is empty
    }

    struct moveScoreList *maxScoreMove = head;
    int maxScore = head->score;

    struct moveScoreList *current = head->next;
    while (current != NULL) {
        if (current->score > maxScore) {
            maxScore = current->score;
            maxScoreMove = current;
        }
        current = current->next;
    }

    return maxScoreMove;
}
struct moveScoreList* findMinScore(struct moveScoreList *head) {
    if (head == NULL) {
        return NULL; // Return NULL if the list is empty
    }

    struct moveScoreList *minScoreMove = head;
    int minScore = head->score;

    struct moveScoreList *current = head->next;
    while (current != NULL) {
        if (current->score < minScore) {
            minScore = current->score;
            minScoreMove = current;
        }
        current = current->next;
    }

    return minScoreMove;
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

bool inCoordProtectorPieceList(int x, int y, struct protectorPieceList *head) {
    struct protectorPieceList *current = head;

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



 struct protectorPieceList*  returnInCoordProtectorPieceList(int x, int y, struct protectorPieceList *head) {
    struct protectorPieceList *current = head;

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
    int yOffsets[] = {-1, -1,  1, 1};
    int xOffsets[] = {-1,  1, -1, 1};
    for (int i = 0; i < 4; ++i) {
        int newY = y + yOffsets[i];
        int newX = x + xOffsets[i];
        while (isValidPosition(newX, newY) && (getSymbol(board[newY][newX]) == 0 ||  getColor(board[newY][newX]) != getColor(board[y][x]) )) {
            moveList = addMove(newX, newY, moveList);
            
            if (getSymbol(board[newY][newX]) != 0 ) {
                //printf("newX = %d newY = %d\n", newX, newY);
                moveList = addMove(newX, newY, moveList);
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

    if(x == 4 && (y == 7 || y == 0) && getMoveAmount(board[y][x]) == 0){
        bool pieceBlock = false;
        for(int i = 5; i < BOARD_SIZE - 1; i++){
            if(getSymbol(board[y][i]) != 0 ){
                pieceBlock = true;
                break;
            }
        }
        if(!pieceBlock && getSymbol(board[y][7]) == 'R' && getMoveAmount(board[y][7]) == 0){
            moveList = addMoveType(x + 2 , y, CASTLING, moveList);
        }
        pieceBlock = false;

        for(int i = 3; i > 0 ; i--){
            if(getSymbol(board[y][i]) != 0 ){
                pieceBlock = true;
                break;
            }
        }
        if( !pieceBlock && getSymbol(board[y][0]) == 'R' && getMoveAmount(board[y][0]) == 0 ){
            moveList = addMoveType(x - 3 , y, CASTLING, moveList);
        }        

    }

    return moveList;
}
// Function to get possible moves for a king
struct listCoords* getKingThreats(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    struct listCoords *moveList = NULL; 

    int yOffsets[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int xOffsets[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; ++i) {

        int newY = y + yOffsets[i];
        int newX = x + xOffsets[i];

        if (isValidPosition(newX, newY) && (board[newY][newX].symbol == 0 || board[newY][newX].color != board[y][x].color)) {
            moveList = addMove(newX, newY, moveList);
        }
    }

    return moveList;
}

// Function to get moves for a piece based on its type
struct listCoords* getPieceMoves(int x, int y, struct piece board[BOARD_SIZE][BOARD_SIZE], struct chessMoveList * chessMoveList , int threatMap[BOARD_SIZE][BOARD_SIZE]) {
    // Get the piece at the specified position
    struct piece currentPiece = board[y][x];

    // Check the piece type and call the appropriate function
    switch (getSymbol(currentPiece)) {
        case 'P':
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
            return NULL;   
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
            return NULL;   
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
void displayBoardProtectingPieces(struct piece board[BOARD_SIZE][BOARD_SIZE],  struct protectorPieceList* protectorPieceList)
{

    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%s%d%s ",YELLOW, 8 - i, WHITE);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("|");
            if(inCoordProtectorPieceList( j, i, protectorPieceList )){
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

void displayBoardProtectingPiecesDebug(struct piece board[BOARD_SIZE][BOARD_SIZE], struct protectorPieceList* protectorPieceList)
{

    for (int i = 0; i < BOARD_SIZE; ++i) {
        printf("%s%d%s ",YELLOW, i, WHITE);
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("|");
            if(inCoordProtectorPieceList( j, i, protectorPieceList )){
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

void printMoveScoreList(struct moveScoreList *moveList) {
    struct moveScoreList *current = moveList;
    
    printf("List of Moves:\n");
    while (current != NULL) {
        printf("(oldX = %d, oldY = %d, newX = %d, newY = %d SCORE = %d)\n", current->oldX,current->oldY, current->newX, current->newY, current->score);
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


void printProtectorPieceList(struct protectorPieceList  *protectorPieceList) {
    struct protectorPieceList  *current = protectorPieceList;

    printf("List of Moves:\n");
    while (current != NULL) {
        printf("coords x - %d y - %d\n", current->x, current->y);
        printMoveList(current->moves);
        
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
    //displayBoardDebug(board);
    //printf("findKing start\n");

    for(int i = 0; i < BOARD_SIZE; i++){

        for(int j = 0; j < BOARD_SIZE; j++){

            if(getSymbol( board[i][j]) == 'K' &&  getColor(board[i][j]) == color){
                *x = j;
                *y = i;
                //printf("findKing end true\n");

                return true;
            }
        }
    }
    //printf("findKing end false\n");

    //printf("King was not found\n");
    return false;
}

void copyBoard(struct piece destBoard[BOARD_SIZE][BOARD_SIZE], struct piece srcBoard[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            destBoard[i][j] = srcBoard[i][j];
        }
    }
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


bool canProtectKing(struct piece board[BOARD_SIZE][BOARD_SIZE], bool color, struct chessMoveList *chessMoveList , int threatMap[BOARD_SIZE][BOARD_SIZE]){
    int kingX, kingY;
    findKing(board, color, &kingX, &kingY);
    //if( !kingHasThreats(threatMap, ) || kingHasMoves)
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(getSymbol(board[i][j]) != 0 && getColor(board[i][j]) == color ){
                struct listCoords* moves = getPieceMoves( j, i, board, chessMoveList, threatMap);
                struct listCoords* movesCopy = moves;
                while(movesCopy != NULL){
                    struct piece checkBoard[BOARD_SIZE][BOARD_SIZE];
                    copyBoard(checkBoard, board); 
                    checkBoard[movesCopy->y][movesCopy->x] = checkBoard[i][j];
                    struct piece newPiece = {0, true};
                    int threatMapNew[BOARD_SIZE][BOARD_SIZE];
                    initializeThreatMap(threatMapNew);
                    createThreatMap(checkBoard, threatMapNew, !color);
                    if(!kingHasThreats(threatMapNew, kingX, kingY)){
                        return true;
                    }
                    movesCopy = movesCopy->next;
                
                }   
                freeMoveList(moves);
            }
        }
    }

    return false; 
}

struct protectorPieceList *protectKingMoves(struct piece board[BOARD_SIZE][BOARD_SIZE], bool color, struct chessMoveList *chessMoveList , int threatMap[BOARD_SIZE][BOARD_SIZE]){
    int kingX, kingY;
    findKing(board, color, &kingX, &kingY);
    struct protectorPieceList *protectorPieceList = NULL; 
    //if( !kingHasThreats(threatMap, ) || kingHasMoves)
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(getSymbol(board[i][j]) != 0 && getColor(board[i][j]) == color){

                struct listCoords* moves = getPieceMoves( j, i, board, chessMoveList, threatMap);
                struct listCoords* movesCopy = moves;
                struct listCoords* protectingMoves = NULL;
                while(movesCopy != NULL){
                    struct piece checkBoard[BOARD_SIZE][BOARD_SIZE];

                    copyBoard(checkBoard, board); 
                    checkBoard[movesCopy->y][movesCopy->x] = checkBoard[i][j];
                    struct piece newPiece = {0, true};
                    int threatMapNew[BOARD_SIZE][BOARD_SIZE];
                    initializeThreatMap(threatMapNew);
                    createThreatMap(checkBoard, threatMapNew, !color);
                    if(!kingHasThreats(threatMapNew, kingX, kingY)){
                        struct listCoords* temp = movesCopy;
                        movesCopy = movesCopy->next;
                        temp->next = protectingMoves;
                        protectingMoves = temp;
                    }
                    else{
                        struct listCoords* temp = movesCopy;
                        movesCopy = movesCopy->next;
                        free(temp);
                    }
                } 
                if(protectingMoves != NULL){
                    protectorPieceList = addProtectorPiece(j, i, protectingMoves, protectorPieceList);
                }  

            }
        }
    }
    return protectorPieceList; 
}

bool checkMate(struct piece board[BOARD_SIZE][BOARD_SIZE], bool color, int threatMap[BOARD_SIZE][BOARD_SIZE]){
    int x, y; 
    if(!findKing(board, color, &x, &y))
        return true;
    
    return kingHasThreats(threatMap, x, y) && !canProtectKing(board,color, NULL, threatMap);
}

bool checkMateT(struct piece board[BOARD_SIZE][BOARD_SIZE], bool color){
    
    struct piece newBoard[BOARD_SIZE][BOARD_SIZE];
    copyBoard(newBoard, board);

    int threatMapOppositeTeam[BOARD_SIZE][BOARD_SIZE];
    initializeThreatMap(threatMapOppositeTeam);
    createThreatMap(board, threatMapOppositeTeam, !color);
    int x, y; 
    //findKing(newBoard, color, &x, &y);
    if(!findKing(board, color, &x, &y)){
        return true;
    }
    //printf("kingHasThreats = %d kingHasMoves = %d canProtectKing = %d\n", kingHasThreats(threatMapOppositeTeam, x, y), kingHasMoves(board,threatMapOppositeTeam, x, y), canProtectKing(board,color, NULL, threatMapOppositeTeam));
    return kingHasThreats(threatMapOppositeTeam, x, y) && !kingHasMoves(board,threatMapOppositeTeam, x, y) && !canProtectKing(board,color, NULL, threatMapOppositeTeam);
}

struct protectorPieceList *getAllMoves(struct piece board[BOARD_SIZE][BOARD_SIZE], bool color){
    int kingX, kingY;
    findKing(board, color, &kingX, &kingY);
    struct protectorPieceList *protectorPieceList = NULL; 
    int threatMapOppositeTeam [BOARD_SIZE][BOARD_SIZE];
    initializeThreatMap(threatMapOppositeTeam);
    createThreatMap(board, threatMapOppositeTeam, !color);


    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(getSymbol(board[i][j]) != 0 && getColor(board[i][j]) == color ){
                struct listCoords* moves = getPieceMoves( j, i, board, NULL , threatMapOppositeTeam); // NULL should be removed later 
                if(moves != NULL){
                    protectorPieceList = addProtectorPiece(j, i, moves, protectorPieceList);
                }  

            }
        }
    }
    return protectorPieceList; 
}


struct protectorPieceList *getAllMovesP(struct piece board[BOARD_SIZE][BOARD_SIZE], bool color){

    int kingX, kingY;

    if(!findKing(board, color, &kingX, &kingY)){
        return NULL;
    }
    int threatMapOppositeTeam [BOARD_SIZE][BOARD_SIZE];
    initializeThreatMap(threatMapOppositeTeam);
    createThreatMap(board, threatMapOppositeTeam, !color);
    if(kingHasThreats(threatMapOppositeTeam, kingX, kingY)){
        return protectKingMoves(board, color, NULL, threatMapOppositeTeam); 
    }

    struct protectorPieceList *protectorPieceList = NULL; 

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(getSymbol(board[i][j]) != 0 && getColor(board[i][j]) == color ){
                struct listCoords* moves = getPieceMoves( j, i, board, NULL , threatMapOppositeTeam); // NULL should be removed later 
                if(moves != NULL){
                    protectorPieceList = addProtectorPiece(j, i, moves, protectorPieceList);
                }  

            }
        }
    }
    return protectorPieceList; 
}

void movePiece(struct piece board[BOARD_SIZE][BOARD_SIZE], int oldX, int oldY, int newX, int newY){
    struct piece emptyPiece = {0, false};
    board[newY][newX] = board[oldY][oldX];
    board[oldY][oldX] = emptyPiece;
}

int evaluate(struct piece board[BOARD_SIZE][BOARD_SIZE]) {
    int score = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int assignedValue;
            switch (getSymbol(board[i][j])) {
                case 'P':
                    assignedValue = PAWN_VALUE;
                    break;
                case 'N':
                    assignedValue = KNIGHT_VALUE;
                    break;
                case 'B':
                    assignedValue = BISHOP_VALUE;
                    break;
                case 'R':
                    assignedValue = ROOK_VALUE;
                    break;
                case 'Q':
                    assignedValue = QUEEN_VALUE;
                    break;
                case 0:
                    assignedValue = 0;
                    break;
            }

            if(getColor(board[i][j])){
                score -= assignedValue;
            }
            else{
                score += assignedValue;
            }
        }
    }
    return score; 
}
int maxi(struct piece board[BOARD_SIZE][BOARD_SIZE], int depth, bool color ) ;
int mini(struct piece board[BOARD_SIZE][BOARD_SIZE], int depth, bool color ) ;

int maxi(struct piece board[BOARD_SIZE][BOARD_SIZE], int depth, bool color) {
    
    
    //struct piece newBoard[BOARD_SIZE][BOARD_SIZE];
    //copyBoard(newBoard, board);
    //int threatMapOppositeTeam[BOARD_SIZE][BOARD_SIZE];
    //initializeThreatMap(threatMapOppositeTeam);
    //createThreatMap(newBoard, threatMapOppositeTeam, color);
    //int x, y; 
    //findKing(board, !color, &x, &y);

    if( checkMateT(board, color)){  //kingHasThreats(threatMapOppositeTeam, x, y)){ //checkMateT(board, !color)  ){ 
        //printf("checkmate\n");
        return -100;
    }

    if (depth == 0) {
        return evaluate(board);
    }

    int max = -50;//INT_MIN; 
    
    struct protectorPieceList* allPieceMoves = getAllMovesP(board, color);
    

    while (allPieceMoves != NULL) {
        struct listCoords* moves = allPieceMoves->moves;

        while(moves != NULL){
            struct piece checkBoard[BOARD_SIZE][BOARD_SIZE];
            copyBoard(checkBoard, board); 
            movePiece(checkBoard, allPieceMoves->x, allPieceMoves->y, moves->x, moves->y );

            int score = mini(checkBoard, depth - 1, !color);
            if (score > max){
                max = score;
            }
            moves = moves->next;
        }
        allPieceMoves = allPieceMoves->next;
    }

    return max;
}

int mini(struct piece board[BOARD_SIZE][BOARD_SIZE], int depth, bool color) {
    //printf("ccccc\n");
    //displayBoardDebug(board);

    //struct piece newBoard[BOARD_SIZE][BOARD_SIZE];
    //copyBoard(newBoard, board);
    //int threatMapCurentTeam[BOARD_SIZE][BOARD_SIZE];
    //initializeThreatMap(threatMapCurentTeam);
    //createThreatMap(newBoard, threatMapCurentTeam, !color);
    //int x, y; 
    //findKing(board, color, &x, &y);
    //printf("king x - %d king y - %d\n", x, y);
    //displayThreatMap(threatMapCurentTeam);

    if( checkMateT(board, color)){ //checkMateT(board, !color)  ){ 
        //printf("checkmate\n");
        return 100;
    }


    
    if (depth == 0) {
        return evaluate(board);
    }

    int min = 50;//INT_MAX; // Use INT_MAX for the worst case for minimizer
    
    struct protectorPieceList* allPieceMoves = getAllMovesP(board, color);

    while (allPieceMoves != NULL) {
        struct listCoords* moves = allPieceMoves->moves;

        while(moves != NULL){
            struct piece checkBoard[BOARD_SIZE][BOARD_SIZE];
            copyBoard(checkBoard, board); 
            movePiece(checkBoard, allPieceMoves->x, allPieceMoves->y, moves->x, moves->y );


            int score = maxi(checkBoard, depth - 1,  !color);

            if (score < min){
                min = score;
            }

            moves = moves->next;
        }
        allPieceMoves = allPieceMoves->next;
    }

    // Free allPieceMoves list here if necessary

    return min;
}

struct moveScoreList*  findBestMove(struct piece chessBoard[BOARD_SIZE][BOARD_SIZE],  bool color) { // bool color
    struct protectorPieceList* allPieceMoves = getAllMoves(chessBoard, color);
    struct moveScoreList* moveScoreList = NULL;

    while (allPieceMoves != NULL) {
        struct listCoords* moves = allPieceMoves->moves;

        while(moves != NULL){
            struct piece checkBoard[BOARD_SIZE][BOARD_SIZE];
            copyBoard(checkBoard, chessBoard); 
            movePiece(checkBoard, allPieceMoves->x, allPieceMoves->y, moves->x, moves->y);
            int score;

            if(!color){
                score = mini(checkBoard, 3, !color);
            }
            else{
                score = maxi(checkBoard, 3, !color);
            }

            moveScoreList = addMoveScoreList( allPieceMoves->x, allPieceMoves->y,  moves->x, moves->y, score, moveScoreList);

            moves = moves->next;
        }
        allPieceMoves = allPieceMoves->next;
    }
    if(!color){
        return findMaxScore(moveScoreList);
    }
    else{
        return findMinScore(moveScoreList);
    }
}



void handleSpecialMoves(struct chessMoveList *chessMoveList, struct piece chessBoard[BOARD_SIZE][BOARD_SIZE], struct piece *emptyPiece) {
    // Check if the last move was an En Passant
    if(chessMoveList->type == EN_PASSANT){
        // Remove the pawn that was captured en passant
        placePiece(getNewX(chessMoveList), getNewY(chessMoveList) + (chessMoveList->turn ? -1 : 1), chessBoard, *emptyPiece);
    }

    // Check if the last move was Castling
    if(chessMoveList->type == CASTLING){
        // Handle queen side castling
        if(getNewX(chessMoveList) == 1){
            placePiece(2, getNewY(chessMoveList), chessBoard, chessBoard[getNewY(chessMoveList)][0]);
            placePiece(0, getNewY(chessMoveList), chessBoard, *emptyPiece);
        }
        // Handle king side castling
        if(getNewX(chessMoveList) == 6){
            placePiece(5, getNewY(chessMoveList), chessBoard, chessBoard[getNewY(chessMoveList)][7]);
            placePiece(7, getNewY(chessMoveList), chessBoard, *emptyPiece);
        }
    }

    // Add other special moves as needed
}

    
// function for the chess game
void chess(){
    // Initialize the chessboard with pieces
    struct piece chessBoard[BOARD_SIZE][BOARD_SIZE];
    initializeBoard(chessBoard);

    // Initialize the list to keep track of chess moves
    struct chessMoveList * chessMoveList = NULL;

    // Define an empty piece to clear specific squares
    struct piece emptyPiece = {0 , false};

    // Clearing specific squares on the chessboard
    placePiece( 1 ,7, chessBoard, emptyPiece);
    placePiece( 2 ,7, chessBoard, emptyPiece);
    placePiece( 4 ,6, chessBoard, emptyPiece);
    placePiece( 5 ,7, chessBoard, emptyPiece);
    placePiece( 6 ,7, chessBoard, emptyPiece);
    
    // Clearing additional squares on the chessboard
    placePiece( 1 ,7, chessBoard, emptyPiece);
    placePiece( 2 ,7, chessBoard, emptyPiece);
    placePiece( 4 ,1, chessBoard, emptyPiece);
    placePiece( 5 ,7, chessBoard, emptyPiece);
    placePiece( 6 ,7, chessBoard, emptyPiece);

    // Boolean to keep track of whose turn it is (false for one player, true for the other)
    bool turn = false;

    //  game loop
    while(true){
        // Display the current state of the chessboard
        displayBoard(chessBoard);

        // Initialize threat maps for both teams
        int threatMapOppositeTeam[BOARD_SIZE][BOARD_SIZE];
        initializeThreatMap(threatMapOppositeTeam);
        createThreatMap(chessBoard, threatMapOppositeTeam, !turn);

        int threatMapCurentTeam[BOARD_SIZE][BOARD_SIZE];
        initializeThreatMap(threatMapCurentTeam);
        createThreatMap(chessBoard, threatMapCurentTeam, turn);

        // Find the position of the king for the current player
        int kingX, kingY;
        findKing(chessBoard, turn, &kingX, &kingY);

        // Initialize a list to keep track of pieces that can protect the king
        struct protectorPieceList *protectorPieceList = NULL; 
        bool kingUnderAttack = false;

        // Check if the king is under attack and if it can be protected
        if(kingHasThreats(threatMapOppositeTeam, kingX, kingY)){
            
            if(canProtectKing(chessBoard, turn, chessMoveList, threatMapOppositeTeam) ){
                protectorPieceList = protectKingMoves(chessBoard, turn, chessMoveList, threatMapOppositeTeam);
                printProtectorPieceList(protectorPieceList);
                printf("\n\n\nking IS Under Attack !!!!\n\n\n\n");
                kingUnderAttack = true;
            } 
            if (kingHasMoves(chessBoard, threatMapOppositeTeam, kingX, kingY)){
                protectorPieceList = addProtectorPiece(kingX, kingY, getKnightMoves(kingX, kingY,chessBoard),protectorPieceList);
            }

            if(!canProtectKing(chessBoard, turn, chessMoveList, threatMapOppositeTeam) && !kingHasMoves(chessBoard, threatMapOppositeTeam, kingX, kingY)) {
                // End the game if the king cannot be protected
                break;
            }
        }
        
        if(turn){
            
            struct moveScoreList* save = findBestMove(chessBoard, turn); // findMaxScore(moveScoreList);
            save->next = NULL;
            //printMoveScoreList(save);
            //displayBoardDebug(chessBoard);
            //placePiece( save->newX , save->newY, chessBoard,  chessBoard[save->oldY][save->oldX]);
            movePiece(chessBoard, save->oldX, save->oldY, save->newX , save->newY);

        }
        // Player's turn to choose a piece
        else{
            int x, y;
            printf("---Please choose a piece---\n");
            while (true){
                // Display the board with protecting pieces if the king is under attack
                if(kingUnderAttack){
                    displayBoardProtectingPieces(chessBoard, protectorPieceList);
                }

                // Handle player input for piece selection
                handelInput(&x, &y); 

                // Validate the selected square
                if(getSymbol(chessBoard[y][x]) == 0 || getColor(chessBoard[y][x]) != turn){
                    printf("Wrong square please try again\n");
                    continue;
                }

                // Check if the selected piece can protect the king if it is under attack
                if(kingUnderAttack && !inCoordProtectorPieceList(x, y, protectorPieceList)){
                    printf("This piece cannot protect King\n");
                    continue; 
                }

                // Get the list of valid moves for the selected piece
                struct listCoords* moves = getPieceMoves(x, y, chessBoard, chessMoveList, threatMapOppositeTeam);

                // Check if the selected piece has any valid moves
                if(listLen(moves) == 0){
                    printf("This piece does not have moves\n");
                    continue;
                }

                // Display the board with possible actions for the selected piece
                displayBoardActions(chessBoard, moves);
                printf("___Please choose a move___\n");
                int newX, newY;
                while (true){
                    // Handle player input for move selection
                    handelInput(&newX, &newY);

                    // Validate the selected move
                    if(!inCoordList(newX, newY, moves)){
                        printf("Wrong move please try again\n");
                        continue;
                    }

                    // Process the selected move
                    struct listCoords* finalMove = returnInCoordList(newX, newY, moves);

                    movePiece(chessBoard, x, y, newX, newY);

                    displayBoard(chessBoard);

                    // Update the chess move list with the new move
                    chessMoveList = addChessMove(x, y, newX, newY, turn, chessBoard[newY][newX], getMoveType(finalMove), chessMoveList);

                    // Handle special moves like En Passant and Castling
                    handleSpecialMoves(chessMoveList, chessBoard, &emptyPiece);

                    // Increment the move count for the moved piece
                    chessBoard[getNewY(chessMoveList)][getNewX(chessMoveList)].moveAmount += 1; 
                    displayBoard(chessBoard);
                    break;
                }

                break;

                // Clean up the list of moves
                freeMoveList(moves);
            }

            // Switch turns
        }
        turn = !turn;

    }

    // Display the winning player
    printf("%s - win\n", turn ? "White":"Black");
}

int main() {
    
    chess();
    printf("end\n");
    printf(WHITE);
    return 0;
}
