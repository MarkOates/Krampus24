
#include <gtest/gtest.h>

#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <AllegroFlare/Testing/WithAllegroRenderingFixture.hpp>
#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/Useful.hpp>


class Krampus24_Gameplay_Entities_BaseTest : public ::testing::Test {};
class Krampus24_Gameplay_EntitiesBaseTestWithAllegroRenderingFixture :
   public AllegroFlare::Testing::WithAllegroRenderingFixture
{};


TEST_F(Krampus24_Gameplay_Entities_BaseTest, can_be_created_without_blowing_up)
{
   Krampus24::Gameplay::Entities::Base base;
}


TEST_F(Krampus24_Gameplay_EntitiesBaseTestWithAllegroRenderingFixture, CAPTURE__VISUAL__draw__will_render_as_expected)
{
   AllegroFlare::Camera3D camera;
   camera.stepout = { 0, 0, 8 };
   camera.tilt = 0.125;
   camera.spin = 0.25;

   //AllegroFlare::Physics::AABB3D aabb3d(
      //AllegroFlare::Vec3D(-1, -1, -1),
      //AllegroFlare::Vec3D(1, 1, 1)
   //);

   Krampus24::Gameplay::Entities::Base entity;
   entity.placement.size = { 1, 1, 1 };
   entity.aabb3d.set_max(entity.placement.size);

   entity.aabb3d_alignment = { 0.5, 0.5, 0.5 };


   //ALLEGRO_COLOR{1.0, 0.65, 1.0, 1.0} // A lil pink color
   ALLEGRO_BITMAP* render_surface = al_get_target_bitmap();
   camera.setup_projection_on(render_surface);

   // Box at origin
   al_set_render_state(ALLEGRO_DEPTH_TEST, 0); // This
   entity.draw_aabb3d();
   //AllegroFlare::draw_crosshair(AllegroFlare::Vec3D(0, 0, 0), ALLEGRO_COLOR{1.0, 0.65, 0.0, 1.0}, 0.65);

   // Box colored with a lil offset
   entity.placement.position = AllegroFlare::Vec3D(4, 0, -1); // A different position
   entity.box_color = ALLEGRO_COLOR{0.0, 0.65, 1.0, 1.0}; // A bluegreen color
   entity.draw_aabb3d();
   al_set_render_state(ALLEGRO_DEPTH_TEST, 1);

   //AllegroFlare::Vec3D offset = AllegroFlare::Vec3D(4, 0, -1);
   //ALLEGRO_COLOR color = ALLEGRO_COLOR{1.0, 0.65, 1.0, 1.0}; // A lil pink color
   //entity->draw_aabb3d();
   //AllegroFlare::draw_crosshair(AllegroFlare::Vec3D(0, 0, 0), ALLEGRO_COLOR{1.0, 0.65, 0.0, 1.0}, 0.65);
   //aabb3d.draw(offset, color);


   al_flip_display();
   al_rest(2);
}


/*
  - name: aabb3d
    type: AllegroFlare::Physics::AABB3D
    init_with: '{}'
    constructor_arg: true
    exposed: true

  - name: aabb3d_alignmet
    type: AllegroFlare::Vec3D
    init_with: '{}'
    constructor_arg: true
    exposed: true
*/


