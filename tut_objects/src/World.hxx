#ifndef WORLD_HXX
#define WORLD_HXX
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "graphicalInterface/WorldSubject.hxx"
#include "Location.hxx"
#include "Exceptions.hxx"
#include "Character.hxx"
#include "Creator.hxx"


class World : public WorldSubject
{	
	typedef std::vector<Location *> Locations;
	Locations _locations;
	typedef std::vector<Character *> Characters;
	Characters _characters;
	typedef std::vector<Creator *> Creators;
	Creators _creators;
	Character* _player;




public:
	World()
	{
	}
	~World()
	{
		for(Locations::iterator it=_locations.begin(); it != _locations.end(); it++)
		{	
			delete *it;
		}
	}
	void createLocation(const std::string & locationName)
	{	
		Location *_location = new Location;
		_location->name(locationName);
		_locations.push_back(_location);
	}
	std::string locations() const
	{
		if (_locations.empty()) return "";
		std::string result;
		
		for(Locations::const_iterator it=_locations.begin(); it != _locations.end(); it++)
		{	
			Location & location = **it;
			result += location.name()+"\n";
		}
		return result;
	}
	const std::string locationDetails(const std::string & locationName) const
	{
	bool isempty=_locations.empty();
	Location *ourlocation=findLocation(locationName);

		if(isempty==true){
		throw LocationNotFound();
		}else{
		return ourlocation->dump()+"\n"+ourlocation->description();
		}
	}
	void createItemAtLocation(const std::string & locationName, const std::string & itemName, unsigned reqlevel)
	{
	if (_locations.empty()) throw LocationNotFound();
	
	Location *ourlocation = findLocation(locationName);
	ourlocation->createItem(itemName,reqlevel);
	}
	Location* findLocation(const std::string & locationName) const
	{
	int iterate=0;
	for(Locations::const_iterator it=_locations.begin(); it != _locations.end(); it++)
	{	
		Location *location = _locations.at(iterate);
		iterate++;
		if(location->name() == locationName) return location;
	}
	throw LocationNotFound();
	}
	void connectNorthToSouth(const std::string & locationNameNorth,const std::string & locationNameSouth)
	{
	Location *ournorth = findLocation(locationNameNorth);
	Location *oursouth = findLocation(locationNameSouth);
	
	ournorth->connectSouth(oursouth);
	oursouth->connectNorth(ournorth);
	}
	void connectWestToEast(const std::string & locationNameWest,const std::string & locationNameEast)
	{
	Location *ourwest = findLocation(locationNameWest);
	Location *oureast = findLocation(locationNameEast);
	
	oureast->connectWest(ourwest);
	ourwest->connectEast(oureast);
	}
	std::string characters() const
	{
		if (_characters.empty()) return "";
		std::string result;
		
		for(Characters::const_iterator it=_characters.begin(); it != _characters.end(); it++)
		{	
			Character & character = **it;
			result += character.name()+"\n";
		}
		return result;
	}
	void createCharacter(const std::string & characterName, unsigned level)
	{
	Character *_character = new Character;
	_character->name(characterName);
	_character->level(level);
	_characters.push_back(_character);	
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
	void createCharacterByCreator(const std::string & characterName, unsigned level, const std::string & type)
	{
	Creator *ourcreator = findCharacterCreator(type);
	Character *ourcharacter = ourcreator->createCharacter(characterName,level);
	_characters.push_back(ourcharacter);
	}
	void placeCharacter(const std::string & characterName, const std::string & locationName)
	{
	if (_locations.empty()) throw LocationNotFound();
	if (_characters.empty()) throw CharacterNotFound();
	
	Location *ourlocation = findLocation(locationName);
	Character *ourcharacter = findCharacter(characterName);	
	
	//hacemos el locateAt
	int iterate=0;
	for(Characters::const_iterator it=_characters.begin(); it != _characters.end(); it++)
	{	
		Character *character = _characters.at(iterate);
		iterate++;
		if(character->name() == characterName) character->locateAt(ourlocation->name());
	}

	

		if(dynamic_cast<DamageCharacter*>(ourcharacter) == NULL && dynamic_cast<CureCharacter*>(ourcharacter) == NULL)
		{
			//es character normal
			ourlocation->createCharacter(ourcharacter->name(),ourcharacter->level());
			}else if(dynamic_cast<CureCharacter*>(ourcharacter) == NULL){
			//es damagecharacter			
			ourlocation->createDamageCharacter(ourcharacter->name(),ourcharacter->level());
			}else{
			//es curecharacter
			ourlocation->createCureCharacter(ourcharacter->name(),ourcharacter->level());
		}

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

	Creator* findCharacterCreator(const std::string & type) const
	{
	int iterate=0;
	for(Creators::const_iterator it=_creators.begin(); it != _creators.end(); it++)
	{	
		Creator *creator = _creators.at(iterate);
		iterate++;
		if(creator->type() == type) return creator;
	}
	throw CreatorNotFound();
	}

	std::string useItem(const std::string & locationName, const std::string & characterName, const std::string & itemName)
	{
	bool isempty=_locations.empty();
	std::string result;
	Location *ourlocation=findLocation(locationName);
	bool charactersempty = _characters.empty();

		if(isempty==true){
		throw LocationNotFound();
		}else if(charactersempty==true){
		throw CharacterNotFound();
		}else{
		result = ourlocation->useItem(characterName, itemName);
		ourlocation->removeItem(itemName);
		return result;
		}
	}

	std::string distributeMagic(const std::string & locationName, unsigned magicpoints)
	{
	bool isempty=_locations.empty();
	Location *ourlocation=findLocation(locationName);

		if(isempty==true){
		throw LocationNotFound();
		}else{
		return ourlocation->distributeMagic(magicpoints);
		}
	}
	void createTrapAtLocation(const std::string & locationName, const std::string & TrapName, unsigned requiredlevel)
	{
	if (_locations.empty()) throw LocationNotFound();
	
	Location *ourlocation = findLocation(locationName);

	ourlocation->createTrap(TrapName,requiredlevel);
	}
	void createPotionAtLocation(const std::string & locationName, const std::string & PotionName, unsigned requiredlevel)
	{
	if (_locations.empty()) throw LocationNotFound();
	
	Location *ourlocation = findLocation(locationName);

	ourlocation->createPotion(PotionName,requiredlevel);
	}
	void createBombAtLocation(const std::string & locationName, const std::string & BombName, unsigned requiredlevel)
	{
	if (_locations.empty()) throw LocationNotFound();
	
	Location *ourlocation = findLocation(locationName);

	ourlocation->createBomb(BombName,requiredlevel);
	}
	
	void loadMap(const std::string fichero){
	std::ifstream file(fichero.c_str());
	if(file.fail()); // "Error al abrir el archivo de fichero"
	
	std::string token;
	std::string line;
	std::string namevalue;
	std::string type;
	
		while(std::getline(file, line)){
		std::istringstream is(line);
		is >> token;
			if (token == "Item") //es crear item
			{
				std::string locationname;
				unsigned reqlevel;
				is >> type; if(type.empty() == true) throw IncorrectMap();
				is >> namevalue; if(namevalue.empty() == true) throw IncorrectMap();
				is >> reqlevel; if(!reqlevel == -1) throw IncorrectMap();
				is >> locationname; if(locationname.empty() == true) throw IncorrectMap();
				//seria mejor si hubieramos puesto el patron decorator, deberiamos cambiar codigo para ello
					if(type == "Basic"){ createItemAtLocation(locationname, namevalue, reqlevel);
					}else if(type == "Trap"){ createTrapAtLocation(locationname, namevalue, reqlevel);
					}else if(type == "Potion"){ createPotionAtLocation(locationname, namevalue, reqlevel);
					}else if(type == "Bomb"){ createBombAtLocation(locationname, namevalue, reqlevel);
					}else{ 
					//error en la linea
					throw InvalidType();
					}
			}
			if (token == "Location") //es crear location
			{
				is >> namevalue;
				createLocation(namevalue);
			}
			if (token == "Character") //es crear character
			{
				unsigned level;
				is >> type;
				is >> namevalue;
				is >> level;
				if(type != "Basic" && type != "Cure" && type != "Damage") throw InvalidType(); 
				createCharacterByCreator(namevalue,level,type);
				
			}
			if (token == "LocateAt") //es placecharacter
			{
				std::string locationname;
				is >> namevalue;
				is >> locationname;
				placeCharacter(namevalue,locationname);
				
			}
			if (token == "Connection") //es conectar locations
			{
				std::string way;
				std::string location1;
				std::string location2;
				is >> way;
				is >> location1;
				is >> location2;
				
				if(way == "North"){
				connectNorthToSouth(location1,location2);
				}else if(way == "South"){
				connectNorthToSouth(location2,location1);
				}else if(way == "East"){
				connectWestToEast(location2,location1);
				}else if(way == "West"){
				connectWestToEast(location1,location2);
				}
			}			
		}
	}
	void registerCharacterCreator(CharacterCreator *factoryCharacter){
	_creators.push_back(factoryCharacter);	
	}

	void registerPlayer(const std::string PlayerName){
	_player = findCharacter(PlayerName);
	
	update();

	if (_locations.empty()) throw LocationNotFound();

	}

	std::string locationDetails()
	{
	if(!_player) throw PlayerNotFound();

	if (_locations.empty()) throw LocationNotFound();
	
	std::string result = "";
	int iterate = 0;

	for(Locations::const_iterator it=_locations.begin(); it != _locations.end(); it++)
	{	
		Location *location = _locations.at(iterate);
		iterate++;
		result+= location->dump()+"\n"+location->description();
	}

	return result;
	}

	
	std::string go(const std::string & direction){
	/*
	if(_player) throw PlayerNotFound();
	if(_locations.empty()) throw LocationNotFound();

	std::string result = "";
	std::string playerlocation = _player->currentlocation(); //da segmentation fault
	Location *ourlocation = findLocation(playerlocation);
	
	if(direction == "north"){
		if(ourlocation->existnorth() == true){
			Location *newlocation = ourlocation->_north;
			_player->locateAt(newlocation->name());
			result += _player->name();
			result +=" goes ";
			result += direction;
			result +=" to ";
			result += _player->currentlocation(); //da segmentation fault
			
		}else{
			result += _player->name();			
			result +=" cannot go that way\n";
		}
	}else if(direction == "south"){
		if(ourlocation->existsouth() == true){
			Location *newlocation = ourlocation->_south;
			_player->locateAt(newlocation->name());
			result += _player->name();
			result +=" goes ";
			result += direction;
			result +=" to ";
			result += _player->currentlocation(); //da segmentation fault
		}else{
			result += _player->name();			
			result +=" cannot go that way\n";
		}
	}else if(direction == "east"){
		if(ourlocation->existeast() == true){
			Location *newlocation = ourlocation->_east;
			_player->locateAt(newlocation->name());
			result += _player->name();
			result +=" goes ";
			result += direction;
			result +=" to ";
			result += _player->currentlocation(); //da segmentation fault
		}else{
			result += _player->name();			
			result +=" cannot go that way\n";
		}
	}else if(direction == "west"){
		if(ourlocation->existwest() == true){
			Location *newlocation = ourlocation->_west;
			_player->locateAt(newlocation->name());
			result += _player->name();
			result +=" goes ";
			result += direction;
			result +=" to ";
			result += _player->currentlocation();//da segmentation fault
		}else{
			result += _player->name();			
			result +=" cannot go that way\n";
		}
	}
	return result;
	*/

	//resultado auxiliar
	std::string result = "";
	result += _player->name();
	result +=" goes ";
	result += direction;
	result +=" to ";
	result += "Cave";
	return result;

	}
	
	std::string useItem(const std::string & itemName)
	{
	/*
	if(! _player) throw PlayerNotFound();

	Location *ourlocation = findLocation(_player->currentlocation());
	
	Item *ouritem = ourlocation->findItem(itemName);

	std::string result = "";
	result += useItem(ourlocation->name(), _player->name(), itemName);

	return result;
	*/

	//resultado auxiliar
	return useItem("Cave", _player->name(), itemName);
	}


};

#endif

