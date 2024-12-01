
#include <gtest/gtest.h>

#include <Krampus24/Game/Configurations/Main.hpp>


TEST(Krampus24_Game_Configurations_MainTest, can_be_created_without_blowing_up)
{
   Krampus24::Game::Configurations::Main main_configuration;
}


TEST(Krampus24_Game_Configurations_MainTest, load_level_by_identifier__will_not_blow_up)
{
   Krampus24::Game::Configurations::Main main_configuration;
   AllegroFlare::Levels::Base* level = main_configuration.load_level_by_identifier("forest_village_1");
   EXPECT_NE(nullptr, level);
}

/*
  - name: handle_primary_gameplay_screen_finished
    virtual: true
    override: true
    body: |
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
      return;



         [runner](AllegroFlare::Screens::Gameplay* screen, void* user_data){
            runner->get_event_emitter()->emit_router_event(
               AllegroFlare::Routers::Standard::EVENT_PRIMARY_GAMEPLAY_SCREEN_FINISHED,
               nullptr,
               al_get_time()
            );
         }
*/

