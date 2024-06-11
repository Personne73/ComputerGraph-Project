#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <math.h>

// g++ -o testgl testgl.cpp -framework OpenGL -framework glut : compile
// ./testgl : execute

void Render()
{
    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(0, 960, 0, 540);
    gluPerspective(45.0f, 960.0f / 540.0f, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 1.0f, 0.0f, // Position de la caméra
              0.0f, 0.0f, -1.0f, // Position du regard
              0.0f, 1.0f, 0.0f); // Vecteur d'orientation

    // Triangle à sa position orthocentrée avec facteur d'échelle pour le rendre visible
    glPushMatrix();
    //glScalef(100.0f, 100.0f, 1.0f); // Appliquer un facteur d'échelle
    glTranslatef(0.f, 0.f, -1.0f); // Centrer le triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-0.5f, -0.5f);
    glEnd();
    glPopMatrix();


    // viewport 1
    //glViewport(0, 0, 480, 270);
    //glScissor(0, 0, 480, 270);
    //glEnable(GL_SCISSOR_TEST);
    //glClearColor(1.f, 0.f, 0.f, 1.f);
    //glClear(GL_COLOR_BUFFER_BIT);

    //glLoadIdentity();
    //glScalef(1.5f, 1.5f, 1.5f);
    //glTranslatef(0.5f, 0.0f, 0.0f);
//    int time = glutGet(GLUT_ELAPSED_TIME);
//    float angle = (time / 1000.0f) * 71.0f;
//
//    glPushMatrix();
//    float matrix[16] = {
//        cos(angle), -sin(angle), 0.0, 0.0,
//        sin(angle), cos(angle), 0.0, 0.0,
//        0.0, 0.0, 1.0, 0.0,
//        0.0, 0.0, 0.0, 1.0
//    };
//
//    glMultMatrixf(matrix);
//    //glRotatef(angle, 0.f, 0.f, 1.f);
//    glBegin(GL_TRIANGLES);
//    glColor3f(1.0f, 0.0f, 0.0f);
//    glVertex2f(0.0f, 0.5f);
//    glColor3f(0.0f, 1.0f, 0.0f);
//    glVertex2f(0.5f, -0.5f);
//    glColor3f(0.0f, 0.0f, 1.0f);
//    glVertex2f(-0.5f, -0.5f);
//    glEnd();
//    glPopMatrix();
    //glDisable(GL_SCISSOR_TEST);

    // viewport 2
    /*glViewport(480, 0, 480, 270);
    glScissor(480, 0, 480, 270);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.f, 1.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.5f, 0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.5f, 0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-0.5f, -0.5f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(0.5f, -0.5f);
    glEnd();
    glDisable(GL_SCISSOR_TEST);*/

    // viewport 3
    /*glViewport(0, 270, 480, 270);
    glScissor(0, 270, 480, 270);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.f, 0.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-0.5f, -0.5f);
    glEnd();
    glDisable(GL_SCISSOR_TEST);*/


    // viewport 4
    /*glViewport(480, 270, 480, 270);
    glScissor(480, 270, 480, 270);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(1.f, 1.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-0.5f, -0.5f);
    glEnd();
    glDisable(GL_SCISSOR_TEST);*/
}

// la ‘callback’ executee par glutDisplayFunc()
void Display()
{
    Render();
    glutSwapBuffers();
    // glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(960,540);
    glutCreateWindow("Triangle");
    //glEnable(GL_CULL_FACE);
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}