
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
      get_framework_bitmap_bin(),
      AllegroFlare::Vec3D(0, 0, 0),
      3.0
   );
   add_entity_to_pool(hen);
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


/*
      Krampus24::Gameplay::Entities::SlidingDoor* result = new Krampus24::Gameplay::Entities::SlidingDoor;
      result->affected_by_environmental_forces = false;
      result->collides_with_player = true;
      result->placement.position = initial_position;
      result->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense

      result->placement.size = { 5.0, 4.0, 5.0 };
      result->aabb3d.set_max(result->placement.size);
      result->aabb3d_alignment = { 0.5, 0.005, 0.5 }; // Just slightly below the floor
      result->initial_position = initial_position;
      result->placement.rotation.y = rotation;
      result->player_can_inspect_or_use = true;
      result->locked = false;
*/


