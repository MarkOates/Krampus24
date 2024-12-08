#pragma once


#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <string>


namespace Krampus24
{
   namespace Game
   {
      class DialogBuilder
      {
      private:
         static AllegroFlare::DialogTree::NodeBank build_a_dialog_node_bank();

      protected:


      public:
         DialogBuilder();
         ~DialogBuilder();

         static AllegroFlare::DialogTree::NodeBank build_dialog_node_bank(std::string identifier="[identifier-discarded]");
      };
   }
}



