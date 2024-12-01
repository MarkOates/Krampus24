#pragma once


#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/Camera2D.hpp>
#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/CollisionObservers/Simple.hpp>
#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/FontBin.hpp>
#include <AllegroFlare/GameConfigurations/Base.hpp>
#include <AllegroFlare/GameEvent.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Physics/CollisionMesh.hpp>
#include <AllegroFlare/Player.hpp>
#include <AllegroFlare/Screens/Gameplay.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <AllegroFlare/VirtualControllers/Base.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Screen.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <functional>
#include <string>
#include <vector>


namespace Krampus24
{
   namespace Gameplay
   {
      class Screen : public AllegroFlare::Screens::Gameplay
      {
      private:
         std::string data_folder_path;
         AllegroFlare::EventEmitter* event_emitter;
         AllegroFlare::BitmapBin* bitmap_bin;
         AllegroFlare::FontBin* font_bin;
         AllegroFlare::ModelBin* model_bin;
         AllegroFlare::GameConfigurations::Base* game_configuration;
         AllegroFlare::Camera2D hud_camera;
         AllegroFlare::Camera3D live_camera;
         std::vector<Krampus24::Gameplay::Entities::Base*> entities;
         AllegroFlare::Physics::CollisionMesh* collision_mesh;
         AllegroFlare::Model3D* visual_mesh;
         AllegroFlare::Vec3D player_spawn_position;
         std::string collision_mesh_identifier;
         std::string visual_mesh_identifier;
         std::string visual_mesh_texture_identifier;
         std::string blocking_filename;
         int gems_collected;
         AllegroFlare::CollisionObservers::Simple collision_observer;
         std::function<void(Krampus24::Gameplay::Screen*, void*)> on_finished_callback_func;
         void* on_finished_callback_func_user_data;
         bool initialized;

      protected:


      public:
         Screen(AllegroFlare::EventEmitter* event_emitter=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::FontBin* font_bin=nullptr, AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::GameConfigurations::Base* game_configuration=nullptr, std::vector<Krampus24::Gameplay::Entities::Base*> entities={}, AllegroFlare::Physics::CollisionMesh* collision_mesh=nullptr, std::string collision_mesh_identifier="basic_baking-01-collision_mesh.obj", std::string visual_mesh_identifier="basic_baking-01-visual.obj", std::string visual_mesh_texture_identifier="basic_baking-01-baked_shadow-reduced.jpg", std::string blocking_filename="basic_baking-02.blocking");
         virtual ~Screen();

         void set_data_folder_path(std::string data_folder_path);
         void set_event_emitter(AllegroFlare::EventEmitter* event_emitter);
         void set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin);
         void set_font_bin(AllegroFlare::FontBin* font_bin);
         void set_model_bin(AllegroFlare::ModelBin* model_bin);
         void set_game_configuration(AllegroFlare::GameConfigurations::Base* game_configuration);
         void set_hud_camera(AllegroFlare::Camera2D hud_camera);
         void set_live_camera(AllegroFlare::Camera3D live_camera);
         void set_entities(std::vector<Krampus24::Gameplay::Entities::Base*> entities);
         void set_collision_mesh(AllegroFlare::Physics::CollisionMesh* collision_mesh);
         void set_visual_mesh(AllegroFlare::Model3D* visual_mesh);
         void set_player_spawn_position(AllegroFlare::Vec3D player_spawn_position);
         void set_collision_mesh_identifier(std::string collision_mesh_identifier);
         void set_visual_mesh_identifier(std::string visual_mesh_identifier);
         void set_visual_mesh_texture_identifier(std::string visual_mesh_texture_identifier);
         void set_blocking_filename(std::string blocking_filename);
         void set_gems_collected(int gems_collected);
         void set_on_finished_callback_func(std::function<void(Krampus24::Gameplay::Screen*, void*)> on_finished_callback_func);
         void set_on_finished_callback_func_user_data(void* on_finished_callback_func_user_data);
         std::string get_data_folder_path() const;
         AllegroFlare::EventEmitter* get_event_emitter() const;
         AllegroFlare::GameConfigurations::Base* get_game_configuration() const;
         AllegroFlare::Camera2D get_hud_camera() const;
         AllegroFlare::Camera3D get_live_camera() const;
         std::vector<Krampus24::Gameplay::Entities::Base*> get_entities() const;
         AllegroFlare::Physics::CollisionMesh* get_collision_mesh() const;
         AllegroFlare::Model3D* get_visual_mesh() const;
         AllegroFlare::Vec3D get_player_spawn_position() const;
         std::string get_collision_mesh_identifier() const;
         std::string get_visual_mesh_identifier() const;
         std::string get_visual_mesh_texture_identifier() const;
         std::string get_blocking_filename() const;
         int get_gems_collected() const;
         std::function<void(Krampus24::Gameplay::Screen*, void*)> get_on_finished_callback_func() const;
         void* get_on_finished_callback_func_user_data() const;
         bool get_initialized() const;
         std::vector<Krampus24::Gameplay::Entities::Base*> &get_entities_ref();
         void initialize();
         void load_or_reload_meshes();
         virtual void on_activate() override;
         virtual void on_deactivate() override;
         void load_or_reload_level_mesh();
         Krampus24::Gameplay::Entities::Base* find_0th_entity();
         void create_and_set_player_input_controller_for_0th_entity();
         void update();
         void render();
         void call_on_finished_callback_func();
         virtual void game_event_func(AllegroFlare::GameEvent* game_event=nullptr) override;
         virtual void primary_update_func(double time_now=0.0f, double delta_time=1.0f) override;
         virtual void primary_render_func() override;
         virtual void key_down_func(ALLEGRO_EVENT* ev=nullptr) override;
         virtual void key_up_func(ALLEGRO_EVENT* ev=nullptr) override;
         virtual void virtual_control_button_up_func(AllegroFlare::Player* player=nullptr, AllegroFlare::VirtualControllers::Base* virtual_controller=nullptr, int virtual_controller_button_num=0, bool is_repeat=false) override;
         virtual void virtual_control_button_down_func(AllegroFlare::Player* player=nullptr, AllegroFlare::VirtualControllers::Base* virtual_controller=nullptr, int virtual_controller_button_num=0, bool is_repeat=false) override;
         virtual void virtual_control_axis_change_func(ALLEGRO_EVENT* ev=nullptr) override;
         ALLEGRO_FONT* obtain_hud_font();
      };
   }
}


