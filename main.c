#include <stdio.h>
#include <stdlib.h>

#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"


static inline void framebuffer_size_callback(GLFWwindow *, int width, int height){
	glViewport(0, 0, width, height);
}

static inline void process_input(GLFWwindow *window){
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(void){
	GLFWwindow *window;
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	unsigned int VBO, VAO;
	const char *vertex_shader_src = "#version 330 core\n"
					"layout (location = 0) in vec3 aPos;\n"
					"void main()\n"
					"{\n"
					"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
					"}\0";
	unsigned int vertex_shader;
	int success;
	char info_log[512];
	const char *fragment_shader_src = "#version 330 core\n"
					"out vec4 FragColor;\n"
					"void main()\n"
					"{\n"
					"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
					"}\0";
	unsigned int fragment_shader, shader_program;


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (NULL == window){
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		printf("Failed to initialise GLAD\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, 800, 600);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf("%s\n", info_log);
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		printf("%s\n", info_log);
	}

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		printf("%s\n", info_log);
	}
	glUseProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)){
		process_input(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}