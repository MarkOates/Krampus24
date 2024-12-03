
#include <gtest/gtest.h>

#include <Krampus24/Gameplay/Entities/Hen.hpp>
#include <Krampus24/Testing/WithConstructFixture.hpp>


class Krampus24_Gameplay_Entities_HenWithConstructFixtureTest : public Krampus24::Testing::WithConstructFixture
{
};


TEST_F(Krampus24_Gameplay_Entities_HenWithConstructFixtureTest, can_be_created_without_blowing_up)
{
   Krampus24::Gameplay::Entities::Hen *hen = Krampus24::Gameplay::Entities::Hen::construct(
      get_framework_model_bin(),
      get_framework_bitmap_bin()
   );
   run_test_construct_loop();
}


