#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


typedef int SudokuBoard[81];    //a sudoku board is represented by an array of 81 integers (going from top row to bottom row, in each from left to right)
                                //0 represents an unknown entry, and valid values are numbers between 1 and 9
typedef int Cell;               //a valid cell is represented by an index between 0 and 80 of the board array
typedef int Val;                //a cell value is a number from between 1 and 9
typedef bool* VCV;              //valid cell values - a boolean array whose 9 entries signify the numbers in 1-9 that are possible for a given cell in a given board
typedef struct stack {int arr[81]; int load;} Stack;

void Push(Stack* stack, int val)
{
    assert(stack->load < 81);
    stack->arr[stack->load] = val;
    stack->load++;
}

int Peek(Stack stack)
{
    assert(stack.load > 0);
    return stack.arr[stack.load-1];
}

int Pop(Stack* stack)
{
    assert(stack->load > 0);
    stack->load--;
    return stack->arr[stack->load];
}

bool IsEmpty(Stack stack) { return stack.load == 0; }

void InitializeStack(Stack* stack)
{
    assert(stack);
    stack->load = 0;
}


bool IsValidVal(Val val) { return 1 <= val && val <= 9; }
bool IsValidCell(Cell cell) { return 0 <= cell && cell < 81; }
bool IsValidVCV(VCV vcv) { return vcv != NULL; }

bool IsValidValue(VCV vcv, Val val) 
{
    assert(IsValidVal(val) && IsValidVCV(vcv));
    return vcv[val-1];
}

void setInvalidValue(VCV vcv, Val val)
{
    assert(IsValidVal(val) && IsValidVCV(vcv));
    vcv[val-1] = false;
}

Cell toCell(int row, int col)
{
    assert(0 <= row && row < 9 && 0 <= col && col < 9);
    return col + 9 * row;
}

void getVCV(SudokuBoard board, Cell cell, VCV result)   //result should be an array of 9 booleans to which we write which values are possible for the cell given the whole board 
{
    assert(IsValidCell(cell) && IsValidVCV(result));

    int i, val, temp, row = cell/9, col = cell%9;

    for(i=0; i<9; i++)
        result[i]=true;

    temp = board[cell];
    board[cell] = 0;
    
    for(i=0; i<9; i++)
    {
        val = board[toCell(row,i)];                         //iterate over column
        if(val != 0) {setInvalidValue(result,val);}
        val = board[toCell(i,col)];                         //iterate over row
        if(val != 0) {setInvalidValue(result,val);}
        val = board[toCell(3*(row/3)+i%3,3*(col/3)+i/3)];   //iterate over block
        if(val != 0) {setInvalidValue(result,val);}
    }

    board[cell] = temp;
}

bool ValidateBoard(SudokuBoard board) {
    bool arr[9];
    Cell cell;
    Val val;
    for(cell=0; cell<81; cell++)
    {
        val = board[cell];
        if(val != 0)
        {
            getVCV(board,cell,arr);
            if(!IsValidValue(arr,val)) return false;
        }
    }
    return true;
}

void PrintBoard(SudokuBoard board) {
	int val, i;
	for(i=0; i<81; i++) {
		val = board[i];
		if(val == 0) printf("_ ");
        else printf("%d ",val);

		if(i%9 == 8) printf("\n");
	}
    printf("\n");
}

int countAndPrintAllSolutions(SudokuBoard board){
    Val val;
    Cell cell;
    bool valid[81*9];

    if(!ValidateBoard(board))
    {
        printf("there are no solutions to the given starting board:\n\n");
        return 0;
    }

    // we use two stacks to keep track of which cells (i.e. indices between 0 and 80) have yet to take values, and to which non given cells we already inserted some value 
    Stack guessed, to_be_guessed;
    InitializeStack(&to_be_guessed);
    InitializeStack(&guessed);

    // Pushing unkown values to the to_be_guessed stack
    for(cell=0; cell<81; cell++)
    {
        val = board[cell];
        if(val == 0)
            Push(&to_be_guessed,cell);
    }

    //check if board is complete
    if(IsEmpty(to_be_guessed))
    {
        PrintBoard(board);
        return 1;
    }

    cell = Pop(&to_be_guessed);
    assert(board[cell] == 0);
    getVCV(board,cell,valid + 9 * cell);
    board[cell] = 1;
    Push(&guessed,cell);

    int numSolutions = 0;
    while (true) {
        assert(!IsEmpty(guessed));
        cell = Peek(guessed);
        val = board[cell];
        assert(1<= val && val <= 10);

        if (val==10) {
            Pop(&guessed);
            board[cell] = 0;
            Push(&to_be_guessed,cell);
            
            if (IsEmpty(guessed)) {
                printf("there are a total of %d solutions to the given starting board:\n\n",numSolutions);
                PrintBoard(board);
                return numSolutions;
            }
            else
                board[Peek(guessed)]++;
        }
        else {
            if(IsValidValue(valid + 9 * cell,board[cell]))
            {
                if (IsEmpty(to_be_guessed)) {
                    assert(ValidateBoard(board));
                    printf("solution number %d:\n\n",numSolutions+1);
                    PrintBoard(board);
                    numSolutions++;
                    board[cell]++;
                }
                else {
                    cell = Pop(&to_be_guessed);
                    assert(board[cell] == 0);
                    getVCV(board,cell,valid + 9 * cell);
                    board[cell] = 1;
                    Push(&guessed,cell);
                }
            }
            else 
                board[cell]++;
        }
    }
}



int main()
{
	SudokuBoard board1 = {
    1,2,3,4,5,8,6,7,9,
    4,5,6,1,7,9,2,3,8,
	7,8,9,2,3,6,1,4,5,
    0,0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,2,
    0,0,0,0,0,0,0,0,3,
	0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0};

    SudokuBoard board2 ={
	0,0,0,0,0,0,0,0,6,
	4,8,0,6,7,0,0,0,0,
	0,6,0,0,1,8,0,0,7,
    0,1,4,0,0,0,0,6,8,
    6,0,0,0,0,0,0,0,5,
    9,7,0,0,0,0,2,4,0,
    8,0,9,3,5,0,0,1,0,
    0,0,0,0,8,7,0,5,2,
    7,0,6,2,0,0,0,0,0
	};

    SudokuBoard board3 ={
    1,5,3,4,9,6,2,0,0,
    4,9,8,2,5,7,0,0,0,
    0,2,6,1,8,3,5,0,0,
    0,4,2,0,1,0,0,0,0,
    9,8,5,6,3,4,0,2,0,
    0,1,7,9,2,0,0,0,0,
    8,3,4,5,7,9,0,0,2,
    2,7,9,3,6,1,0,0,0,
    5,6,1,8,4,2,0,0,3
    };

    //countAndPrintAllSolutions(board1);
	assert(countAndPrintAllSolutions(board2)==7);    
    //printf("all tests passed\n");
    //countAndPrintAllSolutions(board3);
    return 0;
}
