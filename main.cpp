#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>
#include <functional>
#include <unordered_set>
#include <queue>
#include <string>
#include <stack>

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
};

struct Node{
    Board state;
    int pathCost; //G cost
    Node(Board curr, int cost):state(curr), pathCost(cost){} //constructor
    bool operator>(const Node& other)const{ //compares priorqueue
        return pathCost >other.pathCost;
    }
};

vector<Board> randomBoard(){
    vector <Board> rands; //boards , we know depth of (test cases)
    rands.push_back(Board({{1,2,3},{4,5,6},{7,8,0}}, 2,2));
    rands.push_back(Board({{1,2,3},{4,5,6},{0,7,8}}, 2,0));
    rands.push_back(Board({{1,2,3},{5,0,6},{4,7,8}}, 1,1));
    rands.push_back(Board({{1,3,6},{5,0,2},{4,7,8}}, 1,1));
    rands.push_back(Board({{1,3,6},{5,0,7},{4,8,2}}, 1,1));
    rands.push_back(Board({{1,6,7},{5,0,3},{4,8,2}}, 1,1));
    rands.push_back(Board({{7,1,2},{4,8,5},{6,3,0}}, 2,2));
    rands.push_back(Board({{0,7,2},{4,6,1},{3,5,8}}, 0,0));
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
    cout << "Board configuration: "<<endl;
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
    Board boardStart({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, -1, -1); //declaring so no issues declaring inside the if statments
    if(choice ==1){
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
        vector<Board> boards = randomBoard();
        srand(time(0));
        int randB=(rand() % boards.size()); //finds rand number and chooses that board to be used
        boardStart = boards[randB]; // now it reassigns insteaed of redeclaring which is why wasnt printifng
        printBoard(boardStart);
    }
    return boardStart;
}

int misplacedTiles(const Board& boardStart){
    int misplaced=0;
    int goalBoard[3][3]{
        {1,2,3},{4,5,6},{7,8,0}
    };
    for(int i =0; i<3; i++){
        for(int j=0; j<3; j++){
            if(boardStart.board[i][j] != goalBoard[i][j] &&boardStart.board[i][j]!=0){//not epmtpy spot
                misplaced ++;
            }
        }
    }
    return misplaced;
}

int manhattanDistance(const Board& boardStart){
    int Distance=0;
    for(int i =0; i<3; i++){
        for(int j=0;j<3; j++){
            if(boardStart.board[i][j]!= 0){//not empty spot
                int val = boardStart.board[i][j];
                if(val != 0){
                    int x = (val-1)/3;
                    int y =(val-1)%3;
                    Distance += abs(i-x)+abs(j-y);
                }
            }
        }
    }
    return Distance;
}

void pathSol(shared_ptr<Board> goalBoard){
    stack<shared_ptr<Board> > solPath;
    shared_ptr<Board> curr=goalBoard;
    while(curr!=nullptr){
        solPath.push(curr);
        curr=curr->original;
    }
    while(!solPath.empty()){
        printBoard(*solPath.top());
        solPath.pop();
    }
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
            shared_ptr<Board>parentPtr = make_shared<Board>(board);
            Board child(newBoard, x,y, make_shared<Board>(board));
            expand.push_back(child);
        }
    }
    return expand;
}

bool goalState(const Board& board){
    int goal[3][3]={
        {1,2,3},{4,5,6},{7,8,0}
    };
    for(int i =0; i<3; i++){
        for(int j =0; j<3;j++){
            if (board.board[i][j] != goal[i][j]){
                return false; 
            }
        }
    }
    return true;
}

int computeHur(const Board& board, int heurType){//reutnrs h val depending on which algo we use
    if(heurType ==2){
        return misplacedTiles(board);
    }else if(heurType ==3){
        return manhattanDistance(board);
    }else{
        return 0;
    }
}

Board uniformCS(const Board& boardStart, int heurType){
    priority_queue<Node, vector<Node>, greater<Node> >pq;
    unordered_map<string, int> visitedNodes;
    int nodesExpanded =0;
    int maxQueueSize =1;
    int solDepth=0;
    if (goalState(boardStart)){
        cout << "Goal State!" <<endl;
        pathSol(make_shared<Board>(boardStart));
        cout << "Solution depth was" << solDepth<<endl;
        cout << "Number of nodes expanded:" <<nodesExpanded <<endl;
        cout << "Max queue size:"<<maxQueueSize<<endl;
        return boardStart;
    }
    pq.push(Node(boardStart,0)); //initial board in q
    visitedNodes[boardString(boardStart)]=0;
    while(!pq.empty()){
        Node currNode = pq.top();
        pq.pop();
        nodesExpanded++;
        maxQueueSize = max(maxQueueSize, (int)pq.size());
        cout<<"The best state to expand with g(n)= "<<currNode.pathCost;
        cout<< " and h(n)= "<< computeHur(currNode.state, heurType)<< " is... "<<endl;
        cout << " (f(n) = " << currNode.pathCost + computeHur(currNode.state, heurType) << ")" << endl;
        printBoard(currNode.state);
        if(goalState(currNode.state)){
            solDepth = currNode.pathCost;
            cout<< "Goal State!"<<endl;
            pathSol(make_shared<Board>(currNode.state));
            cout << "Solution depth: " << solDepth << endl;
            cout << "Number of nodes expanded: " << nodesExpanded << endl;
            cout << "Max queue size: " << maxQueueSize << endl;
            return currNode.state;
        }
        vector<Board> expandingNodes= Expanding(currNode.state);
        for(int i=0; i< expandingNodes.size(); i++){
            Board& child = expandingNodes[i];
            string compString= boardString(child);
            int ucs= currNode.pathCost + 1;
            int heur = computeHur(child, heurType);
            int totCost = ucs;
            if (visitedNodes.find(compString) ==visitedNodes.end() || ucs < visitedNodes[compString]){
                visitedNodes[compString] =ucs; 
                Board updatedChild(child.board,ucs,heur,child.blankX,child.blankY, make_shared<Board>(currNode.state));
                pq.push(Node(updatedChild, totCost));
            }
        } 
    }
    cout<< "Failure UCS"<<endl;
    return boardStart;
}

void Algorithms(int aChoice, const Board& boardStart){
    Board result;
    if(aChoice == 1){
        result= uniformCS(boardStart,0);
    }else if(aChoice ==2){
        result=uniformCS(boardStart,2);
    }else if(aChoice == 3){
        result=uniformCS(boardStart,3);
    }
    printBoard(result);
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
        boardStart =puzzleRun(puzzleChoice);
    } else if(puzzleChoice == 2){
        boardStart=puzzleRun(puzzleChoice);
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