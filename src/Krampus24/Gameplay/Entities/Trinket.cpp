

#include <Krampus24/Gameplay/Entities/Trinket.hpp>

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


Trinket::Trinket()
   : Krampus24::Gameplay::Entities::Base()
   , event_emitter(nullptr)
   , initial_position(AllegroFlare::Vec3D(0, 0, 0))
   , door(nullptr)
   , frame(nullptr)
   , open_position(0.0f)
   , speed(0.0195f)
   , locked(false)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
   , style(Krampus24::Gameplay::Entities::Trinket::STYLE_NORMAL)
   , uv_offset_x(0.0f)
   , uv_offset_y(0.0f)
   , initialized(false)
{
}


Trinket::~Trinket()
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


uint32_t Trinket::get_state() const
{
   return state;
}


Krampus24::Gameplay::Entities::Trinket::Style Trinket::get_style() const
{
   return style;
}


float Trinket::get_uv_offset_x() const
{
   return uv_offset_x;
}


float Trinket::get_uv_offset_y() const
{
   return uv_offset_y;
}


void Trinket::transform_model(AllegroFlare::Model3D* model, ALLEGRO_TRANSFORM* transform)
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

float Trinket::get_random_rotation()
{
   static AllegroFlare::Random random;
   static std::vector<float> rotations = { 0.0, 0.25, 0.5, 0.75 };
   static std::vector<float> offsets = { -0.125/2, 0.125/2 };
   return random.get_random_element<float>(rotations) + random.get_random_element<float>(offsets);
}

std::vector<Krampus24::Gameplay::Entities::Base*> Trinket::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::EventEmitter* event_emitter, AllegroFlare::Vec3D initial_position, float rotation)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Trinket::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Trinket::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Trinket::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Trinket::construct]: error: guard \"bitmap_bin\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Trinket::construct]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Trinket::construct]: error: guard \"event_emitter\" not met");
   }
   // Main entity
   Krampus24::Gameplay::Entities::Trinket* result = new Krampus24::Gameplay::Entities::Trinket;
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
   result->placement.rotation.y = get_random_rotation();
   result->player_can_inspect_or_use = true;
   result->locked = false;

   // Left door
   result->door = new Krampus24::Gameplay::Entities::Base;
   result->door->model = model_bin->auto_get("tablet-01-body.obj");
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
   result->set_state(STATE_CLOSED);

   return { result, result->door }; //, result->frame }; //, result->right_door };
}

void Trinket::unlock()
{
   locked = false;
   return;
}

void Trinket::lock()
{
   locked = true;
   return;
}

void Trinket::attempt_to_open()
{
   if (locked)
   {
      // TODO: Show or indicate in some way that the door is locked
   }
   else
   {
      set_state(STATE_OPENING);
   }
   return;
}

void Trinket::attempt_to_close()
{
   if (locked)
   {
      // TODO: Show or indicate in some way that the door is locked
   }
   else
   {
      set_state(STATE_CLOSING);
   }
   return;
}

bool Trinket::on_player_inspect_or_use()
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

void Trinket::set_style(Krampus24::Gameplay::Entities::Trinket::Style style)
{
   this->style = style;
   std::tie(uv_offset_x, uv_offset_y) = get_uv_offset_from_style(style);
   return;
}

void Trinket::set_uv_offset_x(float uv_offset_x)
{
   this->style = STYLE_USER_CUSTOM_DEFINED_UV;
   this->uv_offset_x = uv_offset_x;
   return;
}

void Trinket::set_uv_offset_y(float uv_offset_y)
{
   this->style = STYLE_USER_CUSTOM_DEFINED_UV;
   this->uv_offset_y = uv_offset_y;
   return;
}

std::pair<float, float> Trinket::get_uv_offset_from_style(Krampus24::Gameplay::Entities::Trinket::Style style)
{
   if (!((style != STYLE_UNDEF)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Trinket::get_uv_offset_from_style]: error: guard \"(style != STYLE_UNDEF)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Trinket::get_uv_offset_from_style]: error: guard \"(style != STYLE_UNDEF)\" not met");
   }
   switch (style)
   {
      case STYLE_NORMAL: return { 0, 0 }; break;

      case STYLE_NORMAL_DISRUPTED: return { 0.05, 0.0 }; break;

      case STYLE_BARN: return { 0.2, 0.2 }; break;

      case STYLE_FIRE: return { 0.25, 0.20 }; break; // NOTE: This one is not really practical, just for debugging

      default:
         AllegroFlare::Logger::throw_error(
            "ClassName::get_uv_offset_from_style",
            "Unable to handle case for style \"" + std::to_string(style) + "\""
         );
      break;
   }

   AllegroFlare::Logger::throw_error(
      "ClassName::get_uv_offset_from_style",
      "Unexpected code path to here (code: 678sdf678sdf678sdf)"
   );
   return { 0, 0 };
}

void Trinket::draw()
{
   placement.start_transform();

   ALLEGRO_COLOR color = al_color_name("dodgerblue");
   AllegroFlare::Shaders::Base::set_vec3("color_lift", color.r, color.g, color.b);
   AllegroFlare::Shaders::Base::set_float("color_lift_intensity", 0.18);
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

void Trinket::set_open_position(float open_position)
{
   open_position = std::max(std::min(1.0f, open_position), 0.0f);
   this->open_position = open_position;
   door->placement.position.z = open_position * 2;
   //right_door->placement.position.z = -open_position * 2;
   return;
}

void Trinket::on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   //attempt_to_open();
   //set_state(STATE_OPENING);
   //attempt_to_open
   return;
}

void Trinket::on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   //attempt_to_close();
   //set_state(STATE_CLOSING);
   return;
}

void Trinket::on_time_step(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Trinket::on_time_step]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Trinket::on_time_step]: error: guard \"initialized\" not met");
   }
   update_state(time_step, time_now);
   return;
}

void Trinket::play_open_door_sound_effect()
{
   //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
   event_emitter->emit_play_sound_effect_event("open_metal_door");
   return;
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> Trinket::build_audio_controller_sound_effect_list()
{
   std::map<std::string, AllegroFlare::AudioRepositoryElement> sound_effect_elements = {
      { "open_metal_door", { "door-01-opening.ogg", false, "restart" } },
   };
   return sound_effect_elements;
}

void Trinket::set_state(uint32_t state, bool override_if_busy)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Trinket::set_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Trinket::set_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Trinket::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Trinket::set_state]: error: guard \"is_valid_state(state)\" not met");
   }
   if (this->state == state) return;
   if (!override_if_busy && state_is_busy) return;
   uint32_t previous_state = this->state;

   this->state = state;
   state_changed_at = al_get_time();

   switch (state)
   {
      case STATE_OPENING: {
         play_open_door_sound_effect();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
         //set_state(STATE_OPEN);
      } break;

      case STATE_OPEN: {
         set_open_position(1.0f);
         //deactivate_collision_mesh();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
      } break;

      case STATE_CLOSING: {
         play_open_door_sound_effect();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
         //set_state(STATE_CLOSED);
      } break;

      case STATE_CLOSED: {
         set_open_position(0.0f);
         //activate_collision_mesh();
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

void Trinket::update_state(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Trinket::update_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Trinket::update_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Trinket::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Trinket::update_state]: error: guard \"is_valid_state(state)\" not met");
   }
   float age = infer_current_state_age(time_now);

   switch (state)
   {
      case STATE_OPENING: {
         set_open_position(open_position + speed);
         if (open_position >= 1.0) set_state(STATE_OPEN);
      } break;

      case STATE_OPEN: {
      } break;

      case STATE_CLOSING: {
         set_open_position(open_position - speed);
         if (open_position <= 0.0) set_state(STATE_CLOSED);
      } break;

      case STATE_CLOSED: {
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

bool Trinket::is_valid_state(uint32_t state)
{
   std::set<uint32_t> valid_states =
   {
      STATE_OPENING,
      STATE_OPEN,
      STATE_CLOSING,
      STATE_CLOSED
   };
   return (valid_states.count(state) > 0);
}

bool Trinket::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float Trinket::infer_current_state_age(float time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Trinket::infer_current_state_age]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Trinket::infer_current_state_age]: error: guard \"initialized\" not met");
   }
   return (time_now - state_changed_at);
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


