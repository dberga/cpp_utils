#ifndef GameInterface_h
#define GameInterface_h

#include "CommandWidget.hxx"
#include "ExitWidget.hxx"
#include "WorldSubject.hxx"

class WorldSubject;

class GameInterface : public QWidget
{
	Q_OBJECT

	WorldSubject * _world;
	CommandWidget _command;
	ExitWidget _south;
	ExitWidget _north;
	ExitWidget _east;
	ExitWidget _west;
	ExitWidget _location;
	QLabel _output;

	QGridLayout _layout;

public:
	GameInterface(WorldSubject * world)
		: _world(world)
		, _command(this)
		, _south("South")
		, _north("North")
		, _east("East")
		, _west("West")
		, _location("Location")
		, _layout(this)
	{
		_layout.addWidget(&_south, 2, 1);
		_layout.addWidget(&_north, 0, 1);
		_layout.addWidget(&_east, 1, 2);
		_layout.addWidget(&_west, 1, 0);
		_layout.addWidget(&_location, 1, 1);
		_layout.addWidget(&_command, 0, 3, 3, 1);
		_layout.addWidget(&_output, 4, 0, 1, 4);
		_output.setAlignment(Qt::AlignCenter);
		notify();
	}

	void notify();

public slots:
	void goSouth();
	void goNorth();
	void goEast();
	void goWest();
	void use();
};

#endif
