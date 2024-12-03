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
         ALLEGRO_COLOR fog_tint;
         float fog_tint_intensity;
         bool initialized;
         void set_values_to_activated_shader();
         static std::string obtain_vertex_source();
         static std::string obtain_fragment_source();

      protected:


      public:
         Principled();
         ~Principled();

         void set_fog_tint(ALLEGRO_COLOR fog_tint);
         void set_fog_tint_intensity(float fog_tint_intensity);
         ALLEGRO_COLOR get_fog_tint() const;
         float get_fog_tint_intensity() const;
         void initialize();
         void activate();
      };
   }
}



