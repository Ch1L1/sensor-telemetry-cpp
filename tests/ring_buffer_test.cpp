#include "ring_buffer.hpp"
#include "sensor_packet.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>

using telemetry::RingBuffer;
using telemetry::SensorPacket;

TEST(RingBufferTest, NewBufferIsEmpty) {
    RingBuffer<int> rb(4);
    EXPECT_TRUE(rb.empty());
    EXPECT_FALSE(rb.full());
    EXPECT_EQ(rb.size(), 0U);
    EXPECT_EQ(rb.capacity(), 4U);
}

TEST(RingBufferTest, PushPopPreservesFifoOrder) {
    RingBuffer<int> rb(3);
    ASSERT_TRUE(rb.push(1));
    ASSERT_TRUE(rb.push(2));
    ASSERT_TRUE(rb.push(3));
    EXPECT_EQ(rb.size(), 3U);

    EXPECT_EQ(rb.pop(), 1);
    EXPECT_EQ(rb.pop(), 2);
    EXPECT_EQ(rb.pop(), 3);
    EXPECT_TRUE(rb.empty());
}

TEST(RingBufferTest, WrapAroundKeepsOrder) {
    RingBuffer<int> rb(3);
    ASSERT_TRUE(rb.push(1));
    ASSERT_TRUE(rb.push(2));
    ASSERT_TRUE(rb.push(3));
    EXPECT_EQ(rb.pop(), 1);
    EXPECT_EQ(rb.pop(), 2);
    ASSERT_TRUE(rb.push(4));  // tail wraps to index 0
    ASSERT_TRUE(rb.push(5));
    EXPECT_TRUE(rb.full());

    EXPECT_EQ(rb.pop(), 3);
    EXPECT_EQ(rb.pop(), 4);  // head wraps to index 0
    EXPECT_EQ(rb.pop(), 5);
    EXPECT_TRUE(rb.empty());
}

TEST(RingBufferTest, RepeatedWrapAroundManyCycles) {
    RingBuffer<int> rb(2);
    for (int i = 0; i < 100; ++i) {
        ASSERT_TRUE(rb.push(i));
        EXPECT_EQ(rb.pop(), i);
    }
    EXPECT_TRUE(rb.empty());
}

TEST(RingBufferTest, PushToFullBufferFailsAndKeepsContents) {
    RingBuffer<int> rb(2);
    ASSERT_TRUE(rb.push(10));
    ASSERT_TRUE(rb.push(20));
    EXPECT_TRUE(rb.full());

    EXPECT_FALSE(rb.push(30));
    EXPECT_EQ(rb.size(), 2U);
    EXPECT_EQ(rb.pop(), 10);
    EXPECT_EQ(rb.pop(), 20);
    EXPECT_EQ(rb.pop(), std::nullopt);
}

TEST(RingBufferTest, CapacityOneBoundary) {
    RingBuffer<int> rb(1);
    ASSERT_TRUE(rb.push(7));
    EXPECT_TRUE(rb.full());
    EXPECT_FALSE(rb.push(8));
    EXPECT_EQ(rb.pop(), 7);
    EXPECT_TRUE(rb.empty());
    EXPECT_TRUE(rb.push(9));
}

TEST(RingBufferTest, PopFromEmptyReturnsNullopt) {
    RingBuffer<int> rb(2);
    EXPECT_EQ(rb.pop(), std::nullopt);
    EXPECT_EQ(rb.size(), 0U);

    ASSERT_TRUE(rb.push(1));
    EXPECT_EQ(rb.pop(), 1);
    EXPECT_EQ(rb.pop(), std::nullopt);  // empty again after draining
}

TEST(RingBufferTest, ZeroCapacityThrows) {
    EXPECT_THROW(RingBuffer<int>(0), std::invalid_argument);
}

TEST(RingBufferTest, SupportsMoveOnlyTypes) {
    RingBuffer<std::unique_ptr<std::string>> rb(2);
    ASSERT_TRUE(rb.push(std::make_unique<std::string>("abc")));
    auto out = rb.pop();
    ASSERT_TRUE(out.has_value());
    ASSERT_NE(*out, nullptr);
    EXPECT_EQ(**out, "abc");
}

TEST(RingBufferTest, StoresSensorPackets) {
    RingBuffer<SensorPacket> rb(2);
    const SensorPacket p{42U, 1'700'000'000'000, 3.14};
    ASSERT_TRUE(rb.push(p));
    auto out = rb.pop();
    ASSERT_TRUE(out.has_value());
    EXPECT_EQ(*out, p);
}
