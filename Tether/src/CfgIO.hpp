#ifndef CFGIO_HPP
#     define CFGIO_HPP

#include "Cfg.hpp"
#include "IFileIO.hpp"

#include <regex>
#include <string>
#include <utility>

#include "filesystem.hpp"

namespace fs = std::filesystem;

using std::pair;
using std::regex;
using std::string;

class CfgIO : public IFileIO<Cfg>
{
public:
	//any integer is a sequence of digits
	static const std::regex re_int;
	//any float are 2 sequences of digits, sparated by a point
	//as a feature any of the 2 sequences can have 0 characters
	// => . == 0.0 ; 1. == 1.0 ; .5 == 0.5;
	static const std::regex re_flt;
	//a string is any sequence of characters encapsulated in "
	//the first character in the sequence must be "
	//all the characters until the next " are included
	//the second " can be escaped using backslash
	static const std::regex re_str;

	static const regex re_section;
	static const regex re_key;
private:
	string m_filename;

public:
	CfgIO() = default;
	CfgIO(const CfgIO&);
	CfgIO(const fs::path&);
	CfgIO& operator= (const CfgIO&);
	const string& filename() const;
	
	virtual Cfg get() const;
	virtual void save(const Cfg& cfg) const;

public:
	bool isComment(const string& s) const;
	bool isEmpty(const string& s) const;
	string getSectionName(const string& s) const;
	pair<string, string> splitKeyValue(const string& s) const;
	string match(const string& s, std::regex re) const;
	double readFloatstring(const string& s) const;

};

#endif /* CFGIO_HPP */

