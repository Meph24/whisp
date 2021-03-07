#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP

#include <numeric>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

struct CommandLineArgumentParser
{
	unordered_map<string, unsigned int> flag_register;

	CommandLineArgumentParser( unordered_map<string, unsigned int> flag_register )
        : flag_register(flag_register) {}
    
	unordered_map<string, vector<string>> parse ( int argc, char** argv )
	{
		unordered_map<string, vector<string>> args;
		unordered_set<int> flag_indices;
		for (int i = 0; i < argc; i++)
		{
			if( argv[i][0] == '-' ) flag_indices.insert( i );
		}
		for(int i : flag_indices)
		{
			if( argv[i][1] == '-' )
			{
				string arg ( argv[i]+2 );
				if( flag_register.find(arg) != flag_register.end() )
				{
					//collect all additional parameters
					vector<int> to_collect_indices( flag_register[arg], -1 );
                    std::iota(to_collect_indices.begin(), to_collect_indices.end(), i+1);
                    for (int j : to_collect_indices)
                    {
                        if( j < 0 || j >= argc ) throw std::runtime_error( "CommandLineArgumentParser : Not enough arguments for resolving <" + arg + ">, which expects " + std::to_string(flag_register[arg]) + " arguments.\n" );
                        to_collect_indices.push_back( j );
                    }
                    
                    vector<string> argparams;
					for( int ind : to_collect_indices ) 
					{ 
						if(flag_indices.find(ind) != flag_indices.end())
						{
							throw std::runtime_error( "CommandLineArgumentParser : Colliding arguments for argument <" + std::to_string(ind) + ".\n");
						} 
                        argparams.emplace_back( argv[ind] );

 					}//check collisions with other args
                    args[arg] = argparams;
				}
				else
				{
					throw std::runtime_error( "CommandLineArgumentParser : Unspecified Argument <" + arg + ">." );
				}
				
			}
            else throw std::runtime_error( "CommandLineArgumentParser : Single letter argument abbreviations are currently not supported!" );
		}
		return args;
	}
};

struct CommandLineArgumentInterpreter
{
	enum ExecutionMode
	{
		Default,
		RemoteControlSender,
		RemoteControlReceiver,
		Host,
		Client,

		NUM_EXECUTIONMODE
	} execution_mode = ExecutionMode::Default;

	unordered_map<string, vector<string>> args;

    CommandLineArgumentInterpreter( unordered_map<string, vector<string>> args )
	: args(args)
    {   
        if( args.find( "host" ) != args.end() )
            execution_mode = ExecutionMode::Host;
        else if( args.find( "client" ) != args.end() )
            execution_mode = ExecutionMode::Client;
        else if( args.find( "remote-control-sender" ) != args.end() )
            execution_mode = ExecutionMode::RemoteControlSender;
		else if(args.find( "remote-control-receiver" ) != args.end() )
			execution_mode = ExecutionMode::RemoteControlReceiver;
    }
};

#endif // COMMANDLINE_HPP
