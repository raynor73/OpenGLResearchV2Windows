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
static duk_context* g_duktapeContext;

static bool g_isShiftPressed = false;

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

    case GLFW_KEY_E:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'e';
            g_console->processCharacter(Console::Character::letter_e);
        }
        break;

    case GLFW_KEY_I:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'i';
            g_console->processCharacter(Console::Character::letter_i);
        }
        break;

    case GLFW_KEY_L:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'l';
            g_console->processCharacter(Console::Character::letter_l);
        }
        break;

    case GLFW_KEY_N:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'n';
            g_console->processCharacter(Console::Character::letter_n);
        }
        break;

    case GLFW_KEY_P:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'p';
            g_console->processCharacter(Console::Character::letter_p);
        }
        break;

    case GLFW_KEY_Q:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'q';
            g_console->processCharacter(Console::Character::letter_q);
        }
        break;

    case GLFW_KEY_R:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'r';
            g_console->processCharacter(Console::Character::letter_r);
        }
        break;

    case GLFW_KEY_S:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 's';
            g_console->processCharacter(Console::Character::letter_s);
        }
        break;

    case GLFW_KEY_T:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 't';
            g_console->processCharacter(Console::Character::letter_t);
        }
        break;

    case GLFW_KEY_U:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'u';
            g_console->processCharacter(Console::Character::letter_u);
        }
        break;

    case GLFW_KEY_X:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << 'x';
            g_console->processCharacter(Console::Character::letter_x);
        }
        break;

    case GLFW_KEY_PERIOD:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << '.';
            g_console->processCharacter(Console::Character::period);
        }
        break;

    case GLFW_KEY_SEMICOLON:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            cout << ';';
            g_console->processCharacter(Console::Character::semicolon);
        }
        break;

    case GLFW_KEY_MINUS:
        if (g_isShiftPressed) {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                cout << '_';
                g_console->processCharacter(Console::Character::underscore);
            }
        }
        else {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                cout << '-';
                g_console->processCharacter(Console::Character::minus);
            }
        }
        break;

    case GLFW_KEY_9:
        if (g_isShiftPressed) {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                cout << '(';
                g_console->processCharacter(Console::Character::left_parenthesis);
            }
        }
        else {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                cout << '9';
                g_console->processCharacter(Console::Character::number_9);
            }
        }
        break;

    case GLFW_KEY_0:
        if (g_isShiftPressed) {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                cout << ')';
                g_console->processCharacter(Console::Character::right_parenthesis);
            }
        }
        else {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                cout << '0';
                g_console->processCharacter(Console::Character::number_0);
            }
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

    case GLFW_KEY_LEFT_SHIFT:
        if (action == GLFW_PRESS) {
            g_isShiftPressed = true;
        } else if (action == GLFW_RELEASE) {
            g_isShiftPressed = false;
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

        if (g_app->isExitRequested()) {
            glfwDestroyWindow(window);
        }
    }
}

static void initGame() {
    L::setLogger(make_shared<WindowsLogger>());

    //auto serviceLocator = make_shared<ServiceLocator>();

    g_app = make_shared<WindowsApp>();
    g_console = make_shared<Console>();

    g_console->setCommandLineCallback([](string commandLine) {
        duk_push_string(g_duktapeContext, commandLine.c_str());
        if (duk_peval(g_duktapeContext) != 0) {
            cout << "Script error: " << duk_safe_to_string(g_duktapeContext, -1) << endl;
        }
        duk_pop(g_duktapeContext);
    });
}

static duk_ret_t native_print(duk_context* ctx) {
    printf("%s", duk_to_string(ctx, 0));
    return 0;
}

static duk_ret_t native_println(duk_context* ctx) {
    printf("%s\n", duk_to_string(ctx, 0));
    return 0;
}

static duk_ret_t native_requestExit(duk_context* ctx) {
    g_app->requestExit();

    return 0;
}

static void setupScriptLanguage() {
    g_duktapeContext = duk_create_heap_default();
    
    duk_push_c_function(g_duktapeContext, native_print, 1);
    duk_put_global_string(g_duktapeContext, "print");
    
    duk_push_c_function(g_duktapeContext, native_println, 1);
    duk_put_global_string(g_duktapeContext, "println");

    {
        duk_idx_t appObjectId;

        appObjectId = duk_push_object(g_duktapeContext);
        duk_push_c_function(g_duktapeContext, native_requestExit, 0);
        duk_put_prop_string(g_duktapeContext, appObjectId, "exit");
        duk_put_global_string(g_duktapeContext, "app");
    }
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

    setupScriptLanguage();

    mainLoop(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}
