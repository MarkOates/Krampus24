

#include <Krampus24/Gameplay/Entities/ElevatorShaft.hpp>

#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Shaders/Base.hpp>
#include <AllegroFlare/Useful.hpp>
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


ElevatorShaft::ElevatorShaft()
   : Krampus24::Gameplay::Entities::Base()
   , event_emitter(nullptr)
   , collision_mesh(nullptr)
   , initial_position(AllegroFlare::Vec3D(0, 0, 0))
   , shaft_bottom_cap(nullptr)
   , shaft_column(nullptr)
   , car(nullptr)
   , car_body_dynamic_collision_mesh_face_names({})
   , car_door_dynamic_collision_mesh_face_names({})
   , elevation_position(0.0f)
   , num_tiers(4.0f)
   , speed((0.00165f * 3))
   , locked(false)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
   , style(Krampus24::Gameplay::Entities::ElevatorShaft::STYLE_NORMAL)
   , uv_offset_x(0.0f)
   , uv_offset_y(0.0f)
   , initialized(false)
{
}


ElevatorShaft::~ElevatorShaft()
{
   //if (initialized)
   //{
      //AllegroFlare::Logger::info_from(
         //"Krampus24::Gameplay::Entities::Door",
         //"This class will need to destroy its local copies of 
      //);
   //}
   return;
}


uint32_t ElevatorShaft::get_state() const
{
   return state;
}


Krampus24::Gameplay::Entities::ElevatorShaft::Style ElevatorShaft::get_style() const
{
   return style;
}


float ElevatorShaft::get_uv_offset_x() const
{
   return uv_offset_x;
}


float ElevatorShaft::get_uv_offset_y() const
{
   return uv_offset_y;
}


void ElevatorShaft::transform_model(AllegroFlare::Model3D* model, ALLEGRO_TRANSFORM* transform)
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

std::vector<Krampus24::Gameplay::Entities::Base*> ElevatorShaft::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::EventEmitter* event_emitter, AllegroFlare::Physics::CollisionMesh* collision_mesh, std::string name_for_collision_faces, AllegroFlare::Vec3D initial_position, float rotation)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::construct]: error: guard \"bitmap_bin\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::construct]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::construct]: error: guard \"event_emitter\" not met");
   }
   if (!(collision_mesh))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::construct]: error: guard \"collision_mesh\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::construct]: error: guard \"collision_mesh\" not met");
   }

   // Main entity
   Krampus24::Gameplay::Entities::ElevatorShaft* result = new Krampus24::Gameplay::Entities::ElevatorShaft;
   //result->model = model_bin->auto_get("door-01.obj");
   //result->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->affected_by_environmental_forces = false;
   result->collides_with_player = true; // DEVELOPMENT
   result->placement.position = initial_position;
   //result->placement.position.y += 0.001f; // Move slightly up 
   result->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->placement.size = { 4.0, 4.0, 4.0 };
   result->set_num_tiers(4.0); // Needed to set the placement size
   result->aabb3d.set_max(result->placement.size);
   result->aabb3d_alignment = { 0.5, 0.005, 0.5 }; // Just slightly below the floor
   result->initial_position = initial_position;
   result->placement.rotation.y = rotation;
   result->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->elevator_shaft__is_elevator_shaft = true;


   // Shaft
   result->shaft_bottom_cap = model_bin->auto_get("elevator_shaft-03-shaft_bottom_cap.obj");
   result->shaft_column = model_bin->auto_get("elevator_shaft-03-shaft_column.obj");
   result->car = model_bin->auto_get("elevator_shaft-03-car.obj");


   result->player_can_inspect_or_use = true;


   //type: std::pair<std::vector<std::string>, std::vector<AllegroFlare::Physics::CollisionMeshFace*>>
   // Load the collision mesh
   //std::vector<std::string> dynamic_face_names; // This is simply discarded
   //std::vector<AllegroFlare::Physics::CollisionMeshFace*> dynamic_faces;


   // Car collision mesh
   result->collision_mesh = collision_mesh;


   {
      std::string collision_mesh_name = "elevator_shaft-03-collision_mesh-car.obj";
      AllegroFlare::Model3D *mesh = model_bin->auto_get(collision_mesh_name);
      //mesh->displace(result->placement.position);
      ALLEGRO_TRANSFORM placement_transform;
      result->placement.build_transform(&placement_transform);
      transform_model(mesh, &placement_transform);
      result->car_body_dynamic_collision_mesh_face_names =
         collision_mesh->load_dynamic_faces(
            name_for_collision_faces, //"mydoor",
            mesh
         );
      model_bin->destroy(collision_mesh_name);
   }



   //result->collision_mesh = collision_mesh;

   {
      std::string collision_mesh_name = "elevator_shaft-03-collision_mesh-car_door.obj";
      AllegroFlare::Model3D *mesh = model_bin->auto_get(collision_mesh_name);
      //mesh->displace(result->placement.position);
      ALLEGRO_TRANSFORM placement_transform;
      result->placement.build_transform(&placement_transform);
      transform_model(mesh, &placement_transform);
      result->car_door_dynamic_collision_mesh_face_names =
         collision_mesh->load_dynamic_faces(
            name_for_collision_faces + std::string("-door-"), //"mydoor",
            mesh
         );
      model_bin->destroy(collision_mesh_name);
   }



   // Preload the samples
   //result->sample_bin = sample_bin;
   //sample_bin->preload(DOOR_OPEN_SAMPLE_IDENTIFIER);
   result->event_emitter = event_emitter;


   result->initialized = true;
   result->set_state(STATE_AT_BOTTOM);


   result->player_can_inspect_or_use__custom_look_at_logic = [](
         Krampus24::Gameplay::Entities::Base* self,
         AllegroFlare::Vec3D *player_position,
         AllegroFlare::Vec3D *player_view_direction
   ) -> bool {
      // TODO: Figure out this logic
      AllegroFlare::Vec2D self_flat_position(
         self->placement.position.x,
         self->placement.position.z
      );
      AllegroFlare::Vec2D player_flat_position(
         player_position->x,
         player_position->z
      );

      float player_distance_from_center =
         AllegroFlare::manhattan_distance(&self_flat_position, &player_flat_position);
      if (player_distance_from_center < 1.5) return true;

      return false;
   };

   return { result }; //, result->right_door };
}

void ElevatorShaft::set_num_tiers(float num_tiers)
{
   if (!((num_tiers >= 2)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::set_num_tiers]: error: guard \"(num_tiers >= 2)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::set_num_tiers]: error: guard \"(num_tiers >= 2)\" not met");
   }
   if (!((num_tiers <= 6)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::set_num_tiers]: error: guard \"(num_tiers <= 6)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::set_num_tiers]: error: guard \"(num_tiers <= 6)\" not met");
   }
   this->num_tiers = num_tiers;
   placement.size.y = 4.0f*num_tiers;
   aabb3d.set_max(placement.size);
   return;
}

bool ElevatorShaft::attempt_to_move_elevator_up()
{
   if (locked) return false;
   if (!is_state(STATE_AT_BOTTOM)) return false;
   set_state(STATE_GOING_UP);
   return true;
}

bool ElevatorShaft::attempt_to_move_elevator_down()
{
   if (locked) return false;
   if (!is_state(STATE_AT_TOP)) return false;
   set_state(STATE_GOING_DOWN);
   return true;
}

bool ElevatorShaft::on_player_inspect_or_use()
{
   if (is_state(STATE_AT_TOP))
   {
      attempt_to_move_elevator_down();
      //return true;
   }
   else if (is_state(STATE_AT_BOTTOM))
   {
      attempt_to_move_elevator_up();
      //attempt_to_open();
      //return true;
   }
   return false;
}

void ElevatorShaft::lock()
{
   locked = true;
   return;
}

void ElevatorShaft::unlock()
{
   locked = false;
   return;
}

void ElevatorShaft::set_style(Krampus24::Gameplay::Entities::ElevatorShaft::Style style)
{
   this->style = style;
   std::tie(uv_offset_x, uv_offset_y) = get_uv_offset_from_style(style);
   return;
}

void ElevatorShaft::set_uv_offset_x(float uv_offset_x)
{
   this->style = STYLE_USER_CUSTOM_DEFINED_UV;
   this->uv_offset_x = uv_offset_x;
   return;
}

void ElevatorShaft::set_uv_offset_y(float uv_offset_y)
{
   this->style = STYLE_USER_CUSTOM_DEFINED_UV;
   this->uv_offset_y = uv_offset_y;
   return;
}

std::pair<float, float> ElevatorShaft::get_uv_offset_from_style(Krampus24::Gameplay::Entities::ElevatorShaft::Style style)
{
   if (!((style != STYLE_UNDEF)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::get_uv_offset_from_style]: error: guard \"(style != STYLE_UNDEF)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::get_uv_offset_from_style]: error: guard \"(style != STYLE_UNDEF)\" not met");
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

void ElevatorShaft::activate_collision_mesh()
{
   if (!(collision_mesh))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::activate_collision_mesh]: error: guard \"collision_mesh\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::activate_collision_mesh]: error: guard \"collision_mesh\" not met");
   }
   for (auto &face_name : car_door_dynamic_collision_mesh_face_names)
   {
      collision_mesh->activate_dynamic_face(face_name);
   }
   return;
}

void ElevatorShaft::deactivate_collision_mesh()
{
   if (!(collision_mesh))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::deactivate_collision_mesh]: error: guard \"collision_mesh\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::deactivate_collision_mesh]: error: guard \"collision_mesh\" not met");
   }
   for (auto &face_name : car_door_dynamic_collision_mesh_face_names)
   {
      collision_mesh->deactivate_dynamic_face(face_name);
   }
   return;
}

void ElevatorShaft::draw()
{
   ALLEGRO_TRANSFORM initial_transform;

   shaft_bottom_cap->set_texture(texture);
   shaft_column->set_texture(texture);

   al_copy_transform(&initial_transform, al_get_current_transform());

   //placement.start_transform();
   ALLEGRO_TRANSFORM base_transform;
   placement.build_transform(&base_transform);

   AllegroFlare::Shaders::Base::set_float("uv_offset_x", uv_offset_x);
   AllegroFlare::Shaders::Base::set_float("uv_offset_y", uv_offset_y);


   al_use_transform(&base_transform);
   //shaft_bottom_cap->set_texture(texture);
   shaft_bottom_cap->draw();
   //shaft_column->set_texture(texture);


   // TODO: Assemble this into a single mesh
   ///*
   const float shaft_column_height = 4;
   ALLEGRO_TRANSFORM composite_shaft_column_transform;
   ALLEGRO_TRANSFORM shaft_column_transform;
   al_identity_transform(&shaft_column_transform);
   int num_reps = (int)num_tiers;
   //shaft_column->set_texture(texture);

   // Draw the columns
   shaft_column->draw();
   for (int i=0; i<(num_reps-1); i++)
   {
      al_translate_transform_3d(&shaft_column_transform, 0, shaft_column_height, 0);
      al_copy_transform(&composite_shaft_column_transform, &shaft_column_transform);
      al_compose_transform(&composite_shaft_column_transform, &base_transform);

      al_use_transform(&composite_shaft_column_transform);

      //al_use_transform(&base_transform);
      shaft_column->draw();
      //al_translate_transform_3d(&shaft_column_transform, 0, shaft_column_height, 0);
   }
   //*/


   ALLEGRO_TRANSFORM car_transform;
   al_identity_transform(&car_transform);
   al_translate_transform_3d(&car_transform, 0, calculate_local_elevator_car_y_position(), 0);
   al_compose_transform(&car_transform, &base_transform);
   al_use_transform(&car_transform);

   car->set_texture(texture);
   car->draw();


   AllegroFlare::Shaders::Base::set_float("uv_offset_x", 0.0);
   AllegroFlare::Shaders::Base::set_float("uv_offset_y", 0.0);

   al_use_transform(&initial_transform);

   //placement.restore_transform();
   return;
}

float ElevatorShaft::calculate_local_elevator_car_y_position()
{
   return elevation_position * 4 * (num_tiers-1);
}

float ElevatorShaft::calculate_global_elevator_car_y_position()
{
   return placement.position.y + elevation_position * 4.0f * (num_tiers-1);
}

void ElevatorShaft::set_elevation_position(float elevation_position)
{
   elevation_position = std::max(std::min(1.0f, elevation_position), 0.0f);
   this->elevation_position = elevation_position;

   //elevation_position 
   //left_door->placement.position.y = open_position * 2.9;

   //left_door->placement.position.y = open_position * 2.9;
   //right_door->placement.position.y = -open_position * 2.9;
   return;
}

void ElevatorShaft::on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   //if (!locked) set_state(STATE_GOING_UP);
   //else event_emitter->emit_activate_dialog_node_by_name_event("locked_door");
   return;
}

void ElevatorShaft::on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   //if (!locked) set_state(STATE_GOING_DOWN);
   //else event_emitter->emit_activate_dialog_node_by_name_event("locked_door");
   return;
}

void ElevatorShaft::on_time_step(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::on_time_step]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::on_time_step]: error: guard \"initialized\" not met");
   }
   update_state(time_step, time_now);
   return;
}

void ElevatorShaft::play_open_door_sound_effect()
{
   //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
   //event_emitter->emit_play_sound_effect_event("open_metal_door");
   return;
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> ElevatorShaft::build_audio_controller_sound_effect_list()
{
   std::map<std::string, AllegroFlare::AudioRepositoryElement> sound_effect_elements = {
      { "open_metal_door", { "door-01-opening.ogg", false, "restart" } },
   };
   return sound_effect_elements;
}

void ElevatorShaft::set_state(uint32_t state, bool override_if_busy)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::set_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::set_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::set_state]: error: guard \"is_valid_state(state)\" not met");
   }
   if (this->state == state) return;
   if (!override_if_busy && state_is_busy) return;
   uint32_t previous_state = this->state;

   this->state = state;
   state_changed_at = al_get_time();

   switch (state)
   {
      case STATE_GOING_UP: {
         activate_collision_mesh();
         //play_open_door_sound_effect();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
         //set_state(STATE_OPEN);
      } break;

      case STATE_AT_TOP: {
         set_elevation_position(1.0f);
         deactivate_collision_mesh();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
      } break;

      case STATE_GOING_DOWN: {
         activate_collision_mesh();
         //play_open_door_sound_effect();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
         //set_state(STATE_AT_BOTTOM):
      } break;

      case STATE_AT_BOTTOM: {
         set_elevation_position(0.0f);
         deactivate_collision_mesh();
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

void ElevatorShaft::update_state(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::update_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::update_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::update_state]: error: guard \"is_valid_state(state)\" not met");
   }
   float age = infer_current_state_age(time_now);

   //float flip_speed = 4.0f;
   //float flipper = fmod(al_get_time(), flip_speed);
   //if (flipper < flip_speed/2) activate_collision_mesh();
   //else deactivate_collision_mesh();


   switch (state)
   {
      case STATE_GOING_UP: {
         set_elevation_position(elevation_position + speed);
         if (elevation_position >= 1.0) set_state(STATE_AT_TOP);
      } break;

      case STATE_AT_TOP: {
      } break;

      case STATE_GOING_DOWN: {
         set_elevation_position(elevation_position - speed);
         if (elevation_position <= 0.0) set_state(STATE_AT_BOTTOM);
      } break;

      case STATE_AT_BOTTOM: {
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

bool ElevatorShaft::is_valid_state(uint32_t state)
{
   std::set<uint32_t> valid_states =
   {
      STATE_GOING_UP,
      STATE_AT_TOP,
      STATE_GOING_DOWN,
      STATE_AT_BOTTOM
   };
   return (valid_states.count(state) > 0);
}

bool ElevatorShaft::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float ElevatorShaft::infer_current_state_age(float time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::ElevatorShaft::infer_current_state_age]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::ElevatorShaft::infer_current_state_age]: error: guard \"initialized\" not met");
   }
   return (time_now - state_changed_at);
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


