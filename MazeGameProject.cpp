#include<iostream>
#include<conio.h>
#include<windows.h>

#define ARROW_DOWN	80		 
#define ARROW_RIGHT	77
#define ARROW_LEFT	75
#define ARROW_UP	72

#define UP			0
#define DOWN		1
#define RIGHT		2
#define LEFT		3

#define X_AXIS		0
#define Y_AXIS		1

#define WIDTH_MAZE	150
#define HEIGHT_MAZE	50

#define SYMBOL_BLOCK	219u
#define SYMBOL_BOMB		'*'	
#define SYMBOL_EMPTY	' '	
#define SYMBOL_PLAYER	'p'
#define SYMBOL_END		'E'

#define CELL_EMPTY	0
#define CELL_BOMB	1
#define CELL_WALL	2
#define CELL_PLAYER 3
#define CELL_END	4

#define NUMB_PATHS	1 // NUMBER OF POSSIBLE PATHS 

using namespace std;

class Cell{
    private:
		int CellType;
		int Self_x, Self_y;
		int Next_x, Next_y;
		int Prev_x, Prev_y;
		bool Path, Check;
		int Neighbors[4][2];

    public:
        Cell(){
            this -> setCell(CELL_EMPTY);
			this -> setPath(false);			
        }

		void setNeighbors(){
			this -> Neighbors[UP][X_AXIS] =	this -> Self_x;
			this -> Neighbors[UP][Y_AXIS] =	this -> Self_y + 1;

			this -> Neighbors[DOWN][X_AXIS] = this -> Self_x;
			this -> Neighbors[DOWN][Y_AXIS] = this -> Self_y - 1;

			this -> Neighbors[RIGHT][X_AXIS] = this -> Self_x + 1;
			this -> Neighbors[RIGHT][Y_AXIS] = this -> Self_y;

			this -> Neighbors[LEFT][X_AXIS] = this -> Self_x - 1;
			this -> Neighbors[LEFT][Y_AXIS] = this -> Self_y;
		}
		
		int getNeighborsX(int index){
			return this -> Neighbors[index][X_AXIS];
		}

		int getNeighborsY(int index){
			return this -> Neighbors[index][Y_AXIS];
		}

        void setCell(int Type){
        	this -> CellType = Type;
        }

		int getCell(){
			return this -> CellType;
		}

		void setPath(bool flag){
			this -> Path = flag;
			this -> setCheck(flag);
		}

		bool getPath(){
			return this -> Path;
		}

		void setCheck(bool flag){
			this -> Check = flag;
		}

		bool getCheck(){
			return this -> Check;
		}

		void setSelfCell(int x, int y){
			this -> Self_x = x;
			this -> Self_y = y;
			this -> setNeighbors();
		}

		void setNextCell(int move){
			if(move == ARROW_DOWN){
				this -> Next_y = this -> Self_y + 1;
				this -> Next_x = this -> Self_x;
			}
			
			if(move == ARROW_UP){
				this -> Next_y = this -> Self_y - 1;
				this -> Next_x = this -> Self_x;
			}
			
			if(move == ARROW_LEFT){
				this -> Next_x = this -> Self_x - 1;
				this -> Next_y = this -> Self_y;
			}

			if(move == ARROW_RIGHT){
				this -> Next_x = this -> Self_x + 1;
				this -> Next_y = this -> Self_y;
			}
			
		}

		int getNextCell_X(){
			return this -> Next_x;
		}

		int getNextCell_Y(){
			return this -> Next_y;
		}

		void setPrevCell(int x, int y){
			this -> Prev_x = x;
			this -> Prev_y = y;
		}

		int getPrevCell_X(){
			return this -> Prev_x;
		}

		int getPrevCell_Y(){
			return this -> Prev_y;
		}

		void printCell(){
			switch(this -> CellType){
				case CELL_EMPTY:
					cout << (char)SYMBOL_EMPTY;
					break;
				
				case CELL_BOMB:
					cout << (char)SYMBOL_BOMB;
					break;
				
				case CELL_WALL:
					cout << (char)SYMBOL_BLOCK;
					break;
				
				case CELL_PLAYER:
					cout << (char)SYMBOL_PLAYER;
					break;
				
				case CELL_END:
					cout << (char)SYMBOL_END;
					break;
			}
		}
};

class Maze{
	private:
		int Px;
		int Py;
		int Ex;
		int Ey;
		bool GameOver;
		Cell Cells[HEIGHT_MAZE][WIDTH_MAZE];
	
		int randCell(){
			int Array[3] = {CELL_WALL, CELL_BOMB, CELL_EMPTY};
			int index 	 = rand() % 3;
			return Array[index];
		}

		int GenerateMove(int Cx, int Cy){
			int PossibleMove[4] = {ARROW_RIGHT, ARROW_DOWN, ARROW_UP, ARROW_LEFT};
			int index	= rand() %  4;
			int Move	=  PossibleMove[index];
			this -> Cells[Cy][Cx].setNextCell(Move);
			int Tx = this -> Cells[Cy][Cx].getNextCell_X(); 
			int Ty = this -> Cells[Cy][Cx].getNextCell_Y();

			if(this -> Cells[Ty][Tx].getCheck() || this -> Cells[Ty][Tx].getCell() == CELL_WALL)
				this -> GenerateMove(Cx, Cy);
			else
				return Move;
		}

		bool CheckDeadEnd(int x, int y){
			bool IsDedadEnd = true;
			int X, Y;
			for(int i = 0; i < 4; i++){
				X = this -> Cells[y][x].getNeighborsX(i);
				Y = this -> Cells[y][x].getNeighborsY(i);
				Cell NCell = this -> Cells[Y][X]; 
				if(NCell.getCell() == CELL_EMPTY && !NCell.getCheck())
					IsDedadEnd = false;
			}
			return IsDedadEnd;
		}

		void StepBack(int Cx, int Cy){
			this -> Cells[Cy][Cx].setPath(false);
			this -> Cells[Cy][Cx].setCheck(true);
			int Px = this -> Cells[Cy][Cx].getPrevCell_X();
			int Py = this -> Cells[Cy][Cx].getPrevCell_Y();
			this -> setPathFrom(Px, Py);
		}

		void setPathFrom(int Cx, int Cy){
			this -> Cells[Cy][Cx].setCheck(true);
			if(Cx == this -> Ex && Cy == this -> Ey);
			
			else if(Cx == this -> Px && Cy == this -> Py){
				for(int row = 0; row < HEIGHT_MAZE; row++){
					for(int column = 0; column < WIDTH_MAZE; column++){
						this -> Cells[row][column].setPath(false);
						this -> Cells[row][column].setCheck(false);
					}
				}
				this -> setPath();
			}

			else if(this -> CheckDeadEnd(Cx, Cy))
				this -> StepBack(Cx, Cy);
			
			else{
			this -> Cells[Cy][Cx].setPath(true);
			int Move = this -> GenerateMove(Cx, Cy);
			this -> Cells[Cy][Cx].setNextCell(Move);

			int Nx = this -> Cells[Cy][Cx].getNextCell_X();
			int Ny = this -> Cells[Cy][Cx].getNextCell_Y();

			this -> Cells[Ny][Nx].setSelfCell(Nx, Ny);
			this -> Cells[Ny][Nx].setPrevCell(Cx, Cy);

			this -> setPathFrom(Nx, Ny);
			}
			
		}

		void setPath(){
			int Cx = this -> Px;
			int Cy = this -> Py;
			this -> Cells[Cy][Cx].setSelfCell(Cx, Cy);
			this -> Cells[Cy][Cx].setPrevCell(Cx, Cy);
			int PossibleMove[2] = {ARROW_RIGHT, ARROW_DOWN};
			int index	= rand() % 2;
			int Move	= PossibleMove[index];
			this -> Cells[Cy][Cx].setPath(true);
			this -> Cells[Cy][Cx].setNextCell(Move);
			
			int Nx = this -> Cells[Cy][Cx].getNextCell_X();
			int Ny = this -> Cells[Cy][Cx].getNextCell_Y();
			
			this -> Cells[Ny][Nx].setSelfCell(Nx, Ny);
			this -> Cells[Ny][Nx].setPrevCell(Cx, Cy);
			
			this -> setPathFrom(Nx, Ny);
		}

		void setField(){
			for(int row = 0; row < HEIGHT_MAZE; row++){
				for(int column = 0; column < WIDTH_MAZE; column++){
					int CellValue	= this -> Cells[row][column].getCell();
					bool isPath		= this -> Cells[row][column].getPath();
					if(CellValue == CELL_WALL || CellValue == CELL_PLAYER || CellValue == CELL_END || isPath)
						continue;
					
					else{
						int CellType = this -> randCell();
						this -> Cells[row][column].setCell(CellType);
					}
				}
			}
		}

		void movePlayer(int key){
			if(key == ARROW_UP){
				this -> Cells[Py][Px].setCell(CELL_EMPTY);
				this -> printxy(Px, Py, SYMBOL_EMPTY);
				this -> Py--;
				this -> Cells[Py][Px].setCell(CELL_PLAYER);
				this -> printxy(Px, Py, SYMBOL_PLAYER);
			}

			if(key == ARROW_DOWN){
				this -> Cells[Py][Px].setCell(CELL_EMPTY);
				this -> printxy(Px, Py, SYMBOL_EMPTY);
				this -> Py++;
				this -> Cells[Py][Px].setCell(CELL_PLAYER);
				this -> printxy(Px, Py, SYMBOL_PLAYER);
			}

			if(key == ARROW_RIGHT){
				this -> Cells[Py][Px].setCell(CELL_EMPTY);
				this -> printxy(Px, Py, SYMBOL_EMPTY);
				this -> Px++;
				this -> Cells[Py][Px].setCell(CELL_PLAYER);
				this -> printxy(Px, Py, SYMBOL_PLAYER);
			}

			if(key == ARROW_LEFT){
				this -> Cells[Py][Px].setCell(CELL_EMPTY);
				this -> printxy(Px, Py, SYMBOL_EMPTY);
				this -> Px--;
				this -> Cells[Py][Px].setCell(CELL_PLAYER);
				this -> printxy(Px, Py, SYMBOL_PLAYER);
			}

		}

		bool winCheck(int NextStep){
			int Nx, Ny;
			Nx = this -> Px;
			Ny = this -> Py;

			if(NextStep == ARROW_UP)
				Ny--;
			if(NextStep == ARROW_DOWN)
				Ny++;
			if(NextStep == ARROW_RIGHT)
				Nx++;
			if(NextStep == ARROW_LEFT)
				Nx--;
			
			if(this -> Cells[Ny][Nx].getCell() == CELL_END)
				return true;
			else
				return false;
		}
		
		bool bombCheck(int NextStep){
			int Nx, Ny;
			Nx = this -> Px;
			Ny = this -> Py;

			if(NextStep == ARROW_UP)
				Ny--;
			if(NextStep == ARROW_DOWN)
				Ny++;
			if(NextStep == ARROW_RIGHT)
				Nx++;
			if(NextStep == ARROW_LEFT)
				Nx--;
			
			if(this -> Cells[Ny][Nx].getCell() == CELL_BOMB)
				return true;
			else
				return false;
		}

		bool wallCheck(int NextStep){
			int Nx, Ny;
			Nx = this -> Px;
			Ny = this -> Py;

			if(NextStep == ARROW_UP)
				Ny--;
			if(NextStep == ARROW_DOWN)
				Ny++;
			if(NextStep == ARROW_RIGHT)
				Nx++;
			if(NextStep == ARROW_LEFT)
				Nx--;
			
			if(this -> Cells[Ny][Nx].getCell() == CELL_WALL)
				return true;
			else
				return false;
		}

		void gotoxy(int x, int y){
			static HANDLE h = NULL;
			if(!h)
				h = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD c = {x, y};
			SetConsoleCursorPosition(h, c);
		}
		
		void showConsoleCursor (bool showFlag) {
			HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(out, &cursorInfo);
			cursorInfo.bVisible = showFlag;
			SetConsoleCursorInfo(out, &cursorInfo);
		}

		void ClearScreen(){
			system("cls");
		}

		void printxy(int x, int y, char c){
			this -> gotoxy(x, y);
			cout << c;
		}

		void setWalls(){
			for(int row = 0; row < HEIGHT_MAZE; row++){
				for(int column = 0; column < WIDTH_MAZE; column++){
					if(row == 0 || row == HEIGHT_MAZE - 1)
						this -> Cells[row][column].setCell(CELL_WALL);
					
					else if(column == 0 || column == WIDTH_MAZE - 1)
						this -> Cells[row][column].setCell(CELL_WALL);
					
					else
						this -> Cells[row][column].setCell(CELL_EMPTY);
				}
			}
		}

		int Getkeyboard(){
			if(kbhit()){
				getch();
				return getch();
			}
			return 0;
		}

		void setPlayer(){
			this -> Px = 1;
			this -> Py = 1;
			this -> Cells[Py][Px].setCell(CELL_PLAYER);
		}

		void setEnd(){
			this -> Ex = WIDTH_MAZE - 2;
			this -> Ey = HEIGHT_MAZE - 2;
			this -> Cells[Ey][Ex].setCell(CELL_END);
		}

	public:
		Maze(){
			this -> showConsoleCursor(false);
			this -> GameOver = false;
			this -> setWalls();
			this -> setPlayer();
			this -> setEnd();
			this -> setPath();
			this -> setField(); // Comment This Line if you want to just See The Solution Path
		}

		bool getGameOver(){
			return this -> GameOver;
		}

		void printWalls(){
			for(int row = 0; row < HEIGHT_MAZE; row++){
				for(int column = 0; column < WIDTH_MAZE; column++)
					this -> Cells[row][column].printCell();
				cout << endl;
			}
		}
		
		void printField(bool PrintPath = false){
			bool IsPath, CellValue;
			for(int row = 0; row < HEIGHT_MAZE; row++){
				for(int column = 0; column < WIDTH_MAZE; column++){
					CellValue	= this -> Cells[row][column].getCell() == CELL_PLAYER;
					IsPath		= this -> Cells[row][column].getPath();
					if(PrintPath && IsPath && !CellValue){
						cout << '+';
						continue;
					}
					this -> Cells[row][column].printCell();
				}
				cout << endl;
			}
		}

		void execute(){
			while(true){
				int KeyboardStat = this -> Getkeyboard();
				
				if(KeyboardStat == 0)
					break;
				
				else if(wallCheck(KeyboardStat))
					break;
				
				else if(winCheck(KeyboardStat)){
					this -> gotoxy(WIDTH_MAZE / 2, HEIGHT_MAZE);
					cout << "WINNER !" << endl;
					this -> GameOver = true;
					break;
				}

				else if(bombCheck(KeyboardStat)){
					this -> gotoxy(WIDTH_MAZE / 2, HEIGHT_MAZE);
					cout <<"LOSER !" <<endl;
					this -> GameOver = true;
					break;
				}
				
				else
					this -> movePlayer(KeyboardStat);
			}
		}
		
};

int main(){
    Maze maze;
	bool gameStat = maze.getGameOver();
	
	maze.printField(!true); //Convert To Ture for find out the solution :)))
	while(!gameStat){
		maze.execute();
		gameStat = maze.getGameOver();
	}
	
	return 0;
}
/*
	Please Read Comments of Line 522 & 456
*/
