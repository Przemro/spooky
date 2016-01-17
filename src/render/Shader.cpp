//
// Created by dar on 11/20/15.
//

#include <vector>
#include "Shader.h"

using namespace std;

Shader::~Shader() {
    if (!this->isLoaded()) return;
    this->loaded = false;
    glDeleteShader(id);
}

bool Shader::load(const string &file, int type) {
    string file_path = file;
#ifdef __ANDROID__
    file_path = "/sdcard/c003/" + file;
#endif // __ANDROID__
    FILE *fp = fopen(file_path.c_str(), "rt");
    if (!fp) return false;
    this->type = type;
    vector<string> program_vec;
    char sLine[255];
#ifdef __ANDROID__
    if (type == GL_FRAGMENT_SHADER) {
        program_vec.push_back("precision mediump float;");
    }
#endif // __ANDROID__
    while (fgets(sLine, 255, fp))program_vec.push_back(sLine);
    fclose(fp);
    const char **program_str = new const char *[program_vec.size()];
    for (int i = 0; i < program_vec.size(); i++)
        program_str[i] = program_vec.at(i).c_str();

    id = glCreateShader(this->type);
    glShaderSource(id, program_vec.size(), program_str, NULL);
    glCompileShader(id);

    delete[] program_str;

    int compilationStatus;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compilationStatus);
    if (compilationStatus == GL_FALSE) return false;
    this->loaded = true;

    return this->loaded;
}

bool Shader::isLoaded() {
    return loaded;
}

GLuint Shader::getID() {
    return id;
}

