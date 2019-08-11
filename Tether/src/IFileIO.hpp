#ifndef FILEIO_HPP
#     define FILEIO_HPP

template<typename T>
class IFileIO
{
public:
	virtual T load() const = 0;
	virtual void save(const T&) const = 0;
};


#endif /* FILEIO_HPP */
