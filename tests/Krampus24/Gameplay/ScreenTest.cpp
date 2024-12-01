
#include <gtest/gtest.h>

#include <Krampus24/Gameplay/Screen.hpp>

#include <AllegroFlare/Testing/WithAllegroRenderingFixture.hpp>
#include <AllegroFlare/Testing/WithAllegroFlareFrameworksFullFixture.hpp>

//#include <AllegroFlare/Frameworks/Full.hpp>


class TestGameConfiguration : public AllegroFlare::GameConfigurations::Base
{
public:
   // TODO: Upgrade this out of void* to a level base class (once the upstream design is clear)
   virtual AllegroFlare::Levels::Base *load_level_by_identifier(std::string identifier) override
   {
      // NOTE: Not used in this test
      //RPG::Gameplay::Level *result = new RPG::Gameplay::Level();
      //if (identifier == "test_level")
      //{
         //result->set_background_image_identifier("maps/rpg-fit-backgrounds-x2-01.png");
      //}
      //return result;
      return nullptr;
   }
};


class Krampus24_Gameplay_ScreenTest : public ::testing::Test {};
class Krampus24_Gameplay_ScreenTestWithAllegroRenderingFixture
   : public AllegroFlare::Testing::WithAllegroRenderingFixture
{};
class Krampus24_Gameplay_ScreenTestWithAllegroFrameworksFullFixture
   : public AllegroFlare::Testing::WithAllegroFlareFrameworksFullFixture
{};


TEST_F(Krampus24_Gameplay_ScreenTest, can_be_created_without_blowing_up)
{
   Krampus24::Gameplay::Screen screen;
}


static Krampus24::Gameplay::Entities::Base* build_entity(
   AllegroFlare::Vec3D position,
   bool affected_by_environmental_forces = true
)
{
   Krampus24::Gameplay::Entities::Base* result = new Krampus24::Gameplay::Entities::Base();
   result->get_placement_ref().position = position;
   result->get_placement_ref().size = { 0.5, 0.5, 0.5 };
   result->collides_with_player = true;
   result->affected_by_environmental_forces = affected_by_environmental_forces;
   return result;
}



TEST_F(Krampus24_Gameplay_ScreenTestWithAllegroFrameworksFullFixture,
   CAPTURE__TIMED_INTERACTIVE__will_run_as_expected)
{
   // Define our level
   //std::string collision_mesh_identifier = "some_level-03-bumps.obj";
   //std::string visual_mesh_identifier = "a_masonry_floor-01.obj";
   //std::string visual_mesh_texture_identifier = "tiles_dungeon_v1.1-1_to_1.png";
   //std::string visual_mesh_texture_identifier = "bark.jpg";


   std::string collision_mesh_identifier = "basic_baking-02-collision_mesh.obj";
   std::string visual_mesh_identifier = "basic_baking-02-visual.obj";
   std::string visual_mesh_texture_identifier = "basic_baking-01-baked_shadow-reduced.jpg";
   std::string blocking_filename = "basic_baking-02.blocking";


   //std::string visual_mesh_texture_identifier = "basic_baking-01-baked_shadow.png";

   //std::string visual_mesh_identifier = "basic_baking-01-visual.obj";
   //std::string collision_mesh_identifier = "station2-01-collision_mesh.obj";
   //std::string collision_mesh_identifier = "meshfps_level-03-collision_mesh.obj";
   //std::string visual_mesh_identifier = "";
   //std::string visual_mesh_texture_identifier = "";
   //std::string visual_mesh_identifier = "meshfps_level-02.obj";

   //std::string visual_mesh_identifier = "meshfps_level-03-visual.obj";
   //std::string visual_mesh_texture_identifier = "RETRO_TEXTURE_PACK_V15-atlas-resave.png";

   // Setup the screen
   TestGameConfiguration game_configuration;
   Krampus24::Gameplay::Screen screen;
   screen.set_data_folder_path(get_framework_data_folder_path());
   screen.set_event_emitter(get_framework_event_emitter());
   screen.set_bitmap_bin(get_framework_bitmap_bin());
   screen.set_font_bin(get_framework_font_bin());
   screen.set_model_bin(get_framework_model_bin());
   screen.set_game_configuration(&game_configuration);
   screen.set_collision_mesh_identifier(collision_mesh_identifier);
   screen.set_visual_mesh_identifier(visual_mesh_identifier);
   screen.set_visual_mesh_texture_identifier(visual_mesh_texture_identifier);
   screen.set_blocking_filename(blocking_filename);
   screen.initialize();

   // Build a map
   //AllegroFlare::Physics::CollisionMesh *collision_mesh = new AllegroFlare::Physics::CollisionMesh();
   //collision_mesh->set_model(get_framework_model_bin()->operator[](collision_mesh_identifier));
   //collision_mesh->load();
   //screen.set_collision_mesh(collision_mesh);

   // Create some entities
   AllegroFlare::Vec3D player_spawn_position = {2, 0.001, -2};
   
   Krampus24::Gameplay::Entities::Base* player_entity =
      new Krampus24::Gameplay::Entities::Base();
   player_entity->get_placement_ref().size = {0.5, 0.5, 0.5};
   player_entity->get_placement_ref().position = player_spawn_position;
   //player_entity->get_placement_ref().align = {0.5, 0.5, 0.5}; // Align has no effect in this case
   std::vector<Krampus24::Gameplay::Entities::Base*> entities;
   entities.push_back(player_entity); // Player entity
   entities.push_back(build_entity(AllegroFlare::Vec3D(3, 2, -8)));
   entities.push_back(build_entity(AllegroFlare::Vec3D(6, 4, -9)));
   //entities.push_back(build_entity(AllegroFlare::Vec3D(16, 4, -9)));
   //entities.push_back(build_entity(AllegroFlare::Vec3D(-3, 2, -8), false));
   screen.set_entities(&entities);

   // Set the visual mesh
   //AllegroFlare::Model3D *visual_mesh = get_framework_model_bin()->operator[](visual_mesh_identifier);
   //visual_mesh->set_texture(get_framework_bitmap_bin()->operator[](visual_mesh_texture_identifier));
   //screen.set_visual_mesh(visual_mesh);

   // Set the player spawn position
   screen.set_player_spawn_position(player_spawn_position);


   framework_register_and_activate_screen("screen", &screen);

   framework_run_loop(-12);
}


/*
   AllegroFlare::Camera2D hud_camera;
   AllegroFlare::Camera3D live_camera;
      AllegroFlare::Camera3D target_camera;
      if (false)
      { // First person
         target_camera.stepout = AllegroFlare::Vec3D(0, 1.75, 0);
         target_camera.tilt = 0.0;
      }
      if (false)
      { // Third person
         target_camera.stepout = AllegroFlare::Vec3D(0, 1.75, 8);
         target_camera.tilt = 0.3;
      }
      if (false)
      { // Top down
         target_camera.stepout = AllegroFlare::Vec3D(0, 0, 10);
         target_camera.tilt = 0.98;
      }
      if (true)
      { // Gobal birds eye
         target_camera.stepout = AllegroFlare::Vec3D(0, 0, 20);
         target_camera.tilt = 0.98;
      }

      live_camera = target_camera;
*/
