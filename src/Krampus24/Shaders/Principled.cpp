

#include <Krampus24/Shaders/Principled.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Shaders
{


Principled::Principled()
   : AllegroFlare::Shaders::Base(Krampus24::Shaders::Principled::TYPE, obtain_vertex_source(), obtain_fragment_source())
   , world_tint(ALLEGRO_COLOR{1, 1, 1, 1})
   , fog_color(ALLEGRO_COLOR{1, 1, 1, 1})
   , fog_intensity(0.4f)
   , fog_distance(20.0f)
   , uv_offset_x(0.4f)
   , uv_offset_y(20.0f)
   , camera_far_plane(100.0f)
   , initialized(false)
{
}


Principled::~Principled()
{
}


void Principled::set_world_tint(ALLEGRO_COLOR world_tint)
{
   this->world_tint = world_tint;
}


void Principled::set_fog_color(ALLEGRO_COLOR fog_color)
{
   this->fog_color = fog_color;
}


void Principled::set_fog_intensity(float fog_intensity)
{
   this->fog_intensity = fog_intensity;
}


void Principled::set_fog_distance(float fog_distance)
{
   this->fog_distance = fog_distance;
}


void Principled::set_uv_offset_x(float uv_offset_x)
{
   this->uv_offset_x = uv_offset_x;
}


void Principled::set_uv_offset_y(float uv_offset_y)
{
   this->uv_offset_y = uv_offset_y;
}


void Principled::set_camera_far_plane(float camera_far_plane)
{
   this->camera_far_plane = camera_far_plane;
}


ALLEGRO_COLOR Principled::get_world_tint() const
{
   return world_tint;
}


ALLEGRO_COLOR Principled::get_fog_color() const
{
   return fog_color;
}


float Principled::get_fog_intensity() const
{
   return fog_intensity;
}


float Principled::get_fog_distance() const
{
   return fog_distance;
}


float Principled::get_uv_offset_x() const
{
   return uv_offset_x;
}


float Principled::get_uv_offset_y() const
{
   return uv_offset_y;
}


float Principled::get_camera_far_plane() const
{
   return camera_far_plane;
}


void Principled::initialize()
{
   if (!initialized) AllegroFlare::Shaders::Base::initialize();
   initialized = true;
}

void Principled::activate()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Shaders::Principled::activate]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Shaders::Principled::activate]: error: guard \"initialized\" not met");
   }
   AllegroFlare::Shaders::Base::activate();
   set_values_to_activated_shader();
   return;
}

void Principled::set_values_to_activated_shader()
{
   set_float("uv_offset_x", 0.0);
   set_float("uv_offset_y", 0.0);
   set_vec3("fog_color", fog_color.r, fog_color.g, fog_color.b);
   set_float("fog_intensity", fog_intensity);
   set_float("fog_distance", fog_distance);
   set_float("camera_far_plane", camera_far_plane);
   set_vec3("world_tint", world_tint.r, world_tint.g, world_tint.b);
   return;
}

std::string Principled::obtain_vertex_source()
{
   static const std::string source = R"DELIM(
     attribute vec4 al_pos;
     attribute vec4 al_color;
     attribute vec2 al_texcoord;
     uniform mat4 al_projview_matrix;
     uniform bool al_use_tex_matrix;
     uniform mat4 al_tex_matrix;
     varying vec4 varying_color;
     varying vec2 varying_texcoord;

     // UV offset features
     // TODO: Test the UV offset features
     uniform float uv_offset_x;
     uniform float uv_offset_y;

     // Fog features
     uniform float fog_distance;
     varying float fog_depth;
     uniform float camera_far_plane;

     void main()
     {
       varying_color = al_color;
       if (al_use_tex_matrix) {
         vec4 uv = al_tex_matrix * vec4(al_texcoord, 0, 1);
         varying_texcoord = vec2(uv.x + uv_offset_x, uv.y + uv_offset_y);
       }
       else
         varying_texcoord = al_texcoord;
       gl_Position = al_projview_matrix * al_pos;

       // Capture the depth
       //depth = gl_Position.z / 100.0; // NOTE: 100.0 is the far plane in Camera3D
                                     // TODO: pass in far_plane as a varying
                                     // https://stackoverflow.com/a/17621928/6072362
       //depth = gl_Position.z / camera_far_plane; //100.0; // / 100.0; // NOTE: 100.0 is the far plane in Camera3D
                                                 // TODO: pass in far_plane as a varying
                                                 // https://stackoverflow.com/a/17621928/6072362
       fog_depth = clamp(gl_Position.z / fog_distance, 0.0, 1.0);
       //depth = clamp(depth, 0.0, 1.0);
     }
   )DELIM";
   return source;
}

std::string Principled::obtain_fragment_source()
{
   static const std::string source = R"DELIM(
     #ifdef GL_ES
     precision lowp float;
     #endif
     uniform sampler2D al_tex;
     uniform bool al_use_tex;
     uniform bool al_alpha_test;
     uniform int al_alpha_func;
     uniform float al_alpha_test_val;
     varying vec4 varying_color;
     varying vec2 varying_texcoord;
     //varying float depth;
     varying float fog_depth;

     uniform vec3 fog_color;
     uniform float fog_intensity;
     uniform vec3 world_tint;

     bool alpha_test_func(float x, int op, float compare);

     void main()
     {
       vec4 c;
       if (al_use_tex)
         c = varying_color * texture2D(al_tex, varying_texcoord);
       else
         c = varying_color;


       if (!al_alpha_test || alpha_test_func(c.a, al_alpha_func, al_alpha_test_val))
       {
          //gl_FragColor = vec4(depth, depth, depth, 1.0);
          //gl_FragColor = c;
          //float depth_contrib = (1.0 - depth);
          //float inv_depth_contrib = depth;
          //float intensity = 0.5;

          c = c * vec4(world_tint, 1.0);
          gl_FragColor = mix(c, mix(c, vec4(fog_color, 1.0), fog_depth), fog_intensity);

          //gl_FragColor = vec4(
              //c.r * depth_contrib + fog_color.r * inv_depth_contrib,
              //c.g * depth_contrib + fog_color.g * inv_depth_contrib,
              //c.b * depth_contrib + fog_color.b * inv_depth_contrib,
              ////c.g * depth_contrib,
              ////c.b * depth_contrib,
              //1.0
           //);
       }
       else
       {
          discard;
       }
     }

     bool alpha_test_func(float x, int op, float compare)
     {
       if (op == 0) return false;
       else if (op == 1) return true;
       else if (op == 2) return x < compare;
       else if (op == 3) return x == compare;
       else if (op == 4) return x <= compare;
       else if (op == 5) return x > compare;
       else if (op == 6) return x != compare;
       else if (op == 7) return x >= compare;
       return false;
     }
   )DELIM";
   return source;
}


} // namespace Shaders
} // namespace Krampus24


