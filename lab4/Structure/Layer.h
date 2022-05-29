//
// Created by dev on 24.04.2022.
//
#include <vector>
#include <cmath>

#ifndef RUBIKSCUBE_LAYER_H
#define RUBIKSCUBE_LAYER_H

namespace Structure {
    class Layer {
    private:
        size_t size_;
        std::vector<std::vector<char>> state_;

    public:
        explicit Layer(const size_t size, const char color = 'W')
        : size_(size)
        , state_(size, std::vector<char>(size)) {
            for (auto &row : state_) {
                for (auto &block : row) {
                    block = color;
                }
            }
        };

        Layer(const Layer &cpd)//NOLINT
        : size_(cpd.size_)
        , state_(cpd.state_) {}

        /// Rotate layer clockwise
        void rotate_clockwise() {
            std::vector<std::vector<char>> source_state = state_;

            for (size_t edge = 0; edge < std::floor(size_ / 2); ++edge) {
                for (size_t i = 0; i < size_; ++i) {
                    // Up to right_
                    state_[i][size_ - edge - 1] = source_state[edge][i];
                    // right_ to bottom_
                    state_[size_ - edge - 1][size_ - i - 1] = source_state[i][size_ - edge - 1];
                    // bottom_ to left_
                    state_[i][edge] = source_state[size_ - edge - 1][i];
                    // left_ to top_
                    state_[edge][i] = source_state[size_ - i - 1][edge];
                }
            }
        }

        /// Rotate layer counter clockwise
        void rotate_counter_clockwise() {
            std::vector<std::vector<char>> source_state = state_;

            for (size_t edge = 0; edge < std::floor(size_ / 2); ++edge) {
                for (size_t i = 0; i < size_; ++i) {
                    // right_ to up
                    state_[edge][i] = source_state[i][size_ - edge - 1];
                    // bottom_ to right_
                    state_[i][size_ - edge - 1] = source_state[size_ - edge - 1][size_ - i - 1];
                    // left_ to bottom_
                    state_[size_ - edge - 1][i] = source_state[i][edge];
                    // top_ to left_
                    state_[size_ - i - 1][edge] = source_state[edge][i];
                }
            }
        }

        void setup_col(const std::vector<char> &new_column, size_t column_index) {
            for (size_t i = 0; i < size_; ++i) {
                state_[i][column_index] = new_column[i];
            }
        }

        void setup_row(const std::vector<char> &new_row, size_t row_index) {
            state_[row_index] = new_row;
        }

        [[nodiscard]] std::vector<char> get_col(size_t index) const {
            std::vector<char> column(size_);
            for (size_t i = 0; i < size_; ++i) {
                column[i] = state_[i][index];
            }
            return column;
        }

        [[nodiscard]] std::vector<char> get_row(size_t index) const {
            return state_[index];
        }

        [[nodiscard]] std::vector<std::vector<char>> get_state() const {
            return state_;
        }

        bool operator==(const Layer &layer) const {
            for (size_t row = 0; row < size_; ++row) {
                for (size_t col = 0; col < size_; ++col) {
                    if (state_[row][col] != layer.state_[row][col])
                        return false;
                }
            }
            return true;
        }
    };
}

#endif //RUBIKSCUBE_LAYER_H
