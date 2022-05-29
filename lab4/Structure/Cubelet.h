//
// Created by dev on 26.04.2022.
//

#include <vector>

#ifndef RUBIKSCUBE_CUBELET_H
#define RUBIKSCUBE_CUBELET_H

namespace Structure {
    struct Cubelet {
    private:
        std::vector<unsigned char> color_;

    public:
        // Cubelet colors
        std::vector<unsigned int> color;
        // Cubelet size
        GLfloat size;

        explicit Cubelet(GLfloat _size)
        : color(6, 0)
        , size(_size)
        , color_(4, 0) {}

        // X0Y rotation
        void rotateZ() {
            unsigned int tmp = color[5];
            color[5] = color[3];
            color[3] = color[4];
            color[4] = color[2];
            color[2] = tmp;
        }

        // X0Z rotation
        void rotateY() {
            unsigned int tmp = color[2];
            color[2] = color[1];
            color[1] = color[3];
            color[3] = color[0];
            color[0] = tmp;
        }

        // Y0Z rotation
        void rotateX() {
            unsigned int tmp = color[0];
            color[0] = color[4];
            color[4] = color[1];
            color[1] = color[5];
            color[5] = tmp;
        }

        void setColor(int i, unsigned int _color) {
            if (i > color.size())
                return;
            color[i] = _color;
        }

        std::vector<unsigned char> at(int i) {
            color_[0] = color[i] >> 16;
            color_[1] = color[i] >> 8;
            color_[2] = color[i];
            return color_;
        }

        // Draw cube: set up color and perpendiculars
        void draw() {
            glPushMatrix();
            glBegin(GL_QUADS);

            // Up
            glColor3ubv(at(0).data());
            glNormal3f(0, 0, 1);
            glVertex3f(size, size, size);
            glVertex3f(0, size, size);
            glVertex3f(0, 0, size);
            glVertex3f(size, 0, size);

            // Down
            glColor3ubv(at(1).data());
            glNormal3f(0, 0, -1);
            glVertex3f(size, 0, 0);
            glVertex3f(0, 0, 0);
            glVertex3f(0, size, 0);
            glVertex3f(size, size, 0);

            // Front
            glColor3ubv(at(2).data());
            glNormal3f(0, -1, 0);
            glVertex3f(size, 0, size);
            glVertex3f(0, 0, size);
            glVertex3f(0, 0, 0);
            glVertex3f(size, 0, 0);

            // Back
            glColor3ubv(at(3).data());
            glNormal3f(0, 1, 0);
            glVertex3f(size, size, 0);
            glVertex3f(0, size, 0);
            glVertex3f(0, size, size);
            glVertex3f(size, size, size);

            // Left
            glColor3ubv(at(4).data());
            glNormal3f(-1, 0, 0);
            glVertex3f(0, size, size);
            glVertex3f(0, size, 0);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 0, size);

            // Right
            glColor3ubv(at(5).data());
            glNormal3f(1, 0, 0);
            glVertex3f(size, size, 0);
            glVertex3f(size, size, size);
            glVertex3f(size, 0, size);
            glVertex3f(size, 0, 0);

            glEnd();
            glPopMatrix();
        }

        // Draws cube with an offset (x, y, z)
        void draw(double x, double y, double z) {
            glPushMatrix();
            glTranslated(x, y, z);
            draw();
            glPopMatrix();
        }
    };
}

#endif //RUBIKSCUBE_CUBELET_H
