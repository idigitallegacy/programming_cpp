//
// Created by dev on 29.04.2022.
//

#include "Cube.h"

namespace Structure {
    void Cube::rotate_col(size_t col_index, bool orthogonal, bool clockwise) {
        if (orthogonal) {
            std::vector<char> top = top_.get_row(size_ - col_index - 1);
            std::vector<char> left = left_.get_col(size_ - col_index - 1);
            std::vector<char> bottom = bottom_.get_row(col_index);
            std::vector<char> right = right_.get_col(col_index);

            if (clockwise) {
                std::reverse(left.begin(), left.end());
                std::reverse(right.begin(), right.end());
            } else {
                std::reverse(top.begin(), top.end());
                std::reverse(bottom.begin(), bottom.end());
            }

            top_.setup_row(clockwise ? left : right, size_ - col_index -1);
            right_.setup_col(clockwise ? top : bottom, col_index);
            bottom_.setup_row(clockwise ? right : left, col_index);
            left_.setup_col(clockwise ? bottom : top, size_ - col_index - 1);
        } else {
            std::vector<char> front = front_.get_col(col_index);
            std::vector<char> top = top_.get_col(col_index);
            std::vector<char> back = back_.get_col(size_ - col_index - 1);
            std::vector<char> bottom = bottom_.get_col(col_index);

            if (clockwise) {
                std::reverse(bottom.begin(), bottom.end());
                std::reverse(back.begin(), back.end());
            } else {
                std::reverse(top.begin(), top.end());
                std::reverse(back.begin(), back.end());
            }

            top_.setup_col(clockwise ? back : front, col_index);
            back_.setup_col(clockwise ? bottom : top, size_ - col_index - 1);
            bottom_.setup_col(clockwise ? front : back, col_index);
            front_.setup_col(clockwise ? top : bottom, col_index);
        }
    }

    void Cube::rotate_row_inner(size_t row_index, bool clockwise) {
        std::vector<char> front = front_.get_row(row_index);
        std::vector<char> left = left_.get_row(row_index);
        std::vector<char> back = back_.get_row(row_index);
        std::vector<char> right = right_.get_row(row_index);

        front_.setup_row(clockwise ? right : left, row_index);
        left_.setup_row(clockwise ? front : back, row_index);
        back_.setup_row(clockwise ? left : right, row_index);
        right_.setup_row(clockwise ? back : front, row_index);
    }

    void Cube::rotate_column(const std::string &origin, size_t index, bool clockwise) {
        if (origin == "left") {
            if (index == 0) {
                rotate_left_edge(clockwise);
                return;
            }
            if (index == size_ - 1) {
                rotate_right_edge(!clockwise);
                return;
            }

            rotate_col(index, false, clockwise);
            return;
        }
        if (origin == "front") {
            if (index == 0) {
                rotate_front_edge(clockwise);
                return;
            }
            if (index == size_ - 1) {
                rotate_back_edge(!clockwise);
                return;
            }

            rotate_col(index, true, clockwise);
            return;
        }
    }

    void Cube::rotate_row(size_t index, bool clockwise) {
        if (index == 0) {
            rotate_top_edge(clockwise);
            return;
        }
        if (index == size_ - 1) {
            rotate_bottom_edge(!clockwise);
            return;
        }
        rotate_row_inner(index, clockwise);
    }
}
