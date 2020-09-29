#include <iostream>
#include <fstream>
#include<time.h>
#include<vector>
#include<algorithm>
#include<unistd.h>
#include<stdio.h>

using namespace std;

class graph {
    int svalue, nvalue, lvalue, cvalue;
    vector <vector<int>> cordinates_match = {};
    int cor1, cor2;
    int streetcounter;
    vector <vector<int>> final_cordinates;

public:

    bool generate_cordinates(int cor_num, string streetname);
    void assign_values(int svalue, int nvalue, int lvalue, int cvalue);
    void generate_graph(int number);
    void sleep_time(int lvalue);

};
struct point {
    int cor1, cor2;
};

int orientation(point p, point q, point r)
{
    int val = (q.cor2 - p.cor2) * (r.cor1 - q.cor1) -
        (q.cor1 - p.cor1) * (r.cor2 - q.cor2);

    if (val == 0) {
        return 0;  // colinear
    }

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}
bool onSegment(point p, point q, point r)
{
    if (q.cor1 <= max(p.cor1, r.cor1) && q.cor1 >= min(p.cor1, r.cor1) &&
        q.cor2 <= max(p.cor2, r.cor2) && q.cor2 >= min(p.cor2, r.cor2))
        return true;

    return false;
}
int random_generate(int value1, int value2) {
    //srand(time(NULL));
    //int random = rand() % value2 + value1;
   // int random = value1 + (int)rand() % (value2 - value1 + 1);
    // std::cout << "random:" << random << std::endl;
   // return random;

    FILE* urandom;
    urandom = fopen("/dev/urandom", "rb");
    int c;
    c = fgetc(urandom);
    ////std::cout << c << std::endl;
    int randValue;
    randValue = value1 + (int)c % ((value2 + 1) - value1);
    //  free(urandom);
    return randValue;
}
bool check_intersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {

    struct point p1 = { x1,y1 };
    struct point q1 = { x2,y2 };
    struct point p2 = { x3,y3 };
    struct point q2 = { x4,y4 };

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(p1, p2, q1)) {
        return true;
    }
    if (o2 == 0 && onSegment(p1, q2, q1)) {
        return true;
    }
    if (o3 == 0 && onSegment(p2, p1, q2)) {
        return true;
    }
    if (o4 == 0 && onSegment(p2, q1, q2)) {
        return true;
    }
    return false;
}
bool intersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    float a1, a2, b1, b2;

    int xmin_Segment1 = std::min(x1, x2), xmin_segment2 = std::min(x3, x4), ymin_segment1 = std::min(y1, y2), ymin_segment2 = std::min(y3, y4);
    int xmax_segment1 = std::max(x1, x2), xmax_segment2 = std::max(x3, x4), ymax_segment1 = std::max(y1, y2), ymax_segment2 = std::max(y3, y4);
    int xmax = std::max(xmin_Segment1, xmin_segment2), xmin = std::min(xmax_segment1, xmax_segment2);
    int ymax = std::max(ymin_segment1, ymin_segment2), ymin = std::min(ymax_segment1, ymax_segment2);

    if (x1 == x2 && x2 == x3 && x3 == x4) {
        if ((ymin <= y1 && y1 <= ymax) || (ymin <= y2 && y2 <= ymax) || (ymin <= y3 && y3 <= ymax) || (ymin <= y4 && y4 <= ymax)) {
            return true;
        }
    }
    else if (x1 != x2 && x3 != x4) {
        a1 = (y2 - y1) / (x2 - x1);
        b1 = y1 - a1 * x1;
        a2 = (y4 - y3) / (x4 - x3);
        b2 = y3 - a2 * x3;

        if (a1 == a2 && b1 == b2) {
            if (((xmin <= x1 && x1 <= xmax) || (xmin <= x2 && x2 <= xmax) || (xmin <= x3 && x3 <= xmax) || (xmin <= x4 && x4 <= xmax))
                &&
                ((ymin <= y1 && y1 <= ymax) || (ymin <= y2 && y2 <= ymax) || (ymin <= y3 && y3 <= ymax) || (ymin <= y4 && y4 <= ymax)))
            {
                return true;
            }
        }
    }

    float xvalue1 = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4));
    float xvalue2 = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    float yvalue1 = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
    float yvalue2 = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (xvalue2 == 0 || yvalue2 == 0) {
        return false;
    }

    float xcord = xvalue1 / xvalue2;
    float ycord = yvalue1 / yvalue2;
    if (xmin >= xcord && xcord >= xmax && ymin >= ycord && ycord >= ymax) {
        return true;
    }
    return false;
}
void graph::assign_values(int svalue, int nvalue, int lvalue, int cvalue) {
    this->svalue = svalue;
    this->nvalue = nvalue;
    this->lvalue = lvalue;
    this->cvalue = cvalue;

}
void graph::sleep_time(int lvalue) {
    sleep(lvalue);
    //  std::cout << "uncomment value to go in sleep" << endl;
}
bool graph::generate_cordinates(int cor_num, string streetname) {

    int cordinate_num1{}, cordinate_num2{};
    vector<vector<int>> cordinates = {};
    vector<vector<int>> final_cordinates_list = {};
    int counter = 0;
    streetcounter += 1;

    for (int m = 0; m < cor_num; ++m) {
        if (counter >= 50) {
            std::cerr << "Error: failed to generate valid input for " << counter << " simultaneous attempts" << endl;
            //  break;
            return false;
        }
        vector<int> row;
        cordinate_num1 = random_generate(-cvalue, cvalue);
        cordinate_num2 = random_generate(-cvalue, cvalue);
        row.push_back(cordinate_num1);
        row.push_back(cordinate_num2);

        bool same_cordinates = 0;
        for (int i = 0; i < cordinates_match.size(); i++) {
            if (cordinates_match[i][0] == cordinate_num1 && cordinates_match[i][1] == cordinate_num2 || cordinates_match[i][0] == cordinate_num2 && cordinates_match[i][1] == cordinate_num1) {
                //cout << "same segment" << endl;
                same_cordinates = 1;
            }
        }
        if (!same_cordinates) {
            //cout << "ert" << cordinate_num1 << cordinate_num2 << endl;
            cordinates.push_back(row);
            if (cordinates.size() == 3) {
                //final_cordinates_list.clear();
                //cout << cordinates[0][0];
                if (cordinates[0][0] == cordinates[1][0] && cordinates[1][0] == cordinates[2][0] || cordinates[0][1] == cordinates[1][1] && cordinates[1][1] == cordinates[2][1]) {
                    cordinates.pop_back();
                    m--;
                    break;
                }

            }
            else if (cordinates.size() > 3) {
                for (int i = 0; i < cordinates.size() - 1; i++) {
                    //if (i == cordinates.size() - 1) { break; }
                    // auto name = cordinates.front();
                    // cout << cordinates[i][0] << cordinates[i][1] << endl;
                    // cout << cordinates[i+1][0] << cordinates[i+1][1] << endl;
                    int x1 = cordinates[i][0];
                    int y1 = cordinates[i][1];
                    int x2 = cordinates[i + 1][0];
                    int y2 = cordinates[i + 1][1];
                    for (int j = i + 2; j < cordinates.size() - 1; j++) {
                        // cout << cordinates[j-1][0] << cordinates[j-1][1] << endl;
                         //cout << cordinates[j][0] << cordinates[j][1] << endl;
                        int x3 = cordinates[j][0];
                        int y3 = cordinates[j][1];
                        int x4 = cordinates[j + 1][0];
                        int y4 = cordinates[j + 1][1];
                        //cout << "(" << x1 << y1 << ")" << "(" << x2 << y2 << ")" << "(" << x3 << y3 << ")" << "(" << x4 << y4 << ")";
                        bool flag = check_intersection(x1, y1, x2, y2, x3, y3, x4, y4);
                        /*cout << flag;
                        cout << !flag;*/
                        /* if (!flag) {
                             final_cordinates_list.push_back(row);
                         }*/
                        if (flag) {
                            cordinates.pop_back();
                            cordinates.pop_back();
                            m = m - 2;
                            counter += 1;
                        }
                    }
                }
            }
            cordinates_match.push_back(row);
        }
        else {
            m--; //decrementing value for required segment
        }


        /* for (int i = 0; i < cordinates_match.size(); i++) {
             std::cout << "match:" << cordinates_match[i][0] << cordinates_match[i][1] << endl;
         }*/

    }
    /* for (int i = 0; i < cordinates.size(); i++) {
         for (int j = 0; j < cordinates[i].size(); j++) {
             cout << cordinates[i][j] << endl;
         }
     }*/
    if (streetcounter < 3) {
        bool check = 0;
        int intersection_count = 0;
        if (final_cordinates.empty()) {
            for (int i = 0; i < cordinates.size(); i++) {
                vector<int> row1;
                row1.push_back(cordinates[i][0]);
                row1.push_back(cordinates[i][1]);
                //std::cout << "match:" << cordinates[i][0] << cordinates[i][1] << endl;
                final_cordinates.push_back(row1);
            }
        }
        else {
            for (int i = 0; i < final_cordinates.size() - 1; i++) {
                int x1 = final_cordinates[i][0];
                int y1 = final_cordinates[i][1];
                int x2 = final_cordinates[i + 1][0];
                int y2 = final_cordinates[i + 1][1];
                for (int j = 0; j < cordinates.size() - 1; j++) {
                    int x3 = cordinates[j][0];
                    int y3 = cordinates[j][1];
                    int x4 = cordinates[j + 1][0];
                    int y4 = cordinates[j + 1][1];
                  //  cout << "(" << x1 << y1 << ")" << "(" << x2 << y2 << ")" << "(" << x3 << y3 << ")" << "(" << x4 << y4 << ")";
                    check = intersection(x1, y1, x2, y2, x3, y3, x4, y4);
                   // check ? cout << "Yes\n" : cout << "No\n";
                    if (check) {
                        intersection_count += 1;
                        //return true;
                    }
                }
            }
            if (intersection_count == 0) {
                streetcounter -= 1;
                return false;
            }
        }
    }

    string final_cordinates;
    std::cout << "a " << "\"" << streetname << "\"" << " ";
    for (int a = 0; a < cordinates.size(); ++a) {
        std::cout << "(" << cordinates[a][0] << "," << cordinates[a][1] << ")";
    }
    std::cout << endl;
    return true;
}
void graph::generate_graph(int number) {

    vector<string> street;
	   // cordinates_match = {};
    //  final_cordinates = {};

    //for (int n = 0; n < street.size(); ++n) {
      //    std::cout << "r " << "\"" << street[n] << "\"" << endl;
     // }

    int street_number = number;
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    char s;
    bool match_flag = 0;
    while (street_number > 0) {
        string streetname = "";
        for (int j = 0; j < 10; ++j) {
            s = alphanum[rand() % (sizeof(alphanum) - 1)];
            streetname += s;
        }
        for (int k = 0; k < street.size(); ++k) {
            if (street[k] == streetname) {
                match_flag = 1;
                break;
            }
        }
        if (match_flag == 0) {
            street.push_back(streetname);
            int cordinate_num = 0;
            cordinate_num = random_generate(1, nvalue);
            //          cout<<cordinate_num <<endl;
            bool generated = generate_cordinates(cordinate_num + 1, streetname);
            while (!generated) {
                cordinate_num = random_generate(1, nvalue);
                generated = generate_cordinates(cordinate_num + 1, streetname);
            }
        }
        street_number -= 1;
    }
    std::cout << "g" << std::endl;
    fflush(stdout);
    sleep_time(lvalue);
 	 cordinates_match = {};
	final_cordinates = {};
	streetcounter = 0;
    for (int n = 0; n < street.size(); ++n) {
        std::cout << "r " << "\"" << street[n] << "\"" << endl;
    }
    fflush(stdout);

}
int main(int argc, char** argv)
{
    bool errflag = 0;
    int cint_value = 20;
    int sint_value = 10, nint_value = 5, lint_value = 5;
    //    int index;
    int ch;
    std::string cvalue, svalue, nvalue, lvalue;
    opterr = 0;

    while ((ch = getopt(argc, argv, "s:n:l:c:?")) != -1)
        switch (ch)
        {
        case 's':
            svalue = optarg;
            sint_value = atoi(svalue.c_str());
            if (sint_value < 2) {
                std::cerr << "Error: S value should be more than 2" << std::endl;
                errflag = 1;
                return 1;
            }
            break;
        case 'n':
            nvalue = optarg;
            nint_value = atoi(nvalue.c_str());
            if (nint_value < 1) {
                std::cerr << "Error: N value should be more than 1" << std::endl;
                errflag = 1;
                return 1;
            }
            break;
        case 'l':
            lvalue = optarg;
            lint_value = atoi(lvalue.c_str());
            if (lint_value < 5) {
                std::cerr << "Error: L value should be more than 5" << std::endl;
                errflag = 1;
                return 1;
            }
            break;
        case 'c':
            cvalue = optarg;
            cint_value = atoi(cvalue.c_str());
            if (cint_value < 1) {
                std::cerr << "Error: C value should be more than 1" << std::endl;
                errflag = 1;
                return 1;
            }
            break;
        case '?':
            if (optopt == 'c')
                std::cerr << "Error: option -" << optopt
                << " requires an argument." << std::endl;
            else
                std::cerr << "Error: unknown option: " << optopt << std::endl;
            return 1;
        default:
            return 0;
        }

    graph* g1 = new graph();
    g1->assign_values(sint_value, nint_value, lint_value, cint_value);
  //  int streetnum = random_generate(2, sint_value);
//	cout << streetnum << endl;
    //int count =0;

    while (true) {
        //cout << ++count << endl;
		int streetnum = random_generate(2, sint_value);
        g1->generate_graph(streetnum);
    }
    // random_generate(5, lint_value);
    // random_generate(-cint_value, cint_value);

     //if (errflag != 1) {
       //  std::cout << "you can proceed" << std::endl;

     //}

    return 0;

}


