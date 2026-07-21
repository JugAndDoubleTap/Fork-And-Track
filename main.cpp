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
// for getting the arguments for the forked program / command
#include <vector>
using std::vector;


// function prototype
pid_t ForkAndTrack(string& programPath, const vector<string>& programArgs, string& redirectConsole, bool& hideConsole);


// main
int main(int argc, char* argv[])
{
    // loop that checks if argument equals ==
    int dashIndex = -1;
    for (int i = 1; i < argc; ++i)
     {
        if (string(argv[i]) == "--")
         {
            dashIndex = i;
            break;
        }
    }

    // setup arguments specifically for the utility instead of globally
    int fatArgc = (dashIndex != -1) ? dashIndex : argc;
    
    vector<string> programArray;
    if (dashIndex != -1)
     {
        // separates additional arguments to after --
        for (int i = dashIndex + 1; i < argc; ++i)
         {
            programArray.push_back(argv[i]);
        }
    }



    // setup commandline arguments
    ArgumentParser program("Fat (ForkAndTrack)", "1.1.0");
    
    // program / command dir
    program.add_argument("-p", "--program").help("Path to the program / command to be launched").required();
    // toggle for hiding console output
    program.add_argument("-h", "--hide-output").help("Toggle to hide console output").default_value(false).implicit_value(true);
    // argument to redirect console output
    program.add_argument("-r", "--redirect-output").help("Path to redirect the console output to").default_value("");
    // arguments that are passed to the forked program / command
    program.add_argument("arguments").help("Arguments that are passed to the forked program / command")
           .nargs(argparse::nargs_pattern::any).default_value(std::vector<string>{});
    // parse the arguments
    program.parse_args(fatArgc, argv);


    // extracts the argument values and stores in variables

    // string for program / command
    string programPath = program.get<string>("--program");
    // bool for hide console toggle
    bool hideConsole = program.get<bool>("--hide-output");
    // string for console redirection path
    string redirectConsole = program.get<string>("--redirect-output");



    // launch program and get id
    pid_t programID = ForkAndTrack(programPath, programArray, redirectConsole, hideConsole);
    

    // print process id (group id)
    cout << programID << endl;

    // return
    return 0;
}


// executes the command or program and outputs the process id (of the process group)
pid_t ForkAndTrack(string& programPath, const vector<string>& programArgs, string& redirectConsole, bool& hideConsole)
{       // gets pid from fork
        pid_t pid = fork();
        
        // if child process
        if(pid == 0) 
        {   
            // put process id in group
            setpgid(0, 0);


            // eldritch console arguments block
            if (hideConsole) 
            {   
                // hide console output by putting it in dev null
                freopen("/dev/null", "w", stdout);
                freopen("/dev/null", "w", stderr);
            } 
            else if (!redirectConsole.empty())
             {
                // redirect console output to path argument 
                freopen(redirectConsole.c_str(), "w", stdout);
                freopen(redirectConsole.c_str(), "w", stderr);
            }


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