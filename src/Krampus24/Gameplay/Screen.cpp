

#include <Krampus24/Gameplay/Screen.hpp>

#include <AllegroFlare/Camera3D.hpp>
#include <AllegroFlare/EventNames.hpp>
#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Physics/CollisionMeshCollisionStepper.hpp>
#include <AllegroFlare/PlayerInputControllers/Generic.hpp>
#include <AllegroFlare/RouteEventDatas/ActivateScreenByIdentifier.hpp>
#include <AllegroFlare/Routers/Standard.hpp>
#include <Krampus24/BlenderBlockingLoader.hpp>
#include <Krampus24/Game/Scripting/Tree.hpp>
#include <Krampus24/Gameplay/Entities/Door.hpp>
#include <Krampus24/Gameplay/Entities/Hen.hpp>
#include <Krampus24/Gameplay/Entities/Turret.hpp>
#include <Krampus24/Gameplay/PlayerInputControllers/Player.hpp>
#include <Krampus24/Gameplay/Scripting/Empty.hpp>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>


namespace Krampus24
{
namespace Gameplay
{


Screen::Screen(AllegroFlare::EventEmitter* event_emitter, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::FontBin* font_bin, AllegroFlare::ModelBin* model_bin, AllegroFlare::GameConfigurations::Base* game_configuration, std::vector<Krampus24::Gameplay::Entities::Base*> entities, AllegroFlare::Physics::CollisionMesh* collision_mesh, std::string collision_mesh_identifier, std::string visual_mesh_identifier, std::string visual_mesh_texture_identifier, std::string blocking_filename)
   : AllegroFlare::Screens::Gameplay()
   , data_folder_path("[unset-data_folder_path]")
   , audio_controller(nullptr)
   , event_emitter(event_emitter)
   , bitmap_bin(bitmap_bin)
   , font_bin(font_bin)
   , model_bin(model_bin)
   , game_configuration(game_configuration)
   , hud_camera({})
   , live_camera({})
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
   , principled_shader({})
   , rendering_visual_mesh(true)
   , rendering_collision_wiremesh(false)
   , rendering_entity_models(true)
   , rendering_entity_bounding_boxes(false)
   , showing_inspect_hint(false)
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


void Screen::set_live_camera(AllegroFlare::Camera3D live_camera)
{
   this->live_camera = live_camera;
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


AllegroFlare::FontBin* Screen::get_font_bin() const
{
   return font_bin;
}


AllegroFlare::GameConfigurations::Base* Screen::get_game_configuration() const
{
   return game_configuration;
}


AllegroFlare::Camera2D Screen::get_hud_camera() const
{
   return hud_camera;
}


AllegroFlare::Camera3D Screen::get_live_camera() const
{
   return live_camera;
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
   live_camera.near_plane = 0.1;
   live_camera.far_plane = 500.0;

   //live_camera.stepout = AllegroFlare::Vec3D(0, 0.25, 10); // Third person
   //live_camera.tilt = 0.8;

   // Set this levels entities to the scripting logic
   //Krampus24::Game::Scripting::Tree *tree_scripting = new Krampus24::Game::Scripting::Tree;
   //tree_scripting.set_entities(&entities);
   //tree_scripting.set_collision_observer(&collision_observer);
   //tree_scripting.set_font_bin(font_bin);
   //tree_scripting.initialize();
   //scripting = tree_scripting;

   // Load level and entities
   load_or_reload_meshes();


   // Create a player input controller for the 0th entity
   create_and_set_player_input_controller_for_0th_entity();

   principled_shader.initialize();
   principled_shader.set_fog_color(al_color_html("#31687a"));
   principled_shader.set_fog_intensity(0.9);
   principled_shader.set_fog_distance(40.0);

   principled_shader.set_world_tint(ALLEGRO_COLOR{0.3, 0.3, 0.4, 1.0});

   initialized = true;
   return;
}

std::vector<Krampus24::Gameplay::Entities::Base*> Screen::build_entity(Krampus24::BlenderBlockingLoaderEntity* entity)
{
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Screen::build_entity]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Screen::build_entity]: error: guard \"entity\" not met");
   }

   float x = entity->location.x;
   float y = entity->location.z; // Swapping z<->y
   float z = entity->location.y; // Swapping z<->y

   AllegroFlare::Vec3D position = AllegroFlare::Vec3D(x, y, z);

   std::string entity_root_name = entity->get_name_unversioned();
   if (entity_root_name == Krampus24::Gameplay::Entities::Hen::BLENDER_IDENTIFIER)
   {
      //std::cout << "HEN made" << std::endl;
      auto *result = Krampus24::Gameplay::Entities::Hen::construct(model_bin, bitmap_bin, position, 6.0);
      result->name = entity->name;
      return { result };
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::Turret::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      auto *result = Krampus24::Gameplay::Entities::Turret::construct(model_bin, bitmap_bin, position, rotation);
      result->name = entity->name;
      return { result };
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::Door::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      std::vector<Krampus24::Gameplay::Entities::Base*> results = Krampus24::Gameplay::Entities::Door::construct(
         model_bin,
         bitmap_bin,
         event_emitter,
         position,
         rotation
      );
      results[0]->name = entity->name;
      return results;
   }

   //AllegroFlare::Vec3D position;
   bool affected_by_environmental_forces = true;


   Krampus24::Gameplay::Entities::Base* result = new Krampus24::Gameplay::Entities::Base();

   //float x = entity->location.x;
   //float y = entity->location.z; // Swapping z<->y
   //float z = entity->location.y; // Swapping z<->y

   //AllegroFlare::Vec3D position = AllegroFlare::Vec3D(x, y, z);

   result->placement.position = position;
   result->placement.size = { 0.5, 0.5, 0.5 };
   result->aabb3d.set_max(result->placement.size);
   result->collides_with_player = true;
   result->affected_by_environmental_forces = affected_by_environmental_forces;

   result->name = entity->name;
   //std::string entity_root_name = entity->get_name_unversioned();
   if (entity_root_name == "elevator")
   {
      // Do elevator stuff
      result->placement.size = { 4.0, 4.0, 4.0 };
      result->aabb3d.set_max(result->placement.size);
      result->aabb3d_alignment = { 0.5, 0.005, 0.5 };
      result->box_color = ALLEGRO_COLOR{ 1.0, 1.0, 0.4, 1.0 };
   }

   return { result };
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

      // HERE
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
         }
      }
      else
      {
         std::vector<Krampus24::Gameplay::Entities::Base*> result_entities = build_entity(entity);
         for (auto &result_entity : result_entities)
         {
            entities.push_back(result_entity);
         }
      }
   });


   // Load up the sound effects // DEVELOPMENT
   // TODO: Move this to another more appropriate location
   audio_controller->set_and_load_sound_effect_elements(
      Krampus24::Gameplay::Entities::Door::build_audio_controller_sound_effect_list()
   );


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

void Screen::interact_with_focused_inspectable_object()
{
   if (inspectable_entity_that_player_is_currently_colliding_with)
   {
      // Perform the entities local reaction to inspection
      bool entity_scoped_inspection_occurred =
         inspectable_entity_that_player_is_currently_colliding_with->on_player_inspect_or_use();

      // TODO: Perform the scripting's inspection logic
      bool scripting_scoped_inspection_occurred = false;
      if (!entity_scoped_inspection_occurred)
      {
         //scripting->interact_with_focused_object(inspectable_entity_that_player_is_currently_colliding_with);
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

void Screen::show_inspect_hint()
{
   showing_inspect_hint = true;
   return;
}

void Screen::update_inspectable_entity_that_player_is_currently_colliding_with()
{
   auto player_entity = find_0th_entity();
   if (!player_entity) throw std::runtime_error("asjiodfasjdiofajsdiofasjdifoajsdiofjadsoi");

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
         found_colliding_entity = entity;
         break;
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
         show_inspect_hint();
      }
   }
   return;
}

void Screen::update()
{
   float time_now = al_get_time();
   float step_duration = 1.0f;


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
   // Observe change in entity<->entity collisions
   //

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
   }

   for (auto &exited : collision_observer.get_exited())
   {
      Krampus24::Gameplay::Entities::Base* entity =
         static_cast<Krampus24::Gameplay::Entities::Base*>(exited);
      // HERE
      entity->on_exit_player_bbox_collision();
   }

   //
   // Update the position of the camera
   //

   live_camera.position = find_0th_entity()->placement.position;
   live_camera.spin = find_0th_entity()->player__spin;
   live_camera.tilt = find_0th_entity()->player__tilt;



   //
   // Update player collision events against other object
   //

   update_inspectable_entity_that_player_is_currently_colliding_with();



   //
   // Evaluate win condition
   //

   if (scripting->end_state_achieved())
   {
      call_on_finished_callback_func();
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

   if (showing_inspect_hint)
   {
      al_draw_textf(
         obtain_hud_font(),
         ALLEGRO_COLOR{1, 0.65, 0, 1.0},
         1920-300,
         1080/2,
         ALLEGRO_ALIGN_CENTER,
         "[E] Inspect"
      );

   }


   if (scripting) scripting->render_hud();
   /*
   if (power_cell_collected)
   {
      al_draw_textf(
         obtain_hud_font(),
         ALLEGRO_COLOR{1, 0.65, 0, 1.0},
         40,
         30,
         ALLEGRO_ALIGN_LEFT,
         "PRIMARY POWER COIL COLLECTED",
      );
   }
   */


   //ALLEGRO_COLOR col=AllegroFlare::color::azure);
   return;
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
         else
         {
            interact_with_focused_inspectable_object();
         }
      } break;

      case ALLEGRO_KEY_M: {
         rendering_entity_models = !rendering_entity_models;
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

         call_on_paused_callback_func();
         //keyboard_control_caught = true;
      } break;

      default: {
         // Nothing here
      } break;
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

ALLEGRO_FONT* Screen::obtain_hud_font()
{
   return font_bin->auto_get("Oswald-Medium.ttf -52");
}


} // namespace Gameplay
} // namespace Krampus24


