#include "World.hxx"
#include <MiniCppUnit.hxx>


class WorldStructureTests : public TestFixture<WorldStructureTests>
{
public:
	TEST_FIXTURE(WorldStructureTests)
	{
		TEST_CASE(test_locations_afterAddingALocation);
		TEST_CASE(test_locations_byDefault);
		TEST_CASE(test_locations_afterAddingASecondLocation);
		TEST_CASE(test_locationDetails_whenNoLocation);
		TEST_CASE(test_locationDetails_whenEmptyAndUnconnected);
		TEST_CASE(test_locationDetails_whenLocationDoesNotExist);
		TEST_CASE(test_createItemAtLocation_withOneItem);
		TEST_CASE(test_createItemAtLocation_withWrongLocation);
		TEST_CASE(test_locationDetails_whenConnectedNorthSouth);
		TEST_CASE(test_locationDetails_whenConnectedEastWest);
		TEST_CASE(test_locationDetails_whenFullyConnected);
		TEST_CASE(test_characters_whenEmpty);
		TEST_CASE(test_characters_withOneCharacter);
		TEST_CASE(test_characters_withTwoCharacters);
		TEST_CASE(test_placeCharacter_whenCharacterDoesNotExist);
		TEST_CASE(test_locationDetails_withOneCharacter);
		TEST_CASE(test_locationDetails_withManyCharacters);
		TEST_CASE(test_locationDetails_withCharactersItemsAndConnections);
//		TEST_CASE(test_placeCharacter_whenWasAlreadyPlaced);

//		TEST_CASE(test_connectNorthToSouth_whenNorthAlreadyConnected); // Opcional
//		TEST_CASE(test_connectNorthToSouth_whenSouthAlreadyConnected); // Opcional
//		TEST_CASE(test_connectWestToEast_whenEastAlreadyConnected); // Opcional
//		TEST_CASE(test_connectWestToEast_whenWestAlreadyConnected); // Opcional
	}


	void test_locations_afterAddingALocation()
	{
		// No requiere Unit tests adicionales
		World world;
		world.createLocation("Hall");
		ASSERT_EQUALS(
			"Hall\n"
			, world.locations());
	}

	void test_locations_byDefault()
	{
		// No requiere Unit tests adicionales
		World world;
		ASSERT_EQUALS(
			""
			, world.locations());
	}

	void test_locations_afterAddingASecondLocation()
	{
		// No requiere Unit tests adicionales
		World world;
		world.createLocation("Hall");
		world.createLocation("Garden");
		ASSERT_EQUALS(
			"Hall\n"
			"Garden\n"
			, world.locations());
	}
	void test_locationDetails_whenNoLocation()
	{
		World world;
		try
		{
			world.locationDetails("Paradise");
			FAIL("Exception expected");
		}
		catch(LocationNotFound & e)
		{
			ASSERT_EQUALS("Location not found" , e.what());
		}
	}
	void test_locationDetails_whenEmptyAndUnconnected()
	{
		// Requiere un test unitario para Location::dump() que retorne el texto
		// Se puede hacer primero el unitario y despues usar Location::dump()
		// para pasar este funcional, o se puede pasar el funcional con una
		// chapuza (green), implementar el unitario y despues hacer el refactory
		// en el World para usar Location::dump.
		World world;
		world.createLocation("Madrid");
		ASSERT_EQUALS(
			"Location: Madrid\n"
			, world.locationDetails("Madrid"));
	}
	void test_locationDetails_whenLocationDoesNotExist()
	{
		// Este tests es para asegurarnos de que no casquemos solo si empty
		// sino tambien cuando existen otros pero no el que buscamos
		World world;
		world.createLocation("Madrid");
		try
		{
			world.locationDetails("Paradise");
			FAIL("Exception expected");
		}
		catch(LocationNotFound & e)
		{
			ASSERT_EQUALS("Location not found" , e.what());
		}
	}
	void test_createItemAtLocation_withOneItem()
	{
		// Lo ultimo que hay que hacer para pasar este test es llamar
		// a un metodo Location::createItem. Antes ya habreis testeado
		// ese metodo desde los unit tests de Location, para 1, 0, 2.
		// Sugerencia: para probar la composicion location->items usad un metodo
		// 'Location::items()' que devuelva la parte del texto de la 
		// descripcion de Location que corresponde a los items, despues 
		// haced un solo test de que Location::description() de que le
		// sumais esa parte y lo que ya teniamos.
		World world;
		world.createLocation("Hall");
		unsigned requiredLevel = 0;
		world.createItemAtLocation("Hall", "BulletinBoard", requiredLevel);
		ASSERT_EQUALS(
			"Location: Hall\n"
			"\tItem: BulletinBoard [0]\n"
			, world.locationDetails("Hall"));
	}
	void test_createItemAtLocation_withWrongLocation()
	{
		// Despues de pasar este test vereis que teneis codigo de busqueda
		// de location duplicado. Sugerencia de refactor: extraed un metodo
		// 'Location & World::findLocation(name)' y su version const usando
		// const_iterator que sea 
		// 'const Location & World::findLocation(name) const'
		// El codigo que use los findLocation casi lo podreis leer como texto.
		// Cada vez que tengais busquedas de este tipo refactorizarlas en metodos
		// de esta manera.
		World world;
		try
		{
			world.createItemAtLocation("Paradise", "BulletinBoard", 0);
			FAIL("Exception expected");
		}
		catch(LocationNotFound & e)
		{
			ASSERT_EQUALS("Location not found" , e.what());
		}
	}
	void test_locationDetails_whenConnectedNorthSouth()
	{
		// Recordad que aqui la asociacion es opcional, NO multiple, sin
		// propiedad y bidireccional. Usad la misma estrategia que en los
		// Items para componer la salida: testead primero usando un metodo 
		// Location::connections() que retorne solo la parte de la descripcion
		// de las salidas y luego, cuando lo tengais testeais que se añada
		// a Location::description.
		// Bastara conectar este par de direcciones usad un solo metodo
		// Location::connectSouth(Location & other) con tres tests unitarios:
		// LocationTests::test_connections_whenNotConnected()
		//  - facil al principio, pero obligara antes de cada test de los dos
		//    siguientes a empezar con un refactoring (un puntero siempre a null)
		// LocationTests::test_connections_whenConnectedAtSouth()
		//  - en connectSouth, establecer el forward-link _south
		//    del receptor al parametro.
		// LocationTests::test_connections_whenConnectedAtNorth()
		//  - en connectSouth, establecer el back-link _north
		//    del parametro al receptor.
		// LocationTests::test_description_whenConnectedNorthSouth()
		//  - en description() sumad items()
		// Lo ultimo a hacer una vez lo teneis todo preparado, es descomentar
		// este test, pasar el rojo y implementar connectNorthToSouth que busque 
		// los dos locations y llame a connectSouth
		World world;
		world.createLocation("Madrid");
		world.createLocation("Cadiz");
		world.createLocation("Bilbo");
		world.connectNorthToSouth("Madrid","Cadiz");
		world.connectNorthToSouth("Bilbo","Madrid");
		ASSERT_EQUALS(
			"Location: Madrid\n"
			"\tNorth: Bilbo\n"
			"\tSouth: Cadiz\n"
			, world.locationDetails("Madrid"));
	}
	void test_locationDetails_whenConnectedEastWest()
	{
		// Lo mismo que se hizo con North/South se hace ahora con West/East
		// El metodo seria Location::connectEast(Location &) y no haria
		// falta el desconectado.
		World world;
		world.createLocation("Madrid");
		world.createLocation("Valencia");
		world.createLocation("Badajoz");
		world.connectWestToEast("Badajoz","Madrid");
		world.connectWestToEast("Madrid","Valencia");
		ASSERT_EQUALS(
			"Location: Madrid\n"
			"\tEast: Valencia\n"
			"\tWest: Badajoz\n"
			, world.locationDetails("Madrid"));
	}
	void test_locationDetails_whenFullyConnected()
	{
		// Este tests es solo para asegurarnos de que el orden de las
		// conexiones es el mismo siempre. Es posible que os pase
		// sin hacer nada. Si no es asi, necesitais haced el test
		// correspondiente en las clases de abajo o modificar los que
		// tengais si fuera necesario.
		World world;
		world.createLocation("Madrid");
		world.createLocation("Valencia");
		world.createLocation("Cadiz");
		world.createLocation("Bilbo");
		world.createLocation("Badajoz");
		world.connectNorthToSouth("Madrid","Cadiz");
		world.connectNorthToSouth("Bilbo","Madrid");
		world.connectWestToEast("Badajoz","Madrid");
		world.connectWestToEast("Madrid","Valencia");
		ASSERT_EQUALS(
			"Location: Madrid\n"
			"\tNorth: Bilbo\n"
			"\tSouth: Cadiz\n"
			"\tEast: Valencia\n"
			"\tWest: Badajoz\n"
			, world.locationDetails("Madrid"));





	}
	void test_characters_whenEmpty()
	{
		World world;
		ASSERT_EQUALS(
			""
			, world.characters());
	}
	void test_characters_withOneCharacter()
	{
		World world;
		unsigned level = 3;
		world.createCharacter("Sami", level);
		ASSERT_EQUALS(
			"Sami\n"
			, world.characters());
	}
	void test_characters_withTwoCharacters()
	{
		World world;
		world.createCharacter("Sami", 2);
		world.createCharacter("Fijoles", 3);
		ASSERT_EQUALS(
			"Sami\n"
			"Fijoles\n"
			, world.characters());
	}
	void test_placeCharacter_whenCharacterDoesNotExist()
	{
		World world;
		world.createLocation("Hall");
		try
		{
			world.placeCharacter("God", "Hall");
			FAIL("Exception expected");
		}
		catch(CharacterNotFound & e)
		{
			ASSERT_EQUALS("Character not found" , e.what());
		}
	}
	void test_locationDetails_withOneCharacter()
	{
		// Se requieren poder agregar (sin propiedad) cero o un characters
		// en location. Implica pues unitarios en Location.
		// Sugerencia: usar un metodo Location::characters() que retorne
		// la parte de string como hicimos con 'items()' y 'connections()' 
		// Ojo: es muy parecido a lo de antes pero no es composicion, solo
		// nos guardarnos los punteros que nos pasan, sin new's ni deletes.
		// El metodo Location::placeCharacter(Character & ) recibira una 
		// referencia para indicar que no pasa responsabilidad de memoria,
		// pero nos la guardamos como puntero, al principio ha de poder ser cero.
		// En los tests de Location, crearemos Characters en la pila, que sera
		// la que gestionara la memoria.
		World world;
		world.createLocation("Hall");
		world.createCharacter("Sami", 200);
		world.placeCharacter("Sami", "Hall");
		ASSERT_EQUALS(
			"Location: Hall\n"
			"- Sami is here.\n"
			, world.locationDetails("Hall"));
	}
	void test_locationDetails_withManyCharacters()
	{
		// Haced un refactoring en Location para introducir el vector de
		// Characters. 
		// Recordar: duplicar, rellenar, usar, passar el test, limpar.
		World world;
		world.createLocation("Hall");
		world.createCharacter("Sami", 200);
		world.placeCharacter("Sami", "Hall");
		world.createCharacter("Frijoles", 400);
		world.placeCharacter("Frijoles", "Hall");
		ASSERT_EQUALS(
			"Location: Hall\n"
			"- Sami is here.\n"
			"- Frijoles is here.\n"
			, world.locationDetails("Hall"));
	}
	void test_locationDetails_withCharactersItemsAndConnections()
	{
		// Es posible que este test passe sin rojo, si las partes de
		// la descripcion las habeis puesto en este mismo orden,
		// sino es asi, cambiadlo para pasarlo a verde
		World world;
		world.createLocation("Hall");
		world.createLocation("Southern");
		world.createLocation("Northern");
		world.createLocation("Eastern");
		world.createLocation("Western");
		world.connectNorthToSouth("Hall","Southern");
		world.connectNorthToSouth("Northern","Hall");
		world.connectWestToEast("Western","Hall");
		world.connectWestToEast("Hall","Eastern");
		world.createItemAtLocation("Hall", "BulletinBoard", 0);
		world.createCharacter("Sami", 200);
		world.placeCharacter("Sami", "Hall");
		world.createCharacter("Frijoles", 400);
		world.placeCharacter("Frijoles", "Hall");
		ASSERT_EQUALS(
			"Location: Hall\n"
			"\tNorth: Northern\n"
			"\tSouth: Southern\n"
			"\tEast: Eastern\n"
			"\tWest: Western\n"
			"\tItem: BulletinBoard [0]\n"
			"- Sami is here.\n"
			"- Frijoles is here.\n"
			, world.locationDetails("Hall"));
	}

	void test_placeCharacter_whenWasAlreadyPlaced()
	{
		// Aqui vemos que nos conviene tener un backlink en el personaje
		// para poder desconectarlo de la localizacion antigua.
		// De hecho, si el personaje tiene ese back-link estaria bien
		// que el metodo de entrada para conectar el doble enlace fuera
		// de Character (Character::locateAt(Location &) )
		// Implica cambio de diseño, que haremos progresivamente, segun
		// esta guia:
		// - Como las dos clases se apuntaran mutuamente, primero haremos
		//   un forward declaration 'class Location;' en Character.hxx,
		//   en vez de incluir Location.hxx. El codigo que use miembros
		//   de Location tendra que ir forzosamente en Character.cxx
		//   donde si haremos el include de Location.hxx.
		// - CharacterTests::test_locateAt_whenNoPreviousLocation()
		//   testea que si llamamos a Character::locateAt(Location&)
		//   tiene el mismo efecto que Location::placeCharacter(Char&)
		//   Lo tendra porque lo pasaremos en verde delegando en ese
		//   mismo metodo.
		// - Refactor: Creamos miembro Location * _location en Character,
		//   inicializado a 0. En Character::locateAt lo setamos al parametro.
		// - LocationTests::test_unplaceCharacter()
		//   test para Location::unplaceCharacter(Character &) que quita
		//   el personaje del lugar. Lo necesitamos para mover el personaje.
		// - CharacterTests::test_locateAt_whenPreviousLocation()
		//   el test comprueba que si ponemos el personaje primero en un 
		//   location y despues lo ponemos en otro, el anterior no tiene
		//   trazas del personaje.
		//   (Similar a este test funcional pero a bajo nivel)
		//   Lo pasamos usando el nuevo metodo Location::unplaceCharacter
		// - Descomentamos finalmente este test y tenemos un rojo aun.
		// - En World::placeCharacter, llamamos a character::locateAt
		//   en vez de Location::placeCharacter y ya lo tenemos.
		World world;
		world.createLocation("Hall");
		world.createLocation("Garden");
		world.createCharacter("Sami", 200);
		world.placeCharacter("Sami", "Hall");
		world.placeCharacter("Sami", "Garden");
		ASSERT_EQUALS(
			"Location: Hall\n"
			"Location: Garden\n"
			"- Sami is here.\n"
			, world.locationDetails("Hall")+world.locationDetails("Garden"));
	}

	// Opcional (sube nota)
	void test_connectNorthToSouth_whenNorthAlreadyConnected()
	{
		World world;
		world.createLocation("Location");
		world.createLocation("Connection1");
		world.createLocation("Connection2");
		world.connectNorthToSouth("Location", "Connection1");
		world.connectNorthToSouth("Location", "Connection2");
		ASSERT_EQUALS(
			"Location: Location\n"
			"\tSouth: Connection2\n"
			"Location: Connection1\n"
			"Location: Connection2\n"
			"\tNorth: Location\n"
			,
			world.locationDetails("Location") +
			world.locationDetails("Connection1") +
			world.locationDetails("Connection2") );
	}
	// Opcional (sube nota)
	void test_connectNorthToSouth_whenSouthAlreadyConnected()
	{
		World world;
		world.createLocation("Location");
		world.createLocation("Connection1");
		world.createLocation("Connection2");
		world.connectNorthToSouth("Connection1", "Location");
		world.connectNorthToSouth("Connection2", "Location");
		ASSERT_EQUALS(
			"Location: Location\n"
			"\tNorth: Connection2\n"
			"Location: Connection1\n"
			"Location: Connection2\n"
			"\tSouth: Location\n"
			,
			world.locationDetails("Location") +
			world.locationDetails("Connection1") +
			world.locationDetails("Connection2") );
	}
	// Opcional (sube nota)
	void test_connectWestToEast_whenEastAlreadyConnected()
	{
		World world;
		world.createLocation("Location");
		world.createLocation("Connection1");
		world.createLocation("Connection2");
		world.connectWestToEast("Connection1", "Location");
		world.connectWestToEast("Connection2", "Location");
		ASSERT_EQUALS(
			"Location: Location\n"
			"\tWest: Connection2\n"
			"Location: Connection1\n"
			"Location: Connection2\n"
			"\tEast: Location\n"
			,
			world.locationDetails("Location") +
			world.locationDetails("Connection1") +
			world.locationDetails("Connection2") );
	}
	// Opcional (sube nota)
	void test_connectWestToEast_whenWestAlreadyConnected()
	{
		World world;
		world.createLocation("Location");
		world.createLocation("Connection1");
		world.createLocation("Connection2");
		world.connectWestToEast("Location", "Connection1");
		world.connectWestToEast("Location", "Connection2");
		ASSERT_EQUALS(
			"Location: Location\n"
			"\tEast: Connection2\n"
			"Location: Connection1\n"
			"Location: Connection2\n"
			"\tWest: Location\n"
			,
			world.locationDetails("Location") +
			world.locationDetails("Connection1") +
			world.locationDetails("Connection2") );
	}

};

REGISTER_FIXTURE(WorldStructureTests)

