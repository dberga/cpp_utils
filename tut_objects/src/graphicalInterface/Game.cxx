
#include "GameInterface.hxx"
#include "World.hxx"

int main(int argc, char * argv[])
{
	QApplication app(argc, argv);

	World world;
	world.registerCharacterCreator(new BasicCharacterCreator());
	world.registerCharacterCreator(new DamageCharacterCreator());
	world.registerCharacterCreator(new CureCharacterCreator());
	world.loadMap("maps/LargeMap.txt");
	world.registerPlayer("Warrior");

	GameInterface game(&world);
	world.registerObserver(&game);
	game.show();
	return app.exec();
}
