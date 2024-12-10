
#include <gtest/gtest.h>

#include <AllegroFlare/StableVector.hpp>



//class MyContainedObject
//{
//private:
//};


TEST(AllegroFlare_StableVectorTest, can_be_created_without_blowing_up)
{
   AllegroFlare::StableVector<int, std::string> stable_vector;
}


TEST(AllegroFlare_StableVectorTest, add__will_add_elements)
{
   AllegroFlare::StableVector<int, std::string> stable_vector;
   auto key1 = stable_vector.add("Hello");
   auto key2 = stable_vector.add("World");

   EXPECT_EQ(stable_vector.size(), 2);
   EXPECT_EQ(stable_vector.get(key1), "Hello");
   EXPECT_EQ(stable_vector.get(key2), "World");
}


TEST(AllegroFlare_StableVectorTest, contains__will_return_true_if_the_element_exists)
{
   AllegroFlare::StableVector<int, std::string> stable_vector;
   auto key1 = stable_vector.add("First");
   auto key2 = stable_vector.add("Second");

   stable_vector.remove(key1);

   EXPECT_EQ(1, stable_vector.size());
   EXPECT_EQ(false, stable_vector.contains(key1));
   EXPECT_EQ(true, stable_vector.contains(key2));
   EXPECT_EQ("Second", stable_vector.get(key2));
}


TEST(AllegroFlare_StableVectorTest, CanAccessByKey)
{
   AllegroFlare::StableVector<int, std::string> stable_vector;
   auto key1 = stable_vector.add("A");
   auto key2 = stable_vector.add("B");

   EXPECT_NO_THROW({
      EXPECT_EQ(stable_vector.get(key1), "A");
      EXPECT_EQ(stable_vector.get(key2), "B");
   });

   EXPECT_THROW(stable_vector.get(999), std::out_of_range);
}


TEST(AllegroFlare_StableVectorTest, SwapAndPopRemovesCorrectly)
{
   AllegroFlare::StableVector<int, std::string> stable_vector;
   auto key1 = stable_vector.add("Element1");
   auto key2 = stable_vector.add("Element2");
   auto key3 = stable_vector.add("Element3");

   // Remove the middle element
   stable_vector.remove(key2);

   EXPECT_EQ(stable_vector.size(), 2);
   EXPECT_FALSE(stable_vector.contains(key2));
   EXPECT_TRUE(stable_vector.contains(key1));
   EXPECT_TRUE(stable_vector.contains(key3));

   // Validate the remaining keys
   EXPECT_EQ(stable_vector.get(key1), "Element1");
   EXPECT_EQ(stable_vector.get(key3), "Element3");
}


TEST(AllegroFlare_StableVectorTest, CanCheckContainment)
{
   AllegroFlare::StableVector<int, std::string> stable_vector;
   auto key1 = stable_vector.add("Existential");
   EXPECT_TRUE(stable_vector.contains(key1));

   stable_vector.remove(key1);
   EXPECT_FALSE(stable_vector.contains(key1));
}


TEST(AllegroFlare_StableVectorTest, HandlesEmptyContainer)
{
   AllegroFlare::StableVector<int, std::string> stable_vector;
   EXPECT_EQ(stable_vector.size(), 0);
   EXPECT_FALSE(stable_vector.contains(0));

   EXPECT_THROW(stable_vector.get(0), std::out_of_range);
}


