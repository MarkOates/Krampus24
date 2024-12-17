

#include <Krampus24/Gameplay/Entities/PlayerShip.hpp>

#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Shaders/Base.hpp>
#include <AllegroFlare/Vec2D.hpp>
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


PlayerShip::PlayerShip()
   : Krampus24::Gameplay::Entities::Base()
   , event_emitter(nullptr)
   , collision_mesh(nullptr)
   , initial_position(AllegroFlare::Vec3D(0, 0, 0))
   , body(nullptr)
   , right_door(nullptr)
   , dynamic_collision_mesh_face_names({})
   , open_position(0.0f)
   , speed(0.0165f)
   , locked(false)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
   , style(Krampus24::Gameplay::Entities::PlayerShip::STYLE_NORMAL)
   , uv_offset_x(0.0f)
   , uv_offset_y(0.0f)
   , initialized(false)
{
}


PlayerShip::~PlayerShip()
{
   //if (initialized)
   //{
      //AllegroFlare::Logger::info_from(
         //"Krampus24::Gameplay::Entities::PlayerShip",
         //"This class will need to destroy its local copies of 
      //);
   //}
   return;
}


uint32_t PlayerShip::get_state() const
{
   return state;
}


Krampus24::Gameplay::Entities::PlayerShip::Style PlayerShip::get_style() const
{
   return style;
}


float PlayerShip::get_uv_offset_x() const
{
   return uv_offset_x;
}


float PlayerShip::get_uv_offset_y() const
{
   return uv_offset_y;
}


bool PlayerShip::valid_rotation(float rotation)
{
   return true;
   if (rotation == 0.0) return true;
   if (rotation == -0.25) return true;
   if (rotation == -0.5) return true;
   if (rotation == -0.75) return true;
   if (rotation == -1.0) return true;
   if (rotation == 0.25) return true;
   if (rotation == 0.5) return true;
   if (rotation == 0.75) return true;
   if (rotation == 1.0) return true;
   return false;
}

void PlayerShip::transform_model(AllegroFlare::Model3D* model, ALLEGRO_TRANSFORM* transform)
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

std::vector<Krampus24::Gameplay::Entities::Base*> PlayerShip::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::EventEmitter* event_emitter, AllegroFlare::Physics::CollisionMesh* collision_mesh, std::string name_for_collision_faces, AllegroFlare::Vec3D initial_position, float rotation)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::construct]: error: guard \"bitmap_bin\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::construct]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::construct]: error: guard \"event_emitter\" not met");
   }
   if (!(collision_mesh))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::construct]: error: guard \"collision_mesh\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::construct]: error: guard \"collision_mesh\" not met");
   }
   if (!(valid_rotation(rotation)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::construct]: error: guard \"valid_rotation(rotation)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::construct]: error: guard \"valid_rotation(rotation)\" not met");
   }

   // Main entity
   Krampus24::Gameplay::Entities::PlayerShip* result = new Krampus24::Gameplay::Entities::PlayerShip;
   //result->model = model_bin->auto_get("door-01.obj");
   //result->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->affected_by_environmental_forces = false;
   result->collides_with_player = true;
   result->placement.position = initial_position;
   result->player_can_inspect_or_use = true;
   //result->placement.position.y += 0.001f; // Move slightly up 
   result->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->placement.size = { 10.0, 0.5, 10.0 };
   result->aabb3d.set_max(result->placement.size);
   result->aabb3d_alignment = { 0.5, 0.005, 0.5 }; // Just slightly below the floor
   result->initial_position = initial_position;
   result->placement.rotation.y = rotation;

   // Body
   result->body = new Krampus24::Gameplay::Entities::Base;
   result->body->model = model_bin->auto_get("player_ship-01-body.obj");
   result->body->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->body->affected_by_environmental_forces = false;
   result->body->collides_with_player = false;
   result->body->placement.position = { 0.0, 0.0, 0.0 };
   result->body->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->body->placement.size = { 0, 0, 0 };
   //result->left_door->placement.rotation.y = rotation;
   result->body->visible = false;
   //result->left_door->active = false;

   // Right door
   result->right_door = new Krampus24::Gameplay::Entities::Base;
   result->right_door->model = model_bin->auto_get("door-03-right_door.obj");
   result->right_door->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->right_door->affected_by_environmental_forces = false;
   result->right_door->collides_with_player = false;
   result->right_door->placement.position = { 0.0, 0.0, 0.0 };
   result->right_door->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->right_door->placement.size = { 0, 0, 0 };
   //result->right_door->placement.rotation.y = rotation;
   result->right_door->visible = false;
   //result->right_door->active = false;


   //type: std::pair<std::vector<std::string>, std::vector<AllegroFlare::Physics::CollisionMeshFace*>>
   // Load the collision mesh
   //std::vector<std::string> dynamic_face_names; // This is simply discarded
   //std::vector<AllegroFlare::Physics::CollisionMeshFace*> dynamic_faces;


   result->collision_mesh = collision_mesh;

   std::string collision_mesh_name = "player_ship-01-collision_mesh.obj";
   AllegroFlare::Model3D *mesh = model_bin->auto_get(collision_mesh_name);
   //mesh->displace(result->placement.position);
   ALLEGRO_TRANSFORM placement_transform;
   result->placement.build_transform(&placement_transform);
   transform_model(mesh, &placement_transform);
   result->dynamic_collision_mesh_face_names =
      collision_mesh->load_dynamic_faces(
         name_for_collision_faces, //"mydoor",
         mesh
      );
   model_bin->destroy(collision_mesh_name);



   // Preload the samples
   //result->sample_bin = sample_bin;
   //sample_bin->preload(DOOR_OPEN_SAMPLE_IDENTIFIER);
   result->event_emitter = event_emitter;


   result->initialized = true;
   result->set_state(STATE_CLOSED);

   return { result, result->body, result->right_door };
}

void PlayerShip::lock()
{
   locked = true;
   return;
}

void PlayerShip::unlock()
{
   locked = false;
   return;
}

void PlayerShip::set_style(Krampus24::Gameplay::Entities::PlayerShip::Style style)
{
   this->style = style;
   std::tie(uv_offset_x, uv_offset_y) = get_uv_offset_from_style(style);
   return;
}

void PlayerShip::set_uv_offset_x(float uv_offset_x)
{
   this->style = STYLE_USER_CUSTOM_DEFINED_UV;
   this->uv_offset_x = uv_offset_x;
   return;
}

void PlayerShip::set_uv_offset_y(float uv_offset_y)
{
   this->style = STYLE_USER_CUSTOM_DEFINED_UV;
   this->uv_offset_y = uv_offset_y;
   return;
}

std::pair<float, float> PlayerShip::get_uv_offset_from_style(Krampus24::Gameplay::Entities::PlayerShip::Style style)
{
   if (!((style != STYLE_UNDEF)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::get_uv_offset_from_style]: error: guard \"(style != STYLE_UNDEF)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::get_uv_offset_from_style]: error: guard \"(style != STYLE_UNDEF)\" not met");
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

void PlayerShip::activate_collision_mesh()
{
   if (!(collision_mesh))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::activate_collision_mesh]: error: guard \"collision_mesh\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::activate_collision_mesh]: error: guard \"collision_mesh\" not met");
   }
   for (auto &face_name : dynamic_collision_mesh_face_names)
   {
      collision_mesh->activate_dynamic_face(face_name);
   }
   return;
}

void PlayerShip::deactivate_collision_mesh()
{
   if (!(collision_mesh))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::deactivate_collision_mesh]: error: guard \"collision_mesh\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::deactivate_collision_mesh]: error: guard \"collision_mesh\" not met");
   }
   for (auto &face_name : dynamic_collision_mesh_face_names)
   {
      collision_mesh->deactivate_dynamic_face(face_name);
   }
   return;
}

void PlayerShip::draw()
{
   placement.start_transform();

   //std::pair<float, float> uv_offset = get_uv_offset_from_style(style);//STYLE_NORMAL_DISRUPTED);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", uv_offset.first);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", uv_offset.second);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", uv_offset_x);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", uv_offset_y);

   //right_door->placement.start_transform();
   //right_door->model->set_texture(right_door->texture);
   //right_door->model->draw();
   //right_door->placement.restore_transform();

   //body->placement.start_transform();
   body->model->set_texture(body->texture);
   body->model->draw();
   //body->placement.restore_transform();

   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", 0.0);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", 0.0);

   placement.restore_transform();
   return;
}

void PlayerShip::set_open_position(float open_position)
{
   open_position = std::max(std::min(1.0f, open_position), 0.0f);
   this->open_position = open_position;
   //body->placement.position.z = open_position * 2;
   right_door->placement.position.z = -open_position * 2;
   return;
}

void PlayerShip::on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   if (!locked) set_state(STATE_OPENING);
   //else event_emitter->emit_activate_dialog_node_by_name_event("locked_door");
   return;
}

void PlayerShip::on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   if (!locked) set_state(STATE_CLOSING);
   //else event_emitter->emit_activate_dialog_node_by_name_event("locked_door");
   return;
}

void PlayerShip::on_time_step(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::on_time_step]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::on_time_step]: error: guard \"initialized\" not met");
   }
   update_state(time_step, time_now);
   return;
}

void PlayerShip::play_open_door_sound_effect()
{
   //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
   //event_emitter->emit_play_sound_effect_event("open_metal_door");
   return;
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> PlayerShip::build_audio_controller_sound_effect_list()
{
   std::map<std::string, AllegroFlare::AudioRepositoryElement> sound_effect_elements = {
      { "open_metal_door", { "door-01-opening.ogg", false, "restart" } },
   };
   return sound_effect_elements;
}

void PlayerShip::set_state(uint32_t state, bool override_if_busy)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::set_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::set_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::set_state]: error: guard \"is_valid_state(state)\" not met");
   }
   if (this->state == state) return;
   if (!override_if_busy && state_is_busy) return;
   uint32_t previous_state = this->state;

   this->state = state;
   state_changed_at = al_get_time();

   switch (state)
   {
      case STATE_OPENING: {
         //play_open_door_sound_effect();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
         //set_state(STATE_OPEN);
      } break;

      case STATE_OPEN: {
         set_open_position(1.0f);
         //deactivate_collision_mesh();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
      } break;

      case STATE_CLOSING: {
         //play_open_door_sound_effect();
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

void PlayerShip::update_state(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::update_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::update_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::update_state]: error: guard \"is_valid_state(state)\" not met");
   }
   float age = infer_current_state_age(time_now);

   //float flip_speed = 4.0f;
   //float flipper = fmod(al_get_time(), flip_speed);
   //if (flipper < flip_speed/2) activate_collision_mesh();
   //else deactivate_collision_mesh();

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

bool PlayerShip::is_valid_state(uint32_t state)
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

bool PlayerShip::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float PlayerShip::infer_current_state_age(float time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::PlayerShip::infer_current_state_age]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::PlayerShip::infer_current_state_age]: error: guard \"initialized\" not met");
   }
   return (time_now - state_changed_at);
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


