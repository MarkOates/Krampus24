#pragma once


#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Entities/Hen.hpp>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace Entities
      {
         class Hen : public Krampus24::Gameplay::Entities::Base
         {
         public:
            static constexpr char* BLENDER_IDENTIFIER = (char*)"hen";

         private:
            AllegroFlare::Vec3D movement_direction;
            float movement_velocity;
            bool initialized;

         protected:


         public:
            Hen();
            virtual ~Hen();

            static Krampus24::Gameplay::Entities::Hen* construct(AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr);
            virtual void on_time_step(double time_step=0.0f, double time_now=0.0f) override;
         };
      }
   }
}



