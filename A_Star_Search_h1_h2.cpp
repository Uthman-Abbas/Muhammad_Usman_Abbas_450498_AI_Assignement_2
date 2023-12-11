#include <bits/stdc++.h>

using namespace std;

// Goal state
const int goal[3][3] = { {0, 1, 2}, {3, 4, 5}, {6, 7, 8} };

struct Node {
    int puzzle[3][3];
    int g; // Cost from start to node
    int h; // Heuristic Cost from node to goal 
    int f; // Total Cost (f = g + h)
    Node* parent; // Parent node

    Node(int p[3][3], int g, int h, Node* parent) : g(g), h(h), f(g + h), parent(parent) {
        memcpy(this->puzzle, p, sizeof(this->puzzle)); // Creates copy of node and saves in variable memory
    }
};

// Comparator for priority queue
struct CompareNode {
    bool operator()(Node* n1, Node* n2) {
        return n1->f > n2->f; // Check for cost and returns true or false. Ideally algo should follow lower cost
    }
};

// Function to calculate h1 - Number of misplaced tiles
int calculateH1(const int current[3][3]) {
    int misplaced = 0;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (current[i][j] != goal[i][j] && current[i][j] != 0)
                ++misplaced; // Checks for the tile if its the same as goal state otherwise adds it to already opened
    return misplaced;
}

// Function to calculate h2 - Sum of Manhattan distances
int calculateH2(const int current[3][3]) {
    int distance = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (current[i][j] != 0) {
                int targetX = (current[i][j] - 1) / 3; // We subtract 1 because the numbers in the puzzle go from 1 to 8, and we want them to start from 0
                int targetY = (current[i][j] - 1) % 3; //The % is used to get the remainder after division by 3, which indicates the column position
                distance += abs(i - targetX) + abs(j - targetY);
            }
        }
    }
    return distance;
}

// Check if the current state is the goal state
bool isGoal(const int current[3][3]) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (current[i][j] != goal[i][j])
                return false;
    return true;
}

// Function to print the puzzle
void printPuzzle(const int puzzle[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << puzzle[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl; //returns the whole output path in 3x3 pattern until it reaches goal state
}

// Generate possible moves
vector<Node*> generateChildren(Node* node, int (*calculateH)(const int[3][3])) { //creates empty vecotr that holds child nodes
    vector<Node*> children;
    int x = 0, y = 0;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (node->puzzle[i][j] == 0) {
                x = i;
                y = j;
            } //finds the empty space i.e. 0 in this case

    // Possible moves: up, down, left, right
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i], newY = y + dy[i]; // calculates new coordinates
        if (newX >= 0 && newX < 3 && newY >= 0 && newY < 3) {
            int newPuzzle[3][3];
            memcpy(newPuzzle, node->puzzle, sizeof(newPuzzle));
            swap(newPuzzle[x][y], newPuzzle[newX][newY]); //after finding possible moves in adjacent coordinates it saves the next combination as child node

            int h = calculateH(newPuzzle);
            children.push_back(new Node(newPuzzle, node->g + 1, h, node)); //newly created child to the vector of child nodes, pushback sends it to the back of the vector
        }
    }
    return children;
}

// Function to solve the puzzle using A* search
void solve(int initial[3][3], int (*calculateH)(const int[3][3])) {
    priority_queue<Node*, vector<Node*>, CompareNode> openList; //priority queue store nodes based on total costs (f values) such that the node with the lowest cost comes out on top
    map<vector<int>, bool> closedList; //helps avoid visited places

    // Add initial state to open list
    Node* startNode = new Node(initial, 0, calculateH(initial), nullptr);//creates starting node with initial puzzle config, cost values, and no parent 
    openList.push(startNode);

    while (!openList.empty()) { //enters loop as long as list is not empty
        Node* current = openList.top();
        openList.pop();

        // Check if current state is goal
        if (isGoal(current->puzzle)) {
            cout << "Solution found:" << endl;
            vector<Node*> path;
            while (current != nullptr) {
                path.push_back(current);
                current = current->parent;
            }
            reverse(path.begin(), path.end());
            for (Node* state : path) {
                printPuzzle(state->puzzle);
            }
            return;
        }

        // Add current state to closed list
        vector<int> currVec(&current->puzzle[0][0], &current->puzzle[0][0] + 9); //Converts the current puzzle configuration to a vector for storage in the closed list
        closedList[currVec] = true;

        // Generate children
        vector<Node*> children = generateChildren(current, calculateH);
        for (Node* child : children) {
            vector<int> childVec(&child->puzzle[0][0], &child->puzzle[0][0] + 9); //for each child check whether its in the closed list otherwise adds it to opne list
            if (closedList.find(childVec) == closedList.end()) {
                openList.push(child);
            }
        }
    }

    cout << "No solution found." << endl;
}

int main() {
    int initial[3][3] = { {7, 2, 4}, {1, 0, 8}, {6, 3, 5} };

    cout << "Solving with h1 (Misplaced Tiles)..." << endl;
    solve(initial, calculateH1);

    cout << "Solving with h2 (Manhattan Distance)..." << endl;
    solve(initial, calculateH2);

    return 0;
}
