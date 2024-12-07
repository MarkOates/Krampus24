

#include <Krampus24/Blender/ObjectCustomPropertiesLoader.hpp>

#include <AllegroFlare/CSVParser.hpp>
#include <AllegroFlare/Logger.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Blender
{


ObjectCustomPropertiesLoader::ObjectCustomPropertiesLoader(std::string filename)
   : filename(filename)
   , objects({})
   , loaded(false)
{
}


ObjectCustomPropertiesLoader::~ObjectCustomPropertiesLoader()
{
}


void ObjectCustomPropertiesLoader::set_filename(std::string filename)
{
   this->filename = filename;
}


std::map<std::string, Krampus24::Blender::ObjectCustomProperties> ObjectCustomPropertiesLoader::get_objects() const
{
   return objects;
}


bool ObjectCustomPropertiesLoader::get_loaded() const
{
   return loaded;
}


void ObjectCustomPropertiesLoader::for_each_object(std::function<void(std::string, Krampus24::Blender::ObjectCustomProperties*)> function)
{
   if (!(loaded))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Blender::ObjectCustomPropertiesLoader::for_each_object]: error: guard \"loaded\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Blender::ObjectCustomPropertiesLoader::for_each_object]: error: guard \"loaded\" not met");
   }
   for (auto &object : objects) function(object.first, &object.second);
   return;
}

void ObjectCustomPropertiesLoader::load()
{
   if (!((!loaded)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Blender::ObjectCustomPropertiesLoader::load]: error: guard \"(!loaded)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Blender::ObjectCustomPropertiesLoader::load]: error: guard \"(!loaded)\" not met");
   }
   std::ifstream file(filename);
   if (!file)
   {
       AllegroFlare::Logger::throw_error(
         "Krampus24::Blender::ObjectCustomPropertiesLoader",
         "Unable to open file \"" + filename + "\"."
       );
   }

   objects.clear();

   std::string line;
   int line_number = 0;

   while (std::getline(file, line))
   {
      line_number++;

      // Skip empty lines
      // TODO: Add trim
      if (line.empty()) continue;

      // Skip comment lines (lines that start with #)
      // TODO: Trim leading whitespace
      if (line[0] == '#') continue;

      // Capture the tokens
      std::vector<std::string> tokens = AllegroFlare::CSVParser::parse_row(line);

      if (tokens.size() != 4)
      {
         AllegroFlare::Logger::throw_error(
            "Krampus24::Blender::ObjectCustomPropertiesLoader",
            "Error parsing line " + std::to_string(line_number) + ": \"" + line + "\". There was an unexpected "
               "number of tokens (expecting 4)."
         );
      }
      else
      {
         std::string object_name = tokens[0];
         std::string key = tokens[1];
         std::string type = tokens[2];
         std::string value_str = tokens[3];

         // Create a set of custom properties for this object name if it does not already exist
         if (objects.find(object_name) == objects.end())
         {
            objects[object_name] = Krampus24::Blender::ObjectCustomProperties();
         }

         // TODO: Improve parsing and type validation
         if (type == "int")
         {
            objects[object_name].add_int(key, std::stoi(value_str));
         }
         else // An unspported type
         {
            AllegroFlare::Logger::throw_error(
               "Krampus24::Blender::ObjectCustomPropertiesLoader",
               "Error parsing line " + std::to_string(line_number) + ": \"" + line + "\". The type "
                  " \"" + type + "\" is not supported."
            );
         }
      }
   }

   file.close();

   loaded = true;
}


} // namespace Blender
} // namespace Krampus24


