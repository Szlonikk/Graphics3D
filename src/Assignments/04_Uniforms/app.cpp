//
// Created by pbialas on 25.09.2020.
//


#include "app.h"

#include <vector>

#include "spdlog/spdlog.h"

#include "glad/gl.h"

#include "Application/utils.h"

void SimpleShapeApplication::init() {
    /*
     * A utility function that reads the shaders' source files, compiles them and creates the program object,
     * as everything in OpenGL we reference the program by an integer "handle".
     * The input to this function is a map that associates a shader type (GL_VERTEX_SHADER and GL_FRAGMENT_SHADER) with
     * its source file. The source files are located in the PROJECT_DIR/shaders directory, where  PROJECT_DIR is the
     * current assignment directory (e.g., src/Assignments/Triangle).
     */
    auto program = xe::utils::create_program(
            {
                    {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
                    {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}
            });


    if (!program) {
        SPDLOG_CRITICAL("Invalid program");
        exit(-1);
    }

    
    std::vector<GLfloat> vertices = {
        -0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, //0
         0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, //1
        -0.5f,  0.0f, 0.0f,    0.0f, 1.0f, 0.0f, //2
         0.5f,  0.0f, 0.0f,    0.0f, 1.0f, 0.0f, //3
        -0.5f,  0.0f, 0.0f,    1.0f, 0.0f, 0.0f, //4
         0.5f,  0.0f, 0.0f,    1.0f, 0.0f, 0.0f, //5
         0.0f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f, //6
    };

    std::vector<GLubyte> indices = {
        0, 1, 2,
        1, 3, 2,
        4, 5, 6,
    };
    
    GLuint v_buffer_handle;
    OGL_CALL(glCreateBuffers(1, &v_buffer_handle));
    OGL_CALL(glNamedBufferData(v_buffer_handle, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW));
    
    
    GLuint i_buffer_handle;
    OGL_CALL(glCreateBuffers(1, &i_buffer_handle));
    OGL_CALL(glNamedBufferData(i_buffer_handle, indices.size() * sizeof(GLubyte), indices.data(), GL_STATIC_DRAW));

    
    OGL_CALL(glGenVertexArrays(1, &vao_));
    
    
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle));
    
    OGL_CALL(glEnableVertexAttribArray(0));
    OGL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   reinterpret_cast<GLvoid *>(0)));

    OGL_CALL(glEnableVertexAttribArray(1));
    OGL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   reinterpret_cast<GLvoid *>(3*sizeof(GLfloat))));



    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    OGL_CALL(glBindVertexArray(0));
    
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.81f, 1.0f));

    float strength = 0.5f;
    float mix_color[3] = {0.0f, 0.0f, 1.0f}; // Blue color

    GLuint ubo_handle;
    OGL_CALL(glCreateBuffers(1, &ubo_handle));
 
    OGL_CALL(glNamedBufferData(ubo_handle, 8 * sizeof(float), nullptr, GL_DYNAMIC_DRAW));

    // Strength(4 bytes), padding (12 bytes) mix_color (12 bytes)
    OGL_CALL(glNamedBufferSubData(ubo_handle, 0, sizeof(float), &strength)); // strength
    OGL_CALL(glNamedBufferSubData(ubo_handle, 16, 3 * sizeof(float), mix_color)); // mix_color

    // Bind the buffer to the uniform block (binding = 0)
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_handle));


    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));

    OGL_CALL(glUseProgram(program));
}

void SimpleShapeApplication::frame() {
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(0))); 
    OGL_CALL(glBindVertexArray(0));
}
