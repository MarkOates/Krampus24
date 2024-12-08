

#include <Krampus24/Game/DialogBuilder.hpp>

#include <AllegroFlare/DialogTree/NodeBankFactory.hpp>
#include <AllegroFlare/DialogTree/NodeOptions/ExitDialog.hpp>
#include <AllegroFlare/DialogTree/Nodes/MultipageWithOptions.hpp>


namespace Krampus24
{
namespace Game
{


DialogBuilder::DialogBuilder()
{
}


DialogBuilder::~DialogBuilder()
{
}


AllegroFlare::DialogTree::NodeBank DialogBuilder::build_dialog_node_bank(std::string identifier)
{
   // TODO: Test this contains the expected nodes
   AllegroFlare::DialogTree::NodeBank result_node_bank = build_a_dialog_node_bank();

   //AllegroFlare::DialogTree::NodeBank space_suit_dialog_bank =
      //Krampus24::Game::Entities::SpaceSuit::build_dialog_bank();
   //result_node_bank.merge(&space_suit_dialog_bank);

   //auto catwalk_dialog_bank = Krampus24::Game::Levels::Catwalk::build_dialog_node_bank();
   //result_node_bank.merge(&catwalk_dialog_bank);

   //auto vending_machine_dialog_bank = Krampus24::Game::Entities::VendingMachine::build_dialog_node_bank();
   //result_node_bank.merge(&vending_machine_dialog_bank);

   //{
      //auto dialog_node_bank = Krampus24::Game::Entities::DestroyedSpaceSuit::build_dialog_node_bank();
      //result_node_bank.merge(&dialog_node_bank);
   //}

   //{
      //auto dialog_node_bank = Krampus24::Game::Entities::EngineCore::build_dialog_node_bank();
      //result_node_bank.merge(&dialog_node_bank);
   //}

   return result_node_bank;
}

AllegroFlare::DialogTree::NodeBank DialogBuilder::build_a_dialog_node_bank()
{
   AllegroFlare::DialogTree::NodeBank result;
   result.set_nodes({

      { "hydroflora", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "Hydroflora",
            { "Alien mushroom with a green cap and vibrant purple stem." },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "default_inspect_fallback", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "",
            { "There doesn't seem to be anything of importance here." },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "station_alarm_is_activated", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "Control System",
            //{ "There doesn't seem to be anything of importance here." },
            {
               "IMPORTANT: An alarm was triggered from the outside deck.",
               "This is an emergency sitation.",
               "A system outside the ship has been damaged. You must be repair it.",
               "The space suit cabinet has been unlocked. Collect the space suit "
                  "and exit the airlock onto the deck to perform repairs.",
            },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },


      { "inspect_cryo_pod", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "Cryo Pod",
            { "Used for suspending life forms for extended periods of time. Can also be used for general "
               "surgeries, medical proceedures, or adminstration of chemicals." },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "inspect_engine_core", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "Engine Core",
            { "The primary power source for the ship. The Entinite core provides power for all "
                 "systems on the ship, including thrust, stabilization, and life support." },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "inspect_airlock_door_when_locked", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "Airlock Door",
            { "This is the only exit to the space station's exterior.",
              "You cannot go outside without wearing a space suit." },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
   });
   return result;
}


} // namespace Game
} // namespace Krampus24


