#include <bits/stdc++.h>
using namespace std;
#include <random>

class Point{
public:
    int x,y;
    Point(){}
    Point(int X,int Y){
        x=X;
        y=Y;
    }
    void print(){
        cout<<x<<" "<<y<<endl;
    }
    void assign(Point p){
        x=p.x;
        y=p.y;
    }
    bool equal(Point a){
        if(a.x==x && a.y==y)return 1;
        else return 0;
    }
    bool operator<(const Point& other) const {
        if(x!=other.x) {
            return x<other.x;
        }
        return y<other.y;
    }
};

class node{
public:
    Point state;
    node* parent;

    node(){}
    node(Point pos){
        state.assign(pos);
        parent = NULL;
    }
    void set_point(Point p){
        state=p;
    }
    void print(){
        cout<<"state : "<<state.x<<" "<<state.y<<endl;
        cout<<"parent : "<<parent->state.x<<" "<<parent->state.y<<endl;
    }
};

node* newNode(Point p){
    node* temp;
    temp->state=p;
    return temp;
}


int board_height = 30;
int board_width = 30;

int max_iter=900;
int max_step=3;

int num_obs = 15;  //number of obstacles
int obs_dim = 5;  //dimension of obstacles

Point start(10,8);
Point goal(20,25);




int getRandomNumber(int min, int max) {
    random_device rd;
    mt19937 mt(rd());

    uniform_int_distribution<int> distribution(min, max);
    return distribution(mt);
}

// determine overlap of obs with start and goal
bool contain(Point first, Point second, int dim){
    if(second.x>=first.x && second.x<=first.x+obs_dim && second.y>=first.y && second.y<=first.y+obs_dim)return 1;
    return 0;
}

// drawing board
void create_board(Point start, Point goal,vector<vector<char>>&board, int board_height, int board_width, int num_obs, vector<Point>&obs, int obs_dim){
    for (int i=0;i<board.size();i++) {
        for (int j=0;j<board[0].size();j++) {
            if(i==0 || j==0 || i==board_height+1 || j==0 || j==board_width+1)board[i][j] = '.';  
        }
    }

    for(int i=0;i<num_obs;i++){
        for(int j=obs[i].x;j<=min(board_height,obs[i].x+obs_dim);j++){
            for(int k=obs[i].y;k<=min(board_width,obs[i].y+obs_dim);k++){
                board[j][k]='.';
            }
        }
    }

    board[start.x][start.y]='S';
    board[goal.x][goal.y]='G';
}

void print_board(vector<vector<char>>&board){
    int l=board.size();
    int w=board[0].size();
    for(int i=0;i<l;i++){
        for(int j=0;j<w;j++)cout<<board[i][j]<<" ";
        cout<<endl;
    }
}

bool canConnect(Point a, Point b,vector<vector<char>>&board){
    int x1=a.x;
    int x2=b.x;
    int y1=a.y;
    int y2=b.y;
    // board[a.x][a.y]='1';
    // board[b.x][b.y]='2';
    if(x2>=x1 && y2>=y1){
        while(x2>x1 && y2>y1){
            if(board[x2][y2]=='.')return 0;
            x2--;
            y2--;
        }
        while(x2>x1){
            if(board[x2][y2]=='.')return 0;
            x2--;
        }
        while(y2>y1){
            if(board[x2][y2]=='.')return 0;
            y2--;
        }
        return 1;
    }
    else if(x2>=x1 && y2<y1){
        while(x2>x1 && y2<y1){
            if(board[x2][y2]=='.')return 0;
            x2--;
            y2++;
        }
        while(x2>x1){
            if(board[x2][y2]=='.')return 0;
            x2--;
        }
        while(y2<y1){
            if(board[x2][y2]=='.')return 0;
            y2++;
        }
        return 1;
    }
    else if(x2<x1 && y2>=y1){
        while(x2<x1 && y2>y1){
            if(board[x2][y2]=='.')return 0;
            x2++;
            y2--;
        }
        while(x2<x1){
            if(board[x2][y2]=='.')return 0;
            x2++;
        }
        while(y2>y1){
            if(board[x2][y2]=='.')return 0;
            y2--;
        }
        return 1;   
    }
    else{
        while(x2<x1 && y2<y1){
            if(board[x2][y2]=='.')return 0;
            x2++;
            y2++;
        }
        while(x2<x1){
            if(board[x2][y2]=='.')return 0;
            x2++;
        }
        while(y2<y1){
            if(board[x2][y2]=='.')return 0;
            y2++;
        }
        return 1;
    }
}

node* connectNode(Point a, Point b, vector<vector<char>>&board, map<Point,node*>&PointToNode){
    node* par = PointToNode[b];
    node* Child = newNode(a);
    PointToNode[a]=Child;

    Child->parent = par;

    int x2=a.x;
    int x1=b.x;
    int y2=a.y;
    int y1=b.y;
    
    if(x2>=x1 && y2>=y1){
        while(x2>x1 && y2>y1){
            board[x2][y2]='>';
            x2--;
            y2--;
        }
        while(x2>x1){
            board[x2][y2]='>';
            x2--;
        }
        while(y2>y1){
            board[x2][y2]='>';
            y2--;
        }
    }
    else if(x2>=x1 && y2<y1){
        while(x2>x1 && y2<y1){
            board[x2][y2]='>';
            x2--;
            y2++;
        }
        while(x2>x1){
            board[x2][y2]='>';
            x2--;
        }
        while(y2<y1){
            board[x2][y2]='>';
            y2++;
        }
    }
    else if(x2<x1 && y2>=y1){
        while(x2<x1 && y2>y1){
            board[x2][y2]='>';
            x2++;
            y2--;
        }
        while(x2<x1){
            board[x2][y2]='>';
            x2++;
        }
        while(y2>y1){
            board[x2][y2]='>';
            y2--;
        }
    }
    else{
        while(x2<x1 && y2<y1){
            board[x2][y2]='>';
            x2++;
            y2++;
        }
        while(x2<x1){
            board[x2][y2]='>';
            x2++;
        }
        while(y2<y1){
            board[x2][y2]='>';
            y2++;
        }
    }
    // board[b.x][b.y]='0';
    return Child;
}

// Using manhattan distance
node* getClosestNode(node* Node, map<Point,node*>&PointToNode, vector<vector<char>>&board, int max_step){
    node* ClosestNode = newNode(Point(-100,-100));
    Point curState = Node->state;
    int curX = curState.x;
    int curY = curState.y;

    for (int i=1;i<=max_step;i++){
        for(int j=max(curX-i,1);j<=min(curX+i,board_height);j++){
            int k1=curY+(i-(abs(j-curX)));
            int k2=curY-(i-(abs(j-curX))); 
            if(k1<=board_width && PointToNode[Point(j,k1)]!=NULL && canConnect(Point(curX,curY),Point(j,k1),board)){
                ClosestNode = connectNode(Point(curX,curY),Point(j,k1), board, PointToNode);
                return ClosestNode;
            } 
            if(k2>=1 && PointToNode[Point(j,k2)]!=NULL && canConnect(Point(curX,curY),Point(j,k2),board)){
                ClosestNode = connectNode(Point(curX,curY),Point(j,k2), board, PointToNode);                
                return ClosestNode;
            }
        }
    }
    return ClosestNode;
}

void backtrack(node* start, node* goal, vector<vector<char>>&board){
    node* cur = goal;
    while(!cur->state.equal(start->state)){
        board[cur->state.x][cur->state.y] = '=';
    }
}

void RRT(int minX, int maxX, int minY, int maxY, vector<vector<char>>&board, map<Point,node*>&PointToNode){
    bool pathFound=0;
    while(max_iter--){

        // Generating random point
        bool flag=1;
        
        while(1){
            int x = getRandomNumber(minX, maxX);
            int y = getRandomNumber(minY, maxY); 
            
            if(board[x][y]!=' ' && board[x][y]!='G')continue;
            node* Node = newNode(Point(x,y));
            node* parent_node = getClosestNode(Node, PointToNode,board, max_step);
            if(parent_node->state.equal(Point(-100,-100)))break;
            
            minX = max(1,min(minX, x-max_step));
            maxX = min(board_height, max(maxX, x+max_step));
            minY = max(1,min(minY, y-max_step));
            maxY = min(board_width, max(maxY, y+max_step));

            if(Node->state.equal(goal))flag=0;
            break;
        }
        if(flag==0){
            pathFound=1;
            break;
        }
    }
    if(pathFound==1){
        cout<<"Path Found..."<<endl;
    }
    else{
        cout<<"Try again...."<<endl;
    }
};

int main() {

    #ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
    
    // Creating board
    vector<vector<char>>board(board_height+2, vector<char>(board_width+2,' '));

    vector<Point>obs;

    for (int i=0;i<num_obs;i++) {
        while(1){
            int x = getRandomNumber(1, board_height);
            int y = getRandomNumber(1, board_width);
            Point obstacle(x,y);
            if(contain(obstacle,start,obs_dim) || contain(obstacle,goal,obs_dim))continue;
            else{
                obs.push_back(obstacle);
                break;
            }    
        }
    }
    create_board(start, goal, board, board_height, board_width, num_obs, obs, obs_dim);
    print_board(board);
    
    map<Point,node*>PointToNode;

    node* first = newNode(start);
    PointToNode[start]=first;

    int minX = max(1, start.x-max_step);
    int maxX = min(board_height, start.x+max_step);
    int minY = max(1, start.y-max_step);
    int maxY = min(board_width, start.y+max_step);
    
    // RRT(minX, maxX, minY, maxY, board, PointToNode);
    board[start.x][start.y]='S';
    board[goal.x][goal.y]='G';
    // print_board(board);

    
    return 0;
}
