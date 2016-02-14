//
// Created by dar on 1/23/16.
//

#include "Application.h"
#include "window/Game.h"
#include "render/RenderManager.h"
#include "logging.h"

Application::Application() {
    this->renderer = new RenderManager();
    this->window = new Game();
    this->timer = new Timer();
    this->inputManager = new InputManager();
    this->reinit();
    this->resize(1366,750);
}

void Application::reinit() {
    this->renderer->init();
    this->renderer->initWindow(this->window); //TODO
    timer->GetDelta(); //if not called right now, first call in game loop would return a very huge value
    this->inputManager->reload();
}

void Application::update(bool dynamic) {
    if (dynamic) {
        double deltaTime = timer->GetDelta();
        this->accumulator += deltaTime;
        while (accumulator > TIME_STEP) {
            this->getCurrentWindow()->tick(TIME_STEP);
            this->accumulator -= TIME_STEP;
        }
    } else {
        this->getCurrentWindow()->tick(1.0/30);
    }
    this->renderer->render(this->getCurrentWindow());
    if (!MOBILE) this->handleEvents();
    this->inputManager->tick(this->getCurrentWindow());
}

Application::~Application() {
    delete this->window;
}

#ifdef __ANDROID__

#include <jni.h>

Application *application = nullptr;

extern "C" {
JNIEXPORT void JNICALL Java_tk_approach_android_spookytom_JniBridge_init(JNIEnv *env, jobject obj);
JNIEXPORT void JNICALL Java_tk_approach_android_spookytom_JniBridge_resize(JNIEnv *env, jobject obj, jint width, jint height);
JNIEXPORT void JNICALL Java_tk_approach_android_spookytom_JniBridge_tick(JNIEnv *env, jobject obj);
JNIEXPORT void JNICALL Java_tk_approach_android_spookytom_JniBridge_handleTouch(JNIEnv *env, jobject obj, jint i, jint action, jfloat x, jfloat y);
};

JNIEXPORT void JNICALL Java_tk_approach_android_spookytom_JniBridge_init(JNIEnv *env, jobject obj) {
    if (application == nullptr || application->getCurrentWindow() == nullptr) {
        application =  new Application();
    } else {
        application->reinit();
    }
    jclass cls = env->GetObjectClass(obj);
    jmethodID mid = env->GetMethodID(cls, "loadTexture", "()V");
    if (mid == 0) {
        return;
    }
    env->CallVoidMethod(obj, mid);
}

JNIEXPORT void JNICALL Java_tk_approach_android_spookytom_JniBridge_resize(JNIEnv *env, jobject obj, jint width, jint height) {
    application->resize(width, height);
}

JNIEXPORT void JNICALL Java_tk_approach_android_spookytom_JniBridge_tick(JNIEnv *env, jobject obj) {
    application->update(false);
}

JNIEXPORT void JNICALL Java_tk_approach_android_spookytom_JniBridge_handleTouch(JNIEnv *env, jobject obj, jint i,  jint action, jfloat x, jfloat y) {
    application->handleClick(i, action, x, y);
}

#endif //__ANDROID__

void Application::resize(int width, int height) {
    this->getCurrentWindow()->reload(width, height);
    this->renderer->resize(this->getCurrentWindow(), width, height);
}

void Application::handleClick(int i, int action, float x, float y) {
    this->inputManager->handleClick(i, action, x, y);
}

void Application::handleEvents() {
#ifndef __ANDROID__
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
            case SDL_QUIT:
                this->running = false;
                break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    this->resize((unsigned int) e.window.data1, (unsigned int) e.window.data2);
                }
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                this->inputManager->handleKeypress(&e);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONUP: {
                int x, y;
                SDL_GetMouseState(&x, &y);
                this->inputManager->handleClick(e.button.button, e.type == SDL_MOUSEBUTTONDOWN ? 0 : (e.type == SDL_MOUSEBUTTONUP ? 1 : 2), x, y);
                break;
            }
        }
    }
#else //__ANDROID__
    LOGW("SDL is not supported on this platform\n");
#endif //__ANDROID__

}
