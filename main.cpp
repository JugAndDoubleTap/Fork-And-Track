// includes
// for fork, execlp/execvp
#include <unistd.h>
// for strings
#include <string>
using std::string;
// for commandline arguments
#include "argparse.hpp"
using argparse::ArgumentParser;
// for printing
#include <iostream>
using std::cout;
using std::endl;
// for tokenizing the arguments
#include <ranges>
using std::ranges::to;
using std::ranges::views::split;
#include <vector>
using std::vector;



// function prototype
pid_t ForkAndTrack(string& programPath, const vector<string>& programArgs);


// main
int main(int argc, char* argv[])
{
    // setup commandline arguments
    ArgumentParser program("Fat (ForkAndTrack)", "1.0.0");
    
    // program / command dir
    program.add_argument("-p", "--program").help("Path to the program / command to be launched").required();
    // program / command dir
    program.add_argument("-a", "--arguments").help("Arguments to be supplied to the program / command").default_value(string(""));
    // parse the arguments
    program.parse_args(argc, argv);


    // extracts the argument values and stores in variables

    // string for program / command
    string programPath = program.get<string>("--program");
    // string for program / command
    string programArgs = program.get<string>("--arguments");


    // convert arguments string into a vector of strings
    vector<string> programArray = programArgs | split(' ') | to<vector<string>>();


    // launch program and get id
    pid_t programID = ForkAndTrack(programPath, programArray);
    

    // print process id (group id)
    cout << programID << endl;

    // return
    return 0;
}


// executes the command or program and outputs the process id (of the process group)
pid_t ForkAndTrack(string& programPath, const vector<string>& programArgs)
{       // gets pid from fork
        pid_t pid = fork();
        
        // if child process
        if(pid == 0) 
        {   
            // put process id in group (0, 0 means this child process becomes the group leader)
            setpgid(0, 0);

            // extracts the arguments from the vector (also prepends programPath and appends a nullptr to the vector)
            vector<const char*> c_args;
            c_args.push_back(programPath.c_str()); 
            for (const auto& arg : programArgs)
            {
                if (!arg.empty()) c_args.push_back(arg.c_str());
            }
            c_args.push_back(nullptr); 

            // executes command / program
            execvp(programPath.c_str(), const_cast<char* const*>(c_args.data()));
            exit(1); 
        }
        else if (pid > 0)
        {
            // pid inator 
            setpgid(pid, pid); 
        }

        // return pid
        return pid;
}