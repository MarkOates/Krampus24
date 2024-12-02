#pragma once


#include <AllegroFlare/CollisionObservers/Simple.hpp>
#include <AllegroFlare/FontBin.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <allegro5/allegro_font.h>
#include <functional>
#include <map>
#include <string>
#include <vector>


namespace Krampus24
{
   namespace Game
   {
      namespace Scripting
      {
         class Tree
         {
         private:
            AllegroFlare::FontBin* font_bin;
            std::vector<Krampus24::Gameplay::Entities::Base*>* entities;
            bool primary_power_coil_collected;
            bool primary_power_coil_returned_to_ship;
            AllegroFlare::CollisionObservers::Simple* collision_observer;
            std::map<void*, std::function<void()>> on_entity_collision_callbacks;
            bool initialized;

         protected:


         public:
            Tree();
            ~Tree();

            void set_font_bin(AllegroFlare::FontBin* font_bin);
            void set_entities(std::vector<Krampus24::Gameplay::Entities::Base*>* entities);
            void set_primary_power_coil_collected(bool primary_power_coil_collected);
            void set_primary_power_coil_returned_to_ship(bool primary_power_coil_returned_to_ship);
            void set_collision_observer(AllegroFlare::CollisionObservers::Simple* collision_observer);
            void set_on_entity_collision_callbacks(std::map<void*, std::function<void()>> on_entity_collision_callbacks);
            bool get_primary_power_coil_collected() const;
            bool get_primary_power_coil_returned_to_ship() const;
            std::map<void*, std::function<void()>> get_on_entity_collision_callbacks() const;
            bool get_initialized() const;
            void render_hud();
            bool a_0th_entity_exists();
            Krampus24::Gameplay::Entities::Base* find_0th_entity();
            void initialize();
            bool has_on_collision_callback(void* entity=nullptr);
            void call_on_collision_callback(void* entity=nullptr);
            bool entity_with_name_exists(std::string name="[unset-name]");
            Krampus24::Gameplay::Entities::Base* find_entity_by_name_or_throw(std::string name="[unset-name]");
            void link_elevators(std::string elevator_a_name="[unset-elevator_a_name]", std::string elevator_b_name="[unset-elevator_b_name]");
            void travel_player_to_elevators_target(std::string entering_elevator_name="[unset-entering_elevator_name]");
            void build_on_collision_callbacks();
            ALLEGRO_FONT* obtain_hud_font();
         };
      }
   }
}



