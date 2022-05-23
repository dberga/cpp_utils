
#include "CommandWidget.hxx"
#include "GameInterface.hxx"

CommandWidget::CommandWidget(GameInterface * game)
	: _layout(this)
	, _south("Go south")
	, _north("Go north")
	, _east("Go east")
	, _west("Go west")
	, _use("Use item")
	, _itemLabel("Items")
	, _characterLabel("Characters")
{
	setFixedSize(300, 400);
	_layout.addWidget(&_south, 2, 0);
	connect(&_south, SIGNAL(clicked()), game, SLOT(goSouth()));
	_layout.addWidget(&_north, 0, 0);
	connect(&_north, SIGNAL(clicked()), game, SLOT(goNorth()));
	_layout.addWidget(&_east, 1, 0);
	connect(&_east, SIGNAL(clicked()), game, SLOT(goEast()));
	_layout.addWidget(&_west, 3, 0);
	connect(&_west, SIGNAL(clicked()), game, SLOT(goWest()));
	_layout.addWidget(&_use, 4, 0);
	connect(&_use, SIGNAL(clicked()), game, SLOT(use()));
	_layout.addWidget(&_itemLabel, 5, 0);
	_layout.addWidget(&_items, 6, 0);
	_layout.addWidget(&_characterLabel, 7, 0);
	_layout.addWidget(&_characters, 8, 0);
	_layout.setAlignment(Qt::AlignCenter);
}
