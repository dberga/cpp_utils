#ifndef CREATOR_HXX
#define CREATOR_HXX
#include "Character.hxx"
#include "Item.hxx"


////////"ABSTRACT" CREATOR ///////

class Creator
{


public:
	virtual ~Creator() {}
	virtual std::string type() = 0;
	virtual Character* createCharacter(const std::string & characterName, unsigned level) = 0;
};


////////ABSTRACT CHARACTER CREATOR ///////////
class CharacterCreator : public Creator
{


public:
	virtual ~CharacterCreator() {}
	virtual Character* createCharacter(const std::string & characterName, unsigned level) = 0;
	virtual std::string type() = 0;
};
//////// CHARACTER CREATORS ///////////
class BasicCharacterCreator : public CharacterCreator
{


public:
	BasicCharacterCreator()
	{
	}
	Character* createCharacter(const std::string & characterName, unsigned level){
	Character *_character = new Character();
	_character->name(characterName);
	_character->level(level);
	return _character;
	}
	std::string type(){
	return "Basic";
	}
};


class CureCharacterCreator : public CharacterCreator
{


public:
	CureCharacterCreator()
	{
	}
	Character* createCharacter(const std::string & characterName, unsigned level){
	CureCharacter *_character = new CureCharacter();
	_character->name(characterName);
	_character->level(level);
	return _character;
	}
	std::string type(){
	return "Cure";
	}
};

class DamageCharacterCreator : public CharacterCreator
{


public:
	DamageCharacterCreator()
	{
	}
	Character* createCharacter(const std::string & characterName, unsigned level){
	DamageCharacter *_character = new DamageCharacter();
	_character->name(characterName);
	_character->level(level);
	return _character;
	}
	std::string type(){
	return "Damage";
	}
};



#endif

