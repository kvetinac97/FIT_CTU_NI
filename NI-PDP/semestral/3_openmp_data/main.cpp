#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// Helper macro for OpenMP

size_t numThreads = 8;

// Constants

const char COLOR_BLACK = 0;
const char COLOR_WHITE = 1;
const char COLOR_UNDEFINED = 2;

const int MAX_EDGES = 400;
const int MAX_VERTICES = 30;

// Global variables

size_t currentBest = 0;

// Helper structures for edges and states

struct Edge {
    int x, y, w;
    bool operator > ( const Edge & other ) const {
        return w > other.w;
    }
};

struct State {
    Edge matrix [MAX_EDGES];
    size_t matrixLen, matrixPos;
    char colors [MAX_VERTICES];
    size_t weightSum, topTip;
};

// Inner recursion function
void weight_inner_bfs ( queue<State> & q ) {
    State state = q.front();
    q.pop();

    if ( state.topTip + state.weightSum <= currentBest )
        return; // cannot be better, stop
    
    if ( state.matrixPos == state.matrixLen ) {
        if (state.weightSum > currentBest)
            currentBest = state.weightSum; // update best
        return; // reached all edges
    }
    
    // Helper variables
    int xCoord = state.matrix[state.matrixPos].x,
        yCoord = state.matrix[state.matrixPos].y,
        wei = state.matrix[state.matrixPos].w;
    char xColor = state.colors[xCoord],
         yColor = state.colors[yCoord];
    
    // Increment state
    state.matrixPos += 1;
    state.topTip -= wei;
    
    // same color = cannot add edge
    if ( xColor != COLOR_UNDEFINED && xColor == yColor ) {
        q.push(state);
        return weight_inner_bfs(q);
    }
    
    // different color = must add edge
    if ( xColor != COLOR_UNDEFINED && yColor != COLOR_UNDEFINED && xColor != yColor ) {
        state.weightSum += wei;
        q.push(state);
        return weight_inner_bfs(q);
    }
    
    // Try both colorings
    if ( xColor == COLOR_UNDEFINED && yColor == COLOR_UNDEFINED ) {
        // First coloring
        state.colors[xCoord] = COLOR_BLACK;
        
        // Do not add edge
        state.colors[yCoord] = COLOR_BLACK;
        q.push(state);
        
        // Add edge
        state.colors[yCoord] = COLOR_WHITE;
        state.weightSum += wei;
        q.push(state);
        state.weightSum -= wei;
        
        // Second coloring
        state.colors[xCoord] = COLOR_WHITE;
        
        // Do not add edge
        state.colors[yCoord] = COLOR_WHITE;
        q.push(state);
        
        // Add edge
        state.colors[yCoord] = COLOR_BLACK;
        state.weightSum += wei;
        q.push(state);
        state.weightSum -= wei;
    }
    // Fill x based on y
    else if ( xColor == COLOR_UNDEFINED ) {
        // Do not add edge
        state.colors[xCoord] = yColor;
        q.push(state);
        
        // Add edge
        state.colors[xCoord] = yColor == COLOR_BLACK ? COLOR_WHITE : COLOR_BLACK;
        state.weightSum += wei;
        q.push(state);
        state.weightSum -= wei;
    }
    // Fill y based on x
    else {
        // Do not add edge
        state.colors[yCoord] = xColor;
        q.push(state);
        
        // Add edge
        state.colors[yCoord] = xColor == COLOR_WHITE ? COLOR_BLACK : COLOR_WHITE;
        state.weightSum += wei;
        q.push(state);
        state.weightSum -= wei;
    }
}

// Sequential recursion solution function
void weight_seq_rec ( State state ) {
    if ( state.topTip + state.weightSum <= currentBest )
        return; // cannot be better, stop

    if ( state.matrixPos == state.matrixLen ) {
        if (state.weightSum > currentBest)
            #pragma omp critical
            if (state.weightSum > currentBest)
                currentBest = state.weightSum; // update best
        return; // reached all edges
    }

    // Helper variables
    int xCoord = state.matrix[state.matrixPos].x,
            yCoord = state.matrix[state.matrixPos].y,
            wei = state.matrix[state.matrixPos].w;
    char xColor = state.colors[xCoord],
            yColor = state.colors[yCoord];

    // Increment state
    state.matrixPos += 1;
    state.topTip -= wei;

    // same color = cannot add edge
    if ( xColor != COLOR_UNDEFINED && xColor == yColor )
        return weight_seq_rec(state);

    // different color = must add edge
    if ( xColor != COLOR_UNDEFINED && yColor != COLOR_UNDEFINED && xColor != yColor ) {
        state.weightSum += wei;
        return weight_seq_rec(state);
    }

    // Try both colorings
    if ( xColor == COLOR_UNDEFINED && yColor == COLOR_UNDEFINED ) {
        // First coloring
        state.colors[xCoord] = COLOR_BLACK;

        // Do not add edge
        state.colors[yCoord] = COLOR_BLACK;
        weight_seq_rec(state);

        // Add edge
        state.colors[yCoord] = COLOR_WHITE;
        state.weightSum += wei;
        weight_seq_rec(state);
        state.weightSum -= wei;

        // Second coloring
        state.colors[xCoord] = COLOR_WHITE;

        // Do not add edge
        state.colors[yCoord] = COLOR_WHITE;
        weight_seq_rec(state);

        // Add edge
        state.colors[yCoord] = COLOR_BLACK;
        state.weightSum += wei;
        weight_seq_rec(state);
        state.weightSum -= wei;
    }
        // Fill x based on y
    else if ( xColor == COLOR_UNDEFINED ) {
        // Do not add edge
        state.colors[xCoord] = yColor;
        weight_seq_rec(state);

        // Add edge
        state.colors[xCoord] = yColor == COLOR_BLACK ? COLOR_WHITE : COLOR_BLACK;
        state.weightSum += wei;
        weight_seq_rec(state);
        state.weightSum -= wei;
    }
        // Fill y based on x
    else {
        // Do not add edge
        state.colors[yCoord] = xColor;
        weight_seq_rec(state);

        // Add edge
        state.colors[yCoord] = xColor == COLOR_WHITE ? COLOR_BLACK : COLOR_WHITE;
        state.weightSum += wei;
        weight_seq_rec(state);
        state.weightSum -= wei;
    }
}


// Wrapper recursion function
size_t weight ( const vector<Edge> & matrix, size_t vertexCount ) {
    // Create initial state
    State initial = {};
    initial.matrixLen = matrix.size();
    std::copy(matrix.begin(), matrix.end(), initial.matrix);

    initial.matrixPos = 0;
    initial.weightSum = 0;
    
    for ( size_t i = 0; i < vertexCount; ++i )
        initial.colors[i] = COLOR_UNDEFINED;
    
    initial.topTip = 0;
    for ( const auto & edge : matrix )
        initial.topTip += edge.w;

    queue<State> q;
    q.push(initial);

    while (q.size() < numThreads * 2) {
        weight_inner_bfs(q);
    }
    vector<State> copy;
    while (!q.empty()) {
        copy.emplace_back(q.front());
        q.pop();
    }

    size_t i;
    #pragma omp parallel for private(i) default(shared) num_threads(numThreads)
    for (i = 0; i < copy.size(); i++) {
        State state = copy[i];
        weight_seq_rec(state);
    }
    
    return currentBest; // return best result
}

int main(int argc, char ** argv) {
    if (argc > 1)
        numThreads = atoi(argv[1]);
    cout << "=======================" << endl;
    cout << "[OMP] Using " << numThreads << " threads" << endl;

    ifstream ifs ("/home/wrzecond/graf.out");
    istream & is = ifs;

    // Load vertex count
    int vertexCount;
    if (!(is >> vertexCount) || vertexCount > MAX_VERTICES) {
        cerr << "Could not load vertex count or it is too big." << endl;
        return 1;
    }

    // Load matrix
    int x = 0;
    vector<Edge> matrix;

    for (int i = 0; i < vertexCount * vertexCount; ++i) {
        is >> x;
        if (x && (i % vertexCount) < (i / vertexCount)) // put only important edges
            matrix.emplace_back ( Edge { i / vertexCount, i % vertexCount, x } );
    }
    
    // Sort by edge weight (descending)
    std::sort(matrix.begin(), matrix.end(), greater<>());
    cout << "Edges: " << matrix.size() << endl;

    if (is.fail() || matrix.size() > MAX_EDGES) {
        cerr << "Could not load matrix or matrix has too many edges." << endl;
        return 1;
    }

    cout << "Loaded matrix." << endl;

    auto t1 = chrono::high_resolution_clock::now();
    cout << "Weight: " << weight(matrix, vertexCount) << endl;
    auto t2 = chrono::high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    chrono::duration<double, std::milli> ms_double = t2 - t1;
    cout << "Time: " << ms_double.count() << " ms" << endl;
    cout << "=======================" << endl;

    return 0;
}
