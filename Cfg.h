#pragma once

/*
//	Planned Features:
//	Reading of config files
//	Provide fast and easy access to values
//	Load and unload specific sections of configs to 
//	especially loading SPECIFIC sections is an optimisation task
*/

#include <vector>
#include <map>
#include <string>
#include <iostream>

using std::cout;
using std::endl;


using std::vector;
using std::map;
using std::string;


//DEV this stuff is currently in development
//DEV there is some functionality i wanted to add to it
//usage example:
// Cfg cfg({"testcfg.cfg"});
// cout << cfg; //will print the config (already sorted) to your screen
// std::ostream os ("testcfg.cfg"); //to write it back in one file (yes other applications are not possible at this time)
// os << cfg;
// os.close();

class Cfg
{
public:
	class section
	{
	public:
		map <string, int> num;
		map <string, float> flt;
		map <string, string> str;
	
		int setint(string key, int value);
		int setfloat(string key, float value);
		int setstring(string key, string value);	
		int * getint(string key);
		float * getfloat(string key);
		string * getstring(string key);
	};


private:


	class Reader
	{	
	private:

		unsigned int readBegin;

		string section;
		string id;
		string value;

		unsigned int linecounter = 0;

		string line;
		unsigned int currentIndex;

		struct configdea_statemashine
		{
			int q;
		};

		enum configdea_states
		{
			trap = -1,		//default trap state
			q0 = 0,
			q1 = 1,
			q2 = 2,
			q3 = 3,
			q4 = 4,
			q5 = 5,
			q6 = 6,
			q7 = 7,
			q8 = 8,
			q9 = 9,

			NUM_CONFIGDEA_STATES
		};

		void configdea_init(configdea_statemashine* M);

		void configdea_d_check(configdea_statemashine* M, char sym);
		void configdea_d_load(configdea_statemashine* M, char sym);

		int configdea_exit(configdea_statemashine* M);

		enum type_sup
		{
			none = -1,

			integer = 0,
			flt = 1,
			str = 2,

			NUM_TYPE_SUP
		};

		type_sup gettype(string);

		void markReadBegin(int add);
		void markReadEnd(int add , string& container);

		map<string, Cfg::section> buildmap;

		void save();

	public:
		
		int checkfile(string filename);
		map <string,Cfg::section> readfile(string filename);
	}r;





private:
	map<string, section> sections;

public:


	/*
	//	Takes a list of files it should read configs from
	//	will check them first and then load the contents
	*/
	Cfg()
	{}
	Cfg(vector<string> filelist);
	~Cfg();

	int check(string filename);
	int load(string filename);
	int unload();
	int unload(string sectionname);

	

	/*
	//	Functions for extracting a unique key, value par
	//		/param1 : string	section			Name of section the key value pair is specified in
	//		/param2 : string	key				Key of the Key, Value pair
	//
	//		/ret :	  type						int, float, string you choose
	*/
	int * getint(string section, string key);
	float * getfloat(string section, string key);
	string * getstring(string section, string key);

	/*
	//	Function to get a specific section directly
	//		/param : string		sectionname		Name if section you want to have
	//
	//		/ret :	 Cfg::section&				Will give you a section object by reference
	//											Returns false if the section could not be found
	//
	//		Usage:	Handling configs of a specific section very frequently, you might choose a handle of this
	//				this specific section for performance and write efficiency
	//				For example if you're writing a graphics component, sound config is of low interest to you anyway
	//				so you can extract a reference to the graphics section and pass it to the component
	*/
	Cfg::section& getSection(string sectionname);

	/*
	//	Functions for extracting a key, value pair
	//		/param1 : string	key			Key of the Key, Value pair
	//
	//		/ret :	  type					int, float, string you choose
	//
	//	Since key, value pairs can in fact share the same key if they are stored in different sections these
	//	functions may be considered unsafe !
	//	If you are sure the Key of the Key,Value pair is unique you can extract the concrete value without problems
	//	Should you have more than one appearances of the same Key in different sections, you will get the first element the algorythm can find that matches
	//	If you dont know exactly should better use the signature: > type * gettype(string section, string key); <
	*/
	int * getint(string key);
	float * getfloat(string key);
	string * getstring(string key);


	void setint(string section, string key, int value);
	void setfloat(string section, string key, float value);
	void setstring(string section, string key, string value);

	friend std::ostream& operator<< (std::ostream& os, const Cfg& c);

};

