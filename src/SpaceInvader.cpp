#include <cstdio>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utils/colors_transform.h"
#include "structs/buffer.cpp"
#include "structs/sprite.cpp"

void errorCallback(int error, const char* description)
{
  std::cerr << "Error [" << error << "] : " << description << std::endl;
}

uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b)
{
  return (r << 24) | (g << 16) | (b << 8) | 255;
}
void validate_shader(GLuint shader, const char* file = 0)
{
  static const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  GLsizei length = 0;

  glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

  if (length > 0)
  {
    printf("Shader %d(%s) compile error: %s\n", shader, (file ? file : ""), buffer);
  }
}

bool validate_program(GLuint program)
{
  static const GLsizei BUFFER_SIZE = 512;
  GLchar buffer[BUFFER_SIZE];
  GLsizei length = 0;

  glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);

  if (length > 0)
  {
    printf("Program %d link error: %s\n", program, buffer);
    return false;
  }

  return true;
}

int main(void)
{
  const char* vertex_shader =
      "\n"
      "#version 330\n"
      "\n"
      "noperspective out vec2 TexCoord;\n"
      "\n"
      "void main(void){\n"
      "\n"
      "    TexCoord.x = (gl_VertexID == 2)? 2.0: 0.0;\n"
      "    TexCoord.y = (gl_VertexID == 1)? 2.0: 0.0;\n"
      "    \n"
      "    gl_Position = vec4(2.0 * TexCoord - 1.0, 0.0, 1.0);\n"
      "}\n";
  const char* fragment_shader =
      "\n"
      "#version 330\n"
      "\n"
      "uniform sampler2D buffer;\n"
      "noperspective in vec2 TexCoord;\n"
      "\n"
      "out vec3 outColor;\n"
      "\n"
      "void main(void){\n"
      "    outColor = texture(buffer, TexCoord).rgb;\n"
      "}\n";
  const size_t buffer_width = 224;
  const size_t buffer_height = 256;

  glfwSetErrorCallback(errorCallback);
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Space Invaders", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glClearColor(0.4f, 0.3f, 0.4f, 0.0f);

  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    fprintf(stderr, "Error initializing GLEW.\n");
    glfwTerminate();
    return -1;
  }
  int glVersion[2] = {-1, 1};
  glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
  glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

  printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);

  uint32_t clear_color = rgb_to_uint32(0, 128, 0);
  Buffer buffer;
  buffer.width = buffer_width;
  buffer.height = buffer_height;
  buffer.data = new uint32_t[buffer.width * buffer.height];
  buffer_clear(&buffer, clear_color);

  GLuint fullscreen_triangle_vao;
  glGenVertexArrays(1, &fullscreen_triangle_vao);
  glBindVertexArray(fullscreen_triangle_vao);

  GLuint shader_id = glCreateProgram();

  // Create vertex shader
  {
    GLuint shader_vp = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(shader_vp, 1, &vertex_shader, 0);
    glCompileShader(shader_vp);
    validate_shader(shader_vp, vertex_shader);
    glAttachShader(shader_id, shader_vp);

    glDeleteShader(shader_vp);
  }

  // Create fragment shader
  {
    GLuint shader_fp = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shader_fp, 1, &fragment_shader, 0);
    glCompileShader(shader_fp);
    validate_shader(shader_fp, fragment_shader);
    glAttachShader(shader_id, shader_fp);

    glDeleteShader(shader_fp);
  }

  glLinkProgram(shader_id);

  if (!validate_program(shader_id))
  {
    fprintf(stderr, "Error while validating shader.\n");
    glfwTerminate();
    glDeleteVertexArrays(1, &fullscreen_triangle_vao);
    delete[] buffer.data;
    return -1;
  }

  glUseProgram(shader_id);

  GLint location = glGetUniformLocation(shader_id, "buffer");
  glUniform1i(location, 0);

  glDisable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);

  GLuint buffer_texture;
  glGenTextures(1, &buffer_texture);
  glBindTexture(GL_TEXTURE_2D, buffer_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, buffer.width, buffer.height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
               buffer.data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindVertexArray(fullscreen_triangle_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  Sprite alien_sprite;
  alien_sprite.width = 11;
  alien_sprite.height = 8;
  alien_sprite.data = new uint8_t[11 * 8]{
      0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,  // ..@.....@..
      0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,  // ...@...@...
      0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,  // ..@@@@@@@..
      0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,  // .@@.@@@.@@.
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // @@@@@@@@@@@
      1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1,  // @.@@@@@@@.@
      1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1,  // @.@.....@.@
      0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0   // ...@@.@@...
  };

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    buffer_clear(&buffer, clear_color);

    buffer_sprite_draw(&buffer, alien_sprite, 112, 128, rgb_to_uint32(128, 0, 0));

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, buffer.width, buffer.height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data);

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}