//
// Created by dev on 18.05.2022.
//

// Parental class and definitions
#include "DrawableInterface.h"
#include "../CubeHeaders.h"

// STL
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <exception>


// DrawableInterface stuff
#include <GL/glut.h>
#include "../Structure/Cubelet.h"

// Structures and algorithms
#include "../Structure/Layer.h"
#include "../Structure/Cubelet.h"
#include "../Structure/Cube.h"
#include "../Algorithm/ScrambleGenerator.h"

#ifndef RUBIKSCUBE_CUBEINTERFACE_H
#define RUBIKSCUBE_CUBEINTERFACE_H

namespace Object {
    class CubeInterface : public DrawableInterface {
    protected:
        // Edge number that is currently rotating.
        int current = -1;
        // Edge origin: left, front or top
        char current_origin = '\0';
        // Current rotation angle
        float current_rotate_angle = 0;
        // "Pixel" size
        constexpr const static double object_size = CUBELET_SIZE;
        // Window name
        const char *object_type = "Rubik's cube";
        // Are we scrambling now
        bool scramble_state = false;
        // Are we assemblying now
        bool assembly_state = false;
        // Edge colors
        const std::vector<unsigned int> color_ = {COLOR_WHITE, COLOR_YELLOW, COLOR_BLUE, COLOR_GREEN, COLOR_RED, COLOR_ORANGE};

        void keys(unsigned char key, int, int) override;

        void specialKeys(int key, int, int) override;

        void timer(int) override;

        /// Used to display some functions that are independent of user's keyboard. E.g. - scrable()
        virtual bool imitate_key_pressed(char origin, size_t index);

        void display() override;

        void set_material() override;

        /// Generates random scramble
        virtual void generate_scramble(const std::vector<std::string> &from) = 0;

        virtual void Rotate(size_t index, float angle) = 0;

        void draw() override = 0;

        CubeInterface(bool draw, int argc, char **argv)
        : DrawableInterface()
        , draw_(draw) {
            glutInit(&argc, argv);
            glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
            glEnable(GLUT_MULTISAMPLE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glutInitWindowSize(800, 800);
            glutInitWindowPosition(0, 0);
            glutCreateWindow(object_type);
        }

        bool draw_ = false;
    public:
        enum CubeException {
            MOVE_LIMIT = 0,
            INVALID_SCRAMBLE,
            UNDEFINED_ORIGIN,
            UNABLE_TO_LOAD
        };

        virtual void scramble() = 0;

        virtual void scramble(const std::vector<std::string> &from) = 0;

        virtual void assembly() = 0;

        virtual void save_state() = 0;

        virtual void load_state() = 0;
    };
}


#endif //RUBIKSCUBE_CUBEINTERFACE_H
