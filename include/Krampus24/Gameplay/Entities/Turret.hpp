#pragma once


#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Entities/Turret.hpp>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace Entities
      {
         class Turret : public Krampus24::Gameplay::Entities::Base
         {
         public:
            static constexpr char* BLENDER_IDENTIFIER = (char*)"turret";

         private:
            bool initialized;

         protected:


         public:
            Turret();
            ~Turret();

            bool get_initialized() const;
            void initialize();
            static Krampus24::Gameplay::Entities::Turret* construct(AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::Vec3D position={}, float rotation=0.0f);
         };
      }
   }
}



