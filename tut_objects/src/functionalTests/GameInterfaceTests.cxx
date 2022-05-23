#include "World.hxx"
#include <MiniCppUnit.hxx>

class GameInterfaceTests : public TestFixture<GameInterfaceTests>
{
public:
	TEST_FIXTURE(GameInterfaceTests)
	{
		TEST_CASE(test_registerPlayer_whenNoPlayer);
		TEST_CASE(test_registerPlayer_whenNoLocation);
		TEST_CASE(test_locationDetails_whenNoPlayer);
		TEST_CASE(test_locationDetails);
//		TEST_CASE(test_goNorth_whenNoPlayer);
//		TEST_CASE(test_goNorth_whenNoExit);
//		TEST_CASE(test_goNorth);
//		TEST_CASE(test_useItem_whenNoPlayer);
//		TEST_CASE(test_useItem_whenNoItem);
//		TEST_CASE(test_useItem);
	}

	void test_registerPlayer_whenNoPlayer()
	{
		World world;
		try
		{
			world.registerPlayer("Adam");
			FAIL("Exception expected");
		}
		catch(CharacterNotFound & e)
		{
			ASSERT_EQUALS("Character not found" , e.what());
		}
	}

	void test_registerPlayer_whenNoLocation()
	{
		World world;
		world.createCharacter("Adam", 0);
		try
		{
			world.registerPlayer("Adam");
			FAIL("Exception expected");
		}
		catch(LocationNotFound & e)
		{
			ASSERT_EQUALS("Location not found" , e.what());
		}
	}

	void test_locationDetails_whenNoPlayer()
	{
		World world;
		try
		{
			world.locationDetails();
			FAIL("Exception expected");
		}
		catch(PlayerNotFound & e)
		{
			ASSERT_EQUALS("Player not found" , e.what());
		}
	}

	void test_locationDetails()
	{
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Adam", 0);
		world.placeCharacter("Adam", "Paradise");
		world.registerPlayer("Adam");
		ASSERT_EQUALS(
			"Location: Paradise\n"
			"- Adam is here.\n"
			, world.locationDetails());
	}

	void test_goNorth_whenNoPlayer()
	{
		World world;
		try
		{
			world.go("north");
			FAIL("Exception expected");
		}
		catch(PlayerNotFound & e)
		{
			ASSERT_EQUALS("Player not found" , e.what());
		}
	}

	void test_goNorth_whenNoExit()
	{
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Adam", 0);
		world.placeCharacter("Adam", "Paradise");
		world.registerPlayer("Adam");
		ASSERT_EQUALS(
			"Adam cannot go that way\n"
			, world.go("north"));
	}

	void test_goNorth()
	{
		World world;
		world.createLocation("Paradise");
		world.createLocation("Earth");
		world.connectNorthToSouth("Paradise","Earth");
		world.createCharacter("Adam", 0);
		world.placeCharacter("Adam", "Earth");
		world.registerPlayer("Adam");
		ASSERT_EQUALS(
			"Adam goes north to Paradise\n"
			, world.go("north"));
	}

	void test_useItem_whenNoPlayer()
	{
		World world;
		try
		{
			world.useItem("Apple");
			FAIL("Exception expected");
		}
		catch(PlayerNotFound & e)
		{
			ASSERT_EQUALS("Player not found" , e.what());
		}
	}

	void test_useItem_whenNoItem()
	{
		World world;
		world.createLocation("Paradise");
		world.createCharacter("Adam", 0);
		world.placeCharacter("Adam", "Paradise");
		world.registerPlayer("Adam");
		try
		{
			world.useItem("Apple");
			FAIL("Exception expected");
		}
		catch(ItemNotFound & e)
		{
			ASSERT_EQUALS("Item not found" , e.what());
		}
	}

	void test_useItem()
	{
		World world;
		world.createLocation("Paradise");
		world.createItemAtLocation("Paradise", "Apple", 0);
		world.createCharacter("Adam", 0);
		world.placeCharacter("Adam", "Paradise");
		world.registerPlayer("Adam");
		ASSERT_EQUALS(
			"Adam uses Apple at Paradise\n"
			, world.useItem("Apple"));
	}
};

REGISTER_FIXTURE(GameInterfaceTests)

