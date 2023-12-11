#include <bits/stdc++.h> //includes all standard libraries
using namespace std;

const int N = 3;
const vector<vector<int>> goalState = { {0, 1, 2}, {3, 4, 5}, {6, 7, 8} }; //creates vector for goal state

struct Node {
    vector<vector<int>> state;
    int cost;
    string path; //creates structure for node (no need for h function like in A* search)
};

// Function to convert a state to a string (for path tracking)
string stateToString(const vector<vector<int>>& state) { // this func concatenates the individual elements of the 2D vector to create a single string
    string s = "";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            s += to_string(state[i][j]);
        }
    }
    return s;
}

// Function to calculate the number of misplaced tiles
int h1(const vector<vector<int>>& current) {
    int misplaced = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (current[i][j] != goalState[i][j] && current[i][j] != 0)
                misplaced++; //checks for heuristic
        }
    }
    return misplaced;
}

// Function to calculate the sum of distances of tiles from their goal positions
int h2(const vector<vector<int>>& current) {
    int distanceSum = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (current[i][j] != 0) {
                int goalX = current[i][j] / N;
                int goalY = current[i][j] % N;
                distanceSum += abs(i - goalX) + abs(j - goalY);
            }
        }
    }
    return distanceSum; //same logic as in A*
}

// Comparator for the priority queue
struct compareNode {
    bool operator()(const Node& n1, const Node& n2) {
        return n1.cost > n2.cost; 
    }
};

// Function to generate successors of a node
vector<Node> generateSuccessors(const Node& node, int (*heuristic)(const vector<vector<int>>&)) {
    vector<Node> successors; //takes a node
    int zeroX, zeroY;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (node.state[i][j] == 0) {
                zeroX = i;
                zeroY = j;
                break; //finds empty space or 0
            }
        }
    }

    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };
    for (int i = 0; i < 4; ++i) {
        int newX = zeroX + dx[i];
        int newY = zeroY + dy[i]; //possible moves of empty space or 0
        if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
            vector<vector<int>> newState = node.state;
            swap(newState[zeroX][zeroY], newState[newX][newY]);
            successors.push_back({ newState, heuristic(newState), node.path + stateToString(newState) + " " });
        }
    }
    return successors;
}

// Function to perform the Greedy Search
void greedySearch(const vector<vector<int>>& initialState, int (*heuristic)(const vector<vector<int>>&)) {
    priority_queue<Node, vector<Node>, compareNode> pq; //function takes an initial state, a heuristic function, and performs a Greedy Search
    map<string, bool> visited;// Initializes a map to keep track of visited states. The key is a string representation of state, value is boolean for whether the state has been visited
    pq.push({ initialState, heuristic(initialState), stateToString(initialState) + " " }); // Pushes the initial state into the priority queue as a node with its heuristic value

    while (!pq.empty()) { //checks if priority queue is empty or not, runs until pq gets empty
        Node current = pq.top(); //checks for lower cost
        pq.pop(); //removes current node from pq

        if (visited[stateToString(current.state)]) {
            continue;
        }

        visited[stateToString(current.state)] = true;

        if (current.state == goalState) {
            cout << "Goal Reached!" << endl;
            cout << "Path: " << current.path << endl;
            return;
        }

        vector<Node> successors = generateSuccessors(current, heuristic); //generate more nodes and adds after checking if they havent been previously visited
        for (auto& successor : successors) {
            if (!visited[stateToString(successor.state)]) {
                pq.push(successor);
            }
        }
    }

    cout << "Solution not found." << endl;
}

int main() {
    vector<vector<int>> initialState = { {8, 0, 6}, {5, 4, 7}, {2, 3, 1} };

    cout << "Solving with h1 (Misplaced Tiles):" << endl;
    greedySearch(initialState, h1);

    cout << "Solving with h2 (Sum of Distances):" << endl;
    greedySearch(initialState, h2);

    return 0;
}
