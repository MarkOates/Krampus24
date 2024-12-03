

#include <Krampus24/Gameplay/Entities/Hen.hpp>




namespace Krampus24
{
namespace Gameplay
{
namespace Entities
{


Hen::Hen()
   : Krampus24::Gameplay::Entities::Base()
{
}


Hen::~Hen()
{
}


Krampus24::Gameplay::Entities::Hen* Hen::construct()
{
   Krampus24::Gameplay::Entities::Hen* result = new Krampus24::Gameplay::Entities::Hen;
   return result;
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


