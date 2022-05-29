//
// Created by dev on 28.04.2022.
//

#ifndef RUBIKSCUBE_CUBE_H
#define RUBIKSCUBE_CUBE_H

// Parental class and definitions
#include "../CubeHeaders.h"
#include "CubeInterface.h"

namespace Object {
    class Cube : public CubeInterface {
    private:
        // Rubik's cube core info
        Structure::Cube cube_;
        size_t size_ = 3;

        // Softlock
        int nmoves_ = 0;
        int max_nmoves_ = 0;

        // N * N * N cubelets
        std::vector<std::vector<std::vector<Structure::Cubelet>>> cubelet_state_;
        // Displays if [i][j][k] cubelet is rotating now
        std::vector<std::vector<std::vector<bool>>> cubelet_rotation_state_;

        // Scramble/assemble stuff
        std::vector<std::string> scramble_path_;
        std::vector<std::string> solution_path_;
        std::vector<bool> assemble_move_made_;
        std::vector<bool> scramble_move_made_;

        void rotate_cubelets(size_t index, int sign);

        /// Checks if side is assebled
        static bool check_side(const std::vector<std::vector<char>> &side, char needle_color);

        /// Checks if the whole cube is assembled
        bool check_if_assembled();

        /// "Finds" solution
        void find_solution();

        /// Generates random scramble
        void generate_scramble(const std::vector<std::string> &from) final;

        /// Changes inner scan state
        void change_state(const std::string &move);

    protected:

        void draw() final;

        void clear();

        void Rotate(size_t index, float angle) final;

    public:
        explicit Cube(size_t size = CUBE_SIZE_DEFAULT, bool draw = false, int argc = 0, char **argv = nullptr)
                : CubeInterface(draw, argc, argv)
                , max_nmoves_(CUBE_NMOVES_LIMIT * CUBE_NMOVES)
                , cube_(size)
                , size_(size)
                , cubelet_rotation_state_(
                        size, std::vector<std::vector<bool>>(
                                size, std::vector<bool>(size, true)
                                        ))
                , cubelet_state_(
                        size, std::vector<std::vector<Structure::Cubelet>>(
                                size,std::vector<Structure::Cubelet>(
                                        size,Structure::Cubelet(GLfloat(size))
                                        ))) {
            if (draw) {
                app_instance = this;
                translateZ = -object_size * size;
                clear();

                init(*this);

                glutDisplayFunc(staticDisplay);
                glutReshapeFunc(reshape);
                glutKeyboardFunc(staticKeys);
                glutKeyboardUpFunc(staticKeysUp);
                glutSpecialFunc(staticSpecialKeys);
                glutSpecialUpFunc(staticSpecialKeysUp);
                glutTimerFunc(30, staticTimer, 0);
                glutMainLoop();
            }
        }

        void scramble() final;

        void scramble(const std::vector<std::string> &from) final;

        void assembly() final;

        void save_state() final;

        void load_state() final;

        std::vector<std::string> get_scramble() {
            return scramble_path_;
        }

        std::vector<std::string> get_solution() {
            find_solution();
            return solution_path_;
        }
    };
}


#endif //RUBIKSCUBE_CUBE_H
