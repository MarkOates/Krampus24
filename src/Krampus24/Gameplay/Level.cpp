

#include <Krampus24/Gameplay/Level.hpp>




namespace Krampus24
{
namespace Gameplay
{


Level::Level()
   : AllegroFlare::Levels::Base(Krampus24::Gameplay::Level::TYPE)
   , title("[unset-title]")
{
}


Level::~Level()
{
}


void Level::set_title(std::string title)
{
   this->title = title;
}


std::string Level::get_title() const
{
   return title;
}




} // namespace Gameplay
} // namespace Krampus24


