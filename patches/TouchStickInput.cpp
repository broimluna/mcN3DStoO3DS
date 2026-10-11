#include "TouchStick.h"

bool TouchStick::Update(TInputState &state, int x, int y, bool held, bool gameplay) {
    state.lookX = 0;
    state.lookY = 0;
    const auto dx = static_cast<float>(x - centerX);
    const auto dy = static_cast<float>(centerY - y);
    const auto distanceSquared = dx * dx + dy * dy;
    if (held && !state.wasHeld) {
        state.ownsTouch = gameplay && distanceSquared <= radius * radius;
        state.canceled = false;
    }

    const auto consumed = state.ownsTouch;
    state.wasHeld = held;
    if (!held) {
        state.ownsTouch = false;
        state.canceled = false;
        return consumed;
    }

    if (!gameplay) {
        state.canceled = true;
    }

    if (consumed && !state.canceled && distanceSquared > 49.0f) {
        const auto distance = __builtin_sqrtf(distanceSquared);
        const auto strength = distance < 46.0f ? (distance - 7.0f) * (100.0f / 39.0f) : 100.0f;
        state.lookX = static_cast<short>(dx * strength / distance);
        state.lookY = static_cast<short>(dy * strength / distance);
    }

    return consumed;
}
