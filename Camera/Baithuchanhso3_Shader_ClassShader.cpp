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
// gọi hàm nhập bàn phím
void key_callback_Input_dichchuyenCamera_BanPhim(GLFWwindow* window);


const GLuint WIDTH = 1400, HEIGHT = 1000;

// bước 1.3 - camera: khởi tạo các ma trận camera walk around
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);        //vi tri camera
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);     //vector hướng của camera
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//bước 1.3.1 - camera: timing
float deltaTime = 0.0f; //thời gian giữa khung hình hiện tại và khung hình sau
float lastframe = 0.0f;  //thời gian khung hình cuối


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
	int vwidth, vheight;
	glfwGetFramebufferSize(window, &vwidth, &vheight);    // lấy kích thước framebuffer   (chú ý)
	glViewport(0, 0, vwidth, vheight);;


	Shader ourShader("vShader_b3.vertex", "fShader_b3 .frag");

	// 0.0f  220.0f, -220.0f , -300.0f

	//bước 1: khai báo vertex input (vertex data)
	GLfloat vertices[] = {
		// vị trí - position         //texture coordinate (s,t)(u,v)
		220.0f,  0.0f, -220.0f,         1.0f, 0.0f,
		0.0f, 0.0f, -220.0f,            0.0f, 0.0f,
		220.0f, 0.0f, 0.0f,             1.0f, 1.0f,
		0.0f, 0.0f, -220.0f,            0.0f, 0.0f,
		220.0f, 0.0f, 0.0f,             1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,               0.0f, 1.0f,

		220.0f,  -300.0f, -220.0f,         1.0f, 0.0f,
		0.0f, -300.0f, -220.0f,            0.0f, 0.0f,
		220.0f, 0.0f, -220.0f,             1.0f, 1.0f,
		0.0f, -300.0f, -220.0f,            0.0f, 0.0f,
		220.0f, 0.0f, -220.0f,             1.0f, 1.0f,
		0.0f, 0.0f, -220.0f,               0.0f, 1.0f,


		220.0f,  -300.0f, 0.0f,         1.0f, 0.0f,
		220.0f,  -300.0f, -220.0f,         0.0f, 0.0f,
		220.0f,  0.0f, 0.0f,         1.0f, 1.0f,
		220.0f,  -300.0f, -220.0f,         0.0f, 0.0f,
		220.0f,  0.0f, 0.0f,        1.0f, 1.0f,
		220.0f, 0.0f, -220.0f,               0.0f, 1.0f,

		0.0f,  -300.0f, 0.0f,         1.0f, 0.0f,
		220.0f,  -300.0f, 0.0f,         0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,         1.0f, 1.0f,
		220.0f,  -300.0f, 0.0f,         0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,         1.0f, 1.0f,
		220.0f, 0.0f, 0.0f,               0.0f, 1.0f,

		0.0f,  0.0f, 0.0f,         1.0f, 0.0f,
		0.0f,  -300.0f, 0.0f,         0.0f, 0.0f,
		0.0f, 0.0f, -220.0f,         1.0f, 1.0f,
		0.0f,  -300.0f, 0.0f,         0.0f, 0.0f,
		0.0f, 0.0f, -220.0f,         1.0f, 1.0f,
		0.0f, -300.0f, -220.0f,               0.0f, 1.0f,

		220.0f,  -300.0f, 0.0f,         1.0f, 0.0f,
		0.0f,  -300.0f, 0.0f,         0.0f, 0.0f,
		220.0f,  -300.0f, -220.0f,         1.0f, 1.0f,
		0.0f,  -300.0f, 0.0f,         0.0f, 0.0f,
		220.0f,  -300.0f, -220.0f,         1.0f, 1.0f,
		0.0f, -300.0f, -220.0f,               0.0f, 1.0f,
	};

	glm::vec3 List_CubePosition[] = {
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(200.0f,400.0f,540.0f),
		glm::vec3(-200.0f,300.0f,700.0f),
		glm::vec3(-500.0f,500.0f,700.0f),
		glm::vec3(-100.0f,500.0f,700.0f),
	};
	//Bước 2: Khởi tạo VBO, VAO
		//b2.1 VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	//bind VAO
	glBindVertexArray(VAO);

	//b2.2 VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // liên kết (bind) VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//texture attribute 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO , cho phép gọi hàm glVertexAttribPointer trong VBO
	//unbind VAO
	glBindVertexArray(0);

	///Load và tạo 1 texture
	// bước 1: khởi tạo texture
	GLuint texture_wood;
	glGenTextures(1, &texture_wood);
	glBindTexture(GL_TEXTURE_2D, texture_wood);
	// Bước 2: cài đặt tham số texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//cài đặt tham số texture filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// bước 3: tải image, tạo texture + mipmaps
	int width, height, nrChannels;
	unsigned char* image = stbi_load("concrete.jpg", &width, &height, &nrChannels, 0);
	if (image)
	{
		///tạo mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		//đưa ra thông báo lỗi load image
		std::cout << "Failed to Load texture" << std::endl;
	}

	// bước 4: delete image 
	stbi_image_free(image);
	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);  //unbind texture


	while (!glfwWindowShouldClose(window))
	{
		// check sự kiện  (ấn nút bàn phím, chuột,...)
		glfwPollEvents();
		//Render
		//xóa color buffer
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bước 5-texture: gán texture
		glBindTexture(GL_TEXTURE_2D, texture_wood);

		///xác định ma trận biến đổi (các bạn muốn biến đổi đối tượng như nào )
		// bước 1.1: khởi tạo ma trận đơn vị
		glm::mat4 model = glm::mat4(1.0f); // Các ma trận đơn vị dùng để biến đổi
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		//bước 1.2: khởi tạo ma trận camera
		float radius = 0.5f;
		float camX = sin(glfwGetTime() * radius);
		float camZ = cos(glfwGetTime() * radius);

		// sau khi đã định nghĩa hàm cho camera di chuyển với bàn phím
		// bước 1.3.2: khởi tạo thời gian khung hình
		//thời gian mỗi khung hình (per-frame time)
		float currentFrame = glfwGetTime();       //               =0.01        -0                                    =0.011        0.01 
		deltaTime = currentFrame - lastframe;    //deltaTime0 = currentFrame0 - lastframe0;=0.01    //deltaTime1 = currentFrame1 - lastframe1;=0.001
		lastframe = currentFrame;                //lastframe0 = currentFrame0;=0.01                  //lastframe1 = currentFrame1;=0.001
		//input bàn phím 
		key_callback_Input_dichchuyenCamera_BanPhim(window);

		// bước 2: khởi tạo các ma trận transform
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1000.0f));
		projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 10000.0f);
		
		// bước 3: lấy vị trí của uniform
		GLuint UniformLocation_model = glGetUniformLocation(ourShader.IDProgram, "model");
		GLuint UniformLocation_view = glGetUniformLocation(ourShader.IDProgram, "view");
		GLuint UniformLocation_projection = glGetUniformLocation(ourShader.IDProgram, "projection");
		
		// bước 4: gán gái trị uniform vào trong Shader
		glUniformMatrix4fv(UniformLocation_view, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(UniformLocation_projection, 1, GL_FALSE, glm::value_ptr(projection));

		//Buoc 3 Vẽ hình 1 hình tam giác
		ourShader.use();

		glBindVertexArray(VAO);
		glBindVertexArray(VAO);
		for (int i = 0; i < sizeof(List_CubePosition); i++)
		{
			model = glm::translate(view, List_CubePosition[i]);
			//model = glm::rotate(model, (GLfloat)glfwGetTime()/*45.0f*/, glm::vec3(0.0f, 1.0f, 1.0f));//muốn xoay mô hình đối tượng 1 góc 30 9theo trục x
			glUniformMatrix4fv(UniformLocation_model, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		///swap
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//Terminate GLFW, xóa và dọn dẹp tài nguyên sau khi thoát
	glfwTerminate();
	return 0;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// khởi tr=ạo hàm movement
void key_callback_Input_dichchuyenCamera_BanPhim(GLFWwindow* window)
{
	float cameraSpeed = 1000.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront; //cameraPos = cameraPos + cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
