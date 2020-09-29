// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include<list>

using namespace std;

class Graph {
    
    public:
        int vertex;
        int flag;
        vector<vector<int>> adj;

        void addVertex(int vertex);
        void addEdge(int edge1, int edge2, int label);
        void BFS(int src, int dest);
};

void Graph :: addVertex(int vertex) {
    this->vertex = vertex;
    adj.resize(vertex);
}
void Graph :: addEdge(int edge1, int edge2, int label) {
    if (label == 1) {
        for (int i = 0; i < adj.size(); i++) {
            for (int j = 0; j < adj[i].size(); j++) {
                adj[i][j] = 0;
            }
        }
    }
   adj[edge1].push_back(edge2);
   adj[edge2].push_back(edge1);
}

void Graph::BFS(int src, int dest) {

    vector<bool> visited(vertex);
    vector<int> predessor(vertex);

    for (int i = 0; i < predessor.size(); i++) {
        predessor[i] = -1;
        visited[i] = false;
    }

    flag = 0;

    list<int> queue;
    visited[src] = true;
    queue.push_back(src);


    while (!queue.empty()) {
        src = queue.front();
        queue.pop_front();
        for (auto i : adj[src]) {
            if (!visited[i]) {
                visited[i] = true;
                predessor[i] = src;
                queue.push_back(i);

                for (int i = 0; i < adj[src].size();i++) {
                    if (adj[src][i] == dest) {
                        flag = 1;
                    }
                    
                }
            }

        }

    }
     if (flag != 1) {
         cout << "path doesn't exist" << endl;
     }
     else {
        vector<int> path;
        path.push_back(dest);
        while(predessor[dest] != -1){
           path.push_back(predessor[dest]);
           dest = predessor[dest];
        }
        for (int i = path.size() - 1; i >= 0; i--) {
            if (i == 0) {
                cout << path[i] << endl;
            }
            else {
                cout << path[i] << "-";
            }
           
        }
     }

}


int main(int argc, char** argv) {
    // Test code. Replaced with your code

    // separator character
    const char comma = ',';
    int vertex_value{}, source{}, destination{};
    int label = 0;
    int empty_set = 0;

    Graph* g1 = new Graph();

    // read from stdin until EOF
    while (!std::cin.eof()) {
        // print a promt

        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);

        std::string num;

        std::istringstream input1(line);
        input1 >> num;

        if (num == "V") {

            label = 0;
            empty_set = 0;
            int addEdgeLabel = 1;
            int count = 0;
            std::vector<vector<unsigned>> matrix;
            
            std::string line1;
            std::getline(std::cin, line1);
            string final_input = line1.substr(line1.find('{'), line1.find('}'));
            std::istringstream input2(final_input);

            std::vector<string> vertex;
            while (!input1.eof()) {
                string value1;
                input1 >> value1;
                vertex.push_back(value1);
            }
            vertex_value = atoi(vertex.back().c_str());

            char num1;
            int num2{}, num3{};
            input2 >> num1;

            // while there are characters in the input line
            while (!input2.eof()) {

                vector<unsigned> row;

                input2 >> num1;
                if (num1 == '}') {
                    break;
                }
                if (num1 == '<') {
                    input2 >> num2;
                }
                if (num1 == '>') {
                    input2 >> num1;
                    if (num1 == '}') {
                        break;
                    }
                    if (num1 == ',')
                        input2 >> num1;
                    if (num1 == '<')
                        input2 >> num2;
                }
                if (input2.fail() || num2 > vertex_value - 1) {
                    std::cerr << "Error: entered edge is wrong, vertex does not exist\n";
                    label = 1;
                    break;
                }
                else {
                    row.push_back(num2);
                    count += 1;
                }
                
                // read a character
                // Note that whitespace is ignored
                char separator;
                input2 >> separator;

                // if error parsing, or if the character is not a comma
                if (input2.fail() || separator != comma) {
                    std::cerr << "Error parsing separator\n";
                    label = 1;
                    break;
                }

                input2 >> num3;
                if (input2.fail() || num3 > vertex_value - 1) {
                    std::cerr << "Error: Entered edge is wrong, vertex does not exist\n";
                    label = 1;
                    break;
                }
                else {
                    row.push_back(num3);
                }
                    

                if (num2 == num3) {
                    std::cerr << "Error: Edge between same vertex is not allowed" << endl;
                    label = 1;
                    break;
                }
                else {
                    matrix.push_back(row);
                }
                if (count == 2) {
                    addEdgeLabel = 0;
                }

                g1->addVertex(vertex_value);
                // done parsing a line, print the numbers
                if (label != 1) {
                    if (!matrix.empty()) {
                        for (int i = 0; i < matrix.size(); i++) {
                            g1->addEdge(matrix[i][0], matrix[i][1], addEdgeLabel);
                        }
                    }
                }
               
            }
            if (matrix.empty()) {
                empty_set = 1;
            }
            for (int i = 0; i < matrix.size(); i++) {
                for (int j = i + 1; j < matrix.size(); j++) {
                    if (!matrix.empty()) {
                        if ((matrix[i][0] == matrix[j][0] && matrix[i][1] == matrix[j][1]) || (matrix[i][0] == matrix[j][1] && matrix[i][1] == matrix[j][0])) {
                            std::cerr << "Error : same edge is not allowed" << endl;
                            label = 1;
                            break;
                        }
                    }
                }
            }
        }  
        else if (num == "s") {
            if (label != 1) {
                vector<string> input;
                while (!input1.eof()) {
                    string value;
                    input1 >> value;
                    input.push_back(value);
                }
                source = atoi(input[0].c_str());
                destination = atoi(input[1].c_str());

                if (source >= vertex_value || destination >= vertex_value) {
                    std::cerr << "Error: Given vertex for shortest path does not exist" << endl;
                }
                else if (empty_set == 1) {
                    std::cerr << "Error: path doesn't exist" << endl;
                }
                else {
                    g1->BFS(source, destination);
                }

            }
            else {
                std::cerr << "Error: Please enter proper set of vertex and edges" << endl;
            }
            
        }
    }
    std::cout << std::endl;
    
}
