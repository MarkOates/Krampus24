#pragma once


#include <AllegroFlare/CollisionObservers/Simple.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
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
            std::vector<Krampus24::Gameplay::Entities::Base*>* entities;
            AllegroFlare::CollisionObservers::Simple* collision_observer;
            std::map<void*, std::function<void()>> on_entity_collision_callbacks;
            bool initialized;

         protected:


         public:
            Tree();
            ~Tree();

            void set_entities(std::vector<Krampus24::Gameplay::Entities::Base*>* entities);
            void set_collision_observer(AllegroFlare::CollisionObservers::Simple* collision_observer);
            void set_on_entity_collision_callbacks(std::map<void*, std::function<void()>> on_entity_collision_callbacks);
            std::map<void*, std::function<void()>> get_on_entity_collision_callbacks() const;
            bool get_initialized() const;
            bool a_0th_entity_exists();
            Krampus24::Gameplay::Entities::Base* find_0th_entity();
            void initialize();
            bool has_on_collision_callback(void* entity=nullptr);
            void call_on_collision_callback(void* entity=nullptr);
            bool entity_with_name_exists(std::string name="[unset-name]");
            Krampus24::Gameplay::Entities::Base* find_entity_by_name_or_throw(std::string name="[unset-name]");
            void build_on_collision_callbacks();
         };
      }
   }
}



