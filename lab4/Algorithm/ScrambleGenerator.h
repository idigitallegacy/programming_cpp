//
// Created by dev on 25.04.2022.
//
#include <random>
#include <functional>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <exception>

#include "../CubeHeaders.h"

#ifndef RUBIKSCUBE_SCRAMBLEGENERATOR_H
#define RUBIKSCUBE_SCRAMBLEGENERATOR_H

#include <cmath>

namespace Algorithm {
    class Scramble {
    private:
        struct Move {
            uint8_t origin:3;
            uint8_t index:5;
        };

        union NMoves_frame { // Little endian
            uint8_t bytearray[2];
            uint16_t value;
        };

        union Move_frame { // Big endian
            uint8_t bytearray;
            Move value;
        };

        NMoves_frame n_moves{};
        const uint8_t splitter_frame = 0x89;

    public:
        enum ScrambleException {
            STATE_OVERFLOW = 0,
            INDEX_OVERFLOW,
            UNDEFINED_ORIGIN,
            UNDEFINED_FILE_FORMAT
        };
        void generate(std::ofstream &to, size_t size) {
            n_moves.value = (uint8_t) (CUBE_NMOVES);

            // Header
            std::vector<uint8_t> ostream = {n_moves.bytearray[1], n_moves.bytearray[0], splitter_frame};

            // Random
            std::random_device r_origin;
            std::random_device r_index;

            std::mt19937 mt_origin(r_origin());
            std::mt19937 mt_index(r_index());

            // TMP frame
            Move_frame temp_frame{};

            for (uint16_t i = 0; i < n_moves.value; ++i) {
                // Generation
                std::uniform_real_distribution<double> origin_generator(0, 6);
                std::uniform_real_distribution<double> index_generator(0, size);

                // Definition
                auto origin = (uint8_t) origin_generator(mt_origin);
                auto index = (uint8_t) index_generator(mt_index);

                temp_frame.value.origin = origin;
                temp_frame.value.index = index;

                ostream.push_back(temp_frame.bytearray);
            }

            for (unsigned char i : ostream) {
                to.put((char) i);
            }
        }

        void generate(std::ofstream &to, const std::vector<std::string> &from, size_t size) {
            n_moves.value = (uint8_t) (CUBE_NMOVES);

            if (from.size() > n_moves.value)
                throw std::exception("State exceeds configured limit. Aborted.", ScrambleException::STATE_OVERFLOW);

            n_moves.value = from.size();

            // Header
            std::vector<uint8_t> ostream = {n_moves.bytearray[1], n_moves.bytearray[0], splitter_frame};

            // TMP
            Move_frame temp_frame{};
            std::string temp_index;

            for (uint16_t i = 0; i < n_moves.value; ++i) {
                std::string move = from[i];
                temp_index = {move[0], move[1]};
                char origin = move[2];

                int index = std::stoi(temp_index);
                if (index >= size)
                    throw std::exception("Rotation index exceeds cube's size. Aborted.", ScrambleException::INDEX_OVERFLOW);

                switch (origin) {
                    case 'F': {
                        origin = 0x0;
                        break;
                    }
                    case 'f': {
                        origin = 0x1;
                        break;
                    }
                    case 'L': {
                        origin = 0x2;
                        break;
                    }
                    case 'l': {
                        origin = 0x3;
                        break;
                    }
                    case 'U': {
                        origin = 0x4;
                        break;
                    }
                    case 'u': {
                        origin = 0x5;
                        break;
                    }
                    default: {
                        throw std::exception("Undefined origin character. Aborted.", ScrambleException::UNDEFINED_ORIGIN);
                    }
                }

                temp_frame.value.origin = origin;
                temp_frame.value.index = index;

                ostream.push_back(temp_frame.bytearray);
            }

            for (unsigned char i : ostream) {
                to.put((char) i);
            }
        }

        void read(std::ifstream &from, std::vector<std::string> &to) {
            to.clear();
            n_moves.bytearray[1] = from.get();
            n_moves.bytearray[0] = from.get();

            if (from.get() != splitter_frame)
                throw std::exception("Undefined file format. Aborted.", ScrambleException::UNDEFINED_FILE_FORMAT);

            Move_frame temp_frame{};

            for (uint16_t i = 0; i < n_moves.value; ++i) {
                std::string move;

                temp_frame.bytearray = from.get();
                if (temp_frame.value.index < 10)
                    move += '0';
                move += std::to_string(temp_frame.value.index);
                switch (temp_frame.value.origin) {
                    case 0x0: {
                        move += 'F';
                        break;
                    }
                    case 0x1: {
                        move += 'f';
                        break;
                    }
                    case 0x2: {
                        move += 'L';
                        break;
                    }
                    case 0x3: {
                        move += 'l';
                        break;
                    }
                    case 0x4: {
                        move += 'U';
                        break;
                    }
                    case 0x5: {
                        move += 'u';
                        break;
                    }
                    default: {
                        throw std::exception("Undefined origin character. Aborted.", ScrambleException::UNDEFINED_ORIGIN);
                    }
                }
                to.push_back(move);
            }
        }
    };
}

#endif //RUBIKSCUBE_SCRAMBLEGENERATOR_H
