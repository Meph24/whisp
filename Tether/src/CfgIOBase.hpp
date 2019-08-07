#ifndef CFG_BASEIO_HPP
#     define CFG_BASEIO_HPP

#include "Cfg.hpp"

class CfgIOBase
{
public:
	virtual Cfg load() const = 0;
	virtual void save(const Cfg&) const = 0;
};
#endif /* CFG_BASEIO_HPP */
