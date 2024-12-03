#pragma once


#include <functional>
#include <map>


namespace Krampus24
{
   namespace Gameplay
   {
      class ScriptingInterface
      {
      private:

      protected:


      public:
         std::map<void*, std::function<void()>> on_entity_collision_callbacks;
         ScriptingInterface();
         virtual ~ScriptingInterface();

         virtual void render_hud();
         virtual bool end_state_achieved();
         bool has_on_collision_callback(void* entity=nullptr);
         void call_on_collision_callback(void* entity=nullptr);
      };
   }
}



