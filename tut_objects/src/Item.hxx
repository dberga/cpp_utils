#ifndef ITEM_HXX
#define ITEM_HXX
#include <string>



class Item
{
	std::string _name;
	unsigned _requiredLevel;
	bool _isEnabled;
	

public:
	Item()
		: _name("unknown")
		, _requiredLevel(0)
		, _isEnabled(true)
	{
	}
	virtual ~Item()
	{
	}
	const std::string name() const
	{
		return _name;
	}
	void name(const std::string & name)
	{
	_name = name;
	}
	unsigned requiredLevel() const
	{
	return _requiredLevel;
	}
	void requiredLevel(unsigned requiredLevel)
	{
	_requiredLevel = requiredLevel;
	}
	bool isEnabled() const
	{
	return _isEnabled;
	}
	void disable()
	{
	_isEnabled=false;
	}
	void enable()
	{
	_isEnabled=true;
	}
	virtual std::string use(const std::string & characterName, const std::string & locationName)
	{
	return characterName + " uses " + _name + " at " + locationName +"\n";
	}
	std::string receiveMagic(unsigned magicpoints)
	{
	std::string result;
	result += _name;
	result += " receives ";
	result += (magicpoints+48);
	result += " magic points\n";
	return result;
	}
};

class Trap : public Item
{


public:
	Trap()
	{
	}
	std::string use(const std::string & characterName, const std::string & locationName)
	{
	std::string result;
	result += characterName + " uses " + name() + " at " + locationName +"\n";
	result += characterName;
	result += " receives ";
	result += (5+48);
	result += " damage\n";
	return result;
	}
};

class Potion : public Item
{


public:
	Potion()
	{
	}
	std::string use(const std::string & characterName, const std::string & locationName)
	{
	std::string result;
	result += characterName + " uses " + name() + " at " + locationName +"\n";
	result += characterName;
	result += " gains ";
	result += (5+48);
	result += " life\n";
	return result;
	}
};

class Bomb : public Item
{


public:
	Bomb()
	{
	disable();
	}
	std::string use(const std::string & characterName, const std::string & locationName)
	{
		if(isEnabled() == false)
		{
		return name()+" not enabled\n";
		}else{
		std::string result;
		result += characterName + " uses " + name() + " at " + locationName +"\n";
		result += name() + " explodes\n";
		return result;
		}
	}
};
#endif

