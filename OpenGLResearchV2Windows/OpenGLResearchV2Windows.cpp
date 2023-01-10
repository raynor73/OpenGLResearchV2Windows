// OpenGLResearchV2Windows.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include <GL/glew.h>
#include "framework.h"
#include "OpenGLResearchV2Windows.h"
#include <third_party/imgui/imgui.h>
#include <third_party/imgui/imgui_impl_glfw.h>
#include <third_party/imgui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <platform_dependent/windows/utils.h>
#include <platform_dependent/windows/windows_app.h>
#include <platform_dependent/windows/windows_logger.h>
#include <game_engine/console.h>
#include <duktape.h>
#include <game_engine/constants.h>
#include <rendering_engine/opengl_error_detector.h>
#include <rendering_engine/opengl_shaders_repository.h>
#include "research/research_scene_001.h"
#include "research/research_scene_002.h"
#include <sstream>
#include <game_engine/utils.h>
#include <WinBase.h>
#include <stringapiset.h>
#include <platform_dependent/windows/windows_bitmap_data_source.h>
#include <platform_dependent/windows/windows_fs_abstraction.h>
#include <platform_dependent/windows/windows_bitmap_loader.h>
#include <platform_dependent/windows/windows_read_only_fs_abstraction.h>
#include <iomanip>

using namespace GameEngine;
using namespace GameEngine::Utils;
using namespace Windows::Utils;
using namespace std;
using namespace Gdiplus;

#define DEBUG_OPENGL
#define CONSOLE_BUFFER_SIZE 1024
#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 900

static ULONG_PTR g_gdiPlusToken;

static shared_ptr<WindowsApp> g_app;
static shared_ptr<Console> g_console;
static duk_context* g_duktapeContext;
static shared_ptr<OpenGLErrorDetector> g_openGLErrorDetector;
static shared_ptr<OpenGLShadersRepository> g_openGLShadersRepository;
static shared_ptr<BitmapDataSource> g_bitmapDataSource;
static shared_ptr<FsAbstraction> g_fsAbstraction;
static shared_ptr<BitmapLoader> g_bitmapLoader;
static shared_ptr<ReadOnlyFsAbstraction> g_readOnlyFsAbstraction;

static bool g_isShiftPressed = false;
static bool g_isErrorLogged = false;

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

static void GLAPIENTRY openGLDebugCallback(
    GLenum source,
    GLenum type, 
    GLuint id,
    GLenum severity, 
    GLsizei length, 
    const GLchar* message, 
    const void* userParam
) {
    stringstream ss;
    ss << "Debug Message: SOURCE(0x" << setfill('0') << setw(4) << hex << source << "), ";
    ss << "TYPE(0x" << setfill('0') << setw(4) << hex << type << "), ";
    ss << "ID(0x" << setfill('0') << setw(8) << hex << id << "), ";
    ss << "SEVERITY(0x" << setfill('0') << setw(4) << hex << severity << "), ";
    ss << message;
    L::d("OpenGL", ss.str());
    /*printf("Debug Message: SOURCE(0x%04X),"
        "TYPE(0x%04X),"
        "ID(0x%08X),"
        "SEVERITY(0x%04X), \"%s\"\n",
        source, type, id, severity, message);*/
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG_OPENGL
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

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

#ifdef DEBUG_OPENGL
    glDebugMessageCallback(openGLDebugCallback, nullptr);
#endif

    
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");



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

    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS) {
            g_app->requestExit();
        }
        break;

    default:
        break;
    }
}

static void initInput(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

static void printOpenGLInfo() {
    stringstream ss;
    GLint intValue;

    glGetIntegerv(GL_NUM_EXTENSIONS, &intValue);
    for (int i = 0; i < intValue; i++) {
        ss << "GL_EXTENSIONS[" << i << "]: " << reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
        L::d("OpenGL", ss.str());
        ss.str("");
        ss.clear();
    }

    logGLString("OpenGL", "GL_VENDOR", GL_VENDOR);
    logGLString("OpenGL", "GL_RENDERER", GL_RENDERER);
    logGLString("OpenGL", "GL_VERSION", GL_VERSION);
    logGLString("OpenGL", "GL_SHADING_LANGUAGE_VERSION", GL_SHADING_LANGUAGE_VERSION);

    logGLInteger("OpenGL", "GL_MAJOR_VERSION", GL_MAJOR_VERSION);
    logGLInteger("OpenGL", "GL_MINOR_VERSION", GL_MINOR_VERSION);
    logGLInteger64("OpenGL", "GL_MAX_SERVER_WAIT_TIMEOUT", GL_MAX_SERVER_WAIT_TIMEOUT);

    logGLInteger("OpenGL", "GL_MAX_VERTEX_ATTRIBS", GL_MAX_VERTEX_ATTRIBS);

    /*GLint profile;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
    std::cout << ((profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) != 0) << std::endl;*/

    logGLInteger("OpenGL", "GL_SAMPLE_BUFFERS", GL_SAMPLE_BUFFERS);
    if (glGetInteger(GL_SAMPLE_BUFFERS)) {
        logGLInteger("OpenGL", "GL_SAMPLES", GL_SAMPLES);
    }

    logGLInteger("OpenGL", "GL_MAX_RENDERBUFFER_SIZE", GL_MAX_RENDERBUFFER_SIZE);
    logGLInteger("OpenGL", "GL_MAX_COLOR_ATTACHMENTS", GL_MAX_COLOR_ATTACHMENTS);
}

static bool show_demo_window = true;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static void mainLoop(GLFWwindow* window) {
    printOpenGLInfo();

    /*DWORD pathSizeWideChar = GetCurrentDirectory(0, NULL);
    TCHAR* pathBufferWideChar = new TCHAR[uint64_t(pathSizeWideChar) + 1];
    GetCurrentDirectory(pathSizeWideChar, pathBufferWideChar);
    ULONG pathSize = WideCharToMultiByte(CP_ACP, 0, pathBufferWideChar, -1, NULL, 0, NULL, NULL);
    char* pathBuffer = new char[uint64_t(pathSize) + 1L];
    WideCharToMultiByte(CP_ACP, 0, pathBufferWideChar, -1, pathBuffer, 0, NULL, NULL);
    L::d("!@#", pathBuffer);
    delete[] pathBuffer;
    delete[] pathBufferWideChar;*/

    auto scene = make_shared<ResearchScene002>(
        g_openGLErrorDetector, 
        g_openGLShadersRepository,
        g_bitmapDataSource
    );
    //scene->start();

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
        if (g_openGLErrorDetector->isOpenGLErrorDetected()) {
            if (!g_isErrorLogged) {
                g_isErrorLogged = true;
                L::e(
                    Constants::LOG_TAG,
                    "Rendering stopped as OpenGL error has been detected"
                );
            }
        } else {
            //scene->update();

            /*glClear(GL_COLOR_BUFFER_BIT);

            glBegin(GL_TRIANGLES);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f(0.0f, 0.5f);
            glVertex2f(0.5f, -0.5f);
            glEnd();*/
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();



        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        if (g_app->isExitRequested()) {
            glfwDestroyWindow(window);
        }
    }
}

static void initGame() {
    L::setLogger(make_shared<WindowsLogger>());

    g_app = make_shared<WindowsApp>();
    g_console = make_shared<Console>();

    g_console->setCommandLineCallback([](string commandLine) {
        duk_push_string(g_duktapeContext, commandLine.c_str());
        if (duk_peval(g_duktapeContext) != 0) {
            cout << "Script error: " << duk_safe_to_string(g_duktapeContext, -1) << endl;
        }
        duk_pop(g_duktapeContext);
    });

    g_openGLErrorDetector = make_shared<OpenGLErrorDetector>();
    g_openGLShadersRepository = make_shared<OpenGLShadersRepository>(g_openGLErrorDetector);
    g_bitmapLoader = make_shared<WindowsBitmapLoader>();
    g_readOnlyFsAbstraction = make_shared<WindowsReadOnlyFsAbstraction>();
    g_fsAbstraction = make_shared<WindowsFsAbstraction>(g_readOnlyFsAbstraction);
    g_bitmapDataSource = make_shared<WindowsBitmapDataSource>(g_bitmapLoader, g_fsAbstraction);
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
    GdiplusStartupInput gdiplusStartupInput;
    if (GdiplusStartup(&g_gdiPlusToken, &gdiplusStartupInput, NULL) != Ok) {
        showDialog(
            getString(hInstance, IDS_GENERIC_ERROR_TITLE).get(),
            getString(hInstance, IDS_ERROR_INITIALIZING_GDI_PLUS).get()
        );
        return EXIT_FAILURE;
    }

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
