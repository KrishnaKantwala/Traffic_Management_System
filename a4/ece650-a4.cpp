
#include<iostream>
#include<string>
#include<algorithm>
#include <bits/stdc++.h>
#include<sstream>
#include <minisat/core/Solver.h>
#include <vector>
#include <cstring>
#include <iterator>
#include <list>

using namespace std;

void VertexCover(int vertex, vector<vector<unsigned>> matrix, int length){

    //std::cout << "you are inside vertex cover" << std::endl;
    for(int vcsize=1;vcsize<=vertex;vcsize++){

        Minisat::Solver solver;
        vector<vector<Minisat::Lit>> list(vertex);
        Minisat::Lit l;

        for(int m=0;m<vertex;m++){
            for(int n=0;n<vcsize;n++){
                l = Minisat::mkLit(solver.newVar());
                list[m].push_back(l);
            }
        }
        //std::cout << "you are here" << std::endl;
        //clause 1
        for (int m = 0; m < vcsize; m++) {

            Minisat::vec<Minisat::Lit> l1;
            for (int n= 0; n < vertex; n++){ // n is changing first
                l1.push(list[n][m]);
             }
            solver.addClause(l1);
            l1.clear();
        }
         //std::cout << "you are here1" << std::endl;
        //clause 4
         for (int m = 0; m < length; m++){

            Minisat::vec<Minisat::Lit> l1;

            for (int n = 0; n < vcsize; n++){

                l1.push(list[matrix[m][0]][n]);
                l1.push(list[matrix[m][1]][n]);
            }
            solver.addClause(l1);
            l1.clear();
        }

         //std::cout << "you are here2" << std::endl;

        //Clause 2
        for (int m = 0; m < vertex; m++){
            for (int p = 0; p < vcsize - 1; p++){
                for (int q = p + 1; q < vcsize; q++){

                    solver.addClause(~list[m][p], ~list[m][q]);
                }
            }
        }
        
         //std::cout << "you are here3" << std::endl;

        //Clause 3
        for (int m= 0; m < vcsize; m++){
            for (int p = 0; p < vertex - 1; p++){
                for (int q = p + 1; q < vertex; q++){

                    solver.addClause(~list[p][m], ~list[q][m]);
                }
            }
        }

         //std::cout << "you are here4" << std::endl;
        auto satisfy = solver.solve();
        if(satisfy){

            vector<int> cover;
            for (int m = 0; m < vertex; m++){
                for (int n = 0; n < vcsize; n++){
                    if (Minisat::toInt(solver.modelValue(list[m][n])) == 0) {
                        cover.push_back(m);
                    }
                }
            }
            for(size_t i=0; i < cover.size();i++){

		        std::cout << cover[i];

		        if(i!=cover.size() - 1){

			        std::cout << " ";
		        }
		        else{
			        std::cout << std::endl;
		        }
	        }
            break;
        }

    }
}

int main(int argc, char **argv) {

    const char comma = ',';
    int vertex_value{};
    int label = 0;
    int empty_set = 0;

    //Graph* g1 = new Graph();

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
            }
            if (matrix.empty()) {
                empty_set = 1;
            }
            if(empty_set == 1){
                //pass;
            }
            if(label==1){
                //pass;
            }
            for (size_t i = 0; i < matrix.size(); i++) {
                for (size_t j = i + 1; j < matrix.size(); j++) {
                    if (!matrix.empty()) {
                        if ((matrix[i][0] == matrix[j][0] && matrix[i][1] == matrix[j][1]) || (matrix[i][0] == matrix[j][1] && matrix[i][1] == matrix[j][0])) {
                            std::cerr << "Error : same edge is not allowed" << endl;
                            label = 1;
                            break;
                        }
                    }
                }
            }
            //std::cout << vertex_value << std::endl;
            //std::cout << matrix.size() << std::endl;
            int length = matrix.size();
            VertexCover(vertex_value,matrix,length);
        }
    }
 return 0;
}

