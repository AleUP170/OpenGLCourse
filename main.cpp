#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "stb_image.h"
#include "shader_s.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Camera setup
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

//Time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// mouse vars
float lastX = 400.0f, lastY = 300.0;
float yaw = 270.0f, pitch = 0.0f;
bool firstMouse = true;
// Light vars
int lightType = 3;


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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader dirLight("./Shaders/transform.vs", "./Shaders/DirLight.fs");
    Shader pointLight("./Shaders/transform.vs", "./Shaders/PointLight.fs");
    Shader pointLightAtt("./Shaders/transform.vs", "./Shaders/PointLightAtt.fs");
    Shader spotLight("./Shaders/transform.vs", "./Shaders/SpotLight.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //+++++++++++++++++++++++++
    /*float vertices[] = {
        // positions                              // texture coords
         0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };*/

    float vertices[] = {
        // positions                              // texture coords
         0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, //0
         0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, //1
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //2
        -0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  //3

         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f, //4
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f, //5
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f, //6
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,  //7

         0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //8
         0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, //9
        0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //10
        0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //11

         -0.5f,  0.5f, 0.5f,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //12
         -0.5f, -0.5f, 0.5f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f, //13
        -0.5f, -0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, //14
        -0.5f,  0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, //15

        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, //16
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //17
        0.5f,  0.5f, 0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, //18
        0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,  //19

        0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f, //20
        0.5f, -0.5f, 0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f, //21
        -0.5f, -0.5f, 0.5f,    0.0f, -1.0f, 0.0f,   0.0f, 0.0f, //22
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f  //23


    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3,  // second triangle

        4, 5, 6,
        7, 4, 6,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23,
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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
    unsigned char *data = stbi_load("./Resources/Textures/doom_base.jpg", &width, &height, &nrChannels, 0);
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

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    switch (lightType)
    {
    case 0:
        dirLight.use();
        dirLight.setInt("texture1", 0);
        dirLight.setInt("texture2", 1);
        dirLight.setMat4("projection", projection);
        break;
    case 1:
        pointLight.use();
        pointLight.setInt("texture1", 0);
        pointLight.setInt("texture2", 1);
        pointLight.setMat4("projection", projection);
        break;
    case 2:
        pointLightAtt.use();
        pointLightAtt.setInt("texture1", 0);
        pointLightAtt.setInt("texture2", 1);
        pointLightAtt.setMat4("projection", projection);
        break;
    case 3:
        spotLight.use();
        spotLight.setInt("texture1", 0);
        spotLight.setInt("texture2", 1);
        spotLight.setMat4("projection", projection);
        break;
    default:
        break;
    }
    
    float accrot = 0.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        switch (lightType)
        {
        case 0:
            dirLight.use();
            break;
        case 1:
            pointLight.use();
            break;
        case 2:
            pointLightAtt.use();
            break;
        case 3:
            spotLight.use();
            break;
        default:
            break;
        }

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat3 normal        = glm::mat3(1.0f);
        glm::vec4 lPos          = glm::vec4(0.0f, 0.0f, 0.0f, 1.0);
        // Point light
        glm::vec3 coeffs        = glm::vec3(1.0f, 0.35f, 0.44f);
        // Directional light
        glm::vec3 lVec          = glm::vec3(0.0f, 0.0f, 1.0f);
        // Spotlight
        glm::vec3 spotDir       = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec2 spotCoeffs    = glm::vec2(20.0f, 30.0f);

        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        unsigned int lposLoc;
        unsigned int lvecLoc;
        unsigned int coeffsLoc;
        unsigned int cposLoc;
        unsigned int viewLoc;
        unsigned int modelLoc;
        unsigned int normLoc;
        unsigned int spotCoeffsLoc;
        unsigned int spotDirLoc;


        //glUniform4fv(lposLoc, 1, glm::value_ptr(lPos));

        switch (lightType) {
        case 0:
            cposLoc = glGetUniformLocation(dirLight.ID, "cpos");
            viewLoc = glGetUniformLocation(dirLight.ID, "view");
            modelLoc = glGetUniformLocation(dirLight.ID, "model");
            normLoc = glGetUniformLocation(dirLight.ID, "normal");
            lvecLoc = glGetUniformLocation(dirLight.ID, "lvec");
            glUniform3fv(lvecLoc, 1, glm::value_ptr(lVec));
            break;
        case 1:
            cposLoc = glGetUniformLocation(pointLight.ID, "cpos");
            viewLoc = glGetUniformLocation(pointLight.ID, "view");
            modelLoc = glGetUniformLocation(pointLight.ID, "model");
            normLoc = glGetUniformLocation(pointLight.ID, "normal");
            lvecLoc = glGetUniformLocation(pointLight.ID, "lvec");
            lposLoc = glGetUniformLocation(pointLight.ID, "lpos");
            glUniform4fv(lposLoc, 1, glm::value_ptr(lPos));
            break;
        case 2:
            cposLoc = glGetUniformLocation(pointLightAtt.ID, "cpos");
            viewLoc = glGetUniformLocation(pointLightAtt.ID, "view");
            modelLoc = glGetUniformLocation(pointLightAtt.ID, "model");
            normLoc = glGetUniformLocation(pointLightAtt.ID, "normal");
            lvecLoc = glGetUniformLocation(pointLightAtt.ID, "lvec");
            lposLoc = glGetUniformLocation(pointLightAtt.ID, "lpos");
            coeffsLoc = glGetUniformLocation(pointLightAtt.ID, "coeffs");
            glUniform4fv(lposLoc, 1, glm::value_ptr(lPos));
            glUniform3fv(coeffsLoc, 1, glm::value_ptr(coeffs));
            break;
        case 3:
            cposLoc = glGetUniformLocation(spotLight.ID, "cpos");
            viewLoc = glGetUniformLocation(spotLight.ID, "view");
            modelLoc = glGetUniformLocation(spotLight.ID, "model");
            normLoc = glGetUniformLocation(spotLight.ID, "normal");
            lvecLoc = glGetUniformLocation(spotLight.ID, "lvec");
            lposLoc = glGetUniformLocation(spotLight.ID, "lpos");
            coeffsLoc = glGetUniformLocation(spotLight.ID, "coeffs");
            spotDirLoc = glGetUniformLocation(spotLight.ID, "ldir");
            spotCoeffsLoc = glGetUniformLocation(spotLight.ID, "coeffsSpot");
            glUniform4fv(lposLoc, 1, glm::value_ptr(lPos));
            glUniform3fv(coeffsLoc, 1, glm::value_ptr(coeffs));
            glUniform2fv(spotCoeffsLoc, 1, glm::value_ptr(spotCoeffs));
            glUniform3fv(spotDirLoc, 1, glm::value_ptr(spotDir));
            break;
        default:
            cposLoc = glGetUniformLocation(dirLight.ID, "cpos");
            viewLoc = glGetUniformLocation(dirLight.ID, "view");
            modelLoc = glGetUniformLocation(dirLight.ID, "model");
            normLoc = glGetUniformLocation(dirLight.ID, "normal");
            lvecLoc = glGetUniformLocation(dirLight.ID, "lvec");
            glUniform3fv(lvecLoc, 1, glm::value_ptr(lVec));
            break;
        }

        glUniform4fv(cposLoc, 1, glm::value_ptr(cameraPos));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        // Cubo 1 ----------------------------------------------------------------------------------------
        model = glm::translate(model,  glm::vec3(0.0f, 0.0f, -2.0f));
        normal[0][0] = model[0][0]; normal[0][1] = model[0][1]; normal[0][2] = model[0][2];
        normal[1][0] = model[1][0]; normal[1][1] = model[1][1]; normal[1][2] = model[1][2];
        normal[2][0] = model[2][0]; normal[2][1] = model[2][1]; normal[2][2] = model[2][2];
        normal = glm::transpose(glm::inverse(normal));

        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix3fv(normLoc, 1, GL_FALSE, glm::value_ptr(normal));

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Cubo 2 ----------------------------------------------------------------------------------------
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, -3.0f));
        normal[0][0] = model[0][0]; normal[0][1] = model[0][1]; normal[0][2] = model[0][2];
        normal[1][0] = model[1][0]; normal[1][1] = model[1][1]; normal[1][2] = model[1][2];
        normal[2][0] = model[2][0]; normal[2][1] = model[2][1]; normal[2][2] = model[2][2];
        normal = glm::transpose(glm::inverse(normal));
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix3fv(normLoc, 1, GL_FALSE, glm::value_ptr(normal));

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Cubo 3 ----------------------------------------------------------------------------------------
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -3.0f));
        normal[0][0] = model[0][0]; normal[0][1] = model[0][1]; normal[0][2] = model[0][2];
        normal[1][0] = model[1][0]; normal[1][1] = model[1][1]; normal[1][2] = model[1][2];
        normal[2][0] = model[2][0]; normal[2][1] = model[2][1]; normal[2][2] = model[2][2];
        normal = glm::transpose(glm::inverse(normal));
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix3fv(normLoc, 1, GL_FALSE, glm::value_ptr(normal));

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Cubo 4 ----------------------------------------------------------------------------------------
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -3.0f));
        normal[0][0] = model[0][0]; normal[0][1] = model[0][1]; normal[0][2] = model[0][2];
        normal[1][0] = model[1][0]; normal[1][1] = model[1][1]; normal[1][2] = model[1][2];
        normal[2][0] = model[2][0]; normal[2][1] = model[2][1]; normal[2][2] = model[2][2];
        normal = glm::transpose(glm::inverse(normal));
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix3fv(normLoc, 1, GL_FALSE, glm::value_ptr(normal));

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Cubo 5 ----------------------------------------------------------------------------------------
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 1.0f, -3.0f));
        normal[0][0] = model[0][0]; normal[0][1] = model[0][1]; normal[0][2] = model[0][2];
        normal[1][0] = model[1][0]; normal[1][1] = model[1][1]; normal[1][2] = model[1][2];
        normal[2][0] = model[2][0]; normal[2][1] = model[2][1]; normal[2][2] = model[2][2];
        normal = glm::transpose(glm::inverse(normal));
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix3fv(normLoc, 1, GL_FALSE, glm::value_ptr(normal));

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Cubo 6 ----------------------------------------------------------------------------------------
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 1.0f, -3.0f));
        normal[0][0] = model[0][0]; normal[0][1] = model[0][1]; normal[0][2] = model[0][2];
        normal[1][0] = model[1][0]; normal[1][1] = model[1][1]; normal[1][2] = model[1][2];
        normal[2][0] = model[2][0]; normal[2][1] = model[2][1]; normal[2][2] = model[2][2];
        normal = glm::transpose(glm::inverse(normal));
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix3fv(normLoc, 1, GL_FALSE, glm::value_ptr(normal));

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
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
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;


    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    // std::cout <<"xp " << xoffset << " lxp "<< yoffset << std::endl;

    lastX = xpos;
    lastY = ypos;



    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}
