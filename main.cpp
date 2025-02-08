#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>
#include <functional>
#include <unordered_set>
#include <queue>
#include <string>

using namespace std;

struct Board{
    vector<vector<int> > board;
    int ucs; // G, uniform cost search
    int heur; //H, heuristic cost for misplaced tiles and manhattan
    int totCost; //F, g+h total cost
    int blankX; //x and y pos for blank spot 
    int blankY;
    shared_ptr<Board> original; //original board
    //default/goal state
    Board(): ucs(0), heur(0), totCost(0), blankX(0), blankY(0){
        board ={{1, 2, 3},{4, 5, 6},{7, 8, 0}};
    }
    //strufct for board only, the G, H and F are found in later funct
    Board(vector<vector<int> > b, int x, int y,shared_ptr<Board> p = nullptr)
        :board(b), ucs(0), heur(0),totCost(0), blankX(x), blankY(y), original(p) {}
    // struct for full board, when prog basically done
    Board(vector<vector<int> > b, int g, int h, int x, int y, shared_ptr<Board> p = nullptr )
    :board(b), ucs(g), heur(h), totCost(g + h), blankX(x), blankY(y), original(p){}
    //goal state

};

struct Node{
    Board state;
    int pathCost; //G cost
    Node(Board curr, int cost):state(curr), pathCost(cost){} //constructor
    bool operator>(const Node& other)const{ //compares priorqueue
        return pathCost >other.pathCost;
    }
};

bool goalState(const Board& board){
    int goal[3][3]{
        {1,2,3},{4,5,6},{7,8,0}
    };
    for(int i =0; i<3; i++){
        for(int j =0; j<3;j++){
            if (board.board[i][j] !=goal[i][j]){
                return false; 
            }
        }
    }
    return true;
}


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

string boardString(const Board& board){//to make eaiser for UCS
    string boardStr = "";
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            boardStr +=to_string(board.board[i][j]) +",";
        }
    }
    return boardStr;
}


void checkDuplicates(const vector<int>&boardNums){
    for(int i =0; i<boardNums.size(); i++){
        for(int j=i+1; j<boardNums.size();j++){
            if(boardNums[i]== boardNums[j]){
                cout<< "Please use each number only once."<<endl;
                exit(0);
            }
        }
    }
}


void printBoard(const Board& boardPrint){
    cout << "\nBoard configuration:\n";
    for (int i =0; i<boardPrint.board.size(); i++){
        for (int j =0; j<boardPrint.board[i].size(); j++){
            cout << boardPrint.board[i][j] << " ";
        }
        cout << endl;
    }
}


Board puzzleRun(int choice){
    vector<vector<int> > board(3, vector<int>(3)); //initalize board for opt 1 or 2
    int blankX = -1;
    int blankY = -1;
    vector<int> boardNums; //array w numbers
    Board boardStart({{}}, -1, -1); //declaring so no issues declaring inside the if statments
    if(choice ==1){
        //take in array/ struct
        cout<< "Enter 9 integers for the board. Enter 0 for the blank space."<<endl;
        for(int i=0; i<3; i++){
            for(int j=0; j <3; j++){
                cin>>board[i][j];
                boardNums.push_back(board[i][j]);
                if(board[i][j] ==0){
                    blankX = i;
                    blankY = j;
                }
            }
        }
        checkDuplicates(boardNums);
        boardStart = Board(board, blankX, blankY);
    } else if(choice ==2){
        //rand function
        vector<Board> boards = randomBoard();
        srand(time(0));
        int randB=(rand() % boards.size()); //finds rand number and chooses that board to be used
        boardStart = boards[randB]; // now it reassigns insteaed of redeclaring which is why wasnt printifng
    }

    return boardStart;
}


vector<Board> Expanding(const Board& board){
    vector<Board> expand;
    int xMoves[4]={-1,1,0,0};
    int yMoves[4]={0,0,-1,1};
    for(int i =0; i<4; i++){
        int x= board.blankX+xMoves[i];
        int y=board.blankY+yMoves[i];
        if(x >=0 and x <3 and y>=0 and y<3){
            vector<vector<int> > newBoard= board.board;
            swap(newBoard[board.blankX][board.blankY],newBoard[x][y]);
            Board child(newBoard,x,y, make_shared<Board>(board));
            expand.push_back(child);
        }
    }
    return expand;
}

Board uniformCS(const Board& boardStart){
    priority_queue<Node, vector<Node>, greater<Node> >pq;
    pq.push(Node(boardStart,0)); //initial board in q
    unordered_set<string> visitedNodes;
    if(pq.empty() == true){
        cout<< "Failure"<<endl;
    }else{
        Node currNode = pq.top();
        pq.pop();

        if(goalState(boardStart) == true){
            cout<< "Goal state!"<<endl;
            //print sol path
            printBoard(currNode.state);
            return currNode.state;
        }
        vector<Board> expandingNodes= Expanding(currNode.state);
        for(int i=0; i< expandingNodes.size(); i++){
            Board& child = expandingNodes[i];
            string compString= boardString(child);

            if(visitedNodes.find(compString)== visitedNodes.end()){
                visitedNodes.insert(compString);
                int ucs = currNode.pathCost +1; //bc each time we move it inc by 1
                //int heur= computeHur(child);
                //pq.push(Node(child, ucs+heur));
            }
        } 
    }
    cout<< "Failure"<<endl;
    return boardStart;
}

void Algorithms(int aChoice, const Board& boardStart){
    if(aChoice == 1){
        uniformCS(boardStart);
    }else if(aChoice ==2){
        // misplaced tiles
    }else if(aChoice == 3){
        //manhattan 
    }
}


int main(){
    int puzzleChoice;
    int algoChoose;
    Board boardStart;
    cout<< "8-puzzle program:"<< endl;
    cout<< "Would you like to: input your own puzzle(1), recieve a random puzzle(2), quit(3)"<<endl;
    cout<< "Input one number at a time followed by the 'enter' key."<< endl;
    cin>> puzzleChoice;
    if(puzzleChoice == 1){
        //input puzzle
        Board boardStart =puzzleRun(puzzleChoice);
    } else if(puzzleChoice == 2){
        //rand puzzle funct
        Board boardStart=puzzleRun(puzzleChoice);
    } else if(puzzleChoice == 3){
        return 0;
    } else{
        cout<< "Invalid input."<< endl;
        return 0;
    }
    cout<< "Thanks!"<<endl;
    cout<< "Now please choose the number of the algorithm you'd like to see."<<endl;
    cout<< "Uniform Cost Search(1), Misplaced Tile heuristic(2), Manhattan Distance heuristic(3), or quit(4)."<<endl;
    cin>> algoChoose;
    if(algoChoose == 1){
        Algorithms(algoChoose, boardStart);
    }else if(algoChoose ==2){
        Algorithms(algoChoose, boardStart);
    }else if(algoChoose == 3){
        Algorithms(algoChoose, boardStart);
    }else if(algoChoose == 4){
        cout<< "Bye-bye "<<endl;
        return 0;
    }else{
        return 0;
    }

    return 0; 
}

