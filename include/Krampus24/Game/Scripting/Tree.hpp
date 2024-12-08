#pragma once


#include <AllegroFlare/CollisionObservers/Simple.hpp>
#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <AllegroFlare/FontBin.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Entities/Door.hpp>
#include <Krampus24/Gameplay/ScriptingInterface.hpp>
#include <allegro5/allegro_font.h>
#include <string>
#include <utility>
#include <vector>


namespace Krampus24
{
   namespace Game
   {
      namespace Scripting
      {
         class Tree : public Krampus24::Gameplay::ScriptingInterface
         {
         private:
            AllegroFlare::FontBin* font_bin;
            std::vector<Krampus24::Gameplay::Entities::Base*>* entities;
            bool primary_power_coil_collected;
            bool primary_power_coil_returned_to_ship;
            AllegroFlare::CollisionObservers::Simple* collision_observer;
            bool initialized;
            void build_on_collision_callbacks();

         protected:


         public:
            Tree();
            virtual ~Tree();

            void set_font_bin(AllegroFlare::FontBin* font_bin);
            void set_entities(std::vector<Krampus24::Gameplay::Entities::Base*>* entities);
            void set_primary_power_coil_collected(bool primary_power_coil_collected);
            void set_primary_power_coil_returned_to_ship(bool primary_power_coil_returned_to_ship);
            void set_collision_observer(AllegroFlare::CollisionObservers::Simple* collision_observer);
            bool get_primary_power_coil_collected() const;
            bool get_primary_power_coil_returned_to_ship() const;
            bool get_initialized() const;
            virtual void render_hud() override;
            virtual bool end_state_achieved() override;
            bool a_0th_entity_exists();
            Krampus24::Gameplay::Entities::Base* find_0th_entity();
            void initialize();
            std::pair<int, std::string> get_entities_names_in_list();
            bool entity_with_name_exists(std::string name="[unset-name]");
            Krampus24::Gameplay::Entities::Base* find_entity_by_name_or_throw(std::string name="[unset-name]");
            void link_elevators(std::string elevator_a_name="[unset-elevator_a_name]", std::string elevator_b_name="[unset-elevator_b_name]");
            void customize_door_style(std::string door_object_name="[unset-door_object_name]", Krampus24::Gameplay::Entities::Door::Style door_style=Krampus24::Gameplay::Entities::Door::Style::STYLE_BARN);
            void travel_player_to_elevators_target(std::string entering_elevator_name="[unset-entering_elevator_name]");
            virtual AllegroFlare::DialogTree::NodeBank build_dialog_node_bank() override;
            ALLEGRO_FONT* obtain_hud_font();
         };
      }
   }
}



