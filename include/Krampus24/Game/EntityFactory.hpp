#pragma once


#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Physics/CollisionMesh.hpp>
#include <Krampus24/BlenderBlockingLoaderEntity.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <vector>


namespace Krampus24
{
   namespace Game
   {
      class EntityFactory
      {
      private:
         AllegroFlare::EventEmitter* event_emitter;
         AllegroFlare::ModelBin* model_bin;
         AllegroFlare::BitmapBin* bitmap_bin;
         AllegroFlare::Physics::CollisionMesh* collision_mesh;
         bool initialized;

      protected:


      public:
         EntityFactory();
         ~EntityFactory();

         void set_event_emitter(AllegroFlare::EventEmitter* event_emitter);
         void set_model_bin(AllegroFlare::ModelBin* model_bin);
         void set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin);
         void set_collision_mesh(AllegroFlare::Physics::CollisionMesh* collision_mesh);
         bool get_initialized() const;
         void initialize();
         std::vector<Krampus24::Gameplay::Entities::Base*> create_entity(Krampus24::BlenderBlockingLoaderEntity* entity=nullptr);
      };
   }
}



