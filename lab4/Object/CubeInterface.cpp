//
// Created by dev on 18.05.2022.
//

#include "CubeInterface.h"

namespace Object {
    void CubeInterface::keys(unsigned char key, int, int) {
        keystates[key] = true;
        if (current == -1 && !keystates['c'] && keystates['l'] && key >= '0' && key <= '9') {
            current = key - '0';
            current_origin = 'L';
            Rotate(key - '0', -CUBE_TRANSITION_STEP);
            display();
        }
        if (current == -1 && keystates['c'] && keystates['l'] && key >= '0' && key <= '9') {
            current = key - '0';
            current_origin = 'l';
            Rotate(key - '0', CUBE_TRANSITION_STEP);
            display();
        }

        if (current == -1 && !keystates['c'] && keystates['f'] && key >= '0' && key <= '9') {
            current = key - '0';
            current_origin = 'F';
            Rotate(key - '0', -CUBE_TRANSITION_STEP);
            display();
        }
        if (current == -1 && keystates['c'] && keystates['f'] && key >= '0' && key <= '9') {
            current = key - '0';
            current_origin = 'f';
            Rotate(key - '0', CUBE_TRANSITION_STEP);
            display();
        }

        if (current == -1 && !keystates['c'] && keystates['u'] && key >= '0' && key <= '9') {
            current = key - '0';
            current_origin = 'U';
            Rotate(key - '0', -CUBE_TRANSITION_STEP);
            display();
        }
        if (current == -1 && keystates['c'] && keystates['u'] && key >= '0' && key <= '9') {
            current = key - '0';
            current_origin = 'u';
            Rotate(key - '0', CUBE_TRANSITION_STEP);
            display();
        }
    }

    void CubeInterface::specialKeys(int key, int, int) {
        keystates[key] = true;

        if (keystates[GLUT_KEY_DOWN]) {
            xRot += CUBE_TRANSITION_STEP;
            if (xRot >= 360)
                xRot -= 360;
            glutPostRedisplay();
        }

        if (keystates[GLUT_KEY_UP]) {
            xRot -= CUBE_TRANSITION_STEP;
            if (xRot < 0)
                xRot += 360;
            glutPostRedisplay();
        }

        if (keystates[GLUT_KEY_RIGHT]) {
            zRot += CUBE_TRANSITION_STEP;
            if (zRot >= 360)
                zRot -= 360;
            glutPostRedisplay();
        }

        if (keystates[GLUT_KEY_LEFT]) {
            zRot -= CUBE_TRANSITION_STEP;
            if (zRot < 0)
                zRot += 360;
            glutPostRedisplay();
        }

        if (keystates[GLUT_KEY_HOME]) {
            translateZ += CUBE_TRANSITION_STEP;
            glutPostRedisplay();
        }

        if (keystates[GLUT_KEY_END]) {
            translateZ -= CUBE_TRANSITION_STEP;
            glutPostRedisplay();
        }

        if (keystates[GLUT_KEY_F1]) {
            if (current == -1 && !assembly_state)
                assembly();
            glutPostRedisplay();
        }

        if (keystates[GLUT_KEY_F2]) {
            if (current == -1 && !scramble_state)
                scramble();
            glutPostRedisplay();
        }

        if (keystates[GLUT_KEY_F3]) {
            load_state();
        }

        if (keystates[GLUT_KEY_F5]) {
            save_state();
        }
    }

    void CubeInterface::timer(int) {
        glutTimerFunc(90 / CUBE_TRANSITION_STEP, staticTimer, 0);

        if (current != -1 && current_origin == 'L')
            Rotate(current, -CUBE_TRANSITION_STEP);
        if (current != -1 && current_origin == 'l')
            Rotate(current, CUBE_TRANSITION_STEP);

        if (current != -1 && current_origin == 'F')
            Rotate(current, -CUBE_TRANSITION_STEP);
        if (current != -1 && current_origin == 'f')
            Rotate(current, CUBE_TRANSITION_STEP);

        if (current != -1 && current_origin == 'U')
            Rotate(current, -CUBE_TRANSITION_STEP);
        if (current != -1 && current_origin == 'u')
            Rotate(current, CUBE_TRANSITION_STEP);

        if (current == -1 && scramble_state) {
            scramble();
        }

        if (current == -1 && assembly_state) {
            assembly();
        }

        display();
    }

    bool CubeInterface::imitate_key_pressed(char origin, size_t index) {
        if (current != -1)
            return false;

        current = (int) index;
        current_origin = origin;
        if (origin == 'L' || origin == 'F' || origin == 'U') {
            Rotate(index, -CUBE_TRANSITION_STEP);
            display();
        } else if (origin == 'l' || origin == 'f' || origin == 'u') {
            Rotate(index, CUBE_TRANSITION_STEP);
            display();
        }
        return true;
    }

    void CubeInterface::display() {
        glPushMatrix();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1, 0, 0);

        float LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
        float LightEmission[] = {0.0f, 0.0f, 0.8f, 1.0f};
        float LightDiffuse[] = {0.1, 0.1f, 0.1f, 1.0f};
        float LightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        float LightDirection[] = {-0.5f, -0.5f, -0.5f};
        set_material();
        glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, LightDirection);
        glLightfv(GL_LIGHT0, GL_EMISSION, LightEmission);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glTranslatef(GLfloat(0), GLfloat(0), GLfloat(translateZ));
        glRotatef(GLfloat(xRot), GLfloat(1), GLfloat(0), GLfloat(0));
        glRotatef(GLfloat(yRot), GLfloat(0), GLfloat(1), GLfloat(0));
        glRotatef(GLfloat(zRot), GLfloat(0), GLfloat(0), GLfloat(1));
        glTranslatef(object_size / -2.0, object_size / -2.0, object_size / -2.0);

        draw();

        glPopMatrix();
        glutSwapBuffers();
    }

    void CubeInterface::set_material() {
        float MatAmbient[] = CUBE_MATERIAL_AMBIENT;
        float MatDiffuse[] = CUBE_MATERIAL_DIFFUSE;
        float MatSpecular[] = CUBE_MATERIAL_SPECULAR;
        float MatShininess = CUBE_MATERIAL_SHINESS;
        float black[] = CUBE_MATERIAL_BLACK;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    }
}
