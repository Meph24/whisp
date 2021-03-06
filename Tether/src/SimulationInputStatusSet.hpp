#ifndef CONTROLINPUTSTATUSSET_HPP
#     define CONTROLINPUTSTATUSSET_HPP

#include <array>
using std::array;

#include <cstdint>

#include <SFML/Network/Packet.hpp>

#include "glmutils.hpp"

using SignalCounter = std::uint16_t;

typedef std::int16_t IndexTypeBase;
template<IndexTypeBase max_indexable>
struct Index
{	
	typedef IndexTypeBase IndexType;
	typedef std::int32_t IndexCalcType;
private:
	mutable IndexType v;
public:
	constexpr static IndexType mod( const IndexType& x, const IndexType& m)
	{ 
		IndexCalcType _x = x; IndexCalcType _m = m;
		return (IndexType) (_x < 0 ? ((_x % _m) + _m) % _m : _x % _m );
	}

	Index() : v(0) {}
	Index(const IndexType& i) : v( mod(i, max_indexable) ) {}
	Index& operator=(const IndexType& i) { v = mod(i, max_indexable); }
	Index(const Index&) = default;
	Index& operator=(const Index&) = default;

	Index operator+(const Index& other) { return Index(mod(v + other, max_indexable)); }
	Index operator-(const Index& other){ return mod(v - other.v, max_indexable); }
	Index& operator+=(const Index& other) { v = mod( v + other.v, max_indexable); return *this; }
	Index& operator-=(const Index& other) { v = mod(v - other.v, max_indexable); return *this; }
	Index& operator++() { v = mod(++v, max_indexable); return *this; } //pre
	Index operator++(int){ return ++(Index(*this)); } //post
	Index& operator--(){ v = mod(--v, max_indexable); return *this; } //pre
	Index operator--(int){ return --(Index(*this)); } //post

	//since this index i supposed to be sent and may be overwritten without the context of the modulo
	// the value could get into undefined space that way
	// an additional modulus is applied on output, so the type gets robust in this way
	IndexType value() const { v = mod(v, max_indexable); return v; }
	operator IndexType() { return value(); }
};

template<IndexTypeBase max_indexable>
sf::Packet& operator<< (sf::Packet& p, const Index<max_indexable>& i)
{
	return p << i.value();
}


template<IndexTypeBase max_indexable>
sf::Packet& operator>> (sf::Packet& p, Index<max_indexable>& i)
{
	typename Index<max_indexable>::IndexType v; p >> v; i = Index<max_indexable>(v);
	return p;
}


#include "WallClock.hpp"
//this struct shall be kept sendable, that means no dynamic allocation and pointers allowed
struct SimulationInputStatusSet
{
	WallClock::time_point::rep timestamp = 0;
	//stati
	bool debug_screen_active = false;
	SignalCounter restart = 0;
	SignalCounter inventory = 0;
	SignalCounter selection_up = 0;
	SignalCounter selection_down = 0;
	bool draw_aabbs = false;
	SignalCounter benchmark = 0;
	bool slomo = false;
	bool pause = false;
	bool menu = false;
	Index<8> weapon_selection = 0;
	bool clip = false;
	vec3 walk = vec3(0.0f);
	vec3 turn = vec3(0.0f, 0.1f, 0.0f);
	bool trigger = false;

	bool selection_active = 0;
};

sf::Packet& operator<< (sf::Packet& p, const SimulationInputStatusSet& s);
sf::Packet& operator>> (sf::Packet& p, SimulationInputStatusSet& s);

#endif /* CONTROLINPUTSTATUSSET_HPP */
