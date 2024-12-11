

#include <Krampus24/Gameplay/Entities/Zone.hpp>

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Gameplay
{
namespace Entities
{


Zone::Zone()
   : Krampus24::Gameplay::Entities::Base()
   , initialized(false)
{
}


Zone::~Zone()
{
}


std::vector<Krampus24::Gameplay::Entities::Base*> Zone::construct(AllegroFlare::Vec3D position, AllegroFlare::Vec3D size)
{
   Krampus24::Gameplay::Entities::Zone* result = new Krampus24::Gameplay::Entities::Zone;
   result->affected_by_environmental_forces = false;
   result->collides_with_player = true;
   result->placement.position = position;
   result->placement.size = size;
   result->aabb3d.set_max(result->placement.size);

   result->initialized = true;

   return { result };
}

void Zone::draw()
{
   // Zones do not render
   return;
}

void Zone::on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   return;
}

void Zone::on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   return;
}

void Zone::on_time_step(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Zone::on_time_step]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Zone::on_time_step]: error: guard \"initialized\" not met");
   }
   return;
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


