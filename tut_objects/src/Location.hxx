#ifndef LOCATION_HXX
#define LOCATION_HXX
#include "Item.hxx"
#include "Character.hxx"
#include "Exceptions.hxx"
#include "Creator.hxx"
#include <string>
#include <vector>


class Location
{
std::string _name;
typedef std::vector<Item *> Items;
	Items _items;
typedef std::vector<Character *> Characters;
	Characters _characters;


public:

Location *_north;
Location *_south;
Location *_west;
Location *_east;

	Location()
		: _name("unknown")
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
	std::string dump()
	{
	return "Location: "+_name;
	}
	void createItem(const std::string & itemName, unsigned reqlevel)
	{
		Item *_item = new Item;
		_item->name(itemName);
		_item->requiredLevel(reqlevel);
		_items.push_back(_item);
	}
	std::string items() const
	{
		if (_items.empty()) return "";
		std::string result = "\t";
		result += "Item: ";
		
		for(Items::const_iterator it=_items.begin(); it != _items.end(); it++)
		{	
			Item & item = **it;
			result += item.name(); result +=" ["; result += (item.requiredLevel()+48); result += "]\n";
		}
		return result;
	}
	std::string description() const
	{
	std::string result = "";
	result += connections();
	result += items();
	result +=characters();
	return result;
	}
	void connectSouth(Location* & locationSouth)
	{
	_south = locationSouth;
	}
	void connectNorth(Location* & locationNorth)
	{
	_north = locationNorth;
	}
	void connectWest(Location* & locationWest)
	{
	_west = locationWest;
	}
	void connectEast(Location* & locationEast)
	{
	_east = locationEast;
	}
	std::string connections() const
	{ 
		bool existsouth = true;
		bool existnorth = true;
		bool existwest = true;
		bool existeast = true;
	
		if(! _south) existsouth = false;
		if(! _north ) existnorth = false;
		if(! _west ) existwest = false;
		if(! _east ) existeast = false;

		if(existnorth == true && existsouth == true && existeast == true && existwest == true)
		{
		return "\tNorth: "+_north->name()+"\n"+"\tSouth: "+_south->name()+"\n"+"\tEast: "+_east->name()+"\n"+"\tWest: "+_west->name()+"\n";
		}else if(existnorth == true && existsouth == true && existeast == true && existwest == false)
		{
		return "\tNorth: "+_north->name()+"\n"+"\tSouth: "+_south->name()+"\n"+"\tEast: "+_east->name()+"\n";
		}else if(existnorth == true && existsouth == true && existeast == false && existwest == true)
		{
		return "\tNorth: "+_north->name()+"\n"+"\tSouth: "+_south->name()+"\n"+"\tWest: "+_west->name()+"\n";
		}else if(existnorth == true && existsouth == false && existeast == true && existwest == true)
		{
		return "\tNorth: "+_north->name()+"\n"+"\tSouth: "+"\tEast: "+_east->name()+"\n"+"\tWest: "+_west->name()+"\n";
		}else if(existnorth == false && existsouth == true && existeast == true && existwest == true)
		{
		return "\tSouth: "+_south->name()+"\n"+"\tEast: "+_east->name()+"\n"+"\tWest: "+_west->name()+"\n";
		}else if(existnorth == true && existsouth == true && existeast == false && existwest == false)
		{
		return "\tNorth: "+_north->name()+"\n"+"\tSouth: "+_south->name()+"\n";
		}else if(existnorth == false && existsouth == false && existeast == true && existwest == true)
		{
		return "\tEast: "+_east->name()+"\n"+"\tWest: "+_west->name()+"\n";
		}else if(existnorth == false && existsouth == true && existeast == false && existwest == true)
		{
		return "\tSouth: "+_south->name()+"\n"+"\tWest: "+_west->name()+"\n";
		}else if(existnorth == true && existsouth == false && existeast == true && existwest == false)
		{
		return "\tNorth: "+_north->name()+"\n"+"\tEast: "+_east->name()+"\n";
		}else if(existnorth == false && existsouth == true && existeast == true && existwest == false)
		{
		return "\tSouth: "+_south->name()+"\n"+"\tEast: "+_east->name()+"\n";
		}else if(existnorth == true && existsouth == false && existeast == false && existwest == true)
		{
		return "\tNorth: "+_north->name()+"\n"+"\tWest: "+_west->name()+"\n";
		}else if(existnorth == false && existsouth == true && existeast == false && existwest == false)
		{
		return "\tSouth: "+_south->name()+"\n";
		}else if(existnorth == false && existsouth == false && existeast == true && existwest == false)
		{
		return "\tEast: "+_east->name()+"\n";
		}else if(existnorth == false && existsouth == false && existeast == false && existwest == true)
		{
		return "\tWest: "+_west->name()+"\n";
		}else{
		return "";
		}
	}
	void createCharacter(const std::string & characterName, unsigned level)
	{
		Character *_character = new Character;
		_character->name(characterName);
		_character->level(level);
		_characters.push_back(_character);
	}
	std::string characters() const
	{
		if (_characters.empty()) return "";
		std::string result;
		
		for(Characters::const_iterator it=_characters.begin(); it != _characters.end(); it++)
		{	
			Character & character = **it;
			result += "- "+character.name()+" is here.\n";
		}
		return result;
	}
	Character* findCharacter(const std::string & characterName) const
	{
	int iterate=0;
	for(Characters::const_iterator it=_characters.begin(); it != _characters.end(); it++)
	{	
		Character *character = _characters.at(iterate);
		iterate++;
		if(character->name() == characterName) return character;
	}
	throw CharacterNotFound();
	}
	Item* findItem(const std::string & itemName) const
	{
	int iterate=0;
	for(Items::const_iterator it=_items.begin(); it != _items.end(); it++)
	{	
		Item *item = _items.at(iterate);
		iterate++;
		if(item->name() == itemName) return item;
	}
	throw ItemNotFound();
	}
	std::string useItem(const std::string & characterName, const std::string & itemName)
	{
	bool itemsempty = _items.empty();
	Item *ouritem=findItem(itemName);
	Character *ourcharacter = findCharacter(characterName);
		if(itemsempty==true){
			throw ItemNotFound();
			}else if(ourcharacter->level() < ouritem->requiredLevel() ){
			return "The level of " + ourcharacter->name() + " is too low\n";		
			}else if(dynamic_cast<Bomb*>(ouritem) == NULL){
			//no usa bomb
			return ouritem->use(characterName, _name);
			}else{
			//usa bomb
			std::string result;
			result += ouritem->use(characterName, _name);
				if(ouritem->isEnabled() == true){
				int iterate=0;
					for(Characters::const_iterator it=_characters.begin(); it != _characters.end(); it++)
					{	
					Character *character = _characters.at(iterate);
					result += character->receiveMagic(5);
					iterate++;
					}
					for(Items::const_iterator it=_items.begin(); it != _items.end(); it++)
					{	
						Item & item = **it;
						item.enable();
						if(item.name() != ouritem->name()) result += item.receiveMagic(5);
					}
				}
			return result;
			}
	}
	void createDamageCharacter(const std::string & characterName, unsigned level)
	{
	DamageCharacter *_character = new DamageCharacter;
	_character->name(characterName);
	_character->level(level);
	_characters.push_back(_character);	
	}
	void createCureCharacter(const std::string & characterName, unsigned level)
	{
	CureCharacter *_character = new CureCharacter;
	_character->name(characterName);
	_character->level(level);
	_characters.push_back(_character);
	}	
	std::string distributeMagic(unsigned magicpoints)
	{
	std::string result;
		
		int iterate=0;
		for(Characters::const_iterator it=_characters.begin(); it != _characters.end(); it++)
		{	
		Character *character = _characters.at(iterate);
		result += character->receiveMagic(magicpoints);
		iterate++;
		}
		for(Items::const_iterator it=_items.begin(); it != _items.end(); it++)
		{	
			Item & item = **it;
			item.enable();
			result += item.receiveMagic(magicpoints);
		}
	return result;
	}
	void createTrap(const std::string & TrapName, unsigned requiredlevel)
	{
		Trap *_trap = new Trap;
		_trap->name(TrapName);
		_trap->requiredLevel(requiredlevel);
		_items.push_back(_trap);
	}
	void createPotion(const std::string & PotionName, unsigned requiredlevel)
	{
		Potion *_potion = new Potion;
		_potion->name(PotionName);
		_potion->requiredLevel(requiredlevel);
		_items.push_back(_potion);
	}
	void createBomb(const std::string & BombName, unsigned requiredlevel)
	{
		Bomb *_bomb = new Bomb;
		_bomb->name(BombName);
		_bomb->requiredLevel(requiredlevel);
		_items.push_back(_bomb);
	}
	void removeItem(std::string itemName){
		int iterate=0;
		unsigned catched;
		for(Items::const_iterator it=_items.begin(); it != _items.end(); it++)
		{	
			Item *item = _items.at(iterate);
			iterate++;
			if(item->name() == itemName)
			{
			catched = iterate;
			}
		}
		while(catched < _items.size()){
		int next = catched++;
		_items[catched]= _items[next];
		catched++;
		}
		_items.pop_back();
	}
	bool existnorth(){

		if(!_north){
		return false;
		}else{
		return true;
		}
	}

	bool existsouth(){

		if(!_south){
		return false;
		}else{
		return true;
		}
	}

	bool existeast(){

		if(!_east){
		return false;
		}else{
		return true;
		}
	}

	bool existwest(){

		if(!_west){
		return false;
		}else{
		return true;
		}
	}

};
#endif


