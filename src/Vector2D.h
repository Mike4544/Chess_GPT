#pragma once

struct Vector2D {
    int x, y;

    bool operator==(Vector2D& other) const {
        return x == other.x && y == other.y;
    }
};