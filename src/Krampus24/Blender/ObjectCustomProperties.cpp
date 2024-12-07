

#include <Krampus24/Blender/ObjectCustomProperties.hpp>

#include <AllegroFlare/Logger.hpp>
#include <set>
#include <sstream>


namespace Krampus24
{
namespace Blender
{


ObjectCustomProperties::ObjectCustomProperties()
   : string_properties({})
   , bool_properties({})
   , int_properties({})
   , float_properties({})
   , color_properties({})
{
}


ObjectCustomProperties::~ObjectCustomProperties()
{
}


void ObjectCustomProperties::set_string_properties(std::map<std::string, std::string> string_properties)
{
   this->string_properties = string_properties;
}


void ObjectCustomProperties::set_bool_properties(std::map<std::string, bool> bool_properties)
{
   this->bool_properties = bool_properties;
}


void ObjectCustomProperties::set_int_properties(std::map<std::string, int> int_properties)
{
   this->int_properties = int_properties;
}


void ObjectCustomProperties::set_float_properties(std::map<std::string, float> float_properties)
{
   this->float_properties = float_properties;
}


void ObjectCustomProperties::set_color_properties(std::map<std::string, std::string> color_properties)
{
   this->color_properties = color_properties;
}


std::map<std::string, std::string> ObjectCustomProperties::get_string_properties() const
{
   return string_properties;
}


std::map<std::string, bool> ObjectCustomProperties::get_bool_properties() const
{
   return bool_properties;
}


std::map<std::string, int> ObjectCustomProperties::get_int_properties() const
{
   return int_properties;
}


std::map<std::string, float> ObjectCustomProperties::get_float_properties() const
{
   return float_properties;
}


std::map<std::string, std::string> ObjectCustomProperties::get_color_properties() const
{
   return color_properties;
}


std::string ObjectCustomProperties::get_keys_in_list()
{
   // TODO: Test this method
   std::set<std::string> result;
   for (auto &string_property : string_properties) result.insert("\"" + string_property.first + "\" (string)");
   for (auto &int_property : int_properties) result.insert("\"" + int_property.first + "\" (int)");
   for (auto &float_property : float_properties) result.insert("\"" + float_property.first + "\" (float)");
   for (auto &bool_property : bool_properties) result.insert("\"" + bool_property.first + "\" (bool)");
   for (auto &color_property : color_properties) result.insert("\"" + color_property.first + "\" (color)");

   std::stringstream result_str;
   result_str << "[";
   for (auto& res : result)
   {
      result_str << res << ", ";
   }
   result_str << "]";

   return result_str.str();
}

bool ObjectCustomProperties::empty()
{
   return string_properties.empty()
      && bool_properties.empty()
      && int_properties.empty()
      && float_properties.empty()
      && color_properties.empty();
}

void ObjectCustomProperties::clear()
{
   string_properties.clear();
   bool_properties.clear();
   int_properties.clear();
   float_properties.clear();
   color_properties.clear();
}

void ObjectCustomProperties::add_string(std::string name, std::string value)
{
   throw_if_key_already_exists(name, "add_string");
   string_properties[name] = value;
}

void ObjectCustomProperties::add_bool(std::string name, bool value)
{
   throw_if_key_already_exists(name, "add_bool");
   bool_properties[name] = value;
}

void ObjectCustomProperties::add_float(std::string name, float value)
{
   throw_if_key_already_exists(name, "add_float");
   float_properties[name] = value;
}

void ObjectCustomProperties::add_int(std::string name, int value)
{
   throw_if_key_already_exists(name, "add_int");
   int_properties[name] = value;
}

void ObjectCustomProperties::add_color(std::string name, std::string value)
{
   throw_if_key_already_exists(name, "add_color");
   color_properties[name] = value;
}

std::string ObjectCustomProperties::get_string(std::string name)
{
   throw_if_key_does_not_exist(name, "get_string");
   return string_properties[name];
}

bool ObjectCustomProperties::get_bool(std::string name)
{
   throw_if_key_does_not_exist(name, "get_bool");
   return bool_properties[name];
}

float ObjectCustomProperties::get_float(std::string name)
{
   throw_if_key_does_not_exist(name, "get_float");
   return float_properties[name];
}

int ObjectCustomProperties::get_int(std::string name)
{
   throw_if_key_does_not_exist(name, "get_int");
   return int_properties[name];
}

std::string ObjectCustomProperties::get_color(std::string name)
{
   throw_if_key_does_not_exist(name, "get_color");
   return color_properties[name];
}

std::string ObjectCustomProperties::get_type(std::string name)
{
   throw_if_key_does_not_exist(name, "get_type");
   if (float_properties.find(name) != float_properties.end()) return "float";
   if (int_properties.find(name) != int_properties.end()) return "int";
   if (bool_properties.find(name) != bool_properties.end()) return "bool";
   if (string_properties.find(name) != string_properties.end()) return "string";
   if (color_properties.find(name) != color_properties.end()) return "color";
   return "[ERROR-does_not_exist]";
}

bool ObjectCustomProperties::exists(std::string name)
{
   if (float_properties.find(name) != float_properties.end()) return true;
   if (int_properties.find(name) != int_properties.end()) return true;
   if (bool_properties.find(name) != bool_properties.end()) return true;
   if (string_properties.find(name) != string_properties.end()) return true;
   if (color_properties.find(name) != color_properties.end()) return true;
   return false;
}

bool ObjectCustomProperties::is_float(std::string name)
{
   throw_if_key_does_not_exist(name, "is_float");
   if (float_properties.find(name) != float_properties.end()) return true;
   return false;
}

bool ObjectCustomProperties::is_bool(std::string name)
{
   throw_if_key_does_not_exist(name, "is_bool");
   if (bool_properties.find(name) != bool_properties.end()) return true;
   return false;
}

bool ObjectCustomProperties::is_int(std::string name)
{
   throw_if_key_does_not_exist(name, "is_int");
   if (int_properties.find(name) != int_properties.end()) return true;
   return false;
}

bool ObjectCustomProperties::is_string(std::string name)
{
   throw_if_key_does_not_exist(name, "is_string");
   if (string_properties.find(name) != string_properties.end()) return true;
   return false;
}

bool ObjectCustomProperties::is_color(std::string name)
{
   throw_if_key_does_not_exist(name, "is_color");
   if (color_properties.find(name) != color_properties.end()) return true;
   return false;
}

void ObjectCustomProperties::throw_if_key_already_exists(std::string key, std::string while_in_method_name)
{
   if (!exists(key)) return;
   AllegroFlare::Logger::throw_error(
      "AllegroFlare::Blender::ObjectCustomProperties::" + while_in_method_name,
      "The key \"" + key + "\" already exists."
   );
   return;
}

void ObjectCustomProperties::throw_if_key_does_not_exist(std::string key, std::string while_in_method_name)
{
   if (exists(key)) return;
   AllegroFlare::Logger::throw_error(
      "AllegroFlare::Blender::ObjectCustomProperties::" + while_in_method_name,
      "The key \"" + key + "\" does not exist."
   );
   return;
}


} // namespace Blender
} // namespace Krampus24

