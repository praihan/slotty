#include "gtest/gtest.h"

#include <slotty/slotty.h>

#define TEST_NAME Simple

static int modCount = 0;

TEST(TEST_NAME, BasicFunctionality) {
  slotty::event<> evt;

  {
    auto slot = slotty::make_slot(evt);
    evt.connect([]() { ++modCount; }, slot);
    evt.raise();
  }

  evt.raise();

  EXPECT_EQ(modCount, 1);
}