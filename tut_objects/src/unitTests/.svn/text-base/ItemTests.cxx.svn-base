#include "Item.hxx"
#include <MiniCppUnit.hxx>


class ItemTest : public TestFixture<ItemTest>
{
public:
	TEST_FIXTURE(ItemTest)
	{
		TEST_CASE(test_name_byDefault);
		TEST_CASE(test_name_whenChanged);
		TEST_CASE(test_requiredLevel_byDefault);
		TEST_CASE(test_requiredLevel_whenChanged);
		TEST_CASE(test_isEnabled_byDefault);
		TEST_CASE(test_isEnabled_afterDisable);
		TEST_CASE(test_isEnabled_afterEnable);
	}

	void test_name_byDefault()
	{
		Item item;
		ASSERT_EQUALS("unknown", item.name());
	}
	void test_name_whenChanged()
	{
		Item item;
		item.name("An item");
		ASSERT_EQUALS("An item", item.name());
	}
	void test_requiredLevel_byDefault()
	{
		Item item;
		ASSERT_EQUALS(0u, item.requiredLevel());
	}
	void test_requiredLevel_whenChanged()
	{
		Item item;
		item.requiredLevel(40);
		ASSERT_EQUALS(40u, item.requiredLevel());
	}
	void test_isEnabled_byDefault()
	{
		Item item;
		ASSERT_EQUALS(true, item.isEnabled());
	}
	void test_isEnabled_afterDisable()
	{
		Item item;
		item.disable();
		ASSERT_EQUALS(false, item.isEnabled());
	}
	void test_isEnabled_afterEnable()
	{
		Item item;
		item.disable();
		item.enable();
		ASSERT_EQUALS(true, item.isEnabled());
	}

};

REGISTER_FIXTURE(ItemTest)

