//
// Created by dev on 08.05.2022.
//
#include <cmath>

#ifndef RUBIKSCUBE_CUBEHEADERS_H
#define RUBIKSCUBE_CUBEHEADERS_H

// Color pallette
#define COLOR_WHITE 0xe6e6e6
#define COLOR_YELLOW 0xfde910
#define COLOR_BLUE 0x00bfff
#define COLOR_GREEN 0x00ff00
#define COLOR_RED 0xff2400
#define COLOR_ORANGE 0xff9900

// Visual size
#define CUBELET_SIZE 15

// Material
#define CUBE_MATERIAL_AMBIENT {0.33125f, 0.33125f, 0.33125f, 1.0f}
#define CUBE_MATERIAL_DIFFUSE {0.2775f, 0.2775f, 0.2775f, 1.0f}
#define CUBE_MATERIAL_SPECULAR {0.2775f, 0.2775f, 0.2775f, 1.0f}
#define CUBE_MATERIAL_SHINESS 20
#define CUBE_MATERIAL_BLACK {0.1f, 0.1f, 0.1f, 1.0f}

// Rotate step and timing function
#define CUBE_TRANSITION_STEP 5

// Standard size
#define CUBE_SIZE_DEFAULT 3

// NMoves formula
#define CUBE_NMOVES 2.1 * std::pow(size, 3) / std::log2(size) + 1

// NMoves limit multiplier
#define CUBE_NMOVES_LIMIT 2

#endif //RUBIKSCUBE_CUBEHEADERS_H
