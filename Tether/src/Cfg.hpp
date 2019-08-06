#ifndef SRC_CFG_H_
#define SRC_CFG_H_

/*
//	Planned Features:
//	Reading of config files
//	Provide fast and easy access to values
//	Load and unload specific sections of configs to
//	especially loading SPECIFIC sections is an optimisation task
*/

#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <vector>

using std::map;
using std::optional;
using std::ostream;
using std::string;
using std::vector;


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

	class Section
	{
		string m_name;
	public:
		Section(const string& sectionname);
		Section(const Section& section);
		Section& operator= (const Section& section);

		map <string, long> num;
		map <string, double> flt;
		map <string, string> str;

		bool isInt(string key) const;
		bool isFlt(string key) const;
		bool isStr(string key) const;

		const long* getInt(const string& key) const;
		const double* getFlt(const string& key) const;
		const string* getStr(const string& key) const;
		const string& name() const;

		void set(const string& key, long value);
		void set(const string& key, double value);
		void set(const string& key, const string& value);

		friend ostream& operator<< (ostream& os, const Section& section);
	};

	map<string, Section> sections;


	/*
	//	Functions for extracting a unique key, value par
	//		/param1 : string	section			Name of section the key value pair is specified in
	//		/param2 : string	key				Key of the Key, Value pair
	//
	//		/ret :	  type						int, float, string you choose
	*/
	const long* getInt(const string& section, const string& key) const;
	const double* getFlt(const string& section, const string& key) const;
	const string* getStr(const string& section, const string& key) const;

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
	Cfg::Section getSection(const string& sectionname) const;
	void insertSection(const Section& section);
	void createSection(const string& sectionname);

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
	const long * getInt(const string& key) const;
	const double * getFlt(const string& key) const;
	const string * getStr(const string& key) const;


	void set(const string& section, const string& key, long value);
	void set(const string& section, const string& key, double value);
	void set(const string& section, const string& key, const string& value);


	friend std::ostream& operator<< (std::ostream& os, const Cfg& c);

};
#endif /*SRC_CFG_H_*/
