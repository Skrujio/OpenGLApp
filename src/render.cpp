#include <render.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <game.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace render {

    bool init_glfw() {
        if (glfwInit() == GLFW_FALSE) {
            std::cout << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        return true;
    }

    bool init_glad() {
        if (!gladLoadGL(glfwGetProcAddress))
        {
            glfwTerminate();
            std::cout << "Failed to initialize GLAD" << std::endl;
            return false;
        }

        return true;
    }

    GLFWwindow* create_window(WindowSettings settings) {
        GLFWwindow* window = glfwCreateWindow(settings.width, settings.height, settings.name, nullptr, nullptr);
        if (!window)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            return nullptr;
        }
        return window;
    }

    uint load_texture(const char* path) {
        int width {};
        int height {};
        int channels {};
        char* data = (char*)stbi_load(path, &width, &height, &channels, 0);
        if (!data) {
            std::cout << "Failed to load a texture" << std::endl;
            return 0;
        }

        uint texture_id {};
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
        return texture_id;
    }

    std::string read_file_to_string(const char* path) {
        std::ifstream file(path);
        if (file.fail()) {
            std::cout << "Failed to open file" << std::endl;
            return "error";
        }
        std::stringstream buffer;
        return buffer.str();
    }

    enum class Shader {
        vertex,
        geometry,
        fragment,
    };

    GLuint get_gl_shader_type(Shader type) {
        switch(type) {
            case Shader::vertex: return GL_VERTEX_SHADER;
            case Shader::geometry: return GL_GEOMETRY_SHADER;
            case Shader::fragment: return GL_FRAGMENT_SHADER;
        }
    }

    std::string get_string_shader_type(Shader type) {
        switch(type) {
            case Shader::vertex: return "VERTEX";
            case Shader::geometry: return "GEOMETRY";
            case Shader::fragment: return "FRAGMENT";
        }
    }

    void check_compile_errors(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    GLuint load_shader(const char* path, Shader type) {
        std::string shader_code_string = read_file_to_string(path);
        if (shader_code_string == "error") {
            std::cout << "Failed to read shader file" << std::endl;
            return 0;
        }

        GLuint shader_id = glCreateShader(get_gl_shader_type(type));
        const char* shader_code = shader_code_string.c_str();
        glShaderSource(shader_id, 1, &shader_code, nullptr);
        glCompileShader(shader_id);
        check_compile_errors(shader_id, get_string_shader_type(type));
        return shader_id;
    }

    GLuint load_shader_program(const char* vert_path, const char* frag_path) {
        GLuint vert_id = load_shader(vert_path, Shader::vertex);
        GLuint frag_id = load_shader(frag_path, Shader::fragment);
        GLuint program_id = glCreateProgram();
        
        glAttachShader(program_id, vert_id);
        glAttachShader(program_id, frag_id);
        glLinkProgram(program_id);
        check_compile_errors(program_id, "PROGRAM");
        
        glDeleteShader(vert_id);
        glDeleteShader(frag_id);
        return program_id;
    }

    GLuint load_shader_program(const char* vert_path, const char* geom_path, const char* frag_path) {
        GLuint vert_id = load_shader(vert_path, Shader::vertex);
        GLuint geom_id = load_shader(geom_path, Shader::geometry);
        GLuint frag_id = load_shader(frag_path, Shader::fragment);
        GLuint program_id = glCreateProgram();
        
        glAttachShader(program_id, vert_id);
        glAttachShader(program_id, geom_id);
        glAttachShader(program_id, frag_id);
        glLinkProgram(program_id);
        check_compile_errors(program_id, "PROGRAM");
        
        glDeleteShader(vert_id);
        glDeleteShader(geom_id);
        glDeleteShader(frag_id);
        return program_id;
    }

    GLFWwindow* init(WindowSettings settings) {
        if (!init_glfw()) {
            return nullptr;
        }

        GLFWwindow* window = create_window(settings);
        if (!window) {
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);
        
        if (!init_glad()) {
            return nullptr;
        }

        return window;
    }

    void render(GLFWwindow* window, State settings, double render_delta_time, game::State state) {
        glClearColor(settings.clear_color.x, settings.clear_color.y, settings.clear_color.z, settings.clear_color.w);
        glClear(settings.clear_flags);

        glfwSwapBuffers(window);
    }
}