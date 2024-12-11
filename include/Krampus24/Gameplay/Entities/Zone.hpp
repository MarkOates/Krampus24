#pragma once


#include <AllegroFlare/Vec3D.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace Entities
      {
         class Zone : public Krampus24::Gameplay::Entities::Base
         {
         public:
            static constexpr char* BLENDER_IDENTIFIER = (char*)"zone";

         private:
            bool initialized;

         protected:


         public:
            Zone();
            virtual ~Zone();

            static Krampus24::Gameplay::Entities::Base* construct(AllegroFlare::Vec3D position=AllegroFlare::Vec3D(0, 0, 0), AllegroFlare::Vec3D size=AllegroFlare::Vec3D(0, 0, 0));
            virtual void draw() override;
            virtual void on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity=nullptr) override;
            virtual void on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity=nullptr) override;
            virtual void on_time_step(double time_step=0.0f, double time_now=0.0f) override;
         };
      }
   }
}



