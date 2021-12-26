// Include standard headers
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
const GLchar* vertexShaderSource = "#version 330 core\n"// vertex shader
"layout (location=0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position= vec4(position.x,position.y,position.z,1);\n"// đối só 1 để chỉ phân chia bối cảnh
"}\0";
const GLchar* fragmentShaderYELLOWSource = "#version 330 core\n"// khai bao fragment mau vang
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f,0.0f,1.0f,1.0f);\n"
"}\0";
const GLchar* fragmentShaderREDSource = "#version 330 core\n" // khai bao fragment mau do
"out vec4 color;\n"
"void main()\n"
"{\n"
"color= vec4(1.0f,0.0f,0.0f,1.0f);\n"
"}\0";

int main(void)
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
		std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
		std::endl;
	}
	// bước 1.2: biên dịch fragment shader
	GLuint fragmentShaderRED; // mau do
	fragmentShaderRED = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderRED, 1, &fragmentShaderREDSource, NULL);// đưa mã nguồn shader vào sử dụng và biên dịch
	glCompileShader(fragmentShaderRED);
	// kiểm tra Fragment Shader đã đưuọc compile chưa
	glGetShaderiv(fragmentShaderRED, GL_COMPILE_STATUS, &suscess);
	if (!suscess) {
		glGetShaderInfoLog(fragmentShaderRED, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
			std::endl;
	}
	//----------------------------------------------------------
	GLuint fragmentShaderYELLOW; // mau vang
	fragmentShaderYELLOW = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYELLOW, 1, &fragmentShaderYELLOWSource, NULL);// đưa mã nguồn shader vào sử dụng và biên dịch
	glCompileShader(fragmentShaderYELLOW);
	// kiểm tra Fragment Shader đã đưuọc compile chưa
	glGetShaderiv(fragmentShaderYELLOW, GL_COMPILE_STATUS, &suscess);
	if (!suscess) {
		glGetShaderInfoLog(fragmentShaderYELLOW, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<
			std::endl;
	}

	// bước 2: tạo chương trình chạy dữ liệu shader -> render - 1 fragment tương đương 1 shader program riêng
	GLuint ShaderProgramRED;
	ShaderProgramRED = glCreateProgram();
	glAttachShader(ShaderProgramRED,vertextShader); // đưa vertext shader vào chương trình shader đỏ
	glAttachShader(ShaderProgramRED,fragmentShaderRED); // đưa fragment shader đỏ vào chương trình shader đỏ
	glLinkProgram(ShaderProgramRED);
	// kiểm tra các shader đã được link vào chương trình shader chưa
	glGetProgramiv(ShaderProgramRED, GL_LINK_STATUS, &suscess);
	if (!suscess) {
		glGetProgramInfoLog(ShaderProgramRED, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertextShader); // xóa shader sau khi đã liên kết
	glDeleteShader(fragmentShaderRED);
	//-----------------------------------
	GLuint ShaderProgramYELLOW;
	ShaderProgramYELLOW = glCreateProgram();
	glAttachShader(ShaderProgramYELLOW, vertextShader); // đưa vertext shader vào chương trình shader đỏ
	glAttachShader(ShaderProgramYELLOW, fragmentShaderYELLOW); // đưa fragment shader đỏ vào chương trình shader đỏ
	glLinkProgram(ShaderProgramYELLOW);
	// kiểm tra các shader đã được link vào chương trình shader chưa
	glGetProgramiv(ShaderProgramYELLOW, GL_LINK_STATUS, &suscess);
	if (!suscess) {
		glGetProgramInfoLog(ShaderProgramYELLOW, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertextShader); // xóa shader sau khi đã liên kết
	glDeleteShader(ShaderProgramYELLOW);

	// bước 3: khởi tạo tọa độ điểm - dùng NDC để chuẩn hóa
	GLfloat vertices_Tamgiac1[] = {
		// tam giac 1
		-0.5f,-0.5f,0.0f,  //bottom-left
		0.5f,-0.5f,0.0f,   //bottom right
		-0.5f,0.5f,0.0f   //Top Left 
	};
	GLfloat vertices_Tamgiac2[] = {
		//tam giac 2
		-0.5f,0.5f,0.0f,   //Top-left
		0.5f,0.5f,0.0f,   //Top right
		0.5f,-0.5f,0.0f    //Bot Right 
	};

	
	// bước 4: tạo VBO và VAO cho hai object
	GLuint VBOs[2], VAOs[2]; // 2- 2 phần tử tam giác
	glGenVertexArrays(2, VAOs); // tạo VAO - 2 là số lượng VAO tạo ra - số object
	glGenBuffers(2, VBOs); // tạo VBO
	//  Bước 4.1: Bind VAO tam giác 1 vs VBO tam giác 1
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]); // liên kết (bind) VBO tam giác 1
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Tamgiac1), vertices_Tamgiac1, GL_STATIC_DRAW);
	// gán các giá trị thuộc tính cho điểm (cụ thể là màu, tọa độ)- et attributes point
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);// laypout, số lượng thuộc tính, kiểu dữ liệu,mặc định,stride,điểm bắt đầu)
	glEnableVertexAttribArray(0); // lấy điểm đùa tiên trong 1 thuộc tính làm điểm bắt đầu
	// unbind VAO - dùng VAO hiện tại để vẽ hình tiếp theo
	glBindVertexArray(0);

	//----------------------------------tam giác 2 ------------------------------
	
	//  Bước 4.2: Bind VAO tam giác 1 vs VBO tam giác 1
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // liên kết (bind) VBO tam giác 1
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Tamgiac2), vertices_Tamgiac2, GL_STATIC_DRAW);
	// gán các giá trị thuộc tính cho điểm (cụ thể là màu, tọa độ)- et attributes point
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);// laypout, số lượng thuộc tính, kiểu dữ liệu,mặc định,stride,điểm bắt đầu)
	glEnableVertexAttribArray(0); // lấy điểm đùa tiên trong 1 thuộc tính làm điểm bắt đầu
	// unbind VAO - dùng VAO heienj tại để vẽ hình tiếp theo
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	// bước 5: tạo vòng lặp trò chơi
	//Game Loop - dung de ve doi tuong
	while (!glfwWindowShouldClose(window))
	{
		// check sự kiện  (ấn nút bàn phím, chuột,...)
		glfwPollEvents();
		//Render
		//xóa color buffer
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // màu sắc của mà hình sau khi xóa
		glClear(GL_COLOR_BUFFER_BIT); // màu sắc màn hình sau khi đẩy lên

		// kích hoạt chương trình shader red
		glUseProgram(ShaderProgramRED);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3); // hình tam giác, điểm bắt đầu, số lượng điểm
		glBindVertexArray(0);

		// kích hoạt chương trình shader yellow
		glUseProgram(ShaderProgramYELLOW);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3); // hình tam giác, điểm bắt đầu, số lượng điểm
		glBindVertexArray(0);

		///swap
		glfwSwapBuffers(window);//hoán đổi bộ nhớ đệm màu
	}
	//Terminate GLFW, xóa và dọn dẹp tài nguyên sau khi thoát
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(ShaderProgramRED);
	glDeleteProgram(ShaderProgramYELLOW);
	glfwTerminate();// hủy chương trình
	return 0;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
