#include "Cfg.h"

#include<regex>
#include <fstream>
#include <list>

Cfg::Cfg(vector<string> filelist)
{
	for (string filename : filelist)
	{
		load(filename);
	}
}

Cfg::~Cfg()
{
}

// section code

int Cfg::section::setstring(string key, string value)
{
	return str.emplace(key, value).second;
}
int Cfg::section::setint(string key, int value)
{
	return num.emplace(key, value).second;
}
int Cfg::section::setfloat(string key, float value)
{
	return flt.emplace(key, value).second;
}

string * Cfg::section::getstring(string key)
{
	map<string, string>::iterator it = str.find(key);
	if (it != str.end()) return &(it->second);
	return nullptr;
}

int * Cfg::section::getint(string key)
{
	map<string, int>::iterator it = num.find(key);
	if (it != num.end()) return &(it->second);
	return nullptr;
}
float * Cfg::section::getfloat(string key)
{
	map<string, float>::iterator it = flt.find(key);
	if (it != flt.end()) return &(it->second);
	return nullptr;
}

//section code end

//dea code

void Cfg::Reader::configdea_init(configdea_statemashine* M)
{
	M->q = configdea_states::q0;
}

void Cfg::Reader::configdea_d_check(configdea_statemashine* M, char sym)
{
	switch (M->q)
	{

	case configdea_states::q0:

		if (((sym == ' ')))
		{
			M->q = configdea_states::q0;
		}

		else if (((sym == '#')))
		{
			M->q = configdea_states::q1;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q1:

		if ((((sym >= 'a') && (sym <= 'z')) || ((sym >= 'A') && (sym <= 'Z')) || (sym == '_')))
		{
			M->q = configdea_states::q1;
		}

		else if (((sym == ' ')))
		{
			M->q = configdea_states::q2;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q2:

		if (((sym == ' ')))
		{
			M->q = configdea_states::q2;
		}

		else if ((((sym >= 'a') && (sym <= 'z')) || ((sym >= 'A') && (sym <= 'Z')) || (sym == '_')))
		{
			M->q = configdea_states::q3;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q3:

		if ((((sym >= 'a') && (sym <= 'z')) || ((sym >= 'A') && (sym <= 'Z')) || (sym == '_')))
		{
			M->q = configdea_states::q3;
		}

		else if (((sym == ' ')))
		{
			M->q = configdea_states::q4;
		}

		else if (((sym == '=')))
		{
			M->q = configdea_states::q5;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q4:

		if (((sym == '=')))
		{
			M->q = configdea_states::q5;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q5:

		if (((sym == ' ')))
		{
			M->q = configdea_states::q5;
		}

		else if ((((1) && (!(sym == ' ')) && (!(sym == '"')))))
		{
			M->q = configdea_states::q6;
		}

		else if (((sym == '"')))
		{
			M->q = configdea_states::q8;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q6:

		if ((((1) && (!(sym == ' ')))))
		{
			M->q = configdea_states::q6;
		}

		else if (((sym == ' ')))
		{
			M->q = configdea_states::q7;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q7:

		if (((sym == ' ')))
		{
			M->q = configdea_states::q7;
		}

		else if ((((sym >= 'a') && (sym <= 'z')) || ((sym >= 'A') && (sym <= 'Z')) || (sym == '_')))
		{
			M->q = configdea_states::q3;
		}

		else if (((sym == '#')))
		{
			M->q = configdea_states::q1;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q8:

		if ((((1) && (!(sym == '"')) && (!(sym == '\\')))))
		{
			M->q = configdea_states::q8;
		}

		else if (((sym == '\\')))
		{
			M->q = configdea_states::q9;
		}

		else if (((sym == '"')))
		{
			M->q = configdea_states::q7;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q9:

		if (((1)))
		{
			M->q = configdea_states::q8;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::trap:
	default:
		break;
	}
}

void Cfg::Reader::configdea_d_load(configdea_statemashine* M, char sym)
{
	switch (M->q)
	{

	case configdea_states::q0:

		if (((sym == ' ')))
		{
			M->q = configdea_states::q0;
		}

		else if (((sym == '#')))
		{
			M->q = configdea_states::q1;
			markReadBegin(+1);
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q1:

		if ((((sym >= 'a') && (sym <= 'z')) || ((sym >= 'A') && (sym <= 'Z')) || (sym == '_')))
		{
			M->q = configdea_states::q1;
		}

		else if (((sym == ' ')))
		{
			M->q = configdea_states::q2;
			markReadEnd(-1, section);
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q2:

		if (((sym == ' ')))
		{
			M->q = configdea_states::q2;
		}

		else if ((((sym >= 'a') && (sym <= 'z')) || ((sym >= 'A') && (sym <= 'Z')) || (sym == '_')))
		{
			M->q = configdea_states::q3;
			markReadBegin(0);
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q3:

		if ((((sym >= 'a') && (sym <= 'z')) || ((sym >= 'A') && (sym <= 'Z')) || (sym == '_')))
		{
			M->q = configdea_states::q3;
		}

		else if (((sym == ' ')))
		{
			M->q = configdea_states::q4;
			markReadEnd(-1, id);
		}

		else if (((sym == '=')))
		{
			M->q = configdea_states::q5;
			markReadEnd(-1, id);
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q4:

		if (((sym == '=')))
		{
			M->q = configdea_states::q5;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q5:

		if (((sym == ' ')))
		{
			M->q = configdea_states::q5;
		}

		else if ((((1) && (!(sym == ' ')) && (!(sym == '"')))))
		{
			M->q = configdea_states::q6;
			markReadBegin(0);
		}

		else if (((sym == '"')))
		{
			M->q = configdea_states::q8;
			markReadBegin(0);
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q6:

		if ((((1) && (!(sym == ' ')))))
		{
			M->q = configdea_states::q6;
		}

		else if (((sym == ' ')))
		{
			M->q = configdea_states::q7;
			markReadEnd(-1, value);
			save();
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q7:

		if (((sym == ' ')))
		{
			M->q = configdea_states::q7;
		}

		else if ((((sym >= 'a') && (sym <= 'z')) || ((sym >= 'A') && (sym <= 'Z')) || (sym == '_')))
		{
			M->q = configdea_states::q3;
			markReadBegin(0);
		}

		else if (((sym == '#')))
		{
			M->q = configdea_states::q1;
			markReadBegin(+1);
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q8:

		if ((((1) && (!(sym == '"')) && (!(sym == '\\')))))
		{
			M->q = configdea_states::q8;
		}

		else if (((sym == '\\')))
		{
			M->q = configdea_states::q9;
		}

		else if (((sym == '"')))
		{
			M->q = configdea_states::q7;
			markReadEnd(0, value);
			save();
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::q9:

		if (((1)))
		{
			M->q = configdea_states::q8;
		}

		else
		{
			M->q = configdea_states::trap;
		}

		break;

	case configdea_states::trap:
	default:
		break;
	}
}

int Cfg::Reader::configdea_exit(configdea_statemashine* M)
{
	int cs = M->q;
	return ((M->q == q1) || (M->q == q2) || (M->q == q6) || (M->q == q7));
}

Cfg::Reader::type_sup Cfg::Reader::gettype(string in)
{
	// any integer is a sequence of digits
	std::regex i("^[[:d:]]+$");

	// any float is a sequence of digits, then a point, then another sequence of digits
	//	in this case the length of a sequence of digits can be 0 ... meaning that a singe . will be counted as valid ... so at the place, where a digit sequence should be a zero can be interpreted : . = 0.0 ; .5 = 0.5 ;usw
	std::regex f("^[[:d:]]*\\.[[:d:]]*$");

	// a string here starts with a " and is followed by one character which can be followed by a random number of digits, characters and _. It ends with a " again
	std::regex s("^\"[[:alpha:]][_[:d:][:alpha:]]*\"$");

	if (regex_match(in, i)) return type_sup::integer;
	else if (regex_match(in, f)) return type_sup::flt;
	else if (regex_match(in, s)) return type_sup::str;

	return type_sup::none;
}

void Cfg::Reader::markReadBegin(int add)
{
	readBegin = currentIndex + add;
}
void Cfg::Reader::markReadEnd(int add, string& container)
{
	container = line.substr(readBegin, (currentIndex + add) - readBegin + 1);
}

void Cfg::Reader::save()
{
	switch (gettype(value))
	{
	case type_sup::integer:
		buildmap[section].setint(id, stoi(value));
		break;
	case type_sup::flt:
		if (value[0] == '.') value = string("0").append(value);
		if (value[value.size() - 1] == '.') value.append("0");
		buildmap[section].setfloat(id, stof(value));
		break;
	case type_sup::str:
		buildmap[section].setstring(id, value.substr(1, value.size() - 2));
		break;

	case type_sup::none:
		
		break;
	default:

		break;
	}
}

//dea code end

int Cfg::Reader::checkfile(string filename)
{
	std::ifstream in(filename);

	configdea_statemashine M;

	configdea_init(&M);

	while (std::getline(in, line))
	{
		int size = line.size();
		for (int i = 0; i < size; i++)
		{
			configdea_d_check(&M, line[i]);
		}
		configdea_d_check(&M, ' ');
		if (M.q == -1) return -1;
	}

	in.close();

	return 0;
}

map<string, Cfg::section> Cfg::Reader::readfile(string filename)
{
	std::ifstream in(filename);

	configdea_statemashine M;

	buildmap.clear(); //clear the buildmap before filling it again
	configdea_init(&M);

	while (std::getline(in, line))
	{
		int size = line.size();
		
		for (currentIndex = 0; currentIndex < size; currentIndex++)
		{
			if (line[currentIndex] == '\t') line[currentIndex] = ' ';
			configdea_d_load(&M, line[currentIndex]);
		}
		currentIndex++;
		configdea_d_load(&M, ' ');
		if (M.q == -1) { buildmap.clear(); return buildmap; }
	}

	in.close();

	return buildmap;
}

int Cfg::check(string filename)
{
	return r.checkfile(filename);
}

int Cfg::load(string filename)
{
	map<string, section> add = r.readfile(filename);
	if (add.size() == 0) return -1;

	sections.insert(add.begin(), add.end());

	return 0;
}

int Cfg::unload()
{
	sections.clear();
	return 0;
}

int Cfg::unload(string sectionname)
{
	auto it = sections.find(sectionname);
	if (it == sections.end()) return -1;
	sections.erase(it);
	return 0;
}

Cfg::section& Cfg::getSection(string sectionname)
{
	return sections.find(sectionname)->second;
}

int * Cfg::getint(string section, string key)
{
	auto it = sections.find(section);
	if (it != sections.end()) return it->second.getint(key);
	else return nullptr;
}

int * Cfg::getint(string key)
{
	for (auto e : sections)
	{
		auto val = e.second.getint(key);
		if (val) return val;
	}
	return nullptr;
}

float * Cfg::getfloat(string section, string key)
{
	auto it = sections.find(section);
	if (it != sections.end()) return it->second.getfloat(key);
	else return nullptr;
}

string * Cfg::getstring(string section, string key)
{
	auto it = sections.find(section);
	if (it != sections.end()) return it->second.getstring(key);
	else return nullptr;
}




std::ostream& operator<< (std::ostream& os, const Cfg& c)
{

	map <string, int>::iterator i;
	map <string, float>::iterator f;
	map <string, string>::iterator s;

	int akt = 0;
	int use = 1;

	for (auto e : c.sections)
	{
		os << "#" << e.first << endl;

		//fetch 1 string from each of the containers (which are sorted so its the smallest value (strings here so alphabetical))
			i = e.second.num.begin();
			f = e.second.flt.begin();
			s = e.second.str.begin();

		use = 1;
		akt = 0;

		while (use)
		{
			//basically merge sort with 3 containers with target container is the output stream

			if (i == e.second.num.end()) akt = akt | 1;
			if (f == e.second.flt.end()) akt = akt | 2;
			if (s == e.second.str.end()) akt = akt | 4;

			
			switch (akt)
			{
			case 0:
				use = (i->first <= f->first) ? ((i->first <= s->first) ? 1 : 3) : ((f->first <= s->first) ? 2 : 3);
				break;

			case 1:
				use = (f->first <= s->first) ? 2 : 3;
				break;
			case 2:
				use = (i->first <= s->first) ? 1 : 3;
				break;
			case 4:
				use = (i->first <= f->first) ? 1 : 2;
				break;
			case 3:
				use = 3;
				break;
			case 5:
				use = 2;
				break;
			case 6:
				use = 1;
				break;
			case 7:
			default:
				use = 0;
				break;
			}

			switch (use)
			{
			case 1:
				os << "\t" << i->first << " = " << i->second << endl;
				i++;
				break;
			case 2:
				os << "\t" << f->first << " = " << f->second << endl;
				f++;
				break;
			case 3:
				os << "\t" << s->first << " = \"" << s->second << "\"" << endl;
				s++;
				break;
			}
		}
		
	}

	return os;
}