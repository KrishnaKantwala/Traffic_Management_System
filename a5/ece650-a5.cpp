// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

// defined std::cout
#include <iostream>
#include <queue>
#include <tuple>
#include <string>
#include <iterator>
#include <regex>
#include <algorithm>
#include <pthread.h>
#include <ctime>
#include <mutex>

using namespace std;

struct graphStruct {
    vector<std::tuple<int, int>> graph;
    int number_of_vertices{};
    vector<int> result;
};

void *CNFSatVC(void *arguments) {
    auto *g = (struct graphStruct *) arguments;
    vector<std::tuple<int, int>> graph = g->graph;
    int number_of_vertices = g->number_of_vertices;
    // -- allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    bool solverResult = false;
    for (int i = 1; i <= number_of_vertices; ++i) {
        if (!solverResult) {
            vector<Minisat::vec<Minisat::Lit>> kcnfClauses;
            vector<Minisat::vec<Minisat::Lit>> threecnfClauses;
            Minisat::Lit minsatLiterals[number_of_vertices][i];
            for (int n = 1; n <= number_of_vertices; ++n) {
                for (int k = 1; k <= i; ++k) {
                    Minisat::Lit l{};
                    l = Minisat::mkLit(solver->newVar());
                    minsatLiterals[n - 1][k - 1] = l;
                }
            }
            for (int k = 1; k <= i; ++k) {
                Minisat::Lit tempLit[number_of_vertices];
                Minisat::vec<Minisat::Lit> tempLit1;
                for (int n = 1; n <= number_of_vertices; ++n) {
                    tempLit[n - 1] = minsatLiterals[n - 1][k - 1];
                    tempLit1.push(minsatLiterals[n - 1][k - 1]);
                }
                while(tempLit1.size() > 3){
                    Minisat::Lit l{};
                    l = Minisat::mkLit(solver->newVar());
                    Minisat::vec<Minisat::Lit> threecnfClause;
                    threecnfClause.push(l);
                    threecnfClause.push(tempLit1[0]);
                    threecnfClause.push(tempLit1[1]);
                    solver->addClause(threecnfClause);
                    Minisat::vec<Minisat::Lit> tempLit2;
                    for (int j = 2; j < tempLit1.size(); ++j) {
                        tempLit2.push(tempLit1[j]);
                    }
                    tempLit1.clear();
                    tempLit2.push(~l);
                    for (int m = 0; m < tempLit2.size(); ++m) {
                        tempLit1.push(tempLit2[m]);
                    }

                }
                Minisat::vec<Minisat::Lit> threecnfClause;
                for (int i1 = 0; i1 < tempLit1.size(); ++i1) {
                    threecnfClause.push(tempLit1[i1]);
                }
                solver->addClause(threecnfClause);
            }
            for (int n = 1; n <= number_of_vertices; ++n) {
                for (int k = 1; k <= i; ++k) {
                    for (int j = k + 1; j <= i; ++j) {
                        solver->addClause(~minsatLiterals[n - 1][k - 1], ~minsatLiterals[n - 1][j - 1]);
                    }
                }
            }
            for (int k = 1; k <= i; ++k) {
                for (int n = 1; n <= number_of_vertices; ++n) {
                    for (int j = n + 1; j <= number_of_vertices; ++j) {
                        solver->addClause(~minsatLiterals[n - 1][k - 1], ~minsatLiterals[j - 1][k - 1]);
                    }
                }
            }
            for (auto &m : graph) {
                Minisat::vec<Minisat::Lit> gPart;
                for (int k = 1; k <= i; ++k) {
                    gPart.push(minsatLiterals[get<0>(m)][k - 1]);
                    gPart.push(minsatLiterals[get<1>(m)][k - 1]);
                }
                while(gPart.size() > 3){
                    Minisat::Lit l{};
                    l = Minisat::mkLit(solver->newVar());
                    Minisat::vec<Minisat::Lit> threecnfClause;
                    threecnfClause.push(l);
                    threecnfClause.push(gPart[0]);
                    threecnfClause.push(gPart[1]);
                    solver->addClause(threecnfClause);
                    Minisat::vec<Minisat::Lit> tempLit2;
                    for (int j = 2; j < gPart.size(); ++j) {
                        tempLit2.push(gPart[j]);
                    }
                    gPart.clear();
                    tempLit2.push(~l);
                    for (int m = 0; m < tempLit2.size(); ++m) {
                        gPart.push(tempLit2[m]);
                    }

                }
                Minisat::vec<Minisat::Lit> threecnfClause;
                for (int i1 = 0; i1 < gPart.size(); ++i1) {
                    threecnfClause.push(gPart[i1]);
                }
                solver->addClause(threecnfClause);
//                solver->addClause(gPart);
            }
            solverResult = solver->solve();
            if (solverResult) {
                vector<int> result;
                for (int n = 1; n <= number_of_vertices; ++n) {
                    for (int k = 1; k <= i; ++k) {
                        if (!Minisat::toInt(solver->modelValue(minsatLiterals[n - 1][k - 1]))) {
                            result.push_back(n - 1);
                        }
                    }
                }
                sort(result.begin(), result.end());
                g->result = result;
            }
            solver.reset(new Minisat::Solver());
        }

    }
    return nullptr;
}


void *approxVC1(void *arguments) {
    auto *g = (struct graphStruct *) arguments;
    vector<std::tuple<int, int>> graph = g->graph;
    int number_of_vertices = g->number_of_vertices;
    vector<int> result;
    int vertexDegree[number_of_vertices];
    for (int i = 0; i < number_of_vertices; ++i) {
        vertexDegree[i] = 0;
    }
    vector<std::tuple<int, int>> G_temp;
    for (auto &m : graph) {
        vertexDegree[get<0>(m)]++;
        vertexDegree[get<1>(m)]++;
        G_temp.push_back(m);
    }
    while (!G_temp.empty()) {
        int *vertexWithMaxDegreeIndex = max_element(vertexDegree, vertexDegree + number_of_vertices);
        if (vertexDegree[vertexWithMaxDegreeIndex - vertexDegree] > 0) {
            result.push_back(int(vertexWithMaxDegreeIndex - vertexDegree));
            vertexDegree[vertexWithMaxDegreeIndex - vertexDegree] = 0;
            for (auto &i : graph) {
                if (get<0>(i) == vertexWithMaxDegreeIndex - vertexDegree ||
                    get<1>(i) == vertexWithMaxDegreeIndex - vertexDegree) {
                    G_temp.erase(remove(G_temp.begin(), G_temp.end(), i), G_temp.end());
                }
            }
            graph.clear();
            for (auto &m : G_temp) {
                graph.push_back(m);
            }
        }
        for (int i = 0; i < number_of_vertices; ++i) {
            vertexDegree[i] = 0;
        }
        for (auto &m : graph) {
            vertexDegree[get<0>(m)]++;
            vertexDegree[get<1>(m)]++;
        }
    }
    sort(result.begin(), result.end());
    g->result = result;
    return nullptr;
}


void *approxVC2(void *arguments) {
    auto *g = (struct graphStruct *) arguments;
    vector<std::tuple<int, int>> graph = g->graph;
    vector<int> result;
    vector<std::tuple<int, int>> G_temp;
    for (auto &m : graph) {
        G_temp.push_back(m);
    }
    while (!G_temp.empty()) {
        std::tuple<int, int> pickedEdge = graph.front();
        int u = get<0>(pickedEdge);
        int v = get<1>(pickedEdge);
        result.push_back(u);
        result.push_back(v);
        for (auto m: graph) {
            if (get<0>(m) == u || get<1>(m) == u || get<0>(m) == v || get<1>(m) == v) {
                G_temp.erase(remove(G_temp.begin(), G_temp.end(), m), G_temp.end());
            }
        }
        graph.clear();
        for (auto &m : G_temp) {
            graph.push_back(m);
        }
    }
    sort(result.begin(), result.end());
    g->result = result;
    return nullptr;
}

int main() {
    int number_of_vertices = 0;
    vector<std::tuple<int, int>> G;
    string command;
    while (!std::cin.eof()) {
        try {
            getline(cin, command);

            if (!command.empty()) {
                istringstream iss(command);
                vector<string> tokens{istream_iterator<string>{iss},
                                      istream_iterator<string>{}};
                if (tokens[0] == "V") {
                    tokens.erase(tokens.begin());
                    number_of_vertices = stoi(tokens[0]);
                    tokens.erase(tokens.begin());
                    if (!tokens.empty()) {
                        cerr << "Error: incorrect input format \n";
                    }
                } else if (tokens[0] == "E") {
                    G.clear();
                    tokens.erase(tokens.begin());
                    string edges;
                    edges = tokens[0];
                    std::string subject(edges);
                    tokens.erase(tokens.begin());
                    if (!tokens.empty()) {
                        cerr << "Error: incorrect input format \n";
                    } else {
                        try {
                            std::regex re("<(.+?)>");
                            std::sregex_iterator next(subject.begin(), subject.end(), re);
                            std::sregex_iterator end;
                            while (next != end) {
                                std::smatch match = *next;
                                string point;
                                stringstream ss(point);
                                point = match.str(1);
                                if (point.length() > 0) {
                                    std::vector<std::string> comma_separated;
                                    std::string digit;
                                    std::istringstream tokenStream(point);
                                    while (std::getline(tokenStream, digit, ',')) {
                                        comma_separated.push_back(digit);
                                    }
                                    tuple<int, int> coordinate;
                                    if ((number_of_vertices - 1) < stod(comma_separated[0]) ||
                                        stod(comma_separated[0]) < 0) {
                                        cerr << "Error: this vertex is invalid: " << comma_separated[0] << endl;
                                        break;
                                    }
                                    if ((number_of_vertices - 1) < stod(comma_separated[1]) ||
                                        stod(comma_separated[1]) < 0) {
                                        cerr << "Error: this vertex is invalid: " << comma_separated[1] << endl;
                                        break;
                                    }
                                    coordinate = make_tuple(stod(comma_separated[0]), stod(comma_separated[1]));
                                    G.push_back(coordinate);
                                    next++;
                                }

                            }
                            if (!G.empty()) {
                                pthread_t th1;
                                pthread_t th2;
                                pthread_t th3;
                                struct graphStruct CNFSatVCargs, approxVC1args, approxVC2args;
                                CNFSatVCargs.graph = G;
                                approxVC1args.graph = G;
                                approxVC2args.graph = G;
                                CNFSatVCargs.number_of_vertices = number_of_vertices;
                                approxVC1args.number_of_vertices = number_of_vertices;
                                approxVC2args.number_of_vertices = number_of_vertices;
                                pthread_create(&th1, nullptr, &CNFSatVC, (void *) &CNFSatVCargs);
                                pthread_create(&th2, nullptr, &approxVC1, (void *) &approxVC1args);
                                pthread_create(&th3, nullptr, &approxVC2, (void *) &approxVC2args);

                                pthread_join(th1, nullptr);
                                pthread_join(th2, nullptr);
                                pthread_join(th3, nullptr);

                                cout << "CNF-SAT-VC: ";
                                for (auto vertex : CNFSatVCargs.result) {
                                    if (vertex == CNFSatVCargs.result.back()) {
                                        cout << vertex << endl;
                                    } else {
                                        cout << vertex << ",";
                                    }

                                }
                                cout << "APPROX-VC-1: ";
                                for (auto vertex : approxVC1args.result) {
                                    if (vertex == approxVC1args.result.back()) {
                                        cout << vertex << endl;
                                    } else {
                                        cout << vertex << ",";
                                    }

                                }

                                cout << "APPROX-VC-2: ";
                                for (auto vertex : approxVC2args.result) {
                                    if (vertex == approxVC2args.result.back()) {
                                        cout << vertex << endl;
                                    } else {
                                        cout << vertex << ",";
                                    }

                                }

                            } else {
                                cout << "\n";
                                cout << "\n";
                                cout << "";
                            }

                        } catch (std::regex_error &e) {
                            cerr << "Error: this vertex is invalid: \n";
                        }
                    }

                }
            }
        } catch (...) {
            cerr << "Error: incorrect input format \n";
            cerr << "Error: unexpected error happened \n";
        }
    }


    return 0;
}