#include <lcom/lcf.h>
#include "points.h"

void points_init() {
    points.points = 0;
    points.points_sprite = create_sprite(points_xpm, 10, 10, 0, 0, XPM_8_8_8);
}

void points_up() {
    points.points++;
}

void points_down() {
    if (points.points > 0) {
        points.points--;
    }
}

void draw_points() {
    int value = points.points;

    // Set starting draw position (e.g., next to points title)
    int x = points.points_sprite->x + points.points_sprite->width + 10;
    int y = points.points_sprite->y;

    // First, draw the "Points" title
    if (points.points_sprite != NULL) {
        draw_sprite(points.points_sprite);
    }

    // Convert integer to digits
    int digits[10]; // Max 10 digits (more than enough for points)
    int num_digits = 0;

    if (value == 0) {
        digits[0] = 0;
        num_digits = 1;
    } else {
        while (value > 0) {
            digits[num_digits++] = value % 10;
            value /= 10;
        }
    }

    // Digits are in reverse order, draw from left to right
    for (int i = num_digits - 1; i >= 0; i--) {
        Sprite *digit_sprite = create_sprite(digits_xpm[digits[i]], x, y, 0, 0, XPM_8_8_8);
        if (digit_sprite != NULL) {
            draw_sprite(digit_sprite);
            x += digit_sprite->width + 2; // Add spacing between digits
            destroy_sprite(digit_sprite); // Clean up after drawing
        }
    }
}
