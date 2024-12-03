

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
   , fog_tint(ALLEGRO_COLOR{1, 1, 1, 1})
   , fog_tint_intensity(1.0f)
   , initialized(false)
{
}


Principled::~Principled()
{
}


void Principled::set_fog_tint(ALLEGRO_COLOR fog_tint)
{
   this->fog_tint = fog_tint;
}


void Principled::set_fog_tint_intensity(float fog_tint_intensity)
{
   this->fog_tint_intensity = fog_tint_intensity;
}


ALLEGRO_COLOR Principled::get_fog_tint() const
{
   return fog_tint;
}


float Principled::get_fog_tint_intensity() const
{
   return fog_tint_intensity;
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
}

void Principled::set_values_to_activated_shader()
{
   set_vec3("fog_tint", fog_tint.r, fog_tint.g, fog_tint.b);
   set_float("fog_tint_intensity", fog_tint_intensity);
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
     varying float depth;

     void main()
     {
       varying_color = al_color;
       if (al_use_tex_matrix) {
         vec4 uv = al_tex_matrix * vec4(al_texcoord, 0, 1);
         varying_texcoord = vec2(uv.x, uv.y);
       }
       else
         varying_texcoord = al_texcoord;
       gl_Position = al_projview_matrix * al_pos;

       // Capture the depth
       depth = gl_Position.z / 100.0; // NOTE: 100.0 is the far plane in Camera3D
                                       // TODO: pass in far_plane as a varying
                                       // https://stackoverflow.com/a/17621928/6072362
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
     varying float depth;

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
          gl_FragColor = vec4(c.r * depth, c.g * depth, c.b * depth, 1.0);
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


