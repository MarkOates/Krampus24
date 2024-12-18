

#include <Krampus24/Gameplay/Entities/SnakeBot.hpp>

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


SnakeBot::SnakeBot()
   : Krampus24::Gameplay::Entities::Base()
   , event_emitter(nullptr)
   , initial_position(AllegroFlare::Vec3D(0, 0, 0))
   , base(nullptr)
   , rotator(nullptr)
   , speed(0.0195f)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
   , style(Krampus24::Gameplay::Entities::SnakeBot::STYLE_NORMAL)
   , uv_offset_x(0.0f)
   , uv_offset_y(0.0f)
   , segment_rotations({})
   , initialized(false)
{
}


SnakeBot::~SnakeBot()
{
   //if (initialized)
   //{
      //AllegroFlare::Logger::info_from(
         //"Krampus24::Gameplay::Entities::SnakeBot",
         //"This class will need to destroy its local copies of 
      //);
   //}
   return;
}


uint32_t SnakeBot::get_state() const
{
   return state;
}


Krampus24::Gameplay::Entities::SnakeBot::Style SnakeBot::get_style() const
{
   return style;
}


float SnakeBot::get_uv_offset_x() const
{
   return uv_offset_x;
}


float SnakeBot::get_uv_offset_y() const
{
   return uv_offset_y;
}


std::vector<float> SnakeBot::get_segment_rotations() const
{
   return segment_rotations;
}


std::vector<Krampus24::Gameplay::Entities::Base*> SnakeBot::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::EventEmitter* event_emitter, AllegroFlare::Vec3D initial_position, float rotation)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SnakeBot::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SnakeBot::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SnakeBot::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SnakeBot::construct]: error: guard \"bitmap_bin\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SnakeBot::construct]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SnakeBot::construct]: error: guard \"event_emitter\" not met");
   }
   // https://www.youtube.com/watch?v=fSMC4tfOqHM&ab_channel=AkiyukiBrickChannel

   // Main entity
   Krampus24::Gameplay::Entities::SnakeBot* result = new Krampus24::Gameplay::Entities::SnakeBot;

   result->affected_by_environmental_forces = false;
   result->collides_with_player = true;
   result->placement.position = initial_position;
   result->placement.position.y += 0.001f; // Move slightly up
   result->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->placement.size = { 8.0, 8.0, 8.0 };
   result->aabb3d.set_max(result->placement.size);
   result->aabb3d_alignment = { 0.5, 0.005, 0.5 }; // Just slightly below the floor
   result->initial_position = initial_position;
   result->placement.rotation.y = rotation;
   //result->player_can_inspect_or_use = true;

   result->base = model_bin->auto_get("snake_bot-01-base.obj");
   result->rotator = model_bin->auto_get("snake_bot-01-rotator.obj");
   result->texture = bitmap_bin->auto_get("entities_texture-01.png");

   /*
   // Left door
   result->door = new Krampus24::Gameplay::Entities::Base;
   result->door->model = model_bin->auto_get("sliding_door-01-door.obj");
   result->door->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->door->affected_by_environmental_forces = false;
   result->door->collides_with_player = false;
   result->door->placement.position = { 0.0, 0.0, 0.0 };
   result->door->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->door->placement.size = { 0, 0, 0 };
   //result->left_door->placement.rotation.y = rotation;
   result->door->visible = false;
   //result->left_door->active = false;
   */

   /*
   result->frame = new Krampus24::Gameplay::Entities::Base;
   result->frame->model = model_bin->auto_get("sliding_door-01-frame.obj");
   result->frame->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->frame->affected_by_environmental_forces = false;
   result->frame->collides_with_player = false;
   result->frame->placement.position = { 0.0, 0.0, 0.0 };
   result->frame->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->frame->placement.size = { 0, 0, 0 };
   //result->left_door->placement.rotation.y = rotation;
   result->frame->visible = false;
   */

   // Preload the samples
   //result->sample_bin = sample_bin;
   //sample_bin->preload(DOOR_OPEN_SAMPLE_IDENTIFIER);
   result->event_emitter = event_emitter;


   result->initialized = true;
   result->set_state(STATE_CLOSED);

   return { result }; //, result->door, result->frame }; //, result->right_door };
}

bool SnakeBot::on_player_inspect_or_use()
{
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

void SnakeBot::set_style(Krampus24::Gameplay::Entities::SnakeBot::Style style)
{
   this->style = style;
   std::tie(uv_offset_x, uv_offset_y) = get_uv_offset_from_style(style);
   return;
}

void SnakeBot::set_uv_offset_x(float uv_offset_x)
{
   this->style = STYLE_USER_CUSTOM_DEFINED_UV;
   this->uv_offset_x = uv_offset_x;
   return;
}

void SnakeBot::set_uv_offset_y(float uv_offset_y)
{
   this->style = STYLE_USER_CUSTOM_DEFINED_UV;
   this->uv_offset_y = uv_offset_y;
   return;
}

std::pair<float, float> SnakeBot::get_uv_offset_from_style(Krampus24::Gameplay::Entities::SnakeBot::Style style)
{
   if (!((style != STYLE_UNDEF)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SnakeBot::get_uv_offset_from_style]: error: guard \"(style != STYLE_UNDEF)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SnakeBot::get_uv_offset_from_style]: error: guard \"(style != STYLE_UNDEF)\" not met");
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

void SnakeBot::draw()
{
   int num_segments = 3;
   //float segment_distance = 2;
   float segment_distance = 1.25;
   float segment_axis_rotation = 25.0 * ALLEGRO_PI / 180; // * 2 * 0.125;

   placement.start_transform();

   //std::pair<float, float> uv_offset = get_uv_offset_from_style(style);//STYLE_NORMAL_DISRUPTED);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", uv_offset.first);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", uv_offset.second);
   //ALLEGRO_BITMAP *texture = texture;

   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", uv_offset_x);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", uv_offset_y);

   base->set_texture(texture);
   rotator->set_texture(texture);

   ALLEGRO_TRANSFORM initial_transform = *al_get_current_transform();
   ALLEGRO_TRANSFORM change_transform;

   ALLEGRO_TRANSFORM transform;
   ALLEGRO_TRANSFORM compound_transform;
   al_identity_transform(&transform);
   al_identity_transform(&compound_transform);
   //ALLEGRO_TRANSFORM next_start_transform;
   al_identity_transform(&change_transform);
   //al_rotate_transform_3d(&change_transform, 0.0, 1.0, 0.0, al_get_time() * 0.15);
      //al_rotate_transform_3d(&change_transform, 1.0, 0.0, 0.0, segment_axis_rotation);
   al_rotate_transform_3d(&change_transform, 1.0, 0.0, 0.0, segment_axis_rotation);
   al_translate_transform_3d(&change_transform, 0.0, segment_distance, 0.0);
   //al_rotate_transform_3d(&change_transform, 0.0, 0.0, 1.0, segment_axis_rotation);
   //al_translate_transform_3d(&change_transform, 0.0, segment_distance, 0.0);
   //al_rotate_transform_3d(&change_transform, 0.0, 0.0, 1.0, segment_axis_rotation);
   //al_rotate_transform_3d(&change_transform, 0.0, 1.0, 0.0, al_get_time() * 0.15);
   //al_translate_transform_3d(&change_transform, 0.0, segment_distance, 0.0);
   //al_identity_transform(&for_use_transform);


   float rate = 0.175;



   float base_rotation = al_get_time() * 0.175 * 2;



   { // Segment 1
      al_identity_transform(&transform);
      {
         // Previous
      }
      al_use_transform(&transform);
      base->draw();
   }



   {
      // segment 1 rotator
      al_identity_transform(&transform);
      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, base_rotation);
      {
      }
      al_use_transform(&transform);
      rotator->draw();
   }




   { // Segment 2
      // segment 2 base

      al_identity_transform(&transform);
      {
         al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
         //al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
         al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, base_rotation);
         al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
      }
      al_use_transform(&transform);
      base->draw();
   }




   {
      // segment 2 rotator

      al_identity_transform(&transform);
      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);

      {
         al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
         //al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
         al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, base_rotation);
         al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
      }
      al_use_transform(&transform);
      rotator->draw();
   }




   { // Segment 3
      // segment 3 base

      al_identity_transform(&transform);

      al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
      al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
      {
         al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
         //al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
         al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, base_rotation);
         al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
      }
      al_use_transform(&transform);
      base->draw();


   }
   {


      // segment 3 rotator

      al_identity_transform(&transform);

      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);

      al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
      al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
      {
         al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
         //al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
         al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, base_rotation);
         al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
      }
      al_use_transform(&transform);
      rotator->draw();
   }





   { // Segment 4
      // segment 4 base

      al_identity_transform(&transform);

      al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
      al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
      {
         al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
         al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
         al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
         {
            al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
            //al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
            al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, base_rotation);
            al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
         }
      }
      al_use_transform(&transform);
      base->draw();

   }



   {
      // segment 4 rotator

      al_identity_transform(&transform);

      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);

      al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
      al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
      {
         al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
         al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
         al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
         {
            al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
            //al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
            al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, base_rotation);
            al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
         }
      }
      al_use_transform(&transform);
      rotator->draw();
   }




   { // Segment 5
      // segment 4 base

      al_identity_transform(&transform);

      al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
      al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
      {
         al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
         al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
         al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
         {
            al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
            al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
            al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
            {
               al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
               //al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
               al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, base_rotation);
               al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
            }
         }
      }
      al_use_transform(&transform);
      base->draw();
   }



   {
      // segment 4 rotator

      al_identity_transform(&transform);

      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);

      al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
      al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
      al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
      {
         al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
         al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
         al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
         {
            al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
            al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
            al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
            {
               al_rotate_transform_3d(&transform, 1.0, 0.0, 0.0, segment_axis_rotation);
               //al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, al_get_time() * rate);
               al_rotate_transform_3d(&transform, 0.0, 1.0, 0.0, base_rotation);
               al_translate_transform_3d(&transform, 0.0, segment_distance, 0.0);
            }
         }
      }
      al_use_transform(&transform);
      rotator->draw();
   }






      //al_compose_transform(&compound_transform, &change_transform);



   /*
   //for (int i=0; i<1; i++)
   for (int i=0; i<num_segments; i++)
   //for (int i=0; i<num_segments; i++)
   {
      // Buid our offsetting/rotating transform
      al_use_transform(&compound_transform);
      base->draw();
      //al_rotate_transform_3d(&change_transform, 0.0, 1.0, 0.0, 0 * 0.15);
      ////al_rotate_transform_3d(&compound_transform, 0.0, 1.0, 0.0, al_get_time() * 0.15);
      //al_copy_transform(&for_use_transform, &change_transform);
      //al_compose_transform(&for_use_transform, &initial_transform);
      al_use_transform(&compound_transform);
      rotator->draw();

      al_compose_transform(&compound_transform, &change_transform);
      //al_use_transform(&compound_transform);
      //base->draw();

      //al_rotate_transform_3d(&change_transform, 0.0, 1.0, 0.0, al_get_time() * 0.15);
      //al_identity_transform(&transform);
      //al_rotate_transform_3d(&change_transform, 0.0, 0.0, 1.0, segment_axis_rotation);
      //al_translate_transform_3d(&change_transform, 0.0, segment_distance, 0.0);
      //al_rotate_transform_3d(&change_transform, 0.0, 0.0, 1.0, segment_axis_rotation);
      //al_rotate_transform_3d(&change_transform, 1.0, 0.0, 0.0, segment_axis_rotation);
      //al_use_transform(&change_transform);
      //al_compose_transform(&transform, &initial_transform);
      //al_use_transform(&change_transform);
   }
   */

   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", 0.0);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", 0.0);

   placement.restore_transform();
   return;
}

void SnakeBot::on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   //attempt_to_open();
   //set_state(STATE_OPENING);
   //attempt_to_open
   return;
}

void SnakeBot::on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   //attempt_to_close();
   set_state(STATE_CLOSING);
   return;
}

void SnakeBot::on_time_step(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SnakeBot::on_time_step]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SnakeBot::on_time_step]: error: guard \"initialized\" not met");
   }
   update_state(time_step, time_now);
   return;
}

void SnakeBot::play_open_door_sound_effect()
{
   //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
   //event_emitter->emit_play_sound_effect_event("open_metal_door");
   return;
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> SnakeBot::build_audio_controller_sound_effect_list()
{
   std::map<std::string, AllegroFlare::AudioRepositoryElement> sound_effect_elements = {
      { "open_metal_door", { "door-01-opening.ogg", false, "restart" } },
   };
   return sound_effect_elements;
}

void SnakeBot::set_state(uint32_t state, bool override_if_busy)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SnakeBot::set_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SnakeBot::set_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SnakeBot::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SnakeBot::set_state]: error: guard \"is_valid_state(state)\" not met");
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
         //set_open_position(1.0f);
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
      } break;

      case STATE_CLOSING: {
         //play_open_door_sound_effect();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
         //set_state(STATE_CLOSED);
      } break;

      case STATE_CLOSED: {
         //set_open_position(0.0f);
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

void SnakeBot::update_state(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SnakeBot::update_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SnakeBot::update_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SnakeBot::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SnakeBot::update_state]: error: guard \"is_valid_state(state)\" not met");
   }
   float age = infer_current_state_age(time_now);

   switch (state)
   {
      case STATE_OPENING: {
         //set_open_position(open_position + speed);
         //if (open_position >= 1.0) set_state(STATE_OPEN);
      } break;

      case STATE_OPEN: {
      } break;

      case STATE_CLOSING: {
         //set_open_position(open_position - speed);
         //if (open_position <= 0.0) set_state(STATE_CLOSED);
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

bool SnakeBot::is_valid_state(uint32_t state)
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

bool SnakeBot::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float SnakeBot::infer_current_state_age(float time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SnakeBot::infer_current_state_age]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SnakeBot::infer_current_state_age]: error: guard \"initialized\" not met");
   }
   return (time_now - state_changed_at);
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


