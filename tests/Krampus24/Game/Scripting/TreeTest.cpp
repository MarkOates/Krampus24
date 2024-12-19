
#include <gtest/gtest.h>

#include <Krampus24/Game/Scripting/Tree.hpp>


#include <AllegroFlare/Testing/ErrorAssertions.hpp>
#include <AllegroFlare/DeploymentEnvironment.hpp>
#include <AllegroFlare/Testing/WithAllegroFlareFrameworksFullFixture.hpp>



#include <Krampus24/Gameplay/Screen.hpp>
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



class Krampus24_Game_Scripting_TreeTest : public ::testing::Test {};
//class Krampus24_Game_Scripting_TreeTestWithAllegroRenderingFixture
   //: public AllegroFlare::Testing::WithAllegroRenderingFixture
//{};
class Krampus24_Game_Scripting_TreeTestWithAllegroFrameworksFullFixture
   : public AllegroFlare::Testing::WithAllegroFlareFrameworksFullFixture
{};



TEST_F(Krampus24_Game_Scripting_TreeTest, can_be_created_without_blowing_up)
{
   Krampus24::Game::Scripting::Tree tree;
}


TEST_F(Krampus24_Game_Scripting_TreeTest,
   build_storyboard_text_from_file__on_a_file_that_does_not_exist__will_throw_an_error)
{
   AllegroFlare::DeploymentEnvironment deployment_environment("test");
   Krampus24::Game::Scripting::Tree tree;
   tree.set_data_folder_path(deployment_environment.get_data_folder_path());

   tree.build_storyboard_text_from_file("tablet_in_zoo.txt");
}


TEST_F(Krampus24_Game_Scripting_TreeTest, build_storyboard_text_from_file__will_build_the_expected_pages)
{
   AllegroFlare::DeploymentEnvironment deployment_environment("test");
   Krampus24::Game::Scripting::Tree tree;
   tree.set_data_folder_path(deployment_environment.get_data_folder_path());

   EXPECT_THROW_WITH_MESSAGE(
      tree.build_storyboard_text_from_file("a_file_that_does_not_exist.txt"),
      //custom_properties.get_int("key-that-does-not-exist"),
      std::runtime_error,
      "[Krampus24::Game::Scripting::Tree::build_storyboard_text_from_file]: error: "
         "Could not find file \"tests/fixtures/stories/a_file_that_does_not_exist.txt\"."
   );
   //tree.build_storyboard_text_from_file("a_file_that_does_not_exist.txt");
}


/*
      //AllegroFlare::FontBin* font_bin = primary_gameplay_screen->get_font_bin();

      //identifier = primary_gameplay_screen->get_arbitrary_storyboard_screen_identifier_to_start();
*/


/*
      static std::map<std::string, std::function<void()>> items_map = {
         { "pig_storyboard", [this, &result]() { result = {
            create_storyboard_page__text(
              "This is new text from an arbitrary \"pig_storyboard\" storyboard screen."
            ),
         };}},
      };
*/

//quintessence/Krampus24/Game/Configurations/Main.q.yml

TEST_F(Krampus24_Game_Scripting_TreeTestWithAllegroFrameworksFullFixture,
   CAPTURE__TIMED_INTERACTIVE__will_run_as_expected)
{
   // Define our level
   //std::string collision_mesh_identifier = "some_level-03-bumps.obj";
   //std::string visual_mesh_identifier = "a_masonry_floor-01.obj";
   //std::string visual_mesh_texture_identifier = "tiles_dungeon_v1.1-1_to_1.png";
   //std::string visual_mesh_texture_identifier = "bark.jpg";


   //std::string collision_mesh_identifier = "basic_baking-02-collision_mesh.obj";
   //std::string visual_mesh_identifier = "basic_baking-02-visual.obj";
   //std::string visual_mesh_texture_identifier = "basic_baking-01-baked_shadow-reduced.jpg";
   //std::string blocking_filename = "basic_baking-02.blocking";

   //std::string collision_mesh_identifier = "tree-03-collision_mesh.obj";
   //std::string visual_mesh_identifier = "tree-03-visual.obj";
   //std::string visual_mesh_texture_identifier = "RETRO_TEXTURE_PACK_V15-atlas-01.png";
   //std::string blocking_filename = "tree-03.blocking";


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
   screen.set_audio_controller(get_framework_audio_controller());
   screen.set_event_emitter(get_framework_event_emitter());
   screen.set_bitmap_bin(get_framework_bitmap_bin());
   screen.set_font_bin(get_framework_font_bin());
   screen.set_model_bin(get_framework_model_bin());
   screen.set_dialog_system(get_framework_dialog_system());
   screen.set_game_configuration(&game_configuration);
   screen.set_build_scripting_instance_func(
      [this](Krampus24::Gameplay::Screen* screen) -> Krampus24::Gameplay::ScriptingInterface* {

      //std::cout << "---- ****** inside building scripting ----" << std::endl;
      // Build a scripting
      Krampus24::Game::Scripting::Tree *scripting = new Krampus24::Game::Scripting::Tree;
      scripting->set_data_folder_path(get_framework_data_folder_path());
      scripting->set_entities(&screen->get_entities_ref());
      scripting->set_collision_observer(&screen->get_collision_observer_ref());
      //scripting->set_collision_mesh(screen->get_collision_mesh());
      //scripting->set_bitmap_bin(screen->get_bitmap_bin());
      scripting->set_font_bin(screen->get_font_bin());
      scripting->set_dialog_system(screen->get_dialog_system());
      scripting->set_event_emitter(screen->get_event_emitter());
      //scripting->set_data_folder_path(get_framework_data_folder_path());
      scripting->initialize();

      return scripting;
   });

   //screen.set_collision_mesh_identifier(collision_mesh_identifier);
   //screen.set_visual_mesh_identifier(visual_mesh_identifier);
   //screen.set_visual_mesh_texture_identifier(visual_mesh_texture_identifier);
   //screen.set_blocking_filename(blocking_filename);
   screen.set_on_finished_callback_func([](AllegroFlare::Screens::Gameplay*, void*){
      std::cout << "-- inside provided on_finished_callback_func()" << std::endl;
      throw std::runtime_error("Game is completed.");
   });
   screen.initialize();

   // Build a map
   //AllegroFlare::Physics::CollisionMesh *collision_mesh = new AllegroFlare::Physics::CollisionMesh();
   //collision_mesh->set_model(get_framework_model_bin()->operator[](collision_mesh_identifier));
   //collision_mesh->load();
   //screen.set_collision_mesh(collision_mesh);

   // Create some entities
   //AllegroFlare::Vec3D player_spawn_position = {2, 0.001, -2};
   
   //Krampus24::Gameplay::Entities::Base* player_entity =
      //new Krampus24::Gameplay::Entities::Base();
   //player_entity->get_placement_ref().size = {0.5, 0.5, 0.5};
   //player_entity->get_placement_ref().position = player_spawn_position;
   //player_entity->get_placement_ref().align = {0.5, 0.5, 0.5}; // Align has no effect in this case
   //std::vector<Krampus24::Gameplay::Entities::Base*> &entities = screen.get_entities_ref();
   //entities.push_back(player_entity); // Player entity
   //entities.push_back(build_entity(AllegroFlare::Vec3D(3, 2, -8)));
   //entities.push_back(build_entity(AllegroFlare::Vec3D(6, 4, -9)));
   //entities.push_back(build_entity(AllegroFlare::Vec3D(16, 4, -9)));
   //entities.push_back(build_entity(AllegroFlare::Vec3D(-3, 2, -8), false));
   //screen.set_entities(&entities);

   // Set the visual mesh
   //AllegroFlare::Model3D *visual_mesh = get_framework_model_bin()->operator[](visual_mesh_identifier);
   //visual_mesh->set_texture(get_framework_bitmap_bin()->operator[](visual_mesh_texture_identifier));
   //screen.set_visual_mesh(visual_mesh);

   // Set the player spawn position
   //screen.set_player_spawn_position(player_spawn_position);


   framework_register_and_activate_screen("screen", &screen);

   framework_run_loop(-12);
}


