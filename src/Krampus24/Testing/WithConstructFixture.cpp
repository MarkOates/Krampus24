

#include <Krampus24/Testing/WithConstructFixture.hpp>

#include <AllegroFlare/SystemInfo.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Testing
{


WithConstructFixture::WithConstructFixture()
   : AllegroFlare::Testing::WithAllegroFlareFrameworksFullFixture()
   , screen(nullptr)
{
}


WithConstructFixture::~WithConstructFixture()
{
}


void WithConstructFixture::SetUp()
{
   AllegroFlare::Testing::WithAllegroFlareFrameworksFullFixture::SetUp();


   std::string collision_mesh_identifier = "testing_construct-01-collision_mesh.obj";
   std::string visual_mesh_identifier = "testing_construct-01-visual.obj";
   std::string visual_mesh_texture_identifier = "RETRO_TEXTURE_PACK_V15-atlas-01.png";
   std::string blocking_filename = "testing_construct-01.blocking";


   // Setup the screen
   //TestGameConfiguration game_configuration;
   screen = new Krampus24::Gameplay::Screen;
   screen->set_data_folder_path(get_framework_data_folder_path());
   screen->set_audio_controller(get_framework_audio_controller());
   screen->set_event_emitter(get_framework_event_emitter());
   screen->set_bitmap_bin(get_framework_bitmap_bin());
   screen->set_font_bin(get_framework_font_bin());
   screen->set_model_bin(get_framework_model_bin());
   //screen.set_game_configuration(&game_configuration);
   screen->set_collision_mesh_identifier(collision_mesh_identifier);
   screen->set_visual_mesh_identifier(visual_mesh_identifier);
   screen->set_visual_mesh_texture_identifier(visual_mesh_texture_identifier);
   screen->set_blocking_filename(blocking_filename);
   screen->set_on_finished_callback_func([](AllegroFlare::Screens::Gameplay*, void*){
      std::cout << "-- inside provided on_finished_callback_func()" << std::endl;
      throw std::runtime_error("Game is completed.");
   });
   screen->initialize();

   framework_register_screen("screen", screen);



   //framework_register_and_activate_screen("screen", &screen);

   //framework_run_loop(-12);



   //AllegroFlare::SystemInfo HACK_system_info; // TODO: Figure out how to remove this
   //std::string HACK_sysname = HACK_system_info.get_sysname();
   //AllegroFlare::Inventory player_inventory;

   //screen = new AllegroFlare::Prototypes::TileFPS::Screen;
   //screen->set_event_emitter(get_framework_event_emitter());
   //screen->set_bitmap_bin(get_framework_bitmap_bin());
   //screen->set_font_bin(get_framework_font_bin());
   //screen->set_model_bin(get_framework_model_bin());
   //screen->set_data_folder_path(get_framework_data_folder_path());
   //screen->set_HACK_sysname(HACK_sysname);
   //screen->set_player_inventory(&player_inventory);
   //screen->set_player_view_height(1.8); // Should be 1.6 - 2.0
   //screen->set_on_paused_callback_func([](AllegroFlare::Prototypes::TileFPS::Screen*, void*){}); // An empty function
   //screen->set_on_finished_callback_func([](AllegroFlare::Prototypes::TileFPS::Screen*, void*){});
   //screen->initialize();

   // Register the screen into the system
   //framework_register_screen("screen", screen);

   return;
}

void WithConstructFixture::TearDown()
{
   // TODO: Delete the level
   AllegroFlare::Testing::WithAllegroFlareFrameworksFullFixture::TearDown();
}

void WithConstructFixture::run_test_construct_loop(float num_seconds_until_abort)
{
   //if (level) level->on_start();
   framework_activate_screen("screen");
   framework_run_loop(num_seconds_until_abort);
   return;
}

void WithConstructFixture::add_entity_to_pool(Krampus24::Gameplay::Entities::Base* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Testing::WithConstructFixture::add_entity_to_pool]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Testing::WithConstructFixture::add_entity_to_pool]: error: guard \"entity\" not met");
   }
   screen->get_entities_ref().push_back(entity);
   return;
}


} // namespace Testing
} // namespace Krampus24


