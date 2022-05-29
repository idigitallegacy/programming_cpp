//
// Created by dev on 24.04.2022.
//
#include <algorithm>
#include <string>
#include "../CubeHeaders.h"

#include "Layer.h"

#ifndef RUBIKSCUBE_STRUCT_CUBE_H
#define RUBIKSCUBE_STRUCT_CUBE_H

namespace Structure {
    class Cube {
    private:
        Layer front_;
        Layer back_;
        Layer left_;
        Layer right_;
        Layer top_;
        Layer bottom_;

        size_t size_;

        void rotate_col(size_t col_index, bool orthogonal, bool clockwise);

        void rotate_row_inner(size_t row_index, bool clockwise);

    public:
        explicit Cube(size_t size = CUBE_SIZE_DEFAULT)
        : front_(size, 'G')
        , back_(size, 'B')
        , left_(size, 'O')
        , right_(size, 'R')
        , top_(size, 'W')
        , bottom_(size, 'Y')
        , size_(size) {}

        Cube(const Cube &cpd)//NOLINT
        : front_(cpd.front_)
        , back_(cpd.back_)
        , left_(cpd.left_)
        , right_(cpd.right_)
        , top_(cpd.top_)
        , bottom_(cpd.bottom_)
        , size_(cpd.size_) {}

        Layer get_state(const std::string &edge) {
            if (edge == "front") return front_;
            if (edge == "back") return back_;
            if (edge == "left") return left_;
            if (edge == "right") return right_;
            if (edge == "top") return top_;
            if (edge == "bottom") return bottom_;
            return Layer(size_);
        }

        void rotate_front_edge(bool clockwise) {
            if (clockwise)
                front_.rotate_clockwise();
            else
                front_.rotate_counter_clockwise();
            rotate_col(0, true, clockwise);
        }

        void rotate_back_edge(bool clockwise) {
            if (clockwise)
                back_.rotate_clockwise();
            else
                back_.rotate_counter_clockwise();

            rotate_col(size_ - 1, true, !clockwise);
        }

        void rotate_left_edge(bool clockwise) {
            if (clockwise)
                left_.rotate_clockwise();
            else
                left_.rotate_counter_clockwise();
            rotate_col(0, false, clockwise);
        }

        void rotate_right_edge(bool clockwise) {
            if (clockwise)
                right_.rotate_clockwise();
            else
                right_.rotate_counter_clockwise();
            rotate_col(size_ - 1, false, !clockwise);
        }

        void rotate_top_edge(bool clockwise) {
            if (clockwise)
                top_.rotate_clockwise();
            else
                top_.rotate_counter_clockwise();
            rotate_row_inner(0, clockwise);
        }

        void rotate_bottom_edge(bool clockwise) {
            if (clockwise)
                bottom_.rotate_clockwise();
            else
                bottom_.rotate_counter_clockwise();
            rotate_row_inner(size_ - 1, !clockwise);
        }

        /**
         * @param origin "left" or "front" - origin of rotation
         * @param index Column index [0; size)
         * @param clockwise Clockwise or counter-clockwise rotation
         */
        void rotate_column(const std::string &origin, size_t index, bool clockwise);

        void rotate_row(size_t index, bool clockwise);
    };
}

#endif //RUBIKSCUBE_STRUCT_CUBE_H
