
#include "GameInterface.hxx"
#include "WorldSubject.hxx"

void WorldSubject::update()
{
	if (_observer) _observer->notify();
}
