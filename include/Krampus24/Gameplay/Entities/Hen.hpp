#pragma once


#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Entities/Hen.hpp>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace Entities
      {
         class Hen : public Krampus24::Gameplay::Entities::Base
         {
         public:
            static constexpr char* BLENDER_IDENTIFIER = (char*)"hen";

         private:

         protected:


         public:
            Hen();
            ~Hen();

            static Krampus24::Gameplay::Entities::Hen* construct();
         };
      }
   }
}



