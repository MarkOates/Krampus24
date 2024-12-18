#pragma once


#include <AllegroFlare/AudioController.hpp>
#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/Camera2D.hpp>
#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/CollisionObservers/Simple.hpp>
#include <AllegroFlare/DialogSystem/DialogSystem.hpp>
#include <AllegroFlare/Elements/StoryboardPages/Base.hpp>
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
#include <Krampus24/BlenderBlockingLoaderEntity.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Screen.hpp>
#include <Krampus24/Gameplay/ScriptingInterface.hpp>
#include <Krampus24/Shaders/Principled.hpp>
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
      public:
         static constexpr char* DEFAULT_INSPECT_HINT_TEXT = (char*)"Inspect";

      private:
         std::string data_folder_path;
         AllegroFlare::AudioController* audio_controller;
         AllegroFlare::EventEmitter* event_emitter;
         AllegroFlare::DialogSystem::DialogSystem* dialog_system;
         AllegroFlare::BitmapBin* bitmap_bin;
         AllegroFlare::FontBin* font_bin;
         AllegroFlare::ModelBin* model_bin;
         AllegroFlare::GameConfigurations::Base* game_configuration;
         AllegroFlare::Camera2D hud_camera;
         AllegroFlare::Camera3D player_view_camera;
         AllegroFlare::Camera3D live_camera;
         AllegroFlare::Camera3D target_camera;
         float player_spin;
         std::vector<Krampus24::Gameplay::Entities::Base*> entities;
         AllegroFlare::Physics::CollisionMesh* collision_mesh;
         AllegroFlare::Model3D* visual_mesh;
         AllegroFlare::Vec3D player_spawn_position;
         std::string collision_mesh_identifier;
         std::string visual_mesh_identifier;
         std::string visual_mesh_texture_identifier;
         std::string blocking_filename;
         Krampus24::Gameplay::ScriptingInterface* scripting;
         std::function<Krampus24::Gameplay::ScriptingInterface*(Krampus24::Gameplay::Screen*)> build_scripting_instance_func;
         std::string current_location_name;
         std::string current_location_floor;
         float current_location_reveal_counter;
         Krampus24::Shaders::Principled principled_shader;
         bool rendering_visual_mesh;
         bool rendering_collision_wiremesh;
         bool rendering_entity_models;
         bool rendering_entity_bounding_boxes;
         std::string inspect_hint_text;
         bool showing_inspect_hint;
         std::string dev__str_1;
         float dev__float_1;
         float dev__float_2;
         float dev__float_3;
         float dev__float_4;
         bool dev__bool_1;
         AllegroFlare::CollisionObservers::Simple collision_observer;
         Krampus24::Gameplay::Entities::Base* inspectable_entity_that_player_is_currently_colliding_with;
         bool initialized;

      protected:


      public:
         Screen(AllegroFlare::EventEmitter* event_emitter=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::FontBin* font_bin=nullptr, AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::GameConfigurations::Base* game_configuration=nullptr, std::vector<Krampus24::Gameplay::Entities::Base*> entities={}, AllegroFlare::Physics::CollisionMesh* collision_mesh=nullptr, std::string collision_mesh_identifier="tree-0x-collision_mesh.obj", std::string visual_mesh_identifier="tree-0x-visual.obj", std::string visual_mesh_texture_identifier="RETRO_TEXTURE_PACK_V15-atlas-01.png", std::string blocking_filename="tree-0x.blocking");
         virtual ~Screen();

         void set_data_folder_path(std::string data_folder_path);
         void set_audio_controller(AllegroFlare::AudioController* audio_controller);
         void set_event_emitter(AllegroFlare::EventEmitter* event_emitter);
         void set_dialog_system(AllegroFlare::DialogSystem::DialogSystem* dialog_system);
         void set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin);
         void set_font_bin(AllegroFlare::FontBin* font_bin);
         void set_model_bin(AllegroFlare::ModelBin* model_bin);
         void set_game_configuration(AllegroFlare::GameConfigurations::Base* game_configuration);
         void set_hud_camera(AllegroFlare::Camera2D hud_camera);
         void set_player_view_camera(AllegroFlare::Camera3D player_view_camera);
         void set_live_camera(AllegroFlare::Camera3D live_camera);
         void set_target_camera(AllegroFlare::Camera3D target_camera);
         void set_player_spin(float player_spin);
         void set_entities(std::vector<Krampus24::Gameplay::Entities::Base*> entities);
         void set_collision_mesh(AllegroFlare::Physics::CollisionMesh* collision_mesh);
         void set_visual_mesh(AllegroFlare::Model3D* visual_mesh);
         void set_player_spawn_position(AllegroFlare::Vec3D player_spawn_position);
         void set_collision_mesh_identifier(std::string collision_mesh_identifier);
         void set_visual_mesh_identifier(std::string visual_mesh_identifier);
         void set_visual_mesh_texture_identifier(std::string visual_mesh_texture_identifier);
         void set_blocking_filename(std::string blocking_filename);
         void set_build_scripting_instance_func(std::function<Krampus24::Gameplay::ScriptingInterface*(Krampus24::Gameplay::Screen*)> build_scripting_instance_func);
         void set_inspectable_entity_that_player_is_currently_colliding_with(Krampus24::Gameplay::Entities::Base* inspectable_entity_that_player_is_currently_colliding_with);
         std::string get_data_folder_path() const;
         AllegroFlare::AudioController* get_audio_controller() const;
         AllegroFlare::EventEmitter* get_event_emitter() const;
         AllegroFlare::DialogSystem::DialogSystem* get_dialog_system() const;
         AllegroFlare::BitmapBin* get_bitmap_bin() const;
         AllegroFlare::FontBin* get_font_bin() const;
         AllegroFlare::ModelBin* get_model_bin() const;
         AllegroFlare::GameConfigurations::Base* get_game_configuration() const;
         AllegroFlare::Camera2D get_hud_camera() const;
         AllegroFlare::Camera3D get_player_view_camera() const;
         AllegroFlare::Camera3D get_live_camera() const;
         AllegroFlare::Camera3D get_target_camera() const;
         float get_player_spin() const;
         std::vector<Krampus24::Gameplay::Entities::Base*> get_entities() const;
         AllegroFlare::Physics::CollisionMesh* get_collision_mesh() const;
         AllegroFlare::Model3D* get_visual_mesh() const;
         AllegroFlare::Vec3D get_player_spawn_position() const;
         std::string get_collision_mesh_identifier() const;
         std::string get_visual_mesh_identifier() const;
         std::string get_visual_mesh_texture_identifier() const;
         std::string get_blocking_filename() const;
         std::function<Krampus24::Gameplay::ScriptingInterface*(Krampus24::Gameplay::Screen*)> get_build_scripting_instance_func() const;
         Krampus24::Gameplay::Entities::Base* get_inspectable_entity_that_player_is_currently_colliding_with() const;
         bool get_initialized() const;
         std::vector<Krampus24::Gameplay::Entities::Base*> &get_entities_ref();
         AllegroFlare::CollisionObservers::Simple &get_collision_observer_ref();
         void initialize();
         void setup_dialog_system_styling();
         virtual std::vector<Krampus24::Gameplay::Entities::Base*> create_entity(Krampus24::BlenderBlockingLoaderEntity* blender_blocking_entity=nullptr);
         void load_or_reload_meshes();
         virtual void on_activate() override;
         virtual void on_deactivate() override;
         virtual void gameplay_suspend_func() override;
         virtual void gameplay_resume_func() override;
         void load_or_reload_level_mesh();
         bool a_0th_entity_exists();
         Krampus24::Gameplay::Entities::Base* find_0th_entity();
         void create_and_set_player_input_controller_for_0th_entity();
         std::vector<AllegroFlare::Elements::StoryboardPages::Base *> create_arbitrary_storyboard_pages_by_identifier();
         void interact_with_focused_inspectable_object();
         void hide_inspect_hint();
         void show_inspect_hint(std::string inspect_hint_text=DEFAULT_INSPECT_HINT_TEXT);
         bool is_player_looking_at_object(AllegroFlare::Vec3D player_position={}, AllegroFlare::Vec3D player_forward={}, AllegroFlare::Vec3D object_position={}, float angle_threshold_degrees=0.25);
         float player_view_dot_product_to_entity(AllegroFlare::Vec3D player_view_position={}, AllegroFlare::Vec3D player_look_vector={}, AllegroFlare::Vec3D object_position={});
         void update_inspectable_entity_that_player_is_currently_colliding_with();
         void show_location_name(std::string location_name="[unset-location_name]", std::string location_floor="[unset-location_floor]");
         void update();
         void render();
         std::string u(std::string string="[unset-string]");
         void xxxcall_on_finished_callback_func();
         virtual void game_event_func(AllegroFlare::GameEvent* game_event=nullptr) override;
         virtual void primary_update_func(double time_now=0.0f, double delta_time=1.0f) override;
         virtual void primary_render_func() override;
         void unlock_all_doors();
         virtual void key_down_func(ALLEGRO_EVENT* ev=nullptr) override;
         virtual void key_up_func(ALLEGRO_EVENT* ev=nullptr) override;
         virtual void virtual_control_button_up_func(AllegroFlare::Player* player=nullptr, AllegroFlare::VirtualControllers::Base* virtual_controller=nullptr, int virtual_controller_button_num=0, bool is_repeat=false) override;
         virtual void virtual_control_button_down_func(AllegroFlare::Player* player=nullptr, AllegroFlare::VirtualControllers::Base* virtual_controller=nullptr, int virtual_controller_button_num=0, bool is_repeat=false) override;
         virtual void virtual_control_axis_change_func(ALLEGRO_EVENT* ev=nullptr) override;
         ALLEGRO_FONT* obtain_gameplay_hud_font();
         ALLEGRO_FONT* obtain_hud_font();
         ALLEGRO_FONT* obtain_location_font();
         ALLEGRO_FONT* obtain_location_floor_font();
      };
   }
}



