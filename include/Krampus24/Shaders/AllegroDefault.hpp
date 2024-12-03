#pragma once


#include <AllegroFlare/Shaders/Base.hpp>
#include <string>


namespace Krampus24
{
   namespace Shaders
   {
      class AllegroDefault : public AllegroFlare::Shaders::Base
      {
      public:
         static constexpr char* TYPE = (char*)"Krampus24/Shaders/AllegroDefault";

      private:
         bool initialized;
         static std::string obtain_vertex_source();
         static std::string obtain_fragment_source();

      protected:


      public:
         AllegroDefault();
         ~AllegroDefault();

         void initialize();
         void activate();
      };
   }
}



