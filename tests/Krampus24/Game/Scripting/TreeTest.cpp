
#include <gtest/gtest.h>

#include <Krampus24/Game/Scripting/Tree.hpp>
#include <AllegroFlare/Testing/ErrorAssertions.hpp>
#include <AllegroFlare/DeploymentEnvironment.hpp>


TEST(Krampus24_Game_Scripting_TreeTest, can_be_created_without_blowing_up)
{
   Krampus24::Game::Scripting::Tree tree;
}


TEST(Krampus24_Game_Scripting_TreeTest,
   build_storyboard_text_from_file__on_a_file_that_does_not_exist__will_throw_an_error)
{
   AllegroFlare::DeploymentEnvironment deployment_environment("test");
   Krampus24::Game::Scripting::Tree tree;
   tree.set_data_folder_path(deployment_environment.get_data_folder_path());

   tree.build_storyboard_text_from_file("tablet_in_zoo.txt");
}


TEST(Krampus24_Game_Scripting_TreeTest, build_storyboard_text_from_file__will_build_the_expected_pages)
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

