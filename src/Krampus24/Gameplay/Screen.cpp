

#include <Krampus24/Gameplay/Screen.hpp>

#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/EventNames.hpp>
#include <AllegroFlare/Interpolators.hpp>
#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Physics/CollisionMeshCollisionStepper.hpp>
#include <AllegroFlare/PlayerInputControllers/Generic.hpp>
#include <AllegroFlare/RouteEventDatas/ActivateScreenByIdentifier.hpp>
#include <AllegroFlare/Routers/Standard.hpp>
#include <AllegroFlare/StringTransformer.hpp>
#include <AllegroFlare/Useful.hpp>
#include <Krampus24/BlenderBlockingLoader.hpp>
#include <Krampus24/Game/EntityFactory.hpp>
#include <Krampus24/Game/Scripting/Tree.hpp>
#include <Krampus24/Gameplay/Entities/ElevatorShaft.hpp>
#include <Krampus24/Gameplay/Entities/Zone.hpp>
#include <Krampus24/Gameplay/PlayerInputControllers/Player.hpp>
#include <Krampus24/Gameplay/Scripting/Empty.hpp>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <tuple>


namespace Krampus24
{
namespace Gameplay
{


Screen::Screen(AllegroFlare::EventEmitter* event_emitter, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::FontBin* font_bin, AllegroFlare::ModelBin* model_bin, AllegroFlare::GameConfigurations::Base* game_configuration, std::vector<Krampus24::Gameplay::Entities::Base*> entities, AllegroFlare::Physics::CollisionMesh* collision_mesh, std::string collision_mesh_identifier, std::string visual_mesh_identifier, std::string visual_mesh_texture_identifier, std::string blocking_filename)
   : AllegroFlare::Screens::Gameplay()
   , is_deployment_environment_production(false)
   , data_folder_path("[unset-data_folder_path]")
   , audio_controller(nullptr)
   , event_emitter(event_emitter)
   , dialog_system(nullptr)
   , bitmap_bin(bitmap_bin)
   , font_bin(font_bin)
   , model_bin(model_bin)
   , game_configuration(game_configuration)
   , hud_camera({})
   , player_view_camera({})
   , cinematic_camera({})
   , live_camera({})
   , target_camera({})
   , camera_state(CAMERA_STATE_UNDEF)
   , current_playing_cinematic_identifier("[unset-current_playing_cinematic_identifier]")
   , camera_state_is_busy(false)
   , camera_state_changed_at(0.0f)
   , showing_cinematic_black_bars(false)
   , game__central_core_cinematic_triggered(false)
   , player_spin(0.0f)
   , entities(entities)
   , collision_mesh(collision_mesh)
   , visual_mesh(nullptr)
   , player_spawn_position(AllegroFlare::Vec3D(0, 0, 0))
   , collision_mesh_identifier(collision_mesh_identifier)
   , visual_mesh_identifier(visual_mesh_identifier)
   , visual_mesh_texture_identifier(visual_mesh_texture_identifier)
   , blocking_filename(blocking_filename)
   , scripting(nullptr)
   , build_scripting_instance_func({})
   , current_location_name("")
   , current_location_floor("")
   , current_location_reveal_counter(0.0f)
   , principled_shader({})
   , rendering_visual_mesh(true)
   , rendering_collision_wiremesh(false)
   , rendering_entity_models(true)
   , rendering_entity_bounding_boxes(false)
   , inspect_hint_text(DEFAULT_INSPECT_HINT_TEXT)
   , showing_inspect_hint(false)
   , dev__str_1("[unset-def__str_1]")
   , dev__float_1(0.0f)
   , dev__float_2(0.0f)
   , dev__float_3(0.0f)
   , dev__float_4(0.0f)
   , dev__bool_1(false)
   , collision_observer({})
   , inspectable_entity_that_player_is_currently_colliding_with(nullptr)
   , initialized(false)
{
}


Screen::~Screen()
{
   // TODO: Destroy meshes (which should now be owned by this class)
   // TODO: Destroy visual mesh bitmap (which should now be owned by this class)
   return;
}


void Screen::set_is_deployment_environment_production(bool is_deployment_environment_production)
{
   this->is_deployment_environment_production = is_deployment_environment_production;
}


void Screen::set_data_folder_path(std::string data_folder_path)
{
   this->data_folder_path = data_folder_path;
}


void Screen::set_audio_controller(AllegroFlare::AudioController* audio_controller)
{
   this->audio_controller = audio_controller;
}


void Screen::set_event_emitter(AllegroFlare::EventEmitter* event_emitter)
{
   if (get_initialized()) throw std::runtime_error("[Screen::set_event_emitter]: error: guard \"get_initialized()\" not met.");
   this->event_emitter = event_emitter;
}


void Screen::set_dialog_system(AllegroFlare::DialogSystem::DialogSystem* dialog_system)
{
   if (get_initialized()) throw std::runtime_error("[Screen::set_dialog_system]: error: guard \"get_initialized()\" not met.");
   this->dialog_system = dialog_system;
}


void Screen::set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin)
{
   if (get_initialized()) throw std::runtime_error("[Screen::set_bitmap_bin]: error: guard \"get_initialized()\" not met.");
   this->bitmap_bin = bitmap_bin;
}


void Screen::set_font_bin(AllegroFlare::FontBin* font_bin)
{
   if (get_initialized()) throw std::runtime_error("[Screen::set_font_bin]: error: guard \"get_initialized()\" not met.");
   this->font_bin = font_bin;
}


void Screen::set_model_bin(AllegroFlare::ModelBin* model_bin)
{
   if (get_initialized()) throw std::runtime_error("[Screen::set_model_bin]: error: guard \"get_initialized()\" not met.");
   this->model_bin = model_bin;
}


void Screen::set_game_configuration(AllegroFlare::GameConfigurations::Base* game_configuration)
{
   this->game_configuration = game_configuration;
}


void Screen::set_hud_camera(AllegroFlare::Camera2D hud_camera)
{
   this->hud_camera = hud_camera;
}


void Screen::set_player_view_camera(AllegroFlare::Camera3D player_view_camera)
{
   this->player_view_camera = player_view_camera;
}


void Screen::set_cinematic_camera(AllegroFlare::Camera3D cinematic_camera)
{
   this->cinematic_camera = cinematic_camera;
}


void Screen::set_live_camera(AllegroFlare::Camera3D live_camera)
{
   this->live_camera = live_camera;
}


void Screen::set_target_camera(AllegroFlare::Camera3D target_camera)
{
   this->target_camera = target_camera;
}


void Screen::set_player_spin(float player_spin)
{
   this->player_spin = player_spin;
}


void Screen::set_entities(std::vector<Krampus24::Gameplay::Entities::Base*> entities)
{
   this->entities = entities;
}


void Screen::set_collision_mesh(AllegroFlare::Physics::CollisionMesh* collision_mesh)
{
   this->collision_mesh = collision_mesh;
}


void Screen::set_visual_mesh(AllegroFlare::Model3D* visual_mesh)
{
   this->visual_mesh = visual_mesh;
}


void Screen::set_player_spawn_position(AllegroFlare::Vec3D player_spawn_position)
{
   this->player_spawn_position = player_spawn_position;
}


void Screen::set_collision_mesh_identifier(std::string collision_mesh_identifier)
{
   if (get_initialized()) throw std::runtime_error("[Screen::set_collision_mesh_identifier]: error: guard \"get_initialized()\" not met.");
   this->collision_mesh_identifier = collision_mesh_identifier;
}


void Screen::set_visual_mesh_identifier(std::string visual_mesh_identifier)
{
   if (get_initialized()) throw std::runtime_error("[Screen::set_visual_mesh_identifier]: error: guard \"get_initialized()\" not met.");
   this->visual_mesh_identifier = visual_mesh_identifier;
}


void Screen::set_visual_mesh_texture_identifier(std::string visual_mesh_texture_identifier)
{
   if (get_initialized()) throw std::runtime_error("[Screen::set_visual_mesh_texture_identifier]: error: guard \"get_initialized()\" not met.");
   this->visual_mesh_texture_identifier = visual_mesh_texture_identifier;
}


void Screen::set_blocking_filename(std::string blocking_filename)
{
   if (get_initialized()) throw std::runtime_error("[Screen::set_blocking_filename]: error: guard \"get_initialized()\" not met.");
   this->blocking_filename = blocking_filename;
}


void Screen::set_build_scripting_instance_func(std::function<Krampus24::Gameplay::ScriptingInterface*(Krampus24::Gameplay::Screen*)> build_scripting_instance_func)
{
   this->build_scripting_instance_func = build_scripting_instance_func;
}


void Screen::set_inspectable_entity_that_player_is_currently_colliding_with(Krampus24::Gameplay::Entities::Base* inspectable_entity_that_player_is_currently_colliding_with)
{
   this->inspectable_entity_that_player_is_currently_colliding_with = inspectable_entity_that_player_is_currently_colliding_with;
}


bool Screen::get_is_deployment_environment_production() const
{
   return is_deployment_environment_production;
}


std::string Screen::get_data_folder_path() const
{
   return data_folder_path;
}


AllegroFlare::AudioController* Screen::get_audio_controller() const
{
   return audio_controller;
}


AllegroFlare::EventEmitter* Screen::get_event_emitter() const
{
   return event_emitter;
}


AllegroFlare::DialogSystem::DialogSystem* Screen::get_dialog_system() const
{
   return dialog_system;
}


AllegroFlare::BitmapBin* Screen::get_bitmap_bin() const
{
   return bitmap_bin;
}


AllegroFlare::FontBin* Screen::get_font_bin() const
{
   return font_bin;
}


AllegroFlare::ModelBin* Screen::get_model_bin() const
{
   return model_bin;
}


AllegroFlare::GameConfigurations::Base* Screen::get_game_configuration() const
{
   return game_configuration;
}


AllegroFlare::Camera2D Screen::get_hud_camera() const
{
   return hud_camera;
}


AllegroFlare::Camera3D Screen::get_player_view_camera() const
{
   return player_view_camera;
}


AllegroFlare::Camera3D Screen::get_cinematic_camera() const
{
   return cinematic_camera;
}


AllegroFlare::Camera3D Screen::get_live_camera() const
{
   return live_camera;
}


AllegroFlare::Camera3D Screen::get_target_camera() const
{
   return target_camera;
}


uint32_t Screen::get_camera_state() const
{
   return camera_state;
}


std::string Screen::get_current_playing_cinematic_identifier() const
{
   return current_playing_cinematic_identifier;
}


float Screen::get_player_spin() const
{
   return player_spin;
}


std::vector<Krampus24::Gameplay::Entities::Base*> Screen::get_entities() const
{
   return entities;
}


AllegroFlare::Physics::CollisionMesh* Screen::get_collision_mesh() const
{
   return collision_mesh;
}


AllegroFlare::Model3D* Screen::get_visual_mesh() const
{
   return visual_mesh;
}


AllegroFlare::Vec3D Screen::get_player_spawn_position() const
{
   return player_spawn_position;
}


std::string Screen::get_collision_mesh_identifier() const
{
   return collision_mesh_identifier;
}


std::string Screen::get_visual_mesh_identifier() const
{
   return visual_mesh_identifier;
}


std::string Screen::get_visual_mesh_texture_identifier() const
{
   return visual_mesh_texture_identifier;
}


std::string Screen::get_blocking_filename() const
{
   return blocking_filename;
}


std::function<Krampus24::Gameplay::ScriptingInterface*(Krampus24::Gameplay::Screen*)> Screen::get_build_scripting_instance_func() const
{
   return build_scripting_instance_func;
}


Krampus24::Gameplay::Entities::Base* Screen::get_inspectable_entity_that_player_is_currently_colliding_with() const
{
   return inspectable_entity_that_player_is_currently_colliding_with;
}


bool Screen::get_initialized() const
{
   return initialized;
}


std::vector<Krampus24::Gameplay::Entities::Base*> &Screen::get_entities_ref()
{
   return entities;
}


AllegroFlare::CollisionObservers::Simple &Screen::get_collision_observer_ref()
{
   return collision_observer;
}


void Screen::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::initialize]: error: guard \"(!initialized)\" not met");
   }
   if (!(al_is_system_installed()))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::initialize]: error: guard \"al_is_system_installed()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::initialize]: error: guard \"al_is_system_installed()\" not met");
   }
   if (!(al_is_primitives_addon_initialized()))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::initialize]: error: guard \"al_is_primitives_addon_initialized()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::initialize]: error: guard \"al_is_primitives_addon_initialized()\" not met");
   }
   if (!(al_is_font_addon_initialized()))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::initialize]: error: guard \"al_is_font_addon_initialized()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::initialize]: error: guard \"al_is_font_addon_initialized()\" not met");
   }
   if (!(dialog_system))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::initialize]: error: guard \"dialog_system\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::initialize]: error: guard \"dialog_system\" not met");
   }
   if (!(audio_controller))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::initialize]: error: guard \"audio_controller\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::initialize]: error: guard \"audio_controller\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::initialize]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::initialize]: error: guard \"event_emitter\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::initialize]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::initialize]: error: guard \"bitmap_bin\" not met");
   }
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::initialize]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::initialize]: error: guard \"font_bin\" not met");
   }
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::initialize]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::initialize]: error: guard \"model_bin\" not met");
   }
   set_update_strategy(AllegroFlare::Screens::Base::UpdateStrategy::SEPARATE_UPDATE_AND_RENDER_FUNCS);

   //live_camera.stepout = AllegroFlare::Vec3D(0, 1.25, 0); // FPS camera
   live_camera.stepout = AllegroFlare::Vec3D(0, 2.25, 0); // FPS camera
   live_camera.tilt = 0.0;
   live_camera.near_plane = 0.25;   // Variables are: camera near_plane, collision_mesh face offset amount, extra collision stepout, 
   live_camera.far_plane = 500.0;

   player_view_camera = live_camera;

   //live_camera.stepout = AllegroFlare::Vec3D(0, 0.25, 10); // Third person
   //live_camera.tilt = 0.8;

   // Set this levels entities to the scripting logic
   //Krampus24::Game::Scripting::Tree *tree_scripting = new Krampus24::Game::Scripting::Tree;
   //tree_scripting.set_entities(&entities);
   //tree_scripting.set_collision_observer(&collision_observer);
   //tree_scripting.set_font_bin(font_bin);
   //tree_scripting.initialize();

   //scripting = tree_scripting;

   // Setup dialog system styling
   setup_dialog_system_styling();

   // Load level and entities
   load_or_reload_meshes();

   // Create a player input controller for the 0th entity
   create_and_set_player_input_controller_for_0th_entity();

   principled_shader.initialize();
   principled_shader.set_fog_color(al_color_html("#31687a"));
   principled_shader.set_fog_intensity(0.9);
   principled_shader.set_fog_distance(40.0);

   principled_shader.set_world_tint(ALLEGRO_COLOR{0.3, 0.3, 0.4, 1.0});

   set_camera_state(CAMERA_STATE_PLAYER);

   initialized = true;
   return;
}

void Screen::setup_dialog_system_styling()
{
   //dialog_system->set_standard_dialog_box_font_name("Orbitron-Medium.ttf");

   //dialog_system->set_standard_dialog_box_font_name("Exan-Regular.ttf");
   //dialog_system->set_standard_dialog_box_font_size(
      //AllegroFlare::DialogSystem::DialogSystem::DEFAULT_STANDARD_DIALOG_BOX_FONT_SIZE - 8
   //);

   //dialog_system->set_standard_dialog_box_font_name("Michroma-Regular.ttf");
   //dialog_system->set_standard_dialog_box_font_size(
      //AllegroFlare::DialogSystem::DialogSystem::DEFAULT_STANDARD_DIALOG_BOX_FONT_SIZE - 14
   //);
   return;
}

Krampus24::Game::Scripting::Tree* Screen::get_scripting_as()
{
   if (!(scripting))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::get_scripting_as]: error: guard \"scripting\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::get_scripting_as]: error: guard \"scripting\" not met");
   }
   return static_cast<Krampus24::Game::Scripting::Tree*>(scripting);
}

std::vector<Krampus24::Gameplay::Entities::Base*> Screen::create_entity(Krampus24::BlenderBlockingLoaderEntity* blender_blocking_entity)
{
   if (!(blender_blocking_entity))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::create_entity]: error: guard \"blender_blocking_entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::create_entity]: error: guard \"blender_blocking_entity\" not met");
   }
   // TODO: Find a way to move this method into a... Game/ class possibly?
   Krampus24::Game::EntityFactory factory;
   factory.set_event_emitter(event_emitter);
   //factory.set_font_bin(font_bin);
   factory.set_model_bin(model_bin);
   factory.set_bitmap_bin(bitmap_bin);
   factory.set_collision_mesh(collision_mesh);
   factory.initialize();
   return factory.create_entity(blender_blocking_entity);
}

void Screen::load_or_reload_meshes()
{
   player_spawn_position = {4.0, 0.001, 3.5}; // DEVELOPMENT
   float player_initial_spin = ALLEGRO_PI * -0.25;
   //AllegroFlare::Vec3D restored_player_position = player_spawn_position;
   Krampus24::Gameplay::Entities::Base* existing_player_entity = nullptr;
   if (a_0th_entity_exists()) existing_player_entity = find_0th_entity();
   //if (existing_player_entity);




   // Load the collision mesh
   if (collision_mesh)
   {

      model_bin->destroy(collision_mesh_identifier);
      collision_mesh = nullptr;
   }
   collision_mesh = new AllegroFlare::Physics::CollisionMesh();
   collision_mesh->set_model(model_bin->operator[](collision_mesh_identifier));
   collision_mesh->load();

   // Load the visual mesh
   if (visual_mesh)
   {
      model_bin->destroy(visual_mesh_identifier);
      visual_mesh = nullptr;
   }
   visual_mesh = model_bin->operator[](visual_mesh_identifier);
   visual_mesh->set_texture(bitmap_bin->operator[](visual_mesh_texture_identifier));




   // Load the blocking file
   // Delete all entities (except the existing player entity)
   for (auto &entity : entities)
   {
      if (entity == existing_player_entity) continue; // Skip the player entity if it already exists
      delete entity; // Hmm, good luck here
   }

   // Clear the entities (Will re-add the player if it had previously existed)
   entities.clear();
   collision_observer.clear(); // Clear any current collisions, and the subject
   inspectable_entity_that_player_is_currently_colliding_with = nullptr; // TODO: Consider side effects of releasing
                                                                         // the current colliding entity here
   if (showing_inspect_hint) hide_inspect_hint();

   // Create the 0th entity (the player)
   if (existing_player_entity)
   {
      entities.push_back(existing_player_entity);
   }
   else
   {
      Krampus24::Gameplay::Entities::Base* player_entity =
         new Krampus24::Gameplay::Entities::Base();
      player_entity->placement.size = {0.5, 0.5, 0.5};
      player_entity->aabb3d.set_max(player_entity->placement.size);
      player_entity->aabb3d_alignment = { 0.5, 0.05, 0.5 };
      player_entity->placement.position = player_spawn_position;
      player_entity->player__spin = player_initial_spin;
      entities.push_back(player_entity);
   }
   // Create entities from the blocking file
   std::string blocking_file_full_path = data_folder_path + "maps/" + blocking_filename;
   Krampus24::BlenderBlockingLoader blender_blocking_loader(blocking_file_full_path);
   blender_blocking_loader.load();
   blender_blocking_loader.for_each_entity([this, existing_player_entity](Krampus24::BlenderBlockingLoaderEntity* entity){
      //float x = entity->location.x;
      //float y = entity->location.z; // Swapping z<->y
      //float z = entity->location.y; // Swapping z<->y

      std::string entity_root_name = entity->get_name_unversioned();
      if (entity_root_name == "player_spawn")
      {
         float x = entity->location.x;
         float y = entity->location.z; // Swapping z<->y
         float z = entity->location.y; // Swapping z<->y

         AllegroFlare::Vec3D position = AllegroFlare::Vec3D(x, y, z);

         //Krampus24::Gameplay::Entities::Base* player_entity = find_0th_entity();
         player_spawn_position = position;
         //player_entity->placement.position = player_spawn_position; // + AllegroFlare::Vec3D(0, 0.01, 0);
         //float rotation = entity->rotation.z / 360.0;
         //auto *result = Krampus24::Gameplay::Entities::Turret::construct(model_bin, bitmap_bin, position, rotation);
         //result->name = entity->name;
         //return result;
         if (!existing_player_entity)
         {
            Krampus24::Gameplay::Entities::Base* player_entity = find_0th_entity();
            player_entity->placement.position = player_spawn_position;
            float rotation = ALLEGRO_PI * 1.0; // NOTE: This value is not extracted from the entity, and is
                                               // hard-coded here.
            player_entity->player__spin = rotation; //player_spawn_position; // HERE
         }
      }
      else
      {
         //std::vector<Krampus24::Gameplay::Entities::Base*> result_entities = scripting->create_entity(entity);
         std::vector<Krampus24::Gameplay::Entities::Base*> result_entities = create_entity(entity);
         for (auto &result_entity : result_entities)
         {
            entities.push_back(result_entity);
         }
      }
   });
   blender_blocking_loader.for_each_zone([this](Krampus24::BlenderBlockingLoaderBlock* zone){
      //std::string zone_root_name = zone->get_name_unversioned();

      //if (zone_root_name == Krampus24::Gameplay::Entities::Zone::BLENDER_IDENTIFIER)
      //{
         //float x = entity->location.x;
         //float y = entity->location.z; // Swapping z<->y
         //float z = entity->location.y; // Swapping z<->y
         AllegroFlare::Vec3D position = AllegroFlare::Vec3D(
            zone->min_coordinates.x,
            zone->min_coordinates.z,
            zone->min_coordinates.y
         );

         AllegroFlare::Vec3D natural_size = zone->calculate_size();
         AllegroFlare::Vec3D size = AllegroFlare::Vec3D(
            natural_size.x,
            natural_size.z,
            natural_size.y
         );

         //float x = entity->location.x;
         //float y = entity->location.z; // Swapping z<->y
         //float z = entity->location.y; // Swapping z<->y
         //AllegroFlare::Vec3D position = AllegroFlare::Vec3D(x, y, z);

         //float rotation = entity->rotation.z / 360.0;
         //AllegroFlare::Vec3D size = entity->size;
         auto *result = Krampus24::Gameplay::Entities::Zone::construct(position, size);
         result->name = zone->name;
         //return { result };
         entities.push_back(result);
      //}
   });



   // Load up the sound effects // DEVELOPMENT
   // TODO: Move this to another more appropriate location
   /*
   audio_controller->set_and_load_sound_effect_elements(
      Krampus24::Game::Scripting::Tree().build_audio_controller_sound_effect_list()
   );

   audio_controller->set_and_load_music_track_elements(
      Krampus24::Game::Scripting::Tree().build_audio_controller_music_track_list()
   );
   */



   // Load up the scripting
   if (scripting)
   {
      delete scripting;
      scripting = nullptr;
   }


   if (build_scripting_instance_func)
   {
      //std::cout << "---- Building scripting ----" << std::endl;
      scripting = build_scripting_instance_func(this);
      if (!scripting)
      {
         AllegroFlare::Logger::throw_error(
            "Krampus24::Gameplay::Screen::load_or_reload_meshes",
            "When calling the provided \"build_scripting_interface_func\", a nullptr was returned."
         );
      }
   }
   else
   {
      // TODO: Output warning
      AllegroFlare::Logger::warn_from(
         "Krampus24::Gameplay::Screen::load_or_reload_meshes",
         "When loading, there was no \"build_scripting_interface_func\" provided. A generic Empty class will be "
            "used."
      );
      scripting = new Krampus24::Gameplay::Scripting::Empty();
   }


   //Krampus24::Game::Scripting::Tree *tree_scripting = new Krampus24::Game::Scripting::Tree;
   //tree_scripting->set_entities(&entities);
   //tree_scripting->set_collision_observer(&collision_observer);
   //tree_scripting->set_font_bin(font_bin);
   //tree_scripting->initialize();
   //scripting = tree_scripting


   //scripting.build_on_collision_callbacks();

   return;
}

void Screen::on_activate()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::on_activate]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::on_activate]: error: guard \"initialized\" not met");
   }
   static bool first_activation = false;
   if (!first_activation)
   {
      std::cout << "first screen activation" << std::endl;
      event_emitter->emit_play_music_track_event("arrival"); // TODO: Figure out the correct place for this
      first_activation = true;
   }
   //event_emitter->emit_play_music_track_event("arrival"); // TODO: Figure out the correct place for this

   //emit_event_to_update_input_hints_bar();
   //emit_show_and_size_input_hints_bar_event();
   return;
}

void Screen::on_deactivate()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::on_deactivate]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::on_deactivate]: error: guard \"initialized\" not met");
   }
   //emit_hide_and_restore_size_input_hints_bar_event();
   return;
}

void Screen::gameplay_suspend_func()
{
   //gameplay_suspended_at = al_get_time();
   //AllegroFlare::Screens::Gameplay::gameplay_suspend_func(); // Should this be here?
   //if (player_input_controller_exists())
   //{
      //AllegroFlare::Logger::warn_from(
            //"AllegroFlare::Prototypes::TileFPS::Screen::gameplay_suspend_func",
            //"This method requires attention. Currently there is little to no support for player_input_controller "
               //"handling gameplay_suspend_func()"
         //);
      //here__get_player_input_controller()->gameplay_suspend_func();
   //}
   //player_stop_moving();
   // NOTE: This function is called immediately after the gameplay is suspended.
   // TODO: Consider setting states on entities, checking their state timers, etc
   return;
}

void Screen::gameplay_resume_func()
{
   //AllegroFlare::Screens::Gameplay::gameplay_resume_func();

   //if (player_input_controller_exists())
   //{
      //AllegroFlare::Logger::warn_from(
            //"AllegroFlare::Prototypes::TileFPS::Screen::gameplay_suspend_func",
            //"This method requires attention. Currently there is little to no support for player_input_controller "
               //"handling gameplay_resume_func()"
         //);
      //here__get_player_input_controller()->gameplay_resume_func();
   //}
   //player_stop_moving();
   // NOTE: This function is called immediately after the gameplay is resumed.
   // TODO: Consider reviewing states on entities, reviewing their state timers, etc.
   return;

   // Keyboard changes that occurred during the gameplay
   AllegroFlare::SuspendedKeyboardState &suspended_keyboard_state = get_suspended_keyboard_state_ref();
   std::vector<uint32_t> keys_pressed = suspended_keyboard_state.get_keys_pressed();
   std::vector<uint32_t> keys_released = suspended_keyboard_state.get_keys_released();
   float time_now = al_get_time(); // TODO: Inject time when the resume occurred

   //auto entity_control_connector = get_entity_control_connector();
   //if (entity_control_connector)
   //{
      //if (entity_control_connector->is_type(SomePlatformer::EntityControlConnectors::PlayerCharacter::TYPE))
      //{
         // In this techniqe, we'll build a fake ALLEGRO_EVENT and pass it into the entity_control_connector.
         // There could potentially be unidentified side effects with this approach, some example:
         //   1) This event does not pass through the normal global event queue.
         //   2) The "source" and "display" fields are not used in this event, but may need to be present at
         //      some point
         //   3) The control connector may be expecting a full pass of the event through the system before
         //      processing a second "event".
         // Advantage of this approach is that the base class takes key_up_func and key_down_func, so this
         // technique could be used on all EntityControlConnectors::Base classes.

         // Process key releases (a.k.a. "key up")
         //for (auto &key_released : keys_released)
         //{
            //ALLEGRO_EVENT event;
            //event.type = ALLEGRO_EVENT_KEY_UP;
            //event.any.source = nullptr; // TODO: Should I be using a SuspendedKeyboardState event source?
            //event.any.timestamp = time_now;
            //event.keyboard.keycode = key_released;
            //event.keyboard.display = nullptr; // TODO: Consider if al_get_current_display() should be used here

            //entity_control_connector->key_up_func(&event);
         //}

         // Process key presses (a.k.a. "key down")
         //for (auto &key_pressed : keys_pressed)
         //{
            //ALLEGRO_EVENT event;
            //event.type = ALLEGRO_EVENT_KEY_DOWN;
            //event.any.source = nullptr; // TODO: Should I be using a SuspendedKeyboardState event source?
            //event.any.timestamp = time_now;
            //event.keyboard.keycode = key_pressed;
            //event.keyboard.display = nullptr; // TODO: Consider if al_get_current_display() should be used here

            //entity_control_connector->key_down_func(&event);
         //}
      //}
   //}
   return;
}

void Screen::load_or_reload_level_mesh()
{
   return;
}

bool Screen::a_0th_entity_exists()
{
   return (entities.size() > 0);
}

Krampus24::Gameplay::Entities::Base* Screen::find_0th_entity()
{
   if (!((entities.size() > 0)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::find_0th_entity]: error: guard \"(entities.size() > 0)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::find_0th_entity]: error: guard \"(entities.size() > 0)\" not met");
   }
   return entities.at(0);
}

void Screen::create_and_set_player_input_controller_for_0th_entity()
{
   Krampus24::Gameplay::PlayerInputControllers::Player *result_player_input_controller = new
      Krampus24::Gameplay::PlayerInputControllers::Player();
   result_player_input_controller->set_player_entity(find_0th_entity());
   result_player_input_controller->set_move_multiplier(0.0875); // 0.1 is running, 0.5 is walking (slowly)
   result_player_input_controller->initialize();

   set_player_input_controller(result_player_input_controller);

   return;




   /*
   AllegroFlare::PlayerInputControllers::Generic *result_player_input_controller =
      new AllegroFlare::PlayerInputControllers::Generic();

   result_player_input_controller->set_on_time_step_update([this](AllegroFlare::Vec2D control_movement, double, double){
      //find_0th_entity()->get_velocity_ref().position.x = pos.x * 0.1;
      //find_0th_entity()->get_velocity_ref().position.z = pos.y * 0.1;
      //bool moving_forward = false;
      //bool moving_backward = false;
      //bool moving_right = false;
      //bool moving_left = false;

      //control_movement = pos;
      //if (moving_forward) control_movement.y = -1;
      //if (moving_backward) control_movement.y = 1;
      //if (moving_right) control_movement.x = 1;
      //if (moving_left) control_movement.x = -1;

      auto entity = find_0th_entity();

      // Relative to camera:
      float angle = player_spin;
      //float angle = live_camera.spin;
      float x_prime = control_movement.x * std::cos(angle) - control_movement.y * std::sin(angle);
      float y_prime = control_movement.x * std::sin(angle) + control_movement.y * std::cos(angle);
      entity->velocity.position.x = x_prime * 0.1;
      entity->velocity.position.z = y_prime * 0.1;
   });

   set_player_input_controller(result_player_input_controller);
   */
   return;
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base *> Screen::create_arbitrary_storyboard_pages_by_identifier()
{
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::create_arbitrary_storyboard_pages_by_identifier]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::create_arbitrary_storyboard_pages_by_identifier]: error: guard \"font_bin\" not met");
   }
   if (!(scripting))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::create_arbitrary_storyboard_pages_by_identifier]: error: guard \"scripting\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::create_arbitrary_storyboard_pages_by_identifier]: error: guard \"scripting\" not met");
   }
   return scripting->create_arbitrary_storyboard_pages_by_identifier();
}

void Screen::interact_with_focused_inspectable_object()
{
   //spawn_arbitrary_storyboard_screen("pig_storyboard");

   if (inspectable_entity_that_player_is_currently_colliding_with)
   {
      // Perform the entities local reaction to inspection
      bool entity_scoped_inspection_occurred =
         inspectable_entity_that_player_is_currently_colliding_with->on_player_inspect_or_use();

      // TODO: Perform the scripting's inspection logic
      bool scripting_scoped_inspection_occurred = false;
      //if (!entity_scoped_inspection_occurred) // TODO: Consider if a guard here is desired or not
      {
         if (scripting)
         {
            // NOTE: No scripting here
            scripting->interact_with_focused_object(inspectable_entity_that_player_is_currently_colliding_with);
         }
      }

      // If no inspection happened, play a "no inspection sound"
      if (!entity_scoped_inspection_occurred || scripting_scoped_inspection_occurred)
      {
         // TODO: Play "no inspection" sound
      }
   }
   return;
}

void Screen::hide_inspect_hint()
{
   showing_inspect_hint = false;
   return;
}

void Screen::show_inspect_hint(std::string inspect_hint_text)
{
   showing_inspect_hint = true;
   this->inspect_hint_text = inspect_hint_text;
   return;
}

bool Screen::is_player_looking_at_object(AllegroFlare::Vec3D player_position, AllegroFlare::Vec3D player_forward, AllegroFlare::Vec3D object_position, float angle_threshold_degrees)
{
   // Check if the player is looking at an object
   //bool is_player_looking_at_object(const Vector3 &player_position,
                                    //const Vector3 &player_forward,
                                    //const Vector3 &object_position,
                                    //float angle_threshold_degrees)
   {
      // Vector pointing from player to object
      AllegroFlare::Vec3D to_object = object_position - player_position;
      AllegroFlare::Vec3D to_object_normalized = to_object.normalized();

      // Normalize the player's forward vector
      AllegroFlare::Vec3D player_forward_normalized = player_forward.normalized();

      // Calculate the dot product
      float dot_product = player_forward_normalized * to_object_normalized;
      //float dot_product = player_forward_normalized.dot_product(to_object_normalized);

      // Convert angle threshold to radians
      float angle_threshold_radians = angle_threshold_degrees * (ALLEGRO_PI / 180.0f);

      // Calculate the cosine of the threshold angle
      float cos_threshold = std::cos(angle_threshold_radians);

      // If the dot product is greater than the cosine of the threshold angle,
      // the object is within the player's field of view
      return dot_product >= cos_threshold;
   }
}

float Screen::player_view_dot_product_to_entity(AllegroFlare::Vec3D player_view_position, AllegroFlare::Vec3D player_look_vector, AllegroFlare::Vec3D object_position)
{
   // Get tablet entity
   //AllegroFlare::Vec3D player_view_position = player_view_camera.get_real_position();
   //AllegroFlare::Vec3D player_look_vector = player_view_camera.get_viewing_direction();

   // Calculate the vector pointing to the object
   AllegroFlare::Vec3D object_vector = player_view_position - object_position;
   //AllegroFlare::Vec3D object_vector = object_position - player_view_position;

   // Normalize both vectors
   AllegroFlare::Vec3D normalized_object_vector = object_vector.normalized();
   AllegroFlare::Vec3D normalized_player_look_vector = player_look_vector.normalized();

   // Compute the dot product
   float dot_product = normalized_player_look_vector * normalized_object_vector;

   // Return the alignment
   return dot_product;
}

void Screen::update_inspectable_entity_that_player_is_currently_colliding_with()
{
   auto player_entity = find_0th_entity();
   if (!player_entity) throw std::runtime_error("asjiodfasjdiofajsdiofasjdifoajsdiofjadsoi");

   AllegroFlare::Vec3D player_view_position = player_view_camera.position;
   AllegroFlare::Vec3D player_look_vector = player_view_camera.get_viewing_direction();

   Krampus24::Gameplay::Entities::Base *found_colliding_entity = nullptr;
   for (auto &entity : entities)
   {
      if (!entity->active) continue;
      if (entity == player_entity) continue;
      if (!entity->player_can_inspect_or_use) continue; // NOTE: In this case, the entity does not need a
                                                        // collides with player flag in order for inspect feature
                                                        // to work.
      if (entity->collides_aabb3d(player_entity)) 
      {
         //AllegroFlare::Vec3D player_view_position = player_view_camera.position;
         //AllegroFlare::Vec3D player_look_vector = player_view_camera.get_viewing_direction();

         // TODO: The factoring could be improved here a bit, breaking on the found collision entity and assigning
         // to it is duplicated in the conditional branch.

         if (entity->player_can_inspect_or_use__custom_look_at_logic)
         {
            bool inspect_collision_does_hit = entity->player_can_inspect_or_use__custom_look_at_logic(
                  entity,
                  &player_view_position,
                  &player_look_vector
               );
            if (!inspect_collision_does_hit) continue;

            found_colliding_entity = entity;
            break;
         }
         else
         {
            AllegroFlare::Vec3D entity_position = entity->calculate_centroid(); // HERE

            float dot_product_to_entity = player_view_dot_product_to_entity(
               player_view_position,
               player_look_vector,
               entity_position
            );

            bool is_looking_at_entity = -dot_product_to_entity > 0.9;

            //dev__str_1 = entity->name;
            //dev__float_1 = dot_product_to_entity;
            //dev__float_2 = std::abs(dot_product_to_entity);
            //dev__float_3 = AllegroFlare::distance(entity->placement.position, player_entity->placement.position);
            //dev__float_4 = AllegroFlare::distance(entity->placement.position, player_view_position);
            //dev__bool_1 = is_looking_at_entity;

            if (!is_looking_at_entity) continue;

            //if (!player_is_looking_at_object) continue; // TODO: bring in calculation

            found_colliding_entity = entity;
            break;
         }
      }
   }

   if (found_colliding_entity != inspectable_entity_that_player_is_currently_colliding_with)
   {
      // Assign the found entity to be the colliding one
      inspectable_entity_that_player_is_currently_colliding_with = found_colliding_entity;

      // TODO: Some feedback that a new collision occurred (a sound effect for example)
      if (inspectable_entity_that_player_is_currently_colliding_with == nullptr)
      {
         hide_inspect_hint();
      }
      else
      {
         //std::string inspect_hint_text = DEFAULT_INSPECT_HINT_TEXT;
         //if (inspectable_entity_that_player_is_currently_colliding_with->has_custom_inspect_hint_text())
         //{
            //inspect_hint_text = 
               //inspectable_entity_that_player_is_currently_colliding_with->custom_inspect_hint_text;
         //}
         //inspect_hint_text =
         //inspectable_entity_that_player_is_currently_colliding_with->player_can_inspect_or_use__hint_text;

         show_inspect_hint(
            //inspect_hint_text,
            inspectable_entity_that_player_is_currently_colliding_with->player_can_inspect_or_use__hint_text
         );
      }
   }
   return;
}

void Screen::show_location_name(std::string location_name, std::string location_floor)
{
   if (this->current_location_name == location_name && this->current_location_floor == location_floor) return;
   this->current_location_name = location_name;
   this->current_location_floor = location_floor;

   current_location_reveal_counter = 0.0f;
   return;
}

void Screen::update()
{
   float time_now = al_get_time();
   float step_duration = 1.0f;


   current_location_reveal_counter += (1.0 / 60.0);
   if (current_location_reveal_counter > 10.0) current_location_reveal_counter = 10.0;



   // Step each entity via its update function
   for (auto &entity : entities)
   {
      if (!entity->active) continue;
      entity->on_time_step(step_duration, time_now); // Should this be al_get_time or a globally tracked time now?
   }


   //
   // Apply environmental forces (gravity, air drag)
   //

   float gravity = AllegroFlare::Physics::CollisionMeshCollisionStepper::DEFAULT_GRAVITY;
   float air_drag = AllegroFlare::Physics::CollisionMeshCollisionStepper::DEFAULT_AIR_DRAG;
   AllegroFlare::Vec3D velocity_direction = AllegroFlare::Vec3D(0.0f, 1.0f, 0.0f).normalized();

   for (auto &entity : entities)
   {
      if (!entity->active || !entity->affected_by_environmental_forces) continue;

      entity->velocity.position += velocity_direction * gravity * step_duration;
      entity->velocity.position *= (1.0 - air_drag);
   }


   //
   // Step entities
   //

   // Build the entities list to be stepped by the stepper
   std::vector<std::tuple<AllegroFlare::Vec3D*, AllegroFlare::Vec3D*, void*>>
      collision_stepper_entities;
   collision_stepper_entities.reserve(entities.size());
   for (auto &entity : entities)
   {
      if (!entity->active || !entity->collides_with_environment) continue;

      collision_stepper_entities.push_back(std::make_tuple(
         &entity->placement.position,
         &entity->velocity.position,
         (void*)entity
      ));
   }

   if (collision_stepper_entities.empty()) throw std::runtime_error("asdfasdf");

   AllegroFlare::Physics::CollisionMeshCollisionStepper stepper;
   stepper.set_collision_mesh(collision_mesh);
   stepper.set__entities(&collision_stepper_entities);
   stepper.disable_applying_environmental_forces();
   stepper.step(step_duration);


   //
   // Reposition the player vertically if they are attached to an elevator
   //
   {
      auto player_entity = find_0th_entity();
      if (player_entity->is_attached_to_elevator())
      {
         Krampus24::Gameplay::Entities::Base* elevator = player_entity->elevator_entity_attached_to;
         Krampus24::Gameplay::Entities::ElevatorShaft* as =
            static_cast<Krampus24::Gameplay::Entities::ElevatorShaft*>(elevator);
         player_entity->placement.position.y = as->calculate_global_elevator_car_y_position();
         player_entity->velocity.position.y = 0.0;
      }
   }


   //
   // Observe change in entity<->entity collisions
   //
   {
      std::set<void*> collidables;
      auto player_entity = find_0th_entity();
      for (auto &entity : entities)
      {
         if (entity == player_entity) continue;
         if (!entity->active || !entity->collides_with_player) continue;
         collidables.insert(entity);
      }
      collision_observer.set_subject(find_0th_entity());
      collision_observer.set_collidables(collidables);
      collision_observer.set_on_test_collide([](void* subject, void* collidable) -> bool {
        return static_cast<Krampus24::Gameplay::Entities::Base*>(subject)->
        collides_aabb3d(static_cast<Krampus24::Gameplay::Entities::Base*>(collidable));
      });
      collision_observer.process();

      // Process "entered" collisions
      for (auto &entered : collision_observer.get_entered())
      {
         // TODO: Consider extracting this to a method
         Krampus24::Gameplay::Entities::Base* entity =
            static_cast<Krampus24::Gameplay::Entities::Base*>(entered);
         // TODO: Add scripting for collecting gems

         // TODO: Incorporate this gem collection logic
         //entity->active = false;
         //entity->visible = false;
         //gems_collected++;
         //entity>on_enter_player_bbox_collision();
         entity->on_enter_player_bbox_collision(player_entity);


         if (scripting && scripting->has_on_collision_callback(entity))
         {
            scripting->call_on_collision_callback(entity);
         }


         if (entity->elevator_shaft__is_elevator_shaft)
         {
            player_entity->attach_to_elevator(entity);
         }


         if (entity->zone__is_zone)
         {
            if (entity->name == "central_column_f1")
            {
               show_location_name("Central Column", "Floor 1");
               if (!game__central_core_cinematic_triggered)
               {
                  start_cinematic_camera("central_core_cinematic");
                  game__central_core_cinematic_triggered = true;
               }
            }
            else if (entity->name == "docking_bay") show_location_name("Docking Bay", "Sub Level");
            else if (entity->name == "library") show_location_name("Library", "Floor 1");
            else if (entity->name == "vr_room") show_location_name("VR Room", "Floor 1");
            else if (entity->name == "zoo") show_location_name("Zoo (Farm)", "Floor 1");

            else if (entity->name == "hydroponics_bay") show_location_name("Hydroponics Bay", "Floor 2");
            else if (entity->name == "medical_bay") show_location_name("Medical Bay", "Floor 2");
            else if (entity->name == "mess_hall") show_location_name("Mess Hall", "Floor 2");
            else if (entity->name == "central_column_f2") show_location_name("Central Column", "Floor 2");

            else if (entity->name == "central_column_armory_f3") show_location_name("Armory", "Floor 3");

            else if (entity->name == "central_column_chryo_f4")
            {
               show_location_name("Cryostasis", "Floor 4");
               if (!get_scripting_as()->get_destruct_sequence_running())
               {
                  event_emitter->emit_play_music_track_event("stasis");
               }
            }

            else if (entity->name == "central_column_f5") show_location_name("Power Coil Room", "Top Floor");

            else
            {
               throw std::runtime_error("!!! Unhandled zone name \"" + entity->name + "\"");
            }
         }
      }

      for (auto &exited : collision_observer.get_exited())
      {
         Krampus24::Gameplay::Entities::Base* entity =
            static_cast<Krampus24::Gameplay::Entities::Base*>(exited);
         entity->on_exit_player_bbox_collision();

         if (entity->elevator_shaft__is_elevator_shaft)
         {
            player_entity->detach_from_elevator();
         }
      }
   }


   //
   // Update the position of the camera
   //

   // player_view_camera
   player_view_camera.position = find_0th_entity()->placement.position;
   player_view_camera.position.y += 2.25;
   player_view_camera.stepout = { 0, 0, 0 }; //AllegroFlare::Vec3D(0, 2.25, 0); // FPS camera
   player_view_camera.spin = find_0th_entity()->player__spin;
   player_view_camera.tilt = find_0th_entity()->player__tilt;


   // Use the player_view_camera as the live_camera
   update_cinematic_camera();
   update_camera_state();
   //live_camera = player_view_camera;
   //update_cinematic_camera();



   //
   // Update player collision events against other object
   // NOTE: This relies on the player_view_camera, so it should be done after updating the camera
   //

   update_inspectable_entity_that_player_is_currently_colliding_with();



   //
   // Evaluate win condition
   //

   if (scripting && scripting->end_state_achieved())
   {
      call_on_finished_callback_func();
   }

   return;
}

void Screen::start_cinematic_camera(std::string cinematic_identifier)
{
   if (!is_camera_state(CAMERA_STATE_PLAYER)) return;

   if (current_playing_cinematic_identifier == "central_core_cinematic")
   {
      // TODO: Play horn thing
   }

   this->current_playing_cinematic_identifier = cinematic_identifier;
   set_camera_state(CAMERA_STATE_CINEMATIC);
   return;
}

void Screen::update_cinematic_camera(double time_now, double delta_time)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::update_cinematic_camera]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::update_cinematic_camera]: error: guard \"initialized\" not met");
   }
   if (!is_camera_state(CAMERA_STATE_CINEMATIC)) return; // DEVELOPMENT
   //Camera3D point_A; 
   if (current_playing_cinematic_identifier == "central_core_cinematic")
   {
      float cinematic_age = infer_current_camera_state_age();
      float duration = 6.0f;
      float normalized_duration = std::min(1.0f, std::max(0.0f, (cinematic_age / duration)));
      dev__float_1 = normalized_duration;

      if (normalized_duration >= 1.0) set_camera_state(CAMERA_STATE_PLAYER);
      else
      {
         float start_spin = -0.9;
         AllegroFlare::Camera3D point_A = player_view_camera;
         point_A.position = {-1, 8, 0};
         point_A.stepout = {0, 9, 0};
         point_A.tilt = 0.0;
         point_A.spin = start_spin + 1.4;

         AllegroFlare::Camera3D point_B;
         point_B.position = {-1, 18, 0};
         //point_B.position = {-1, 32, 0};
         point_B.stepout = {0, 7, 0};
         point_B.tilt = -1.2;
         point_B.spin = start_spin + -0.2;

         //cinematic_camera = point_A;
         float smoothed = AllegroFlare::Interpolators::slow_in_out(normalized_duration);
         blend_camera(&point_A, &point_B, smoothed);

         cinematic_camera = point_A;
      }
   }
   else
   {
      float duration = 2.0f;
      AllegroFlare::Camera3D point_A; 
      point_A.position = {-1, -18, 0};
      cinematic_camera = point_A;
      if (infer_current_camera_state_age() > duration)
      {
         // cinematic is over
      }
   }

   return;
}

void Screen::render()
{
   ALLEGRO_BITMAP *target_bitmap = al_get_target_bitmap();
   live_camera.setup_projection_on(target_bitmap); // TODO: Check if clear color and remove;


   al_clear_to_color(principled_shader.get_fog_color());
   //al_clear_to_color(ALLEGRO_COLOR{0, 0, 0, 1}); // TODO: Check clear to color here



   principled_shader.activate();

   // Draw the visual mesh
   if (rendering_visual_mesh)
   {
      if (visual_mesh)
      {
         visual_mesh->draw();
      }
   }

   // Draw the entities
   if (rendering_entity_models)
   {
      for (auto &entity : entities)
      {
         //entity->draw();
         if (!entity->active) continue;
         if (!entity->visible) continue;

         entity->draw();

         //if (!entity->model) continue;
         //if (entity->texture) entity->model->set_texture(entity->texture);
         //entity->placement.start_transform();
         //entity->model->draw();
         //entity->placement.restore_transform();
      }
   }

   principled_shader.deactivate();

   // TODO: Draw the entities (models?, bounding boxes?)
   if (rendering_entity_bounding_boxes)
   {
      al_set_render_state(ALLEGRO_DEPTH_TEST, 0);
      auto player_entity = find_0th_entity();
      for (auto &entity : entities)
      {
         if (!entity->active || !entity->visible) continue;
         if (entity == player_entity) continue;


         // TODO: Consider that you may wish to draw a box for the model placement(?)
         //// TODO: Remove these lines and have them replaced by entity->draw_aabb3d()
         //std::vector<ALLEGRO_VERTEX> box_line_vertices = entity->build_line_list_vertices();
         //std::vector<ALLEGRO_VERTEX> box_triangle_vertices = entity->build_triangle_list_vertices_for_faces();
         //al_draw_prim(&box_line_vertices[0], nullptr, nullptr, 0, box_line_vertices.size(), ALLEGRO_PRIM_LINE_LIST);
         //al_draw_prim(&box_triangle_vertices[0], nullptr, nullptr, 0, box_triangle_vertices.size(), ALLEGRO_PRIM_TRIANGLE_LIST);


         entity->draw_aabb3d(); // TODO: Have this replace the above legacy code
      }
      al_set_render_state(ALLEGRO_DEPTH_TEST, 1);
   }

   // NOTE: For now, don't clear so that update() (with the legacy classes) has an opportunity to render debug
   // visuals
   if (rendering_collision_wiremesh)
   {
      al_set_render_state(ALLEGRO_DEPTH_TEST, 0);
      if (collision_mesh)
      {
         // TODO: Consider using additive mesh
         collision_mesh->draw(ALLEGRO_COLOR{0.2, 0.2, 0.3, 0.3});
      }
      al_set_render_state(ALLEGRO_DEPTH_TEST, 1);
   }


   hud_camera.setup_dimensional_projection(target_bitmap);

   // TODO: Consider performance implications of clearing the depth buffer here
   al_clear_depth_buffer(1.0);



   // Show the cinematic black bars
   if (showing_cinematic_black_bars)
   {
      current_location_reveal_counter = 0.0; // HACK
      float bar_height = 120;
      al_draw_filled_rectangle(0, 0, 1920, bar_height, ALLEGRO_COLOR{0, 0, 0, 1});
      al_draw_filled_rectangle(0, 1080-bar_height, 1920, 1080, ALLEGRO_COLOR{0, 0, 0, 1});
   }




   // TODO: Consider motion effects for showing/revealing
   if (!get_gameplay_suspended())
   {
      if (showing_inspect_hint)
      {
         if (scripting)
         {
            scripting->draw_inspect_hint(inspect_hint_text);
         }

         // TODO: Consider moving inspect hint to Scripting
         /*
         float o = 0.8;
         al_draw_textf(
            obtain_gameplay_hud_font(),
            ALLEGRO_COLOR{0.0f*o, 1.0f*o, 1.0f*o, 1.0f*o},
            1920/2,
            1080/6 * 5,
            ALLEGRO_ALIGN_CENTER,
            "[E] %s",
            inspect_hint_text.c_str()
         );
         */
      }


      if (current_location_reveal_counter < 5.0)
      {
         //float reveal_opacity = std::max(current_location_reveal_counter
         
         ALLEGRO_FONT *font = obtain_location_font();
         ALLEGRO_FONT *floor_font = obtain_location_floor_font();
         float y = 1080/5 * 4 - 36;
         al_draw_textf(
            font,
            ALLEGRO_COLOR{1.0, 1.0, 1.0, 1.0},
            200, //1920/2,
            y,
            //1920,
            //al_get_font_line_height(font),
            ALLEGRO_ALIGN_LEFT,
            u(current_location_floor).c_str()
         );
         al_draw_textf(
            floor_font,
            ALLEGRO_COLOR{1.0, 1.0, 1.0, 1.0},
            200, //1920/2,
            y + al_get_font_line_height(font),
            //1920,
            //al_get_font_line_height(font),
            ALLEGRO_ALIGN_LEFT,
            u(current_location_name).c_str()
         );

      }


      if (scripting) scripting->render_hud();
   }


   // HERE
   //dev__float_1 = find_0th_entity()->placement.position.x;
   //dev__float_2 = find_0th_entity()->placement.position.y;
   //dev__float_3 = find_0th_entity()->placement.position.z;

   bool drawing_dev_data = false;
   if (drawing_dev_data)
   {
      ALLEGRO_FONT *font = obtain_hud_font();
      float padding = 40;

      al_draw_multiline_textf(
         font,
         ALLEGRO_COLOR{1, 0.65, 0, 1.0},
         padding,
         1080/2,
         1920 - padding * 2,
         al_get_font_line_height(font),
         ALLEGRO_ALIGN_LEFT,
         "str_1: %s\n"
            "float_1: %f\n"
            "float_2: %f\n"
            "float_3: %f\n"
            "float_4: %f\n"
            "bool_1: %i\n",
         dev__str_1.c_str(),
            dev__float_1,
            dev__float_2,
            dev__float_3,
            dev__float_4,
            dev__bool_1
      );
   }


   //ALLEGRO_COLOR col=AllegroFlare::color::azure);
   return;
}

std::string Screen::u(std::string string)
{
   return AllegroFlare::StringTransformer(string).expand(2).upcase().get_text();
   //return AllegroFlare::StringTransformer(string).expand(2).upcase().get_text();
}

void Screen::xxxcall_on_finished_callback_func()
{
   // TODO: Test this callback call
   //if (on_finished_callback_func) on_finished_callback_func(this, on_finished_callback_func_user_data);
}

void Screen::game_event_func(AllegroFlare::GameEvent* game_event)
{
   if (!(game_event))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::game_event_func]: error: guard \"game_event\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::game_event_func]: error: guard \"game_event\" not met");
   }
   if (scripting) scripting->game_event_func(game_event);

   //if (game_event->is_type("trigger_central_core_cinematic"))
   //{
      //start_cinematic_camera("central_core_cinematic");
   //}

   // game_configuration->handle_game_event(game_event);
   return;
}

void Screen::primary_update_func(double time_now, double delta_time)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::primary_update_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::primary_update_func]: error: guard \"initialized\" not met");
   }
   AllegroFlare::Screens::Gameplay::primary_update_func(time_now, delta_time);
   // Update stuff here (take into account delta_time)
   update();
   if (scripting) scripting->update_step(time_now, delta_time);
   return;
}

void Screen::primary_render_func()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::primary_render_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::primary_render_func]: error: guard \"initialized\" not met");
   }
   // Render stuff here
   render();
   return;
}

void Screen::unlock_all_doors()
{
   if (!(scripting))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::unlock_all_doors]: error: guard \"scripting\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::unlock_all_doors]: error: guard \"scripting\" not met");
   }
   auto as = static_cast<Krampus24::Game::Scripting::Tree*>(scripting);
   as->unlock_mega_door("mega_door.001");
   as->unlock_door("door.003");
   as->unlock_door("door.005");
   as->unlock_sliding_door("sliding_door.001");
   as->unlock_door("door.006");
   as->unlock_sliding_door("sliding_door.002");
   as->unlock_door("door.011");
   return;
}

bool Screen::deployment_environment_is_not_production()
{
   return !is_deployment_environment_production;
}

void Screen::key_down_func(ALLEGRO_EVENT* ev)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::key_down_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::key_down_func]: error: guard \"initialized\" not met");
   }
   if (!(ev))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::key_down_func]: error: guard \"ev\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::key_down_func]: error: guard \"ev\" not met");
   }
   AllegroFlare::Screens::Gameplay::key_down_func(ev);


   bool shift = ev->keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT;
   bool ctrl = ev->keyboard.modifiers & ALLEGRO_KEYMOD_COMMAND;


   switch(ev->keyboard.keycode)
   {
      case ALLEGRO_KEY_E: {
         if (!shift) interact_with_focused_inspectable_object();
      } break;
   }


   if (deployment_environment_is_not_production())
   {
      //bool shift = ev->keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT;
      //bool ctrl = ev->keyboard.modifiers & ALLEGRO_KEYMOD_COMMAND;

      //bool keyboard_control_caught = false;
      // This method is DEBUGGING
      switch(ev->keyboard.keycode)
      {
         case ALLEGRO_KEY_W: {
            //toggle_drawing_debug_info();
         } break;

         case ALLEGRO_KEY_A: {
            //toggle_minimap_visibility();
         } break;

         case ALLEGRO_KEY_S: {
            //toggle_minimap_visibility();
         } break;

         case ALLEGRO_KEY_D: {
            //toggle_minimap_visibility();
         } break;

         case ALLEGRO_KEY_J: {
            //live_camera.spin -= ALLEGRO_PI * 0.25;
            //player_spin -= ALLEGRO_PI * 0.25;
         } break;

         case ALLEGRO_KEY_K: {
            //live_camera.spin += ALLEGRO_PI * 0.25;
            //player_spin += ALLEGRO_PI * 0.25;
         } break;

         case ALLEGRO_KEY_U: {
            //live_camera.tilt -= ALLEGRO_PI * 0.125;
         } break;

         case ALLEGRO_KEY_N: {
            //live_camera.tilt += ALLEGRO_PI * 0.125;
         } break;

         // DEVELOPMENT keys

         case ALLEGRO_KEY_0: {
            auto player_entity = find_0th_entity();
            player_entity->placement.position = AllegroFlare::Vec3D(0, 0, 0);
            player_entity->velocity.position = AllegroFlare::Vec3D(0, 0, 0);
         } break;

         case ALLEGRO_KEY_9: {
            auto player_entity = find_0th_entity();
            player_entity->placement.position = player_spawn_position;
            player_entity->velocity.position = AllegroFlare::Vec3D(0, 0, 0);
         } break;

         case ALLEGRO_KEY_L: {
            unlock_all_doors();
         } break;

         case ALLEGRO_KEY_R: {
            if (ctrl)
            {
               std::cout << "> Reload meshes...";
               load_or_reload_meshes();
               std::cout << "done" << std::endl;
            }
         } break;

         case ALLEGRO_KEY_V: {
            rendering_visual_mesh = !rendering_visual_mesh;
         } break;

         case ALLEGRO_KEY_E: {
            if (shift) rendering_entity_bounding_boxes = !rendering_entity_bounding_boxes;
            //else
            //{
               //interact_with_focused_inspectable_object();
            //}
         } break;

         case ALLEGRO_KEY_P: {
            start_cinematic_camera("central_core_cinematic");
         } break;

         case ALLEGRO_KEY_M: {
            rendering_entity_models = !rendering_entity_models;
         } break;

         case ALLEGRO_KEY_X: {
            get_scripting_as()->place_all_animals_in_escape_pod();
         } break;

         case ALLEGRO_KEY_C: {
            rendering_collision_wiremesh = !rendering_collision_wiremesh;
         } break;


         //case ALLEGRO_KEY_W:
         //case ALLEGRO_KEY_UP: {
            //player_up_pressed = true;
         //} break;

         //case ALLEGRO_KEY_A:
         //case ALLEGRO_KEY_LEFT: {
            //player_left_pressed = true;
         //} break;

         //case ALLEGRO_KEY_S:
         //case ALLEGRO_KEY_DOWN: {
            //player_down_pressed = true;
         //} break;

         //case ALLEGRO_KEY_D:
         //case ALLEGRO_KEY_RIGHT: {
            //player_right_pressed = true;
         //} break;

         //case ALLEGRO_KEY_C: {
            //set_player_controlled_entity(find_primary_camera());
         //} break;

         //case ALLEGRO_KEY_Q: {
            //keyboard_control_caught = true;
            //set_player_controlled_entity(find_primary_camera());
         //} break;

         //case ALLEGRO_KEY_SPACE:
         //case ALLEGRO_KEY_E:
         //case ALLEGRO_KEY_I: {
            //if (infer_player_controlled_entity_is_camera())
            //{
               //interact_with_focused_object(); // TODO: Find a way to move this to the controller
               //keyboard_control_caught = true;
            //}
         //} break;

         case ALLEGRO_KEY_ESCAPE: {
            // NOTE: For production, you will want to emit the EVENT_PAUSE_GAME. This will crash during test because
            // there is no router. Externally, the body for on_paused_callback might be:
            //event_emitter->emit_router_event(AllegroFlare::Routers::Standard::EVENT_PAUSE_GAME);
            
            // NOTE: For testing, previously the call_on_finished_callback_func was called:
            // call_on_finished_callback_func();

            //call_on_paused_callback_func();
            //keyboard_control_caught = true;
         } break;

         default: {
            // Nothing here
         } break;
      }
   }


   //if (player_input_controller_exists())
   //{
      // TODO: Consider that keyboard_control_caught might need to be taken into account
      //get_player_input_controller()->key_down_func(ev);
   //}


   return;
}

void Screen::key_up_func(ALLEGRO_EVENT* ev)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::key_up_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::key_up_func]: error: guard \"initialized\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::key_up_func]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::key_up_func]: error: guard \"event_emitter\" not met");
   }
   AllegroFlare::Screens::Gameplay::key_up_func(ev);

   // This method is DEBUGGING
   switch(ev->keyboard.keycode)
   {
      //case ALLEGRO_KEY_UP:
      //case ALLEGRO_KEY_W: {
         //player_up_pressed = false;
      //} break;

      //case ALLEGRO_KEY_LEFT:
      //case ALLEGRO_KEY_A: {
         //player_left_pressed = false;
      //} break;

      //case ALLEGRO_KEY_DOWN:
      //case ALLEGRO_KEY_S: {
         //player_down_pressed = false;
      //} break;

      //case ALLEGRO_KEY_RIGHT:
      //case ALLEGRO_KEY_D: {
         //player_right_pressed = false;
      //} break;

      default: {
         // Nothing here
      } break;
   }

   //if (player_input_controller_exists())
   //{
      // TODO: Consider that keyboard_control_caught might need to be taken into account
      //get_player_input_controller()->key_up_func(ev);
   //}

   return;
}

void Screen::virtual_control_button_up_func(AllegroFlare::Player* player, AllegroFlare::VirtualControllers::Base* virtual_controller, int virtual_controller_button_num, bool is_repeat)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::virtual_control_button_up_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::virtual_control_button_up_func]: error: guard \"initialized\" not met");
   }
   AllegroFlare::Screens::Gameplay::virtual_control_button_up_func(
      player, virtual_controller, virtual_controller_button_num, is_repeat
   );

   // TODO: this function
   return;
}

void Screen::virtual_control_button_down_func(AllegroFlare::Player* player, AllegroFlare::VirtualControllers::Base* virtual_controller, int virtual_controller_button_num, bool is_repeat)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::virtual_control_button_down_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::virtual_control_button_down_func]: error: guard \"initialized\" not met");
   }
   AllegroFlare::Screens::Gameplay::virtual_control_button_down_func(
      player, virtual_controller, virtual_controller_button_num, is_repeat
   );

   // For now, any button will interact
   //interact_with_focused_inspectable_object();

   // TODO: this function
   //call_on_finished_callback_func(); // Consider technique to exit
   return;
}

void Screen::virtual_control_axis_change_func(ALLEGRO_EVENT* ev)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::virtual_control_axis_change_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::virtual_control_axis_change_func]: error: guard \"initialized\" not met");
   }
   // TODO: this function
   return;
}

void Screen::set_camera_state(uint32_t camera_state, bool override_if_busy)
{
   if (!(is_valid_camera_state(camera_state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::set_camera_state]: error: guard \"is_valid_camera_state(camera_state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::set_camera_state]: error: guard \"is_valid_camera_state(camera_state)\" not met");
   }
   if (this->camera_state == camera_state) return;
   if (!override_if_busy && camera_state_is_busy) return;
   uint32_t previous_camera_state = this->camera_state;

   switch (camera_state)
   {
      case CAMERA_STATE_PLAYER:
         showing_cinematic_black_bars = false;
      break;

      case CAMERA_STATE_BLENDING_TO_CINEMATIC:
         showing_cinematic_black_bars = true;
      break;

      case CAMERA_STATE_CINEMATIC:
         showing_cinematic_black_bars = true;
      break;

      case CAMERA_STATE_BLENDING_TO_PLAYER:
         showing_cinematic_black_bars = true;
      break;

      default:
         AllegroFlare::Logger::throw_error(
            "ClassName::set_camera_state",
            "Unable to handle case for camera_state \"" + std::to_string(camera_state) + "\""
         );
      break;
   }

   this->camera_state = camera_state;
   camera_state_changed_at = al_get_time();

   return;
}

void Screen::update_camera_state(float time_now)
{
   if (!(is_valid_camera_state(camera_state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::update_camera_state]: error: guard \"is_valid_camera_state(camera_state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::update_camera_state]: error: guard \"is_valid_camera_state(camera_state)\" not met");
   }
   float age = infer_current_camera_state_age(time_now);

   switch (camera_state)
   {
      case CAMERA_STATE_PLAYER:
         live_camera = player_view_camera;
      break;

      case CAMERA_STATE_BLENDING_TO_CINEMATIC:
      break;

      case CAMERA_STATE_CINEMATIC:
         live_camera = cinematic_camera;
      break;

      case CAMERA_STATE_BLENDING_TO_PLAYER:
      break;

      default:
         AllegroFlare::Logger::throw_error(
            "ClassName::update_camera_state",
            "Unable to handle case for camera_state \"" + std::to_string(camera_state) + "\""
         );
      break;
   }

   return;
}

bool Screen::is_valid_camera_state(uint32_t camera_state)
{
   std::set<uint32_t> valid_camera_states =
   {
      CAMERA_STATE_PLAYER,
      CAMERA_STATE_BLENDING_TO_CINEMATIC,
      CAMERA_STATE_CINEMATIC,
      CAMERA_STATE_BLENDING_TO_PLAYER,
   };
   return (valid_camera_states.count(camera_state) > 0);
}

bool Screen::is_camera_state(uint32_t possible_camera_state)
{
   return (camera_state == possible_camera_state);
}

float Screen::infer_current_camera_state_age(float time_now)
{
   return (time_now - camera_state_changed_at);
}

void Screen::blend_camera(AllegroFlare::Camera3D* source_, AllegroFlare::Camera3D* target_, float mul)
{
   if (!(source_))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::blend_camera]: error: guard \"source_\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::blend_camera]: error: guard \"source_\" not met");
   }
   if (!(target_))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::blend_camera]: error: guard \"target_\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::blend_camera]: error: guard \"target_\" not met");
   }
   //void blend(AllegroFlare::Camera3D* source_, AllegroFlare::Camera3D* target_)
   {
      AllegroFlare::Camera3D &source = *source_;
      AllegroFlare::Camera3D &target = *target_;
      //float mul = 0.1;
      
      //source.position = (source.position - target.position) * mul + source.position;
      source.position = (target.position - source.position) * mul + source.position;
      source.spin = (target.spin - source.spin) * mul + source.spin;
      source.tilt = (target.tilt - source.tilt) * mul + source.tilt;
      source.stepout = (target.stepout - source.stepout) * mul + source.stepout;
      source.zoom = (target.zoom - source.zoom) * mul + source.zoom;
      source.near_plane = (target.near_plane - source.near_plane) * mul + source.near_plane;
      source.far_plane = (target.far_plane - source.far_plane) * mul + source.far_plane;
    }
}

ALLEGRO_FONT* Screen::obtain_gameplay_hud_font()
{
   return font_bin->auto_get("Oswald-Medium.ttf -60");
}

ALLEGRO_FONT* Screen::obtain_hud_font()
{
   return font_bin->auto_get("Oswald-Medium.ttf -52");
}

ALLEGRO_FONT* Screen::obtain_location_font()
{
   return font_bin->auto_get("Michroma-Regular.ttf -48");
}

ALLEGRO_FONT* Screen::obtain_location_floor_font()
{
   return font_bin->auto_get("Michroma-Regular.ttf -32");
}


} // namespace Gameplay
} // namespace Krampus24


