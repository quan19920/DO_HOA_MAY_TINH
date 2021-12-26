//khai báo và định thư viện stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// Include GLEW
#include <GL/glew.h>

//
#include "Shader.h"

// Include GLFW
#include <GLFW/glfw3.h>

//include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLFWwindow* window;


//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 1200, HEIGHT = 800;


int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Tạo 1 cửa sổ GLFWwindow 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bai thuc hanh so 3 - class shader", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//Đặt biến glewExperimental về true  (bắt buộc)
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Định nghĩa kích thước viewport
	 // Define the viewport dimensions
	int vpwidth, vpheight;
	glfwGetFramebufferSize(window, &vpwidth, &vpheight);    // lấy kích thước framebuffer   (chú ý)
	glViewport(0, 0, vpwidth, vpheight);;


	Shader ourShader("vShader_b3.vertex", "fShader_b3 .frag");


	//bước 1: khai báo vertex input (vertex data)
	GLfloat vertices[] = {
		// vị trí - position      ///color         //texture coordinate (s,t)
		-500.5f,-500.5f,0.0f,		1.0f,0.0f,0.0f,	   0.0f,0.0f,//bottom-left
		500.5f,-500.5f,0.0f,	    0.0f,1.0f,0.0f,    1.0f,0.0f,//bottom right
		500.5f,500.5f,0.0f,	       0.0f,0.0f,1.0f,     1.0f,1.0f, //Top  right
	  -500.5f,500.5f,0.0f,	       0.0f,0.0f,1.0f,     0.0f,1.0f //Top  left
	};

	GLuint indices[] = {
		0,1,2,  //tam giac 1
		3,2,0    //tam giac 2
	};
	//Bước 2: Khởi tạo VBO, VAO
		//b2.1 VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	//bind VAO
	glBindVertexArray(VAO);
	//b2.3 EBO
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // liên kết (bind) VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//b2.2 VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // liên kết (bind) VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Color attribute 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO , cho phép gọi hàm glVertexAttribPointer trong VBO
	//unbind VAO
	glBindVertexArray(0);




	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		// check sự kiện  (ấn nút bàn phím, chuột,...)
		glfwPollEvents();
		//Render
		//xóa color buffer
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		///xác định ma trận biến đổi (các bạn muốn biến đổi đối tượng như nào )
		// bước 1: khởi tạo ma trận đơn vị
		glm::mat4 model = glm::mat4(1.0f); // Các ma trận đơn vị dùng để biến đổi
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// bước 2: khởi tạo các ma trận transform
		model = glm::rotate(model, (GLfloat)glfwGetTime()*200.0f, glm::vec3(0.0f, 0.0f, 1.0f));//muốn xoay mô hình đối tượng 1 góc 30 9theo trục x
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2000.0f));
		projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 2500.0f);
		// bước 3: lấy vị trí của uniform
		GLuint UniformLocation_model = glGetUniformLocation(ourShader.IDProgram, "model");
		GLuint UniformLocation_view = glGetUniformLocation(ourShader.IDProgram, "view");
		GLuint UniformLocation_projection = glGetUniformLocation(ourShader.IDProgram, "projection");
		// bước 4: gán gái trị uniform vào trong Shader
		glUniformMatrix4fv(UniformLocation_model, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(UniformLocation_view, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(UniformLocation_projection, 1, GL_FALSE, glm::value_ptr(projection));

		//Buoc 3 Vẽ hình 1 hình tam giác
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		///swap
		glfwSwapBuffers(window);
	}
	//Terminate GLFW, xóa và dọn dẹp tài nguyên sau khi thoát
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
