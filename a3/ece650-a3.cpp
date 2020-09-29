#include <unistd.h>
#include <iostream>
#include <fstream>
#include<time.h>
#include<vector>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>

int readline(void){
	
	while (!std::cin.eof()) {
		std::string line;
		std::getline(std::cin, line);
		if (line.size () > 0)
			std::cout << line << std::endl;
	}
	return 0;
}

int main (int argc, char **argv) {

    //bool errflag = 0;
    std::string cvalue, svalue, nvalue, lvalue;
    int cint_value = 20, sint_value = 10, nint_value = 5, lint_value = 5;
    int ch;
	opterr = 0;

    while ((ch = getopt (argc, argv, "s:n:l:c:?")) != -1)
        switch (ch)
        {
        case 's':
            svalue = optarg;
            sint_value = atoi(svalue.c_str());
            if(sint_value < 2){
                    std::cerr << "Error: S value should be more than 2" << std::endl;
                    return 1;
            }
            break;
        case 'n':
            nvalue = optarg;
            nint_value = atoi(nvalue.c_str());
            if(nint_value < 1){
                std::cerr << "Error: N value should be more than 1" << std::endl;
                    return 1;
            }
            break;
		case 'l':
            lvalue = optarg;
            lint_value = atoi(lvalue.c_str());
            if(lint_value < 5){
                    std::cerr << "Error: L value should be more than 5" << std::endl;
                    return 1;
            }
            break;
        case 'c':
            cvalue = optarg;
            cint_value = atoi(cvalue.c_str());
            if(cint_value < 1){
                    std::cerr << "Error: C value should be more than 1" << std::endl;
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
	
	
	argv[0] = {(char *)"./rgen"};
    char *a1pipe[] = {(char *)"/usr/bin/python3.6",(char *)"ece650-a1.py",NULL};
    char *a2pipe[] = {(char *)"./ece650-a2",NULL};
	pid_t child_pid1;
	pid_t child_pid2;
	pid_t child_pid3;
	pid_t child_pid4;
	std::vector<pid_t> kids;
	
	int rgena1[2];
    pipe(rgena1);
    int a1a2[2];
    pipe(a1a2);


    child_pid1 = fork();
    if (child_pid1 == 0) {
		
        dup2(rgena1[1], STDOUT_FILENO);
		close(rgena1[0]);
		close(rgena1[1]);
        execv(argv[0], argv);
        perror ("Error: Could not execute rgen");
    }
    else if (child_pid1 < 0) {
        std::cerr << "Error: could not fork Rgen" << std::endl;
		return 1;
    }
    kids.push_back(child_pid1);

	child_pid2 = fork();
    if (child_pid2 == 0) {
		dup2(rgena1[0], STDIN_FILENO);
		close(rgena1[0]);
		close(rgena1[1]);
		dup2(a1a2[1], STDOUT_FILENO);
		close(a1a2[0]);
		close(a1a2[1]);
        execv(a1pipe[0],a1pipe);
        perror ("Error: Could not execute ece650-a1.py");
    }
    else if (child_pid2 < 0) {
        std::cerr << "Error: could not fork a1-ece.py" << std::endl;
		return 1;
    }
    kids.push_back(child_pid2);

	child_pid3 = fork();
    if (child_pid3 == 0) {
        dup2(a1a2[0], STDIN_FILENO);
		close(a1a2[0]);
		close(a1a2[1]);
        execv(a2pipe[0], a2pipe);
        perror ("Error: Could not execute ece650-a2");
      }
	else if (child_pid3 < 0) {
          std::cerr << "Error: could not fork a2-ece" << std::endl;
		  return 1;
      }
    kids.push_back(child_pid3); 
    
	child_pid4 = fork();
    if (child_pid4 == 0) {
        dup2(a1a2[1], STDOUT_FILENO);
		close(a1a2[0]);
		close(a1a2[1]);
		
		dup2(a1a2[1],STDOUT_FILENO);
        close(a1a2[0]);
        close(a1a2[1]);

		return readline();
    }
	else if (child_pid4 < 0) {
          std::cerr << "Error: could not fork a2-ece" << std::endl;
		  return 1;
    }
    kids.push_back(child_pid4); 
	
	int monitor;
    wait(&monitor);	

	for (pid_t k :kids){
        int status;
        kill(k,SIGTERM);
        waitpid(k,&status,0);

    }
	
	return 0;
}
