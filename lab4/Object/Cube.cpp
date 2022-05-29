//
// Created by dev on 28.04.2022.
//

#include "Cube.h"

namespace Object {
    void Cube::find_solution() {
        int move_counter = 0;
        while (move_counter < scramble_move_made_.size() && scramble_move_made_[move_counter])
            ++move_counter;
        std::vector<std::string> real_scramble_path_ = scramble_path_;
        real_scramble_path_.erase(real_scramble_path_.begin() + move_counter, real_scramble_path_.end());
        solution_path_ = real_scramble_path_;
        std::reverse(solution_path_.begin(), solution_path_.end());
        for (auto &move: solution_path_) {
            std::string index = {move[0], move[1]};
            char origin = move[2];
            switch (origin) {
                default: {
                    throw std::exception("Undefined rotation origin", CubeException::UNDEFINED_ORIGIN);
                }
                case 'U': {
                    move = index + 'u';
                    break;
                }
                case 'u': {
                    move = index + 'U';
                    break;
                }
                case 'L': {
                    move = index + 'l';
                    break;
                }
                case 'l': {
                    move = index + 'L';
                    break;
                }
                case 'F': {
                    move = index + 'f';
                    break;
                }
                case 'f': {
                    move = index + 'F';
                    break;
                }
            }
        }
        assemble_move_made_.resize(scramble_path_.size());
    }

    void Cube::change_state(const std::string &move) {
        std::string j = {move[0], move[1]};
        int index = std::stoi(j);
        char origin = move[2];

        switch (origin) {
            default: {
                throw std::exception("Undefined rotation origin", CubeException::UNDEFINED_ORIGIN);
            }
            case 'U': {
                cube_.rotate_row(index, true);
                break;
            }
            case 'u': {
                cube_.rotate_row(index, false);
                break;
            }
            case 'L': {
                cube_.rotate_column("left", index, true);
                break;
            }
            case 'l': {
                cube_.rotate_column("left", index, false);
                break;
            }
            case 'F': {
                cube_.rotate_column("front", index, true);
                break;
            }
            case 'f': {
                cube_.rotate_column("front", index, false);
                break;
            }
        }
    }

    void Cube::clear() {
        nmoves_ = 0;
        assembly_state = false;
        scramble_state = false;
        scramble_path_.clear();
        solution_path_.clear();
        scramble_move_made_.clear();
        assemble_move_made_.clear();

        current_rotate_angle = 0.0f;
        current = -1;

        int i, j, k;

        // Up
        for (i = 0; i < size_; ++i)
            for (j = 0; j < size_; ++j)
                cubelet_state_[i][j][size_ - 1].setColor(0, color_[0]);

        // Down
        for (i = 0; i < size_; ++i)
            for (j = 0; j < size_; ++j)
                cubelet_state_[i][j][0].setColor(1, color_[1]);

        // Front
        for (k = 0; k < size_; ++k)
            for (j = 0; j < size_; ++j)
                cubelet_state_[j][0][k].setColor(2, color_[2]);

        // Back
        for (k = 0; k < size_; ++k)
            for (j = 0; j < size_; ++j)
                cubelet_state_[j][size_ - 1][k].setColor(3, color_[3]);

        // Left
        for (i = 0; i < size_; ++i)
            for (k = 0; k < size_; ++k)
                cubelet_state_[0][k][i].setColor(4, color_[4]);

        // Right
        for (i = 0; i < size_; ++i)
            for (k = 0; k < size_; ++k)
                cubelet_state_[size_ - 1][k][i].setColor(5, color_[5]);

        // Setting up cubelet size
        // this is a 1/3 size multiplied by K < 1 to make a space between cubelets
        for (i = 0; i < size_; ++i)
            for (j = 0; j < size_; ++j)
                for (k = 0; k < size_; ++k)
                    cubelet_state_[i][j][k].size = (GLfloat) ((object_size / 3.0) * 0.9);
    }

    void Cube::draw() {
        // Drawing a core - black cube sized K * size
        glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(GLfloat(((object_size / 3.0) * size_) / 2),
                     GLfloat(((object_size / 3.0) * size_) / 2),
                     GLfloat(((object_size / 3.0) * size_) / 2));
        glutSolidCube(object_size * size_ * 0.2);
        glPopMatrix();

        // ok[i][j][k] displays the cubelet state: is it rotating or not.
        for (int i = 0; i < size_; ++i)
            for (int j = 0; j < size_; ++j)
                for (int k = 0; k < size_; ++k)
                    cubelet_rotation_state_[i][j][k] = true;

        if (current != -1) {
            glPushMatrix();

            size_t i, j, k;
            if (current_origin == 'U' || current_origin == 'u') {
                k = size_ - current - 1;
                // Layer state is 'rotating'
                for (i = 0; i < size_; ++i)
                    for (j = 0; j < size_; ++j)
                        cubelet_rotation_state_[i][j][k] = false;

                // Rotating edge: moving to the center, rotating and moving backwards
                glTranslated(GLfloat(((object_size / 3.0) * size_) / 2), GLfloat(((object_size / 3.0) * size_) / 2),
                             0);
                glRotatef(current_rotate_angle, 0, 0, 1);
                glTranslated(-GLfloat(((object_size / 3.0) * size_) / 2),
                             -GLfloat(((object_size / 3.0) * size_) / 2), 0);

                // Drawing cubelets offsetted to (size / 3) * (i, j, k)
                for (i = 0; i < size_; ++i)
                    for (j = 0; j < size_; ++j)
                        cubelet_state_[i][j][k].draw(GLfloat(object_size / 3 * i), GLfloat(object_size / 3 * j),
                                                     GLfloat(object_size / 3 * k));
            }

            if (current_origin == 'L' || current_origin == 'l') {
                i = size_ - current - 1;
                // Layer state is 'rotating'
                for (j = 0; j < size_; ++j)
                    for (k = 0; k < size_; ++k)
                        cubelet_rotation_state_[i][j][k] = false;

                // Rotating edge: moving to the center, rotating and moving backwards
                glTranslated(0, GLfloat(GLfloat(((object_size / 3.0) * size_) / 2)),
                             GLfloat(((object_size / 3.0) * size_) / 2));
                glRotatef(current_rotate_angle, 1, 0, 0);
                glTranslated(0, -GLfloat(((object_size / 3.0) * size_) / 2),
                             -GLfloat(((object_size / 3.0) * size_) / 2));

                // Drawing cubelets offsetted to (size / 3) * (i, j, k)
                for (j = 0; j < size_; ++j)
                    for (k = 0; k < size_; ++k)
                        cubelet_state_[i][j][k].draw(object_size / 3 * i, object_size / 3 * j, object_size / 3 * k);
            }
            if (current_origin == 'F' || current_origin == 'f') {
                j = size_ - current - 1;
                // Layer state is 'rotating'
                for (i = 0; i < size_; ++i)
                    for (k = 0; k < size_; ++k)
                        cubelet_rotation_state_[i][j][k] = false;

                // Rotating edge: moving to the center, rotating and moving backwards
                glTranslated(GLfloat(GLfloat(((object_size / 3.0) * size_) / 2)), 0,
                             GLfloat(((object_size / 3.0) * size_) / 2));
                glRotatef(current_rotate_angle, 0, 1, 0);
                glTranslated(-GLfloat(GLfloat(((object_size / 3.0) * size_) / 2)), 0,
                             -GLfloat(((object_size / 3.0) * size_) / 2));

                // Drawing cubelets offsetted to (size / 3) * (i, j, k)
                for (i = 0; i < size_; ++i)
                    for (k = 0; k < size_; ++k)
                        cubelet_state_[i][j][k].draw(object_size / 3 * i, object_size / 3 * j, object_size / 3 * k);
            }
            glPopMatrix();
        }

        for (int i = 0; i < size_; ++i)
            for (int j = 0; j < size_; ++j)
                for (int k = 0; k < size_; ++k)
                    if (cubelet_rotation_state_[i][j][k])
                        // Drawing cubelets that weren't rotated at the previous step
                        cubelet_state_[i][j][k].draw(object_size / 3 * i, object_size / 3 * j, object_size / 3 * k);
    }

    void Cube::rotate_cubelets(size_t index, int sign) {
        size_t i, j, k;
        // sign is a direction: sign == -1 is counter-clockwise (we should rotate 3 times)

        if (sign == -1 && current_origin != 'F' && current_origin != 'f')
            sign = 3;

        if (current_origin == 'F' || current_origin == 'f')
            sign += 2;

        while (sign--) {
            std::vector<std::vector<Structure::Cubelet>> tmp(
                    size_,std::vector<Structure::Cubelet>
                            (size_,Structure::Cubelet(object_size))
                             );
            if (current_origin == 'U' || current_origin == 'u') {
                // Left
                k = size_ - index - 1;
                for (i = 0; i < size_; ++i)
                    for (j = 0; j < size_; ++j)
                        tmp[j][size_ - i - 1] = cubelet_state_[i][j][k];

                // rotating it
                for (i = 0; i < size_; ++i)
                    for (j = 0; j < size_; ++j)
                        tmp[i][j].rotateZ(), cubelet_state_[i][j][k] = tmp[i][j];
            }
            if (current_origin == 'L' || current_origin == 'l') {
                // Front
                i = size_ - current - 1;
                for (j = 0; j < size_; ++j)
                    for (k = 0; k < size_; ++k)
                        tmp[k][size_ - j - 1] = cubelet_state_[i][j][k];

                // rotating it
                for (j = 0; j < size_; ++j)
                    for (k = 0; k < size_; ++k)
                        tmp[j][k].rotateY(), cubelet_state_[i][j][k] = tmp[j][k];
            }
            if (current_origin == 'F' || current_origin == 'f') {
                // Up
                j = size_ - current - 1;
                for (i = 0; i < size_; i++)
                    for (k = 0; k < size_; k++)
                        tmp[k][size_ - i - 1] = cubelet_state_[i][j][k];
                for (i = 0; i < size_; i++)
                    for (k = 0; k < size_; k++)
                        tmp[i][k].rotateX(), cubelet_state_[i][j][k] = tmp[i][k];
            }
        }
    }


    void Cube::Rotate(size_t index, float angle) {
        if (nmoves_ >= max_nmoves_) {
            std::cerr << "Moving cube unavailable: move limit has been reached.\n";
            return;
        }
        if (index >= size_) {
            std::cerr << "Index " << index + 1 << " exceeds cube's size: " << size_ << " x " << size_ << '\n';
            return;
        }
        if (current != -1) {
            current_rotate_angle += angle;
            if ((int) current_rotate_angle % 90 != 0)
                current = (int) index;
            else {
                if (current_rotate_angle < 0)
                    rotate_cubelets(index, 1);
                else
                    rotate_cubelets(index, -1);
                current = -1;
                current_rotate_angle = 0;

                std::string move;
                if (index < 10)
                    move += "0";
                move += std::to_string(index) + current_origin;

                if (!scramble_state && !assembly_state) {
                    scramble_path_.push_back(move);
                    scramble_move_made_.push_back(true);
                }
                change_state(move);
                ++nmoves_;
            }
        }
    }

    bool Cube::check_side(const std::vector<std::vector<char>> &side, char needle_color) {
        for (auto &row: side) {
            for (auto &color: row)
                if (color != needle_color)
                    return false;
        }
        return true;
    }

    bool Cube::check_if_assembled() {
        return check_side(cube_.get_state("left").get_state(), 'O') &&
               check_side(cube_.get_state("front").get_state(), 'G') &&
               check_side(cube_.get_state("right").get_state(), 'R') &&
               check_side(cube_.get_state("back").get_state(), 'B') &&
               check_side(cube_.get_state("top").get_state(), 'W') &&
               check_side(cube_.get_state("bottom").get_state(), 'Y');
    }

    void Cube::generate_scramble(const std::vector<std::string>& from) {
        Algorithm::Scramble scramble_generator;
        std::ofstream scramble_out("schema.scramble");

        if (from.empty()) {
            try {
                scramble_generator.generate(scramble_out, size_);
            } catch (std::exception &e) {
                std::cerr << "Scrambling failed: " << e.what() << '\n';
                throw std::exception("Scrambling exception", CubeException::INVALID_SCRAMBLE);
            }
        } else {
            try {
                scramble_generator.generate(scramble_out, from, size_);
            } catch (std::exception &e) {
                std::cerr << "Scrambling with vector:\n\t";
                for (auto &i : from)
                    std::cerr << i << ' ';
                std::cerr << '\n' << "Failed: " << e.what() << '\n';
                throw std::exception("Invalid scramble input", CubeException::INVALID_SCRAMBLE);
            }
        }

        scramble_out.close();
        std::ifstream scramble_in("schema.scramble");

        try {
            scramble_generator.read(scramble_in, scramble_path_);
        } catch (std::exception &e) {
            std::cerr << "Scramble reading failed: " << e.what() << '\n';
        }
        scramble_in.close();

        scramble_move_made_.resize(scramble_path_.size());
        assemble_move_made_.resize(scramble_path_.size());
    }

    void Cube::scramble() {
        if (scramble_path_.empty()) {
            generate_scramble(std::vector<std::string>{});
        }

        for (size_t i = 0; i < scramble_path_.size(); ++i) {
            if (draw_ && !scramble_move_made_[i]) {
                scramble_state = true;
                std::string move = scramble_path_[i];
                std::string j = {move[0], move[1]};
                int index = std::stoi(j);
                char origin = move[2];

                if (imitate_key_pressed(origin, index))
                    scramble_move_made_[i] = true;
                return;
            }
            if (i == scramble_path_.size() - 1) {
                scramble_state = false;
                return;
            }
            if (!draw_) {
                std::string move = scramble_path_[i];
                change_state(move);
            }
        }
    }

    void Cube::scramble(const std::vector<std::string> &from) {
        generate_scramble(from);
        scramble();
    }

    void Cube::assembly() {
        if (check_if_assembled()) {
            clear();
            return;
        }

        if (solution_path_.empty())
            find_solution();

        for (size_t i = 0; i < solution_path_.size(); ++i) {
            if (draw_ && !assemble_move_made_[i]) {
                assembly_state = true;

                std::string move = solution_path_[i];
                std::string j = {move[0], move[1]};
                int index = std::stoi(j);
                char origin = move[2];

                if (imitate_key_pressed(origin, index))
                    assemble_move_made_[i] = true;
                return;
            }
            if (i == solution_path_.size() - 1) {
                assembly_state = false;
                return;
            }
            if (!draw_) {
                std::string move = solution_path_[i];
                change_state(move);
            }
        }
    }

    void Cube::save_state() {
        Algorithm::Scramble scramble_saver;

        std::ofstream savefile("schema.scramble");
        try {
            scramble_saver.generate(savefile, scramble_path_, size_);
        } catch (std::exception &e) {
            std::cerr << "Saving state failed: " << e.what() << '\n';
            throw e;
        }
        savefile.close();
        std::cout << "State has been saved to ./schema.scramble\n";
    }

    void Cube::load_state() {
        if (!check_if_assembled())
            throw std::exception("Unable to load state from file: cube is not assembled.", UNABLE_TO_LOAD);
        Algorithm::Scramble scramble_loader;
        std::ifstream loadfile("schema.scramble");

        try {
            scramble_loader.read(loadfile, scramble_path_);
        } catch (std::exception &e) {
            std::cerr << "Loading state failed: " << e.what() << '\n';
            throw e;
        }
        scramble_move_made_.resize(scramble_path_.size());
        assemble_move_made_.resize(scramble_path_.size());
        scramble();
    }
}
