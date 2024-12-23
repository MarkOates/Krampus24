

#include <Krampus24/Gameplay/Entities/Prop.hpp>

#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Random.hpp>
#include <AllegroFlare/Shaders/Base.hpp>
#include <AllegroFlare/Vec2D.hpp>
#include <allegro5/allegro_color.h>
#include <cmath>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Gameplay
{
namespace Entities
{


Prop::Prop()
   : Krampus24::Gameplay::Entities::Base()
   , event_emitter(nullptr)
   , model_bin(nullptr)
   , initial_position(AllegroFlare::Vec3D(0, 0, 0))
   , door(nullptr)
   , frame(nullptr)
   , speed(0.0195f)
   , locked(false)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
   , trinket_type(Krampus24::Gameplay::Entities::Prop::PROP_TYPE_UNDEF)
   , lift_color(ALLEGRO_COLOR{1, 1, 1, 1})
   , lift_color_intensity(0.0f)
   , uv_offset_x(0.0f)
   , uv_offset_y(0.0f)
   , initialized(false)
{
}


Prop::~Prop()
{
   //if (initialized)
   //{
      //AllegroFlare::Logger::info_from(
         //"Krampus24::Gameplay::Entities::Console",
         //"This class will need to destroy its local copies of 
      //);
   //}
   return;
}


uint32_t Prop::get_state() const
{
   return state;
}


Krampus24::Gameplay::Entities::Prop::PropType Prop::get_trinket_type() const
{
   return trinket_type;
}


ALLEGRO_COLOR Prop::get_lift_color() const
{
   return lift_color;
}


float Prop::get_lift_color_intensity() const
{
   return lift_color_intensity;
}


float Prop::get_uv_offset_x() const
{
   return uv_offset_x;
}


float Prop::get_uv_offset_y() const
{
   return uv_offset_y;
}


void Prop::transform_model(AllegroFlare::Model3D* model, ALLEGRO_TRANSFORM* transform)
{
   //validate_initialized_or_output_to_cerr("transform");
   //validate_not_vertex_buffer("transform");
   ALLEGRO_TRANSFORM normal_transform;
   al_copy_transform(&normal_transform, transform);
   normal_transform.m[3][0] = 0.0f;
   normal_transform.m[3][1] = 0.0f;
   normal_transform.m[3][2] = 0.0f;

   // TODO: Test this
   auto &vertices = model->vertices;
   for (unsigned i=0; i<vertices.size(); i++)
   {
      al_transform_coordinates_3d(
         transform,
         &vertices[i].x,
         &vertices[i].y,
         &vertices[i].z
      );

      // Also transform the normal:
      al_transform_coordinates_3d(
         &normal_transform,
         &vertices[i].nx,
         &vertices[i].ny,
         &vertices[i].nz
     );

      AllegroFlare::Vec3D to_be_normalized =
         AllegroFlare::Vec3D(vertices[i].nx, vertices[i].ny, vertices[i].nz).normalized();

      vertices[i].nx = to_be_normalized.x;
      vertices[i].ny = to_be_normalized.y;
      vertices[i].nz = to_be_normalized.z;
   }
   return;
}

float Prop::get_random_rotation()
{
   static AllegroFlare::Random random;
   static std::vector<float> rotations = { 0.0, 0.25, 0.5, 0.75 };
   static std::vector<float> offsets = { -0.125/2, 0.125/2 };
   return random.get_random_element<float>(rotations) + random.get_random_element<float>(offsets);
}

Krampus24::Gameplay::Entities::Base* Prop::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::EventEmitter* event_emitter, AllegroFlare::Vec3D initial_position, float rotation)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Prop::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Prop::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Prop::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Prop::construct]: error: guard \"bitmap_bin\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Prop::construct]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Prop::construct]: error: guard \"event_emitter\" not met");
   }
   // Main entity
   Krampus24::Gameplay::Entities::Prop* result = new Krampus24::Gameplay::Entities::Prop;
   //result->model = model_bin->auto_get("door-01.obj");
   //result->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->affected_by_environmental_forces = true;
   result->collides_with_player = true;
   result->placement.position = initial_position;
   //result->placement.position.y += 0.001f; // Move slightly up
   result->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense

   result->placement.size = { 3.0, 0.5, 3.0 };
   result->aabb3d.set_max(result->placement.size);
   result->aabb3d_alignment = { 0.5, 0.0, 0.5 }; // Just slightly below the floor
   result->initial_position = initial_position;
   result->placement.rotation.y = rotation;
   result->player_can_inspect_or_use = true;
   result->locked = false;



   result->model_bin = model_bin;


   // TODO: Load all the models for the trinket types:
   model_bin->preload("tablet-01-body.obj");
   //model_bin->preload("tablet-01-body.obj");
   //model_bin->preload("tablet-01-body.obj");
   //model_bin->preload("tablet-01-body.obj");




   // Left door
   result->door = new Krampus24::Gameplay::Entities::Base;
   result->door->model = model_bin->auto_get("trinket-02-medal_of_honor.obj");
   //result->door->model = model_bin->auto_get("tablet-01-body.obj");
   //result->door->model = model_bin->auto_get("trinket-medal_of_honor-01-body.obj");
   result->door->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->door->affected_by_environmental_forces = false;
   result->door->collides_with_player = false;
   result->door->placement.position = { 0.0, 0.0, 0.0 };
   result->door->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->door->placement.size = { 0, 0, 0 };
   //result->left_door->placement.rotation.y = rotation;
   result->door->visible = false;
   //result->left_door->active = false;

   //result->frame = new Krampus24::Gameplay::Entities::Base;
   //result->frame->model = model_bin->auto_get("console-01-frame.obj");
   //result->frame->texture = bitmap_bin->auto_get("entities_texture-01.png");
   //result->frame->affected_by_environmental_forces = false;
   //result->frame->collides_with_player = false;
   //result->frame->placement.position = { 0.0, 0.0, 0.0 };
   //result->frame->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   //result->frame->placement.size = { 0, 0, 0 };
   //result->left_door->placement.rotation.y = rotation;
   //result->frame->visible = false;

   // Load our collision mesh for a dynamically blocking door when locked


   // Load all the models for the trinket types

   /*
   result->collision_mesh = collision_mesh;
   std::string collision_mesh_name = "console-01-collision_mesh.obj";
   AllegroFlare::Model3D *mesh = model_bin->auto_get(collision_mesh_name);
   ALLEGRO_TRANSFORM placement_transform;
   result->placement.build_transform(&placement_transform);
   transform_model(mesh, &placement_transform);
   result->dynamic_collision_mesh_face_names =
      collision_mesh->load_dynamic_faces(
         name_for_collision_faces, //"mydoor",
         mesh
      );
   model_bin->destroy(collision_mesh_name);
   */





   /*
   // Right door
   result->right_door = new Krampus24::Gameplay::Entities::Base;
   result->right_door->model = model_bin->auto_get("sliding_door-01-door.obj");
   result->right_door->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->right_door->affected_by_environmental_forces = false;
   result->right_door->collides_with_player = false;
   result->right_door->placement.position = { 0.0, 0.0, 0.0 };
   result->right_door->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->right_door->placement.size = { 0, 0, 0 };
   //result->right_door->placement.rotation.y = rotation;
   result->right_door->visible = false;
   //result->right_door->active = false;
   */

   // Preload the samples
   //result->sample_bin = sample_bin;
   //sample_bin->preload(DOOR_OPEN_SAMPLE_IDENTIFIER);
   result->event_emitter = event_emitter;


   result->initialized = true;
   result->set_state(STATE_IDLE);
   result->set_trinket_type(PROP_TYPE_TABLE); // NOTE: The default

   return result; //, result->frame }; //, result->right_door };
}

void Prop::unlock()
{
   locked = false;
   return;
}

void Prop::lock()
{
   locked = true;
   return;
}

bool Prop::on_player_inspect_or_use()
{
   // This will spawn a dialog, but the dialog depends on what was assigned to it
   //if (is_state(STATE_OPEN) || is_state(STATE_OPENING))
   //{
      //attempt_to_close();
      //return true;
   //}
   //else if (is_state(STATE_CLOSED) || is_state(STATE_CLOSING))
   //{
      //attempt_to_open();
      //return true;
   //}
   return false;
}

void Prop::set_trinket_type(Krampus24::Gameplay::Entities::Prop::PropType trinket_type)
{
   this->trinket_type = trinket_type;
   switch (trinket_type)
   {
      case PROP_TYPE_CAUTION_FLOOR: {
         door->model = model_bin->auto_get("assorted_props-01-caution_floor.obj");
         lift_color = al_color_name("yellow");
         lift_color_intensity = 0.18;
      }; break;

      case PROP_TYPE_TABLE: {
         door->model = model_bin->auto_get("assorted_props-01-table.obj");
         lift_color = al_color_name("white");
         lift_color_intensity = 0.18;
      }; break;

      /*
      case PROP_TYPE_MEDAL_OF_HONOR: {
         //door->model = model_bin->auto_get("trinket-medal_of_honor-01-body.obj");
         door->model = model_bin->auto_get("trinket-02-medal_of_honor.obj");
         lift_color = al_color_name("yellow");
         lift_color_intensity = 0.1;
      }; break;

      case PROP_TYPE_CARNATIONS: {
         //door->model = model_bin->auto_get("trinket-medal_of_honor-01-body.obj");
         door->model = model_bin->auto_get("trinket-02-carnations.obj");
         lift_color = al_color_name("white");
         lift_color_intensity = 0.1;
      }; break;

      case PROP_TYPE_TEDDY_BEAR: {
         //door->model = model_bin->auto_get("trinket-medal_of_honor-01-body.obj");
         door->model = model_bin->auto_get("trinket-02-teddy_bear.obj");
         lift_color = al_color_name("sienna");
         lift_color_intensity = 0.1;
      }; break;

      case PROP_TYPE_FAMILY_PHOTOS: {
         //door->model = model_bin->auto_get("trinket-medal_of_honor-01-body.obj");
         door->model = model_bin->auto_get("trinket-02-family_photos.obj");
         lift_color = al_color_name("mistyrose");
         lift_color_intensity = 0.1;
      }; break;
      */

      default: {
         throw std::runtime_error("Error: UNKNOWN PROP TYPE");
      }; break;
   }
   return;
}

void Prop::draw()
{
   placement.start_transform();

   ALLEGRO_COLOR color = lift_color; // al_color_name("dodgerblue");
   float intensity = lift_color_intensity; // 0.18;

   AllegroFlare::Shaders::Base::set_vec3("color_lift", color.r, color.g, color.b);
   AllegroFlare::Shaders::Base::set_float("color_lift_intensity", intensity);
   AllegroFlare::Shaders::Base::set_int("color_lift_blend_mode", 2);



   //AllegroFlare::Shaders::Base::set_float("color_lift", al_color_name("lightpink"));
   //AllegroFlare::Shaders::Base::set_float("color_lift_intensity", 0.0);

   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", uv_offset_x);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", uv_offset_y);

   //door->placement.start_transform();
   door->model->set_texture(door->texture);
   door->model->draw();
   //door->placement.restore_transform();

   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", 0.0);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", 0.0);
   AllegroFlare::Shaders::Base::set_int("color_lift_blend_mode", 0);
   AllegroFlare::Shaders::Base::set_float("color_lift_intensity", 0.0);

   placement.restore_transform();
   return;
}

void Prop::on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   //attempt_to_open();
   //set_state(STATE_OPENING);
   //attempt_to_open
   return;
}

void Prop::on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   //attempt_to_close();
   //set_state(STATE_CLOSING);
   return;
}

void Prop::on_time_step(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Prop::on_time_step]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Prop::on_time_step]: error: guard \"initialized\" not met");
   }
   update_state(time_step, time_now);
   return;
}

void Prop::play_open_door_sound_effect()
{
   //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
   event_emitter->emit_play_sound_effect_event("open_metal_door");
   return;
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> Prop::build_audio_controller_sound_effect_list()
{
   std::map<std::string, AllegroFlare::AudioRepositoryElement> sound_effect_elements = {
      { "open_metal_door", { "door-01-opening.ogg", false, "restart" } },
   };
   return sound_effect_elements;
}

void Prop::set_state(uint32_t state, bool override_if_busy)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Prop::set_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Prop::set_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Prop::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Prop::set_state]: error: guard \"is_valid_state(state)\" not met");
   }
   if (this->state == state) return;
   if (!override_if_busy && state_is_busy) return;
   uint32_t previous_state = this->state;

   this->state = state;
   state_changed_at = al_get_time();

   switch (state)
   {
      //case STATE_OPENING: {
         //play_open_door_sound_effect();
         ////sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
         //set_state(STATE_OPEN);
      //} break;

      //case STATE_CLOSING: {
         //play_open_door_sound_effect();
         ////sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
         ////set_state(STATE_CLOSED);
      //} break;

      //case STATE_CLOSED: {
         ////set_open_position(0.0f);
         ////activate_collision_mesh();
      //} break;

      case STATE_IDLE: {
      } break;

      default:
         AllegroFlare::Logger::throw_error(
            "ClassName::set_state",
            "Unable to handle case for state \"" + std::to_string(state) + "\""
         );
      break;
   }

   return;
}

void Prop::update_state(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Prop::update_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Prop::update_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Prop::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Prop::update_state]: error: guard \"is_valid_state(state)\" not met");
   }
   float age = infer_current_state_age(time_now);

   switch (state)
   {
      case STATE_IDLE: {
      } break;

      default:
         AllegroFlare::Logger::throw_error(
            "ClassName::update_state",
            "Unable to handle case for state \"" + std::to_string(state) + "\""
         );
      break;
   }

   return;
}

bool Prop::is_valid_state(uint32_t state)
{
   std::set<uint32_t> valid_states =
   {
      //STATE_OPENING,
      //STATE_OPEN,
      //STATE_CLOSING,
      STATE_IDLE
   };
   return (valid_states.count(state) > 0);
}

bool Prop::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float Prop::infer_current_state_age(float time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Prop::infer_current_state_age]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Prop::infer_current_state_age]: error: guard \"initialized\" not met");
   }
   return (time_now - state_changed_at);
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


