#include "CfgIO.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::cerr;
using std::ifstream;
using std::ofstream;
using std::regex;
using std::regex_match;
using std::string;
using std::stringstream;

const std::regex CfgIO::re_int = std::regex(R"(\s*(\d+)\s*)");
const std::regex CfgIO::re_flt = std::regex(R"(\s*(\d*\.\d*)\s*)");
const std::regex CfgIO::re_str = std::regex(R"(\"[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*\")");
const std::regex CfgIO::re_section = regex(R"(\s*\[\s*(.*)\s*\]\s*)");
const std::regex CfgIO::re_key = regex(R"(\s*(\w*)\s*)");

CfgIO::CfgIO(const fs::path& file)
	:
	m_filename(file.string())
{}

CfgIO::CfgIO(const CfgIO& other)
	:
	m_filename(other.m_filename)
{}

CfgIO& CfgIO::operator=(const CfgIO& other)
{
	m_filename = other.m_filename;
	return *this;
}

const string& CfgIO::filename() const
{
	return m_filename;
}

bool CfgIO::isComment(const string& s) const
{
	return (s.size() && s[0] == '#');
}

bool CfgIO::isEmpty(const string& s) const
{
	std::regex re ( R"(^\s*$)" );
	return regex_match(s, re);
}

string CfgIO::getSectionName(const string& s) const
{
	std::smatch match;
	std::regex_match(s, match, re_section);
	if(! match.size()) return "";

	stringstream ss(match[1]);
	string first_word; ss >> first_word;
	return first_word;
}

pair<string, string> CfgIO::splitKeyValue(const string& s) const
{
	size_t splitpoint = s.find_first_of("=");
	string key, value;
	key = match(s.substr(0, splitpoint), re_key);

	value = s.substr(splitpoint+1);
	{
		size_t first_real_literal = value.find_first_not_of(" \t");
		value = value.substr(first_real_literal);
	}
	{
		size_t last_real_literal = value.find_last_not_of(" \t");
		value = value.substr(0, last_real_literal + 1);
	}

	return pair<string, string> (key, value);
}


string CfgIO::match(const string& s, std::regex re) const
{
	string match_string = "";
	std::smatch m;
	std::regex_match(s, m, re);

	return m[1];
}

double CfgIO::readFloatstring(const string& s) const
{
	string m = s;
	if(m[0] == '.')
	{
		m = "0" + m;
	}
	if(m[m.size()-1] == '.')
	{
		m = m + "0";
	}
	return std::stod(m);
}

Cfg CfgIO::get() const
{
	ifstream file (m_filename);
	if (file.fail())
	{
		std::cerr << "CfgIO: File <" << m_filename << "> could not be opened!\n";
	}

	Cfg cfg;
	string line;

	string current_section = "";
	while(std::getline(file, line))
	{	
		if(isEmpty(line) || isComment(line))
			continue;

		{
			string section_from_line = getSectionName(line);
			if(section_from_line != "")
			{
				current_section = section_from_line;
				cfg.createSection(section_from_line);
				continue;
			}
		}

		if(current_section == "")
		{
			cerr << "CfgIO: Parsing Error : Read Word without a section!\n";
		}

		string key, value;
		auto p = splitKeyValue(line);
		key = p.first; value = p.second;
		
		string m = match(value, re_flt);
		if(m!="")
		{
			//isfloat
			double d = readFloatstring(m);
			cfg.set(current_section, key, d);
			continue;
		}
		
		m = match(value, re_int);
		if(m!="")
		{
			//isfloat
			long l = std::stol(m);
			cfg.set(current_section, key, l);
			continue;
		}

		if(value[0] == '\"')
		{
			//its a string
			if(	
				value.size() > 1 && 
				value[1] == '\"' && 
				value[2] == '\"'
			  )
			{
				//is multiline
				value = "";
				string stringline;
				while(std::getline(file, stringline))
				{
					{
						stringstream stringline_stripstream(stringline);
						string first_word_of_stringline;
						stringline_stripstream >> first_word_of_stringline;
						if(first_word_of_stringline == "\"\"\"")
						{
							value = value.substr(0, value.size()-1);
							break;
						}
					}
					value += stringline + '\n';
				}
				cfg.set(current_section, key, value);
				continue;
			}
			else
			{
				//is single line
				//find the encapsulating "
				size_t first_quote = value.find_first_of('\"');
				size_t last_quote = value.find_last_of('\"');
				value = value.substr(first_quote+1, last_quote - first_quote - 1);
				cfg.set(current_section, key, value);
				continue;
			}
		}
	}
	file.close();
	return cfg;
}

void CfgIO::save( const Cfg& cfg ) const
{
	ofstream file (m_filename);
	file << cfg;
	file.close();
}
