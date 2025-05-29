#include <lcom/lcf.h>
#include "points.h"

#define HIGHSCORE_FILE "/home/lcom/labs/proj/highscore.txt"

int highscore = 0;
static Sprite *highscore_sprite = NULL;

int load_highscore() {
    FILE *f = fopen(HIGHSCORE_FILE, "r");
    if(f){
        fscanf(f, "%d", &highscore);
        fclose(f);
    }
    return highscore;
}

void save_highscore(int score) {
    FILE *f = fopen(HIGHSCORE_FILE, "w");
    if(f){
        fprintf(f, "%d", score);
        fclose(f);
    }
}

void points_init() {
    points.points = 0;
    points.points_sprite = create_sprite(points_xpm, 10, 10, 0, 0, XPM_8_8_8);
    highscore = load_highscore();
    highscore_sprite = create_sprite(highscore_xpm, 10, 50, 0, 0, XPM_8_8_8);
}

void points_up() {
    points.points++;
    if (points.points > highscore) {
        highscore = points.points;
        save_highscore(highscore);
    }
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

void draw_highscore() {
    int value = highscore;
    int x = highscore_sprite->x + highscore_sprite->width + 10;
    int y = highscore_sprite->y;

    if (highscore_sprite != NULL) {
        draw_sprite(highscore_sprite);
    }

    int digits[10];
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

    for (int i = num_digits - 1; i >= 0; i--) {
        Sprite *digit_sprite = create_sprite(digits_xpm[digits[i]], x, y, 0, 0, XPM_8_8_8);
        if (digit_sprite != NULL) {
            draw_sprite(digit_sprite);
            x += digit_sprite->width + 2;
            destroy_sprite(digit_sprite);
        }
    }
}

void destroy_points() {
    if (points.points_sprite != NULL) {
        destroy_sprite(points.points_sprite);
        points.points_sprite = NULL;
    }
}
