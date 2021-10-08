#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "stb_image.h"
#include "shader_s.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Variables movimiento
float movX(0), movY(0), movZ(0);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("./Shaders/transform.vs", "./Shaders/transform.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // texture coords
        
         0.5f,  0.5f, 0.5f,   1.0f, 0.0f, // 0
         0.5f, -0.5f, 0.5f,   1.0f, 1.0f, // 1
        -0.5f, -0.5f, 0.5f,   0.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.5f,   0.0f, 0.0f, // 3

         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // 4
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 5

         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // 6
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 7
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 8
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // 9

         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 10
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 11

         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 12
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 13


        // Para mostrar textura correctamente en las tapas
    };
    unsigned int indices[] = {
        // Frente
        0, 1, 3, // first triangle
        1, 2, 3,  // second triangle

        // Derecha
        4, 5, 0,
        5, 1, 0,

        // Atrás
        9, 8, 6,
        8, 7, 6,

        // Izquierda
        3, 2, 9,
        2, 8, 9,

        // Arriba
        10, 0, 11,
        0, 3, 11,

        // Abajo
        1, 12, 2,
        12, 13, 2,
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("./Resources/Textures/doom_base.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("./Resources/Textures/nier.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    glEnable(GL_DEPTH_TEST);

    // Cámara
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));

    // Proyección
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    ourShader.setMat4("projection", projection);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 model1 = glm::mat4(1.0f); // Cubo estático
        glm::mat4 model2 = glm::mat4(1.0f); // Cubo rotando
        glm::mat4 model3 = glm::mat4(1.0f); // Cubo moviéndose

        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glBindVertexArray(VAO);

        // Dibuando modelo 1
        model1 = glm::translate(model1, glm::vec3(-2.0f, 0.0f, 0.0f));
        model1 = glm::rotate(model1, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // Pasando al shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
        // Render
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Dibujando modelo 2
        model2 = glm::rotate(model2, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model2 = glm::rotate(model2, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        // Pasando al shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        // Render
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Dibujando modelo 3
        model3 = glm::translate(model3, glm::vec3(2.0f, 0.0f, 0.0f));
        model3 = glm::rotate(model3, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model3 = glm::translate(model3, glm::vec3(movX, movY, movZ));
        // Pasando al shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        // Render
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movY += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movY -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movX += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movX -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        movZ += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        movZ -= 0.001f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}