#pragma once

#include <cstdint>

namespace telemetry {

/// A single sensor reading.
struct SensorPacket {
    std::uint32_t sensor_id{0};
    std::int64_t timestamp_ms{0};
    double value{0.0};

    friend bool operator==(const SensorPacket&, const SensorPacket&) = default;
};

}  // namespace telemetry
