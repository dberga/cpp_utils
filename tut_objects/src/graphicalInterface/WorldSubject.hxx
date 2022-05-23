#ifndef WorldSubject_h
#define WorldSubject_h

#include <string>
#include "GameInterface.hxx"

class WorldSubject
{
	GameInterface * _observer;

public:
	WorldSubject()
		: _observer(0)
	{
	}
	void registerObserver(GameInterface * observer)
	{
		_observer = observer;
	}
	void update();
	virtual std::string locationDetails() = 0;
	virtual std::string go(const std::string & direction) = 0;
	virtual std::string useItem(const std::string & itemName) = 0;
};

#endif
