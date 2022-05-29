//
// Created by dev on 02.05.2022.
//
#include <GL/glut.h>
#include <functional>
#include <ctime>

#ifndef RUBIKSCUBE_DRAW_H
#define RUBIKSCUBE_DRAW_H

namespace Object {
    class DrawableInterface {
    private:

        virtual void keys(unsigned char key, int, int) = 0;

        void keysUp(int key, int, int) {
            keystates[key] = false;
        }

        virtual void specialKeys(int key, int, int) = 0;

        void specialKeysUp(int key, int, int) {
            keystates[key] = false;
        }

        virtual void timer(int) = 0;

    protected:
        double translateZ = -30.0;
        int xRot = 110, yRot = 180, zRot = 30;

        const char *object_type = "Empty window";

        constexpr const static double object_size = 1;

        virtual void draw() = 0;

        virtual void display() = 0;

        virtual void set_material() {}

        static void staticDisplay() {
            if (app_instance) app_instance->display();
        }

        static void staticKeys(unsigned char key, int x, int y) {
            if (app_instance) app_instance->keys(key, x, y);
        }

        static void staticKeysUp(unsigned char key, int x, int y) {
            if (app_instance) app_instance->keysUp(key, x, y);
        }

        static void staticSpecialKeys(int key, int x, int y) {
            if (app_instance) app_instance->specialKeys(key, x, y);
        }

        static void staticSpecialKeysUp(int key, int x, int y) {
            if (app_instance) app_instance->specialKeysUp(key, x, y);
        }

        static void staticTimer(int x) {
            if (app_instance) app_instance->timer(x);
        }

        static void init(DrawableInterface &context) {
            glClearColor(0.1, 0.1, 0.1, 1);
            // инициализируем случайные числа
            srand(time(nullptr));

            context.set_material();
            // освещение
            glShadeModel(GL_SMOOTH);
            glEnable(GL_LIGHT0);
            glEnable(GL_COLOR_MATERIAL);
            glColorMaterial(GL_FRONT, GL_AMBIENT);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LIGHTING);
        }

        bool keystates[256]{};

        static void reshape(int w, int h) {
            glViewport(0, 0, w, h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            GLfloat fAspect = (GLfloat) w / (GLfloat) h;
            gluPerspective(60, fAspect, 1, 1000.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }

        static DrawableInterface *app_instance;

        DrawableInterface() = default;

    public:
        DrawableInterface(DrawableInterface &) = delete;

        DrawableInterface &operator=(DrawableInterface &) = delete;
    };
}


#endif //RUBIKSCUBE_DRAW_H
