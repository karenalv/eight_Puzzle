#include <iostream>
#include <vector>

using namespace std;

struct Board{
    vector<vector<int> > board;
    int ucs; // G, uniform cost search
    int heur; //H, heuristic cost for misplaced tiles and manhattan
    int totCost; //F, g+h total cost
    int blankX; //x and y pos for blank spot 
    int blankY;
    shared_ptr<Board> original; //original board

    //strufct for board only, the G, H and F are found in later funct
    Board(vector<vector<int> > b, int x, int y,shared_ptr<Board> p = nullptr)
        :board(b), ucs(0), heur(0),totCost(0), blankX(x), blankY(y), original(p) {}

    // struct for full board, when prog basically done
    Board(vector<vector<int> > b, int g, int h, int x, int y, shared_ptr<Board> p = nullptr )
    :board(b), ucs(g), heur(h), totCost(g + h), blankX(x), blankY(y), original(p){}
    
};

vector<Board> randomBoard(){
    vector <Board> rands; //boards , we know depth of (test cases)
    rands.push_back(Board({{1,2,3},{4,5,6},{7,8,0}}, 3,3));
    rands.push_back(Board({{1,2,3},{4,5,6},{0,7,8}}, 3,1));
    rands.push_back(Board({{1,2,3},{5,0,6},{4,7,8}}, 2,2));
    rands.push_back(Board({{1,3,6},{5,0,2},{4,7,8}}, 2,2));
    rands.push_back(Board({{1,3,6},{5,0,7},{4,8,2}}, 2,2));
    rands.push_back(Board({{1,6,7},{5,0,3},{4,8,2}}, 2,2));
    rands.push_back(Board({{7,1,2},{4,8,5},{6,3,0}}, 3,3));
    rands.push_back(Board({{0,7,2},{4,6,1},{3,5,8}}, 1,1));
    
    return rands;
}

int puzzleRun(int choice){
    vector<vector<int> > board(3, vector<int>(3)); //initalize board for opt 1 or 2
    int blankX = -1;
    int blankY = -1;
    Board boardStart({{}}, -1, -1); //declaring so no issues declaring inside the if statments
    if(choice ==1){
        //take in array/ struct
        cout<< "Enter 9 integers for the board. Enter 0 for the blank space"<<endl;
        for(int i=0; i<3; i++){
            for(int j=0; j <3; j++){
                cin>>board[i][j];
                if(board[i][j] ==0){
                    blankX = i;
                    blankY = j;
                }
            }
        }
        boardStart = Board(board, blankX, blankY);
        // Board b(board, 2,2,2,2, nullptr);
        // //print board
        // cout << "\nBoard configuration:\n";
        // for (const auto &row : boardStart.board) {
        //     for (int num : row) {
        //         cout << num << " ";
        //     }
        //     cout << endl;
        // }

    }
    else if(choice ==2){
        //rand function
        vector<Board> boards = randomBoard();
        //cout << "Number of predefined boards: " << boards.size() << endl;
        srand(time(0));
        int randB=(rand() % boards.size()); //finds rand number and chooses that board to be used
        //cout << "Selected random board index: " << randB << endl; //makes sure boards are there and bening passed in
        boardStart = boards[randB]; // now it reassigns insteaed of redeclaring which is why wasnt printifng

    }

    cout << "\nBoard configuration:\n";
        for (const auto &row : boardStart.board) {
            for (int num : row) {
                cout << num << " ";
            }
            cout << endl;
        }

    return 0;
}



int main(){
    int puzzleChoice=0;
    cout<< "8-puzzle program:"<< endl;
    cout<< "Would you like to: input your own puzzle(1), recieve a random puzzle(2), quit (3)"<<endl;
    cin>> puzzleChoice;
    if(puzzleChoice == 1){
        //input puzzle
    } else if(puzzleChoice == 2){
        //rand puzzle funct
    } else if(puzzleChoice == 3){
        return 0;
    } else{
        cout<< "Invalid input"<< endl;
        return 0;
    }

    int final = puzzleRun(puzzleChoice);
    //cout<< puzzleChoice;

    

    return 0; 
}

