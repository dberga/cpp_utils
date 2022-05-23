#include "Location.hxx"
#include <MiniCppUnit.hxx>


class LocationTest : public TestFixture<LocationTest>
{
public:
	TEST_FIXTURE(LocationTest)
	{
		TEST_CASE(test_name_byDefault);
		TEST_CASE(test_name_whenChanged);
		TEST_CASE(test_dump);
		TEST_CASE(test_createItem_and_items);
		//TEST_CASE(test_connections_whenNotConnected);
		//TEST_CASE(test_connections_whenConnectedAtSouth);
		//TEST_CASE(test_connections_whenConnectedAtNorth);
		//TEST_CASE(test_description_whenConnectedNorthSouth);
	}

	void test_name_byDefault()
	{
		Location location;
		ASSERT_EQUALS("unknown", location.name());
	}
	void test_name_whenChanged()
	{
		Location location;
		location.name("A location");
		ASSERT_EQUALS("A location", location.name());
	}
	void test_dump()
	{
		Location location;
		location.name("DUMPIWUMPI");
		ASSERT_EQUALS("Location: DUMPIWUMPI", location.dump());
	}
	void test_createItem_and_items()
	{
		Location location;
		location.name("DUMPIWUMPI");
		location.createItem("Hacha", 5);
		ASSERT_EQUALS("\tItem: Hacha [5]\n", location.items());
	}
	void test_connections_whenNotConnected()
	{
	Location *location;
	location->name("DUMPIWUMPI");
	ASSERT_EQUALS("", location->connections());
	}
	void test_connections_whenConnectedAtSouth()
	{
	Location *location;
	location->name("DUMPIWUMPI");
	Location *location2;
	location2->name("ABAIX");
	location->connectSouth(location2);
	ASSERT_EQUALS("\tSouth: ABAIX\n", location->connections());
	}
	void test_connections_whenConnectedAtNorth()
	{
	Location *location;
	location->name("DUMPIWUMPI");
	Location *location2;
	location2->name("DALT");
	location->connectNorth(location2);
	ASSERT_EQUALS("\tNorth: DALT\n", location->connections());
	}
	void test_description_whenConnectedNorthSouth()
	{
	Location *location;
	location->name("DUMPIWUMPI");
	Location *location2;
	location2->name("SURT PER BAIX");
	location->connectSouth(location2);
	Location *location3;
	location3->name("DALT");
	location->connectNorth(location3);
	ASSERT_EQUALS("\tNorth: DALT\n"
		       "\tSouth: ABAIX\n", location->connections());
	}

};

REGISTER_FIXTURE(LocationTest)

