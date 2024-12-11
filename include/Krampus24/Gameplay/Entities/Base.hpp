#pragma once


#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/Physics/AABB2D.hpp>
#include <AllegroFlare/Physics/AABB3D.hpp>
#include <AllegroFlare/Physics/TileMapCollisionStepperStepResult.hpp>
#include <AllegroFlare/Placement3D.hpp>
#include <AllegroFlare/PlayerInputControllers/Base.hpp>
#include <AllegroFlare/SceneGraph/Entities/Base.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <vector>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace Entities
      {
         class Base : public AllegroFlare::SceneGraph::Entities::Base
         {
         public:
            static constexpr char* TYPE = (char*)"Krampus24/Gameplay/Entities/Base";

         private:
            std::string type;

         protected:


         public:
            AllegroFlare::Model3D* model;
            ALLEGRO_BITMAP* texture;
            AllegroFlare::Placement3D placement;
            AllegroFlare::Placement3D velocity;
            AllegroFlare::Physics::AABB3D aabb3d;
            AllegroFlare::Vec3D aabb3d_alignment;
            AllegroFlare::Physics::AABB2D hit_box_2d;
            AllegroFlare::PlayerInputControllers::Base* player_input_controller;
            std::vector<AllegroFlare::Vec3D> box_corners;
            ALLEGRO_COLOR box_color;
            std::string name;
            bool active;
            bool visible;
            bool affected_by_environmental_forces;
            bool collides_with_environment;
            bool collides_with_player;
            bool collides_with_enemies;
            bool player_can_inspect_or_use;
            Krampus24::Gameplay::Entities::Base* elevator__target;
            float player__spin;
            float player__tilt;
            bool zone__is_zone;
            Base(std::string type=Krampus24::Gameplay::Entities::Base::TYPE, AllegroFlare::Model3D* model=nullptr, ALLEGRO_BITMAP* texture=nullptr, AllegroFlare::Placement3D placement={}, AllegroFlare::Placement3D velocity={}, AllegroFlare::Physics::AABB3D aabb3d={}, AllegroFlare::Vec3D aabb3d_alignment={}, AllegroFlare::Physics::AABB2D hit_box_2d={}, AllegroFlare::PlayerInputControllers::Base* player_input_controller=nullptr);
            virtual ~Base();

            void set_type(std::string type);
            std::string get_type() const;
            bool has_player_input_controller();
            virtual void destroy();
            virtual void draw();
            virtual void on_time_step(double time_step=0.0f, double time_now=0.0f);
            virtual void on_tile_map_collision_stepper_step(AllegroFlare::Physics::TileMapCollisionStepperStepResult* collision_step_result=nullptr);
            virtual void on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity=nullptr);
            virtual void on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity=nullptr);
            virtual void on_enter_player_position_collision(Krampus24::Gameplay::Entities::Base* player_entity=nullptr);
            virtual void on_exit_player_position_collision(Krampus24::Gameplay::Entities::Base* player_entity=nullptr);
            virtual bool on_player_inspect_or_use();
            bool collides(Krampus24::Gameplay::Entities::Base* other=nullptr);
            bool collides_aabb3d(Krampus24::Gameplay::Entities::Base* other=nullptr);
            AllegroFlare::Vec3D calculate_aabb3d_offset_from_alignment();
            void draw_aabb3d();
            void draw_origin();
            void calculate_box_corners();
            ALLEGRO_COLOR build_color(float opacity=1.0f);
            std::vector<ALLEGRO_VERTEX> build_line_list_vertices();
            std::vector<ALLEGRO_VERTEX> build_triangle_list_vertices_for_faces();
         };
      }
   }
}



