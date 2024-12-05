#pragma once


#include <AllegroFlare/Shaders/Base.hpp>
#include <allegro5/allegro.h>
#include <string>


namespace Krampus24
{
   namespace Shaders
   {
      class Principled : public AllegroFlare::Shaders::Base
      {
      public:
         static constexpr char* TYPE = (char*)"Krampus24/Shaders/Principled";

      private:
         ALLEGRO_COLOR world_tint;
         ALLEGRO_COLOR fog_color;
         float fog_intensity;
         float fog_distance;
         float camera_far_plane;
         bool initialized;
         void set_values_to_activated_shader();
         static std::string obtain_vertex_source();
         static std::string obtain_fragment_source();

      protected:


      public:
         Principled();
         ~Principled();

         void set_world_tint(ALLEGRO_COLOR world_tint);
         void set_fog_color(ALLEGRO_COLOR fog_color);
         void set_fog_intensity(float fog_intensity);
         void set_fog_distance(float fog_distance);
         void set_camera_far_plane(float camera_far_plane);
         ALLEGRO_COLOR get_world_tint() const;
         ALLEGRO_COLOR get_fog_color() const;
         float get_fog_intensity() const;
         float get_fog_distance() const;
         float get_camera_far_plane() const;
         void initialize();
         void activate();
      };
   }
}


