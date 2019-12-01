#include "Cfg.hpp"

#include <iostream>
#include <regex>
#include <string>
#include <fstream>

using std::cerr;
using std::regex_match;
using std::string;



Cfg::Section::Section(const string& sectionname)
	:
	m_name(sectionname)
{}

Cfg::Section::Section(const Cfg::Section& other)
	:
	m_name(other.m_name),
	num(other.num),
	flt(other.flt),
	str(other.str)
{}

Cfg::Section& Cfg::Section::operator= (const Section& other)
{
	m_name = other.m_name;
	num = other.num;
	flt = other.flt;
	str = other.str;

	return *this;
}

bool Cfg::Section::isInt(string key) const
{
	return num.count(key);
}

bool Cfg::Section::isFlt(string key) const
{
	return flt.count(key);
}

bool Cfg::Section::isStr(string key) const
{
	return str.count(key);
}

const string* Cfg::Section::getStr(const string& key) const
{
	if(str.count(key) && isStr(key))
	{
		return &(str.at(key));
	}
	cerr << "Cfg: Key for String <" << key << "> not found!\n";
	return nullptr;
}

const double* Cfg::Section::getFlt(const string& key) const
{
	if(flt.count(key) && isFlt(key))
	{
		return &(flt.at(key));
	}
	cerr << "Cfg: Key for Float <" << key << "> not found!\n";
	return nullptr;
}

const long* Cfg::Section::getInt(const string& key) const
{
	if(num.count(key) && isInt(key))
	{
		return &(num.at(key));
	}
	cerr << "Cfg: Key for Integer <" << key << "> not found!\n";
	return nullptr;
}

const string& Cfg::Section::name() const
{
	return m_name;
}

void Cfg::Section::set(const string& key, long value)
{
	num[key] = value;
}

void Cfg::Section::set(const string& key, double value)
{
	flt[key] = value;
}

void Cfg::Section::set(const string& key, const string& value)
{
	str[key] = value;
}

ostream& operator<<(ostream& os, const Cfg::Section& section)
{
	map<string, string> printmap;
	for(auto& e : section.num)
	{
		string key, value;
		key = e.first;
		value = std::to_string(e.second);
		printmap.emplace(key, value); 
	}
	for(auto& e : section.flt)
	{
		if(!printmap.count(e.first))
		{
			printmap.emplace(e.first, std::to_string(e.second));
			string key, value;
			key = e.first;
			value = std::to_string(e.second);
			printmap.emplace(key, value); 
		}	
	}
	for(auto& e : section.str)
	{
		if(!printmap.count(e.first))
		{
			if(e.second.find_first_of("\n") == string::npos)
			{
				//singleline
				string key, value;
				key = e.first;
				value = "\""+e.second+"\"";
				printmap.emplace(key, value); 
			}
			else
			{
				string key, value;
				key = e.first;
				value = "\"\"\"\n" + e.second + "\n\"\"\"";
				printmap.emplace(key, value); 
			}
		}
	}

	os << "[" + section.m_name + "]\n";
	for(auto& e : printmap)
	{
		os << e.first << " = " << e.second << '\n';
	}
	return os;
}

const long* Cfg::getInt(const string& section, const string& key) const
{
	if(sections.count(section))
	{
		return sections.at(section).getInt(key);
	}
	cerr << "Cfg: Int for <" << key << "> not found!\n";
	return nullptr;
}

const double* Cfg::getFlt(const string& section, const string& key) const
{
	if(sections.count(section))
	{
		return sections.at(section).getFlt(key);
	}
	cerr << "Cfg: Flt for <" << key << "> not found!\n";
	return nullptr;
}

const string* Cfg::getStr(const string& section, const string& key) const
{
	if(sections.count(section))
	{
		return sections.at(section).getStr(key);
	}
	cerr << "Cfg: String for <" << key << "> not found!\n";
	return nullptr;
}

Cfg::Section Cfg::getSection(const string& sectionname) const
{
	Cfg::Section new_section(sectionname);
	if(sections.count(sectionname))
	{
		new_section = sections.at(sectionname);
	}
	return new_section;
}

void Cfg::insertSection(const Cfg::Section& section)
{
	if(!sections.count(section.name()))
	{
		createSection(section.name());
	}
	sections.at(section.name()) = section;
}

void Cfg::createSection(const string& sectionname)
{
	if(sections.count(sectionname))
	{
		return;
	}
	else
	{
		sections.emplace(sectionname, Cfg::Section(sectionname));
	}
}

const long* Cfg::getInt(const string& key) const
{
	for(auto& s : sections)
	{
		return s.second.getInt(key);
	}
	cerr << "Cfg: Int for <" << key << "> not found!\n";
	return nullptr;
}

const double* Cfg::getFlt(const string& key) const
{
	for(auto& s : sections)
	{
		return s.second.getFlt(key);
	}
	cerr << "Cfg: Flt for <" << key << "> not found!\n";
	return nullptr;
}

const string* Cfg::getStr(const string& key) const
{
	for(auto& s : sections)
	{
		return s.second.getStr(key);
	}
	cerr << "Cfg: String for <" << key << "> not found!\n";
	return nullptr;
}

void Cfg::set(const string& section, const string& key, long value)
{
	if(sections.count(section))
	{
		sections.at(section).set(key, value);
	}
}

void Cfg::set(const string& section, const string& key, double value)
{
	if(sections.count(section))
	{
		sections.at(section).set(key, value);
	}
}

void Cfg::set(const string& section, const string& key, const string& value)
{
	if(sections.count(section))
	{
		sections.at(section).set(key, value);
	}
}

std::ostream& operator<< (std::ostream& os, const Cfg& c)
{
	for(auto& e : c.sections)
	{
		os << e.second << '\n';
	}
	return os;
}
