#pragma once


#include <AllegroFlare/Testing/WithAllegroFlareFrameworksFullFixture.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Screen.hpp>


namespace Krampus24
{
   namespace Testing
   {
      class WithConstructFixture : public AllegroFlare::Testing::WithAllegroFlareFrameworksFullFixture
      {
      private:
         Krampus24::Gameplay::Screen* screen;

      protected:


      public:
         WithConstructFixture();
         virtual ~WithConstructFixture();

         virtual void SetUp() override;
         virtual void TearDown() override;
         void run_test_construct_loop(float num_seconds_until_abort=60.0f);
         void add_entity_to_pool(Krampus24::Gameplay::Entities::Base* entity=nullptr);
      };
   }
}



