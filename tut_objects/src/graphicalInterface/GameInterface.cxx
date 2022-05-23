
#include "GameInterface.hxx"
#include "WorldSubject.hxx"

#include <sstream>

void GameInterface::notify()
{
	std::string state, token, name;
	std::vector<std::string> items, characters;

	_south.update("");
	_north.update("");
	_east.update("");
	_west.update("");

	state = _world->locationDetails();
	std::istringstream is(state);
	is >> token >> name;
	_location.update(name);
	while (is >> token)
	{
		is >> name;
		if (token == "South:") _south.update(name);
		if (token == "North:") _north.update(name);
		if (token == "East:") _east.update(name);
		if (token == "West:") _west.update(name);
		if (token == "Item:")
		{
			items.push_back(name);
			is >> name;
		}
		if (token == "-") {
			characters.push_back(name);
			is >> name >> name;
		}
	}
	_command.updateItems(items);
	_command.updateCharacters(characters);
}

void GameInterface::goSouth()
{
	std::string result = _world->go("south");
	_output.setText(QString::fromStdString(result));
}

void GameInterface::goNorth()
{
	std::string result = _world->go("north");
	_output.setText(QString::fromStdString(result));
}

void GameInterface::goEast()
{
	std::string result = _world->go("east");
	_output.setText(QString::fromStdString(result));
}

void GameInterface::goWest()
{
	std::string result = _world->go("west");
	_output.setText(QString::fromStdString(result));
}

void GameInterface::use()
{
	std::string item = _command.getItem();
	if (item == "") _output.setText("There is no item here");
	else
	{
		std::string result = _world->useItem(item);
		_output.setText(QString::fromStdString(result));
	}
}

