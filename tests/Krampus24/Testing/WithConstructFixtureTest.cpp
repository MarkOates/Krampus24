
#include <gtest/gtest.h>

#include <Krampus24/Testing/WithConstructFixture.hpp>


class Krampus24_Testing_WithConstructFixtureTest : public Krampus24::Testing::WithConstructFixture
{
};



TEST_F(Krampus24_Testing_WithConstructFixtureTest, will_work_as_expected_as_a_test_fixture)
{
   //TestFixtureClass with_construct_fixture;
   run_test_construct_loop();
}


//TEST(Krampus24_Testing_WithConstructFixtureTest, can_be_created_without_blowing_up)
//{
   //Krampus24::Testing::WithConstructFixture with_construct_fixture;
//}


//TEST(Krampus24_Testing_WithConstructFixtureTest, run__returns_the_expected_response)
//{
   //Krampus24::Testing::WithConstructFixture with_construct_fixture;
   //std::string expected_string = "Hello World!";
   //EXPECT_EQ(expected_string, with_construct_fixture.run());
//}


