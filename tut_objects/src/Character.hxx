#ifndef CHARACTER_HXX
#define CHARACTER_HXX
#include <string>

class Character
{
	std::string _name;
	unsigned _level;
	unsigned _life;
	std::string _location;

public:
	Character()
		: _name("unknown")
		, _level(0)
		, _life(10)
		, _location("0")
	{
	}
	virtual ~Character()
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
	unsigned level() const
	{
		return _level;
	}
	void level(unsigned level){
	_level = level;
	}
	unsigned life() const
	{
		return _life;
	}
	void damage(unsigned damage)
	{
		if(damage > _life){
	 	_life=0;
		}else{
		_life = _life - damage;
		}
	}
	void cure(unsigned heal)
	{
		_life = _life + heal;
		if(_life>=10)
		{
		_life=10;
		} 
	}
	void location(const std::string locationName)
	{
	_location = locationName;
	}
	std::string currentlocation()
	{
	return _location;
	}
	void locateAt(const std::string locationName)
	{
	_location = locationName;
	}
	virtual std::string receiveMagic(unsigned magicpoints)
	{
	std::string result;
	result += _name;
	result += " receives ";
	result += (magicpoints+48);
	result += " magic points\n";
	return result;
	}

};
class DamageCharacter : public Character
{

public:
	DamageCharacter()
	{
	}
	std::string receiveMagic(unsigned magicpoints)
	{
	std::string result;
	result += name();
	result += " receives ";
	result += (magicpoints+48);
	result += " magic points\n";
		result += name();
		result += " receives ";
		result += (magicpoints+48);
		result += " damage\n";
	return result;
	}
};
class CureCharacter : public Character
{

public:
	CureCharacter()
	{
	}
	
	std::string receiveMagic(unsigned magicpoints)
	{
	std::string result;
	result += name();
	result += " receives ";
	result += (magicpoints+48);
	result += " magic points\n";
		result += name();
		result += " gains ";
		result += (magicpoints+48);
		result += " life\n";
	return result;
	}
};


#endif

