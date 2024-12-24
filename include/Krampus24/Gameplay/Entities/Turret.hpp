#pragma once


#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/Model3D.hpp>
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
            int power_bar_level;
            AllegroFlare::Model3D* body;
            AllegroFlare::Model3D* power_bar_1;
            AllegroFlare::Model3D* power_bar_2;
            AllegroFlare::Model3D* power_bar_3;
            AllegroFlare::Model3D* power_bar_4;

         protected:


         public:
            Turret();
            virtual ~Turret();

            bool get_initialized() const;
            void initialize();
            static Krampus24::Gameplay::Entities::Turret* construct(AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::Vec3D position={}, float rotation=0.0f);
            virtual void draw() override;
            virtual bool on_player_inspect_or_use() override;
         };
      }
   }
}



