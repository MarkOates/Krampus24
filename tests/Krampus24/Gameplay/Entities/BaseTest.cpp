
#include <gtest/gtest.h>

#include <Krampus24/Gameplay/Entities/Base.hpp>


TEST(Krampus24_Gameplay_Entities_BaseTest, can_be_created_without_blowing_up)
{
   Krampus24::Gameplay::Entities::Base base;
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


