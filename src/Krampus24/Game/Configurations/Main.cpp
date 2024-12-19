

#include <Krampus24/Game/Configurations/Main.hpp>

#include <AllegroFlare/Achievement.hpp>
#include <AllegroFlare/DialogTree/NodeBankFactory.hpp>
#include <AllegroFlare/Elements/StoryboardPages/AdvancingText.hpp>
#include <AllegroFlare/EventNames.hpp>
#include <AllegroFlare/GameEventDatas/AchievementUnlocked.hpp>
#include <AllegroFlare/GameProgressAndStateInfos/Base.hpp>
#include <AllegroFlare/Runners/Complete.hpp>
#include <AllegroFlare/StoryboardPageFactory.hpp>
#include <AllegroFlare/StringTransformer.hpp>
#include <Krampus24/Game/Scripting/Tree.hpp>
#include <Krampus24/Gameplay/Entities/Door.hpp>
#include <Krampus24/Gameplay/Level.hpp>
#include <Krampus24/Gameplay/Screen.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Game
{
namespace Configurations
{


Main::Main()
   : AllegroFlare::GameConfigurations::Complete(Krampus24::Game::Configurations::Main::TYPE)
   , primary_gameplay_screen(nullptr)
   , primary_gameplay_subscreen(nullptr)
   , pause_screen(nullptr)
{
}


Main::~Main()
{
}


std::string Main::app_icon_1024_filename()
{
   return "icons/allegro-flare-generic-icon-1024.png";
}

std::string Main::app_title()
{
   return "Untitled Game";
}

std::string Main::title_screen_title()
{
   return "Untitled Game";
}

std::string Main::primary_display_icon_filename()
{
   return "icons/allegro-flare-generic-icon-1024.png";
}

std::vector<std::tuple<std::string, AllegroFlare::Achievement*, bool, bool>> Main::build_achievements()
{
   return {};
}

AllegroFlare::Screens::Gameplay* Main::create_primary_gameplay_screen(AllegroFlare::Runners::Complete* runner)
{
   if (!(runner))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::create_primary_gameplay_screen]: error: guard \"runner\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::create_primary_gameplay_screen]: error: guard \"runner\" not met");
   }
   // TODO: Find some way to remove the Runners::Complete dependency, consider injecting
   // the bin dependencies individually into this method

   Krampus24::Gameplay::Screen *result = new Krampus24::Gameplay::Screen;

   result->set_data_folder_path(runner->get_framework()->get_data_folder_path());
   //result->set_asset_studio_database(&runner->get_framework()->get_asset_studio_database_ref());
   result->set_audio_controller(&runner->get_framework()->get_audio_controller_ref());
   result->set_font_bin(runner->get_font_bin());
   result->set_bitmap_bin(runner->get_bitmap_bin());
   result->set_model_bin(runner->get_model_bin());
   result->set_event_emitter(runner->get_event_emitter());
   result->set_dialog_system(&runner->get_framework()->get_dialog_system_ref());
   //result->set_game_configuration(this);
   result->set_build_scripting_instance_func(
      [](Krampus24::Gameplay::Screen* screen) -> Krampus24::Gameplay::ScriptingInterface* {

      //std::cout << "---- ****** inside building scripting ----" << std::endl;
      // Build a scripting
      Krampus24::Game::Scripting::Tree *scripting = new Krampus24::Game::Scripting::Tree;
      scripting->set_data_folder_path(screen->get_data_folder_path());
      scripting->set_entities(&screen->get_entities_ref());
      scripting->set_collision_observer(&screen->get_collision_observer_ref());
      scripting->set_font_bin(screen->get_font_bin());
      scripting->set_dialog_system(screen->get_dialog_system());
      scripting->set_event_emitter(screen->get_event_emitter());
      scripting->initialize();

      return scripting;
   });
   result->set_on_finished_callback_func(//[](AllegroFlare::Screens::Gameplay*, void*){
      [runner](AllegroFlare::Screens::Gameplay* screen, void* user_data){
         runner->get_event_emitter()->emit_router_event(
            AllegroFlare::Routers::Standard::EVENT_PRIMARY_GAMEPLAY_SCREEN_FINISHED,
            nullptr,
            al_get_time()
         );
      }
      // HERE: Trigger a game won (DEVELOPMENT)
      //std::cout << "-- inside provided on_finished_callback_func()" << std::endl;
   //}
   );

   result->initialize();


   // TODO: Determine if this is this the right spot for this
   runner->get_event_emitter()->emit_play_music_track_event("opening");


   primary_gameplay_screen = result;

   return result;
}

AllegroFlare::Screens::Subscreen::Screen* Main::create_primary_gameplay_subscreen(AllegroFlare::Runners::Complete* runner)
{
   if (!((!primary_gameplay_subscreen)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::create_primary_gameplay_subscreen]: error: guard \"(!primary_gameplay_subscreen)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::create_primary_gameplay_subscreen]: error: guard \"(!primary_gameplay_subscreen)\" not met");
   }
   AllegroFlare::EventEmitter* event_emitter = runner->get_event_emitter();
   AllegroFlare::BitmapBin* bitmap_bin = runner->get_bitmap_bin();
   AllegroFlare::FontBin* font_bin = runner->get_font_bin();

   // Build the subscreen_element
   AllegroFlare::Screens::Subscreen::Element *subscreen_element = new AllegroFlare::Screens::Subscreen::Element;
   subscreen_element->set_bitmap_bin(bitmap_bin);
   subscreen_element->set_font_bin(font_bin);
   subscreen_element->initialize();

   // Build and assemble the subscreen
   primary_gameplay_subscreen = new AllegroFlare::Screens::Subscreen::Screen;
   primary_gameplay_subscreen->set_event_emitter(event_emitter);
   primary_gameplay_subscreen->set_bitmap_bin(bitmap_bin);
   primary_gameplay_subscreen->set_font_bin(font_bin);
   primary_gameplay_subscreen->set_subscreen_element(subscreen_element);

   // Set the exit screen behavior
   primary_gameplay_subscreen->set_on_exit_callback_func(
      [event_emitter](AllegroFlare::Screens::Subscreen::Screen* pause_screen, void* user_data){
         event_emitter->emit_router_event(
            AllegroFlare::Routers::Standard::EVENT_UNPAUSE_GAME,
            nullptr,
            al_get_time()
         );
      }
   );
   primary_gameplay_subscreen->set_on_exit_callback_func_user_data(nullptr);

   primary_gameplay_subscreen->initialize();

   return primary_gameplay_subscreen;
}

AllegroFlare::Screens::PauseScreen* Main::create_pause_screen(AllegroFlare::Runners::Complete* runner)
{
   if (!((!pause_screen)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::create_pause_screen]: error: guard \"(!pause_screen)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::create_pause_screen]: error: guard \"(!pause_screen)\" not met");
   }
   AllegroFlare::FontBin* font_bin = runner->get_font_bin();
   AllegroFlare::BitmapBin* bitmap_bin = runner->get_bitmap_bin();
   AllegroFlare::EventEmitter* event_emitter = runner->get_event_emitter();

   std::vector<std::pair<std::string, std::string>> menu_options = {
      { "Resume", "resume_gameplay" },
      { "Exit to Title Screen", "exit_to_title_screen" },
   };

   // NOTE: No pause screen is used in this game, however, a pause screen is needed because upstream in the system
   // a  cannot be registered as a screen. // TODO: Fix this
   pause_screen = new AllegroFlare::Screens::PauseScreen;
   pause_screen->set_font_bin(font_bin);
   pause_screen->set_bitmap_bin(bitmap_bin);
   pause_screen->set_event_emitter(event_emitter);
   pause_screen->set_menu_options(menu_options);

   //pause_screen->set_foreground(shared_foreground);
   //pause_screen->set_background(shared_background);
   // TODO: Configure menu items
   // TODO: Add actions to menu items

   pause_screen->set_on_menu_choice_callback_func(
      [event_emitter](AllegroFlare::Screens::PauseScreen* pause_screen, void* user_data){
         // TODO: Perform different action depending on selected menu item
         // TODO: Use event emitter from pause_screen

         std::string current_menu_option_value = pause_screen->infer_current_menu_option_value();
         if (current_menu_option_value == "resume_gameplay")
         {
            event_emitter->emit_router_event(
               AllegroFlare::Routers::Standard::EVENT_UNPAUSE_GAME,
               nullptr,
               al_get_time()
            );
         }
         else if (current_menu_option_value == "exit_to_title_screen")
         {
            // TODO: Add a "are you sure?" step here
            event_emitter->emit_router_event(
               AllegroFlare::Routers::Standard::EVENT_ACTIVATE_TITLE_SCREEN,
               nullptr,
               al_get_time()
            );
         }
         else
         {
            AllegroFlare::Logger::throw_error(
               "AllegroFlare::GameConfigurations::Base::create_pause_screen"
               "While in the \"on_menu_choice_callback_func\", there was no option to handle the menu value \""
                  + current_menu_option_value + "\"."
            );
         }
      }
   );
   pause_screen->set_on_menu_choice_callback_func_user_data(nullptr);

   // Set the exit screen behavior
   pause_screen->set_on_exit_callback_func(
      [event_emitter](AllegroFlare::Screens::PauseScreen* pause_screen, void* user_data){
         event_emitter->emit_router_event(
            AllegroFlare::Routers::Standard::EVENT_UNPAUSE_GAME,
            nullptr,
            al_get_time()
         );
      }
   );
   pause_screen->set_on_exit_callback_func_user_data(nullptr);

   return pause_screen;
}

void Main::handle_primary_gameplay_screen_paused()
{
   if (!(primary_gameplay_screen))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::handle_primary_gameplay_screen_paused]: error: guard \"primary_gameplay_screen\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::handle_primary_gameplay_screen_paused]: error: guard \"primary_gameplay_screen\" not met");
   }
   // TODO: Add tests for this behavior
   // TODO: Consider having and using event_emitter from self
   AllegroFlare::EventEmitter* event_emitter = primary_gameplay_screen->get_event_emitter();
   primary_gameplay_screen->suspend_gameplay();
   event_emitter->emit_router_event(
      AllegroFlare::Routers::Standard::EVENT_ACTIVATE_PAUSE_SCREEN,
      //AllegroFlare::Routers::Standard::EVENT_ACTIVATE_PRIMARY_GAMEPLAY_SUBSCREEN,
      nullptr,
      al_get_time()
   );
   return;
}

void Main::handle_primary_gameplay_screen_unpaused()
{
   if (!(primary_gameplay_screen))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::handle_primary_gameplay_screen_unpaused]: error: guard \"primary_gameplay_screen\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::handle_primary_gameplay_screen_unpaused]: error: guard \"primary_gameplay_screen\" not met");
   }
   // TODO: Add tests for this behavior
   // TODO: Consider having and using event_emitter from self
   AllegroFlare::EventEmitter* event_emitter = primary_gameplay_screen->get_event_emitter();
   primary_gameplay_screen->resume_suspended_gameplay();
   event_emitter->emit_router_event(
      AllegroFlare::Routers::Standard::EVENT_ACTIVATE_PRIMARY_GAMEPLAY_SCREEN,
      nullptr,
      al_get_time()
   );
   return;
}

void Main::handle_primary_gameplay_screen_finished()
{
   { // DEVELOPMENT
      primary_gameplay_screen->get_event_emitter()->emit_router_event(
         AllegroFlare::Routers::Standard::EVENT_WIN_GAME,
         nullptr,
         al_get_time()
      );
   }
   /*
   AllegroFlare::Elements::LevelSelect &level_select_element = 
      get_runner()->get_level_select_screen_ref().get_level_select_element_ref();
   bool level_was_won = primary_gameplay_screen->infer_level_was_won();

   if (level_was_won)
   {
      std::string current_level_identifier = primary_gameplay_screen->get_current_level_identifier();

      // In the game_progress_and_state_info, set the level as completed and save
      game_progress_and_state_info.add_level_cleared(current_level_identifier);
      game_progress_and_state_info.save();

      // On the level select screen, mark the level as completed
      //std::string current_level_identifier = primary_gameplay_screen->get_current_level_identifier();
      level_select_element.add_to_completed_list(current_level_identifier);

      // If all the levels are completed, and you just played the last level, the game is beaten
      bool all_levels_are_completed =
         level_select_element.get_completed_list().size() == level_select_element.get_levels_list().size();
      bool just_played_last_level =
         current_level_identifier == build_level_list_for_level_select_screen_by_identifier().back().second;
      if (all_levels_are_completed && just_played_last_level)
      {
         primary_gameplay_screen->get_event_emitter()->emit_router_event(
            AllegroFlare::Routers::Standard::EVENT_WIN_GAME,
            nullptr,
            al_get_time()
         );
      }
      else
      {
         primary_gameplay_screen->get_event_emitter()->emit_router_event(
            AllegroFlare::Routers::Standard::EVENT_ACTIVATE_LEVEL_SELECT_SCREEN,
            nullptr,
            al_get_time()
         );
      }
   }
   else
   {
      primary_gameplay_screen->get_event_emitter()->emit_router_event(
         AllegroFlare::Routers::Standard::EVENT_ACTIVATE_LEVEL_SELECT_SCREEN,
         nullptr,
         al_get_time()
      );
   }
   */
   return;
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base *> Main::create_intro_logos_storyboard_pages()
{
   if (!(primary_gameplay_screen))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::create_intro_logos_storyboard_pages]: error: guard \"primary_gameplay_screen\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::create_intro_logos_storyboard_pages]: error: guard \"primary_gameplay_screen\" not met");
   }
   AllegroFlare::StoryboardPageFactory page_factory;
   page_factory.set_font_bin(primary_gameplay_screen->get_font_bin());
   page_factory.set_bitmap_bin(primary_gameplay_screen->get_bitmap_bin());
   page_factory.set_model_bin(primary_gameplay_screen->get_model_bin());

   std::vector<AllegroFlare::Elements::StoryboardPages::Base *> result = {
      page_factory.create_clubcatt_logo_page(),
      page_factory.create_image_page(
            primary_gameplay_screen->get_bitmap_bin()->operator[]("clubcatt-website-01.jpg")
         ),
   };

   return result;
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base *> Main::create_intro_storyboard_pages()
{
   return {};
}

std::string Main::u(std::string string)
{
   return AllegroFlare::StringTransformer(string).upcase().get_text();
}

AllegroFlare::Elements::StoryboardPages::Base* Main::create_storyboard_page__text(AllegroFlare::FontBin* font_bin, std::string page_text)
{
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::create_storyboard_page__text]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::create_storyboard_page__text]: error: guard \"font_bin\" not met");
   }
   if (!((!page_text.empty())))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::create_storyboard_page__text]: error: guard \"(!page_text.empty())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::create_storyboard_page__text]: error: guard \"(!page_text.empty())\" not met");
   }
   auto page = new AllegroFlare::Elements::StoryboardPages::AdvancingText(font_bin, page_text);
   return page;
}

void Main::handle_arbitrary_storyboard_screen_finished()
{
   // TODO: Consider destroying pages in storyboard. Otherwise leakage, k?

   // Reset the name to blank
   //primary_gameplay_screen->set_arbitrary_storyboard_screen_identifier_to_start("currently-unset");

   // Emit route event to return to game
   primary_gameplay_screen->get_event_emitter()->emit_router_event(
      AllegroFlare::Routers::Standard::EVENT_ACTIVATE_PRIMARY_GAMEPLAY_SCREEN,
      nullptr,
      al_get_time()
   );
   return;
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base *> Main::create_arbitrary_storyboard_pages_by_identifier(std::string identifier)
{
   if (!(primary_gameplay_screen))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::create_arbitrary_storyboard_pages_by_identifier]: error: guard \"primary_gameplay_screen\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::create_arbitrary_storyboard_pages_by_identifier]: error: guard \"primary_gameplay_screen\" not met");
   }
   return primary_gameplay_screen->create_arbitrary_storyboard_pages_by_identifier(); // NOTE: Naming is weird
                                                                                      // on this func

   //AllegroFlare::FontBin* font_bin = primary_gameplay_screen->get_font_bin();

   //identifier = primary_gameplay_screen->get_arbitrary_storyboard_screen_identifier_to_start();

   //std::vector<AllegroFlare::Elements::StoryboardPages::Base *> result = {};

   //if (identifier == "pig_storyboard")
   //{
      //result =
      //{
         //create_storyboard_page__text(font_bin,
           //"This is text from an arbitrary \"pig_storyboard\" storyboard screen."
         //),
      //};
   //}
   //else
   //{
      //AllegroFlare::Logger::throw_error(
         //"AllegroFlare::GameConfigurations::Base::create_arbitrary_storyboard_pages_by_identifier"
         //"Foobar boobaz"
      //);
   //}

   //return result;
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base *> Main::create_new_game_intro_storyboard_pages()
{
   if (!(primary_gameplay_screen))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::create_new_game_intro_storyboard_pages]: error: guard \"primary_gameplay_screen\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::create_new_game_intro_storyboard_pages]: error: guard \"primary_gameplay_screen\" not met");
   }
   AllegroFlare::FontBin* font_bin = primary_gameplay_screen->get_font_bin();

   //return {};

   std::vector<AllegroFlare::Elements::StoryboardPages::Base *> result =
   {
      create_storyboard_page__text(font_bin,
        "Mission Log [Recording live]:\n\nI'm in route to the derelect space station TR-33 and should be docking "
           "there in under a minute.\n\n"
      ),
      create_storyboard_page__text(font_bin,
        "Nobody seems to know what happened at the station, but "
        "sublight communications stopped several years ago.\n\nHonestly, there's probably nothing "
        "but decayed bodies at this point.\n\n"
      ),
      create_storyboard_page__text(font_bin,
        "My mission is to make my way to the top floor of the TR-33 station, and retrieve what's left of the "
        "power coil.  It'll be worth a fortune on the dark market, even if there's no charge left."
      ),
      create_storyboard_page__text(font_bin,
        "I'm docking my ship at the DOCKING PORT at the bottom of the station, which is at the sub-level below "
           "level 1.\n\n"
        "Based on the fragmented maps I obtained, I'll need to take a series of 5 elevator shafts to "
           "the top floor, where the power coil is located."
      ),
      create_storyboard_page__text(font_bin,
        "Hm, what else was there...\n\n"
      ),
      create_storyboard_page__text(font_bin,
        "Oh, the date for this log.  I always forget.\n\n"
      ),
      create_storyboard_page__text(font_bin,
        "Today's date is...\n\n"
        "December 25 2124?\n\n"
        "Heh, Christmas day, how quaint.\n\n"
      ),
      create_storyboard_page__text(font_bin,
        "Let's hope Santa brings me a nice shiny power coil."
      ),
   };
   return result;
}

std::vector<std::pair<std::string, std::string>> Main::build_title_screen_menu_options()
{
   //AllegroFlare::Logger::throw_error(
      //"AllegroFlare::GameConfigurations::Base::build_title_screen_menu_options",
         //"Not implemented in the base class. This method must be implemented in the derived class"
   //);
   std::vector<std::pair<std::string, std::string>> options = {
      //{ "Continue",          "continue_from_last_save" },       // TODO: If game session is saved and valid
      //{ "Load a Saved Game", "goto_load_a_saved_game_screen" }, // TODO: If game session is saved and valid,
                                                                // and the game supports save slots
      { "Start New Game",    "start_new_game" },                // TODO: If the game session has not begun
      //{ "Achievements",      "goto_achievements_screen" },
      //{ "Settings",          "goto_settings_screen" },
      { "Settings",          "goto_display_settings_screen" },
      //{ "Version",           "goto_version_screen" },
      //{ "Credits",           "goto_credits_screen" },           // TODO: If game has been won
      { "Quit",              "quit" },
   };
   return options;
}

void Main::load_audio_controller(AllegroFlare::AudioController* audio_controller)
{
   if (!(audio_controller))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::load_audio_controller]: error: guard \"audio_controller\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::load_audio_controller]: error: guard \"audio_controller\" not met");
   }
   //AllegroFlare::AudioController *audio_controller = get_framework_audio_controller();
   audio_controller->set_and_load_sound_effect_elements(
      Krampus24::Game::Scripting::Tree().build_audio_controller_sound_effect_list()
   );
   audio_controller->set_and_load_music_track_elements(
      Krampus24::Game::Scripting::Tree().build_audio_controller_music_track_list()
   );
   // An example of how to load a sound effect track:
   //audio_controller->set_and_load_sound_effect_elements({
      //{ "menu_move", { "ci-menu_move-01.ogg", false, "restart" } },
      //{ "menu_select", { "ci-menu_choice-01.ogg", false, "restart" } },
   //});

   // How to play a sound effect:
   // event_emitter->emit_play_music_track_event("menu_select");

   //// An example of how to load a music track:
   //audio_controller.set_and_load_music_track_elements({
      ////{ "intro_music", { "wanderer-01.ogg", true, "ignore" } },
   //});

   // How to play a music track:
   // event_emitter->emit_play_music_track_event("intro_music");
   return;
}

void Main::load_last_played_session_or_start_new(AllegroFlare::GameSession* game_session)
{
   if (!(game_session))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::load_last_played_session_or_start_new]: error: guard \"game_session\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::load_last_played_session_or_start_new]: error: guard \"game_session\" not met");
   }
   // TODO: This method
   //AllegroFlare::GameProgressAndStateInfos::Base *game_progress_and_state_info =
     //game_session->get_game_progress_and_state_info();
   return;
}

void Main::setup_new_game_progress_and_state_info(AllegroFlare::GameSession* game_session)
{
   if (!(game_session))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Configurations::Main::setup_new_game_progress_and_state_info]: error: guard \"game_session\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Configurations::Main::setup_new_game_progress_and_state_info]: error: guard \"game_session\" not met");
   }
   // TODO: This method
   //AllegroFlare::Logger::throw_error(
      //"AllegroFlare::GameConfigurations::Base::setup_new_game_progress_and_state_info",
      //"Not implemented in the base class. This method must be implemented in the derived class"
   //);
   //AllegroFlare::GameProgressAndStateInfos::Base *game_progress_and_state_info =
     //game_session->get_game_progress_and_state_info();
   return;
}

std::vector<AllegroFlare::LoadASavedGame::SaveSlots::Base*> Main::build_save_slots_for_load_a_saved_game_screen()
{
   /*
   std::vector<AllegroFlare::LoadASavedGame::SaveSlots::Base*> result = {
      new AllegroFlare::LoadASavedGame::SaveSlots::Empty(), // TODO: Fill this list with items from save file
      new AllegroFlare::LoadASavedGame::SaveSlots::Empty(),
      new AllegroFlare::LoadASavedGame::SaveSlots::Empty(),
   };
   return result;
   */
   return {};
}

void Main::handle_game_event(AllegroFlare::GameEvent* game_event)
{
   //// TODO: Handle top-level game events here
   //if (game_event->is_type(AllegroFlare::GameEventDatas::ScreenActivated::NAME))
   //{
      //AllegroFlare::GameEventDatas::ScreenActivated* as =
        //static_cast<AllegroFlare::GameEventDatas::ScreenActivated*>(game_event->get_data());

      //// TODO: Handle game-specific logic for a after a screen switch
   //}
   if (game_event->get_type() == AllegroFlare::GameEventDatas::AchievementUnlocked::NAME)
   {
      if (game_event->get_data()->is_type(AllegroFlare::GameEventDatas::AchievementUnlocked::TYPE))
      {
         AllegroFlare::GameEventDatas::AchievementUnlocked *as =
            static_cast<AllegroFlare::GameEventDatas::AchievementUnlocked *>(game_event->get_data());
         // TODO: Handle saving progress of achievements. Something like below.
         // See this guide:
         // https://docs.google.com/document/d/1QdDVa6giZOmGbF61dgom1ZJ_Ev5OFvZEM2Bc453RjGw/edit
         //mark_achievement_as_unlocked_and_save_progress(as->get_achievement_identifier());
      }
      else
      {
         throw std::runtime_error("Gameplay/Runner/game_event_func: unexpected AchievementUnlocked event data type");
      }
   }
   return;
}

void Main::continue_from_last_save()
{
   // TODO: Consider adding this method as part of the standard system
   return;
}

std::vector<std::pair<std::string, std::string>> Main::build_level_list_for_level_select_screen_by_identifier(std::string identifier)
{
   std::vector<std::pair<std::string, std::string>> result = {
      //{ "Forest Village 1", "forest_village_1" },
      //{ "Forest Village 2", "forest_village_2" },
      //{ "Forest", "forest_1" },
      //{ "Crystal Cave", "crystal_cave_1" },
      //{ "Desert Town", "desert_town_3" },
      //{ "Town 2", "town_2" },
      //{ "Cave 1", "cave_1" },
      //{ "Town 1", "town_1" },
   };
   return result;
}

AllegroFlare::DialogTree::NodeBank Main::build_dialog_bank_by_identifier(std::string identifier)
{
   // TODO: Test this contains the expected nodes
   AllegroFlare::DialogTree::NodeBank result_node_bank;

   // TODO: Consider joining the system nodes outside of the LevelFactory
   //AllegroFlare::DialogTree::NodeBank system_node_bank =
      //AllegroFlare::DialogTree::NodeBankFactory::build_common_system_dialogs_node_bank();
   //result_node_bank.merge(&system_node_bank);


   // HACK!! For production, we're building a dialog tree here, as well as in the game system.  It leaves
   // dangling pointers and all these things, it's unsure what the side effects could be from double-building
   // this dialog node bank
   Krampus24::Game::Scripting::Tree tree;
   result_node_bank = tree.build_dialog_node_bank();


   return result_node_bank;
}

AllegroFlare::Levels::Base* Main::load_level_by_identifier(std::string identifier)
{
   // TODO: Replace void* with a better type (once the design is clear)
   Krampus24::Gameplay::Level *result = new Krampus24::Gameplay::Level();

   std::map<std::string, std::function<void()>> items_map = {
      { "", [](){
         AllegroFlare::Logger::warn_from(
            "Krampus24::GameConfigurations::Main::load_level_by_identifier",
            "Loading a completely blank empty level."
         );
         // TODO: Add warning: loading an empty level
      }},
      { "forest_village_1", [](){
         //result->set_background_image_identifier("maps/forest-village-1.png");
      }},
      //{ "forest_village_2", [](){
         //result->set_background_image_identifier("maps/forest-village-2.png");
      //}},
      //{ "forest_1", [result](){
         //result->set_background_image_identifier("maps/forest-1.png");
      //}},
      //{ "crystal_cave_1", [result](){
         //result->set_background_image_identifier("maps/crystal-cave-1.png");
      //}},
      //{ "desert_town_3", [result](){
         //result->set_background_image_identifier("maps/desert-town-3.png");
      //}},
      //{ "town_2", [result](){
         //result->set_background_image_identifier("maps/town-2.png");
      //}},
      //{ "cave_1", [result](){
         //result->set_background_image_identifier("maps/rpg-fit-backgrounds-ex-101.png");
      //}},
      //{ "town_1", [result](){
         //result->set_background_image_identifier("maps/rpg-fit-backgrounds-x2-01.png");
      //}},
   };

   // locate and call the function to handle the item
   if (items_map.count(identifier) == 0)
   {
      bool item_handled = false;
      //if (unrecognized_key_callback_func)
      //{
         //item_handled = unrecognized_key_callback_func();
      //}

      if (!item_handled)
      {
         // item not found
         std::stringstream error_message;
         error_message << "[Krampus24::Game::Configurations::Main::load_level]: error: Cannot load the item with the identifier \""
                       << identifier << "\", it does not exist.";
         throw std::runtime_error(error_message.str());
      }
   }
   else
   {
      // call the item
      items_map[identifier]();
   }

   return result;
}


} // namespace Configurations
} // namespace Game
} // namespace Krampus24


