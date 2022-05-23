#include "World.hxx"
#include <MiniCppUnit.hxx>

#include <iostream>

class UseItemTests : public TestFixture<UseItemTests>
{
public:
	TEST_FIXTURE(UseItemTests)
	{
		TEST_CASE(test_useItem_whenNoLocation);
		TEST_CASE(test_useItem_whenNoCharacter);
		TEST_CASE(test_useItem_whenNoItem);
		TEST_CASE(test_useItem_whenLevelTooLow);
		TEST_CASE(test_useBasicItem);
		TEST_CASE(test_distributeMagic_whenNoLocation);
		TEST_CASE(test_distributeMagic);
		TEST_CASE(test_distributeMagic_onDamageCharacter);
		TEST_CASE(test_distributeMagic_onCureCharacter);
		TEST_CASE(test_useTrap);
		TEST_CASE(test_useTrapTwice);
		TEST_CASE(test_usePotion);
		TEST_CASE(test_usePotionTwice);
		TEST_CASE(test_useBomb_whenNotEnabled);
		TEST_CASE(test_useBomb);
		TEST_CASE(test_useBombTwice);
		TEST_CASE(test_useBombOnManyThings);
	}

	void test_useItem_whenNoLocation()
	{
		// requiere de un método "useItem" en World.hxx
		// requiere de un método "useItem" en Location.hxx
		// requiere de un método "use" en Item.hxx
		World world;
		try
		{
			world.useItem("Paradise", "Eve", "Apple");
			FAIL("Exception expected");
		}
		catch(LocationNotFound & e)
		{
			ASSERT_EQUALS("Location not found" , e.what());
		}
	}
	void test_useItem_whenNoCharacter()
	{
		World world;
		world.createLocation("Paradise");
		try
		{
			world.useItem("Paradise", "Eve", "Apple");
			FAIL("Exception expected");
		}
		catch(CharacterNotFound & e)
		{
			ASSERT_EQUALS("Character not found" , e.what());
		}
	}
	void test_useItem_whenNoItem()
	{
		// requiere de una nueva excepción "ItemNotFound"
		// requiere buscar instancias de "Item" en Location.hxx
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		try
		{
			world.useItem("Paradise", "Eve", "Apple");
			FAIL("Exception expected");
		}
		catch(ItemNotFound & e)
		{
			ASSERT_EQUALS("Item not found" , e.what());
		}
	}
      void test_useItem_whenLevelTooLow()
	{
			World world;
			world.createLocation("Paradise");
			world.createCharacter("Eve", 0);
			world.placeCharacter("Eve", "Paradise");
			world.createItemAtLocation("Paradise", "Apple", 1);
	    ASSERT_EQUALS("The level of Eve is too low\n"
				, world.useItem("Paradise", "Eve", "Apple"));
	}
	void test_useBasicItem()
	{
		// requiere de dos parámetros del método "use" en Item.hxx:
		//  - el personaje que usa el objeto
		//  - el lugar donde se usa
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		world.createItemAtLocation("Paradise", "Apple", 0);
    ASSERT_EQUALS(
			"Eve uses Apple at Paradise\n"
			, world.useItem("Paradise", "Eve", "Apple"));
	}
	void test_distributeMagic_whenNoLocation()
	{
		// requiere de un método "distributeMagic" en World.hxx
		// requiere de un método "distributeMagic" en Location.hxx
		World world;
		try
		{
			world.distributeMagic("Paradise", 5);
			FAIL("Exception expected");
		}
		catch(LocationNotFound & e)
		{
			ASSERT_EQUALS("Location not found" , e.what());
		}
	}
	void test_distributeMagic()
	{
		// requiere de un método "receiveMagic" en Item.hxx
		// requiere de un método "receiveMagic" en Character.hxx
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		world.createItemAtLocation("Paradise", "Apple", 0);
		world.createItemAtLocation("Paradise", "Lemon", 0);
		ASSERT_EQUALS(
      "Eve receives 5 magic points\n"
      "Apple receives 5 magic points\n"
      "Lemon receives 5 magic points\n"
			, world.distributeMagic("Paradise", 5));
	}
	void test_distributeMagic_onDamageCharacter()
	{
		// requiere de un método "createDamageCharacter" en World.hxx
		// requiere de una nueva clase que hereda de Character
		World world;
		world.createLocation("Paradise");
		world.createDamageCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
    ASSERT_EQUALS(
      "Eve receives 5 magic points\n"
			"Eve receives 5 damage\n"
			, world.distributeMagic("Paradise", 5));
	}
	void test_distributeMagic_onCureCharacter()
	{
		// requiere de un método "createCureCharacter" en World.hxx
		// requiere de una nueva clase que hereda de Character
		World world;	
		world.createLocation("Paradise");
		world.createCureCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
    ASSERT_EQUALS(
      "Eve receives 5 magic points\n"
			"Eve gains 5 life\n"
			, world.distributeMagic("Paradise", 5));
	}
	void test_useTrap()
	{
		// requiere hacer refactoring de Item con el patrón Decorator
		// requiere de un método "createTrapAtLocation" en World.hxx
		// requiere de un método "createTrap" en Location.hxx
		// una trampa está compuesta de dos comportamientos:
		//  - causar 5 daño al personaje que lo usa
		//  - desaparecer después de usarse
		//		(requiere de un método "removeItem" en Location.hxx)
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		world.createTrapAtLocation("Paradise", "Trap", 0);
    		ASSERT_EQUALS(
			"Eve uses Trap at Paradise\n"
			"Eve receives 5 damage\n"
			, world.useItem("Paradise", "Eve", "Trap"));
	}
	void test_useTrapTwice()
	{
		// como la trampa desaparece no se puede usar dos veces
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		world.createTrapAtLocation("Paradise", "Trap", 0);
	  world.useItem("Paradise", "Eve", "Trap");
		try
		{
			world.useItem("Paradise", "Eve", "Trap");
			FAIL("Exception expected");
		}
		catch(ItemNotFound & e)
		{
			ASSERT_EQUALS("Item not found" , e.what());
		}
	}
	void test_usePotion()
	{
		// requiere de un método "createPotionAtLocation" en World.hxx
		// requiere de un método "createPotion" en Location.hxx
		// una poción está compuesta de dos comportamientos:
		//  - curar 5 daño al personaje que lo usa
		//  - desaparecer después de usarse
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		world.createPotionAtLocation("Paradise", "Cure", 0);
    ASSERT_EQUALS(
			"Eve uses Cure at Paradise\n"
			"Eve gains 5 life\n"
			, world.useItem("Paradise", "Eve", "Cure"));
	}
	void test_usePotionTwice()
	{
		// como la poción desaparece no se puede usar dos veces
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		world.createPotionAtLocation("Paradise", "Cure", 0);
	  world.useItem("Paradise", "Eve", "Cure");
		try
		{
			world.useItem("Paradise", "Eve", "Cure");
			FAIL("Exception expected");
		}
		catch(ItemNotFound & e)
		{
			ASSERT_EQUALS("Item not found" , e.what());
		}
	}
	void test_useBomb_whenNotEnabled()
	{
		// requiere de un método "createBombAtLocation" en World.hxx
		// requiere de un método "createBomb" en Location.hxx
		// una bomba está compuesta de tres comportamientos:
		//  - distribuir 5 puntos mágicos en el lugar donde se usa
		//  - desaparecer después de usarse
		//  - inicialmente desactivado, necesita energía mágica para activarse
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		world.createBombAtLocation("Paradise", "Bomb", 0);
    ASSERT_EQUALS(
			"Bomb not enabled\n"
			, world.useItem("Paradise", "Eve", "Bomb"));
	}
	void test_useBomb()
	{
		// la bomba desaparece antes de distribuir energía mágica
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		world.createBombAtLocation("Paradise", "Bomb", 0);
    		world.distributeMagic("Paradise", 0);
    ASSERT_EQUALS(
			"Eve uses Bomb at Paradise\n"
      "Bomb explodes\n"
      "Eve receives 5 magic points\n"
			, world.useItem("Paradise", "Eve", "Bomb"));
	}
	void test_useBombTwice()
	{
		// como la bomba desaparece no se puede usar dos veces
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		world.createBombAtLocation("Paradise", "Bomb", 0);
    world.distributeMagic("Paradise", 0);
	  world.useItem("Paradise", "Eve", "Bomb");
		try
		{
			world.useItem("Paradise", "Eve", "Bomb");
			FAIL("Exception expected");
		}
		catch(ItemNotFound & e)
		{
			ASSERT_EQUALS("Item not found" , e.what());
		}
	}
	void test_useBombOnManyThings()
	{
		World world;
		world.createLocation("Paradise");
		world.createCureCharacter("Eve", 0);
		world.placeCharacter("Eve", "Paradise");
		world.createDamageCharacter("Adam", 1);
		world.placeCharacter("Adam", "Paradise");
		world.createBombAtLocation("Paradise", "Bomb", 0);
		world.createItemAtLocation("Paradise", "Apple", 0);
		world.createItemAtLocation("Paradise", "Lemon", 0);
    world.distributeMagic("Paradise", 0);
    ASSERT_EQUALS(
			"Eve uses Bomb at Paradise\n"
      "Bomb explodes\n"
      "Eve receives 5 magic points\n"
			"Eve gains 5 life\n"
      "Adam receives 5 magic points\n"
			"Adam receives 5 damage\n"
      "Apple receives 5 magic points\n"
      "Lemon receives 5 magic points\n"
			, world.useItem("Paradise", "Eve", "Bomb"));
	}
};

REGISTER_FIXTURE(UseItemTests)

