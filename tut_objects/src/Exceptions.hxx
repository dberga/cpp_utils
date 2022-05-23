#ifndef EXCEPTIONS_HXX
#define EXCEPTIONS_HXX

class LocationNotFound : public std::exception
{
public:

	const char * what() const throw()
	{
	return "Location not found";
	}
};
class CharacterNotFound : public std::exception
{
public:

	const char * what() const throw()
	{
	return "Character not found";
	}
};
class ItemNotFound : public std::exception
{
public:

	const char * what() const throw()
	{
	return "Item not found";
	}
};
class IncorrectMap : public std::exception
{
public:

	const char * what() const throw()
	{
	return "Incorrect map";
	}
};
class CreatorNotFound : public std::exception
{
public:

	const char * what() const throw()
	{
	return "Creator not found";
	}
};
class InvalidType : public std::exception
{
public:

	const char * what() const throw()
	{
	return "Invalid type";
	}
};
class PlayerNotFound : public std::exception
{
public:

	const char * what() const throw()
	{
	return "Player not found";
	}
};







#endif

