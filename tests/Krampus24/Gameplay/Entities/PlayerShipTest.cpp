
#include <gtest/gtest.h>

#include <Krampus24/Gameplay/Entities/PlayerShip.hpp>
#include <Krampus24/Testing/WithConstructFixture.hpp>


class Krampus24_Gameplay_Entities_PlayerShipWithConstructFixtureTest : public Krampus24::Testing::WithConstructFixture
{
};


TEST_F(Krampus24_Gameplay_Entities_PlayerShipWithConstructFixtureTest, can_be_created_without_blowing_up)
{
   std::string name ="mydoor";
   std::vector<Krampus24::Gameplay::Entities::Base*> entities = Krampus24::Gameplay::Entities::PlayerShip::construct(
   //Krampus24::Gameplay::Entities::PlayerShip *door =
   //Krampus24::Gameplay::Entities::PlayerShip::construct(
      get_framework_model_bin(),
      get_framework_bitmap_bin(),
      get_framework_event_emitter(),
      get_screen_collision_mesh(),
      name,
      AllegroFlare::Vec3D(0, 0, 0)
      //-0.125 / 2
      //-0.25
   );
   for (auto &entity : entities) add_entity_to_pool(entity);

   // Add the sound effects
   AllegroFlare::AudioController *audio_controller = get_framework_audio_controller();
   audio_controller->set_and_load_sound_effect_elements(
      Krampus24::Gameplay::Entities::PlayerShip::build_audio_controller_sound_effect_list()
   );

   run_test_construct_loop();
}


/*
   float distance(const Vec3D &point1, const Vec3D &point2)
   {
      // TODO: add tests for this function
      return sqrt(
           ((point1.x - point2.x) * (point1.x - point2.x))
         + ((point1.y - point2.y) * (point1.y - point2.y))
         + ((point1.z - point2.z) * (point1.z - point2.z))
      );
   }



   float manhattan_distance(const vec2d *point1, const vec2d *point2)
   {
      // may consider writing out the abs function
      return std::abs(point2->x - point1->x) + std::abs(point2->y - point1->y);
   }

   float distance_squared(const vec2d &point1, const vec2d &point2)
   {
      return ((point1.x - point2.x) * (point1.x - point2.x)) + ((point1.y - point2.y) * (point1.y - point2.y));
   }
*/


/*
         AllegroFlare::Vec2D prime(x_prime, y_prime);
         float angle_in_radians = prime.get_angle();
         float angle_in_degrees = angle_in_radians * (180.0 / ALLEGRO_PI);
         float angle_in_units = angle_in_degrees / 360.0f;
*/


