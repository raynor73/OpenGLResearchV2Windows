// OpenGLResearchV2Windows.cpp : Defines the entry point for the application.
//

#include <GL/glew.h>
#include "framework.h"
#include "OpenGLResearchV2Windows.h"
#include <GLFW/glfw3.h>
#include <platform_dependent/windows/utils.h>
#include <platform_dependent/windows/windows_app.h>
#include <platform_dependent/windows/windows_logger.h>
#include <game_engine/console.h>
#include <duktape.h>

using namespace GameEngine;
using namespace Windows::Utils;
using namespace std;

#define CONSOLE_BUFFER_SIZE 1024
#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 900

static shared_ptr<WindowsApp> g_app;
static shared_ptr<Console> g_console;

static bool setupConsolse(HINSTANCE hInstance) {
    if (!createNewConsole(CONSOLE_BUFFER_SIZE)) {
        showDialog(
            getString(hInstance, IDS_GENERIC_ERROR_TITLE).get(),
            getString(hInstance, IDS_ERROR_OPENING_CONSOLE).get()
        );
        return false;
    }

    return true;
}

static GLFWwindow* initOpenGL(HINSTANCE hInstance) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        showDialog(
            getString(hInstance, IDS_GENERIC_ERROR_TITLE).get(),
            getString(hInstance, IDS_ERROR_INITIALIZING_GLFW).get()
        );
        return nullptr;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game Engine", NULL, NULL);
    if (!window) {
        showDialog(
            getString(hInstance, IDS_GENERIC_ERROR_TITLE).get(),
            getString(hInstance, IDS_ERROR_CREATING_MAIN_WINDOW).get()
        );

        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        showDialog(
            getString(hInstance, IDS_GENERIC_ERROR_TITLE).get(),
            getString(hInstance, IDS_ERROR_INITIALIZING_GLEW).get()
        );
        return nullptr;
    }

    return window;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_A:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'a';
            g_console->processCharacter(Console::Character::letter_a);
        }
        break;

    case GLFW_KEY_Q:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'q';
            g_console->processCharacter(Console::Character::letter_q);
        }
        break;

    case GLFW_KEY_BACKSPACE:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << "\b \b";
            g_console->processCharacter(Console::Character::backspace);
        }
        break;

    case GLFW_KEY_ENTER:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << '\n';
            g_console->processCharacter(Console::Character::enter);
        }
        break;

    default:
        break;
    }
    /*switch (key) {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_ESC, true);
        }
        else if (action == GLFW_RELEASE) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_ESC, false);
        }
        break;

    case GLFW_KEY_W:
        if (action == GLFW_PRESS) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_W, true);
        }
        else if (action == GLFW_RELEASE) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_W, false);
        }
        break;

    case GLFW_KEY_S:
        if (action == GLFW_PRESS) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_S, true);
        }
        else if (action == GLFW_RELEASE) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_S, false);
        }
        break;

    case GLFW_KEY_A:
        if (action == GLFW_PRESS) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_A, true);
        }
        else if (action == GLFW_RELEASE) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_A, false);
        }
        break;

    case GLFW_KEY_D:
        if (action == GLFW_PRESS) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_D, true);
        }
        else if (action == GLFW_RELEASE) {
            g_keyboardInput->setKeyPressed(KeyboardInput::KEY_D, false);
        }
        break;
    }*/
}

static void initInput(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

static void mainLoop(GLFWwindow* window) {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Game logic */
        /*g_mouseInput->update();
        auto activeScene = g_sceneManager->activeScene();
        if (activeScene != nullptr) {
            activeScene->update();
            g_renderingEngine->render(*activeScene);
        }*/

        /* Render here */
        /*glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        /*if (g_app->isExitRequested()) {
            glfwDestroyWindow(window);
        }*/
    }
}

static void initGame() {
    L::setLogger(make_shared<WindowsLogger>());

    //auto serviceLocator = make_shared<ServiceLocator>();

    g_app = make_shared<WindowsApp>();
    g_console = make_shared<Console>();

    g_console->setCommandLineCallback([](string commandLine) {
        cout << commandLine << endl;
    });

    duk_context* ctx = duk_create_heap_default();
    duk_eval_string(ctx, "1+2");
    printf("1+2=%d\n", (int)duk_get_int(ctx, -1));
    duk_destroy_heap(ctx);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    if (!setupConsolse(hInstance)) {
        return EXIT_FAILURE;
    }

    GLFWwindow* window = initOpenGL(hInstance);
    if (window == NULL) {
        return EXIT_FAILURE;
    }
    initInput(window);

    initGame();

    mainLoop(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}
