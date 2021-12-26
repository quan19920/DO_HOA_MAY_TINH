// Project sử dụng EBO và Uniform cho việc đổi màu các đỉnh
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;


//khai báo function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 1200, HEIGHT = 800;

// khởi tạo trình biên dịch cho ngôn ngữ Shader (OpenGL Shading language) - 1 màu tương đương 1 fragment shader
const GLchar* vertexShaderSource = 
"#version 330 core\n"// vertex shader
"layout (location=0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position= vec4(position.x,position.y,position.z,1);\n"// đối só 1 để chỉ phân chia bối cảnh
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n" // khai bao fragment mau do
"out vec4 color;\n"
"void main()\n"
"{\n"
"color= vec4(1.0f,0.0f,1.0f,1.0f);\n"
"}\0";


int main()
{
	// khởi tạo glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Tạo 1 cửa sổ GLFWwindow 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Khoi tao cua so", nullptr, nullptr);// đổi tên
	// kiểm tra xem của sổ đã được tạo chưa
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl; // câu lệnh đưa ra màn hình
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//Đặt biến glewExperimental về true  (bắt buộc)
	glewExperimental = GL_TRUE;
	// khởi tạo glew
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Định nghĩa kích thước viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);    // lấy kích thước framebuffer   (chú ý)
	glViewport(0, 0, width, height);;

	// bước 1: tạo trình biên dịch Shader tự động
	// bước 1.1: biên dịch vertex shader
	GLuint vertextShader;
	vertextShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertextShader, 1, &vertexShaderSource, NULL);// đưa mã nguồn shader vào sử dụng và biên dịch
	glCompileShader(vertextShader);
	// kiểm tra vertex Shader đã đưuọc compile chưa
	GLint suscess;
	GLchar infoLog[512];
	glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &suscess);
	if (!suscess) {
		glGetShaderInfoLog(vertextShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
			std::endl;
	}
	// bước 1.2: biên dịch fragment shader
	GLuint fragmentShader; // mau do
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);// đưa mã nguồn shader vào sử dụng và biên dịch
	glCompileShader(fragmentShader);
	// kiểm tra Fragment Shader đã đưuọc compile chưa
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &suscess);
	if (!suscess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
			std::endl;
	}
	
	// bước 2: tạo chương trình chạy dữ liệu shader -> render - 1 fragment tương đương 1 shader program riêng
	GLuint ShaderProgram;
	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertextShader); // đưa vertext shader vào chương trình shader đỏ
	glAttachShader(ShaderProgram, fragmentShader); // đưa fragment shader đỏ vào chương trình shader đỏ
	glLinkProgram(ShaderProgram);

	// kiểm tra các shader đã được link vào chương trình shader chưa
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &suscess);
	if (!suscess) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertextShader); // xóa shader sau khi đã liên kết
	glDeleteShader(ShaderProgram);
	
	// bước 3: khởi tạo tọa độ điểm - dùng NDC để chuẩn hóa
	GLfloat vertices_hinhvuong[] = {
		0.5f,0.5f,0.0f,  //top-right 0
		0.5f,-0.5f,0.0f,   //bottom right 1 
		-0.5f,-0.5f,0.0f,    //bot Left 2
		-0.5f,0.5f,0.0f    //Top Left  3
	};
	GLuint indices[] = {
		0,1,3,	//tam giac 1
		1,2,3	//tam giac 2
	};
	
	// bước 4: khởi tạo VAO, VBO, EBO
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bước 4.1: Bind VAO
	glBindVertexArray(VAO);
	// bước 4.2: bind vào VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_hinhvuong), vertices_hinhvuong, GL_STATIC_DRAW);
	// bước 4.3: bind vào EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// bước 4.4: set attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// bước 4.5: unbind vào VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO , cho phép gọi hàm glVertexAttribPointer trong VBO
// unbind VAO
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		// check sự kiện  (ấn nút bàn phím, chuột,...)
		glfwPollEvents();
		//Render
		//xóa color buffer
		glClearColor(0.5f, 0.2f, 0.3f, 1.0f); // màu sắc của mà hình sau khi xóa
		glClear(GL_COLOR_BUFFER_BIT); // màu sắc màn hình sau khi đẩy lên

		glUseProgram(ShaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		///swap
		glfwSwapBuffers(window);//hoán đổi bộ nhớ đệm màu
	}
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(ShaderProgram);
	glfwTerminate();
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

