#include <iostream>
#include <vector>

using namespace std;

struct Board{
    vector<vector<int>> board;
    int ucs; // G, uniform cost search
    int heur; //H, heuristic cost for misplaced tiles and manhattan
    int totCost; //F, g+h total cost
    int blankX; //x and y pos for blank spot 
    int blankY;
    shared_ptr<Board> original; //original board

    Board(vector<vector<int>> b, int g, int h, int x, int y, shared_ptr<Board> p = nullptr)
    :board(b), ucs(g), heur(h), totCost(g + h), blankX(x), blankY(y), original(p){}
    
};

int puzzleRun(int choice){
    vector<vector<int>> board(3, vector<int>(3)); //initalize board for opt 1 or 2
    if(choice ==1){
        //take in array/ struct
        cout<< "Enter 9 integers for the board. Enter 0 for the blank space"<<endl;

    }
    else if(choice ==2){
        //rand function
    }
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

