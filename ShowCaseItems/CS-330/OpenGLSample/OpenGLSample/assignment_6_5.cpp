#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>

using namespace std; // Standard namespace
//Using tut_05_03 as starting point

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Final Project - Wes Roberts"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        GLuint vao;         // Handle for the vertex array object
        GLuint vbo;         // Handle for the vertex buffer object
        GLuint nVertices;   // Number of indices of the mesh
    };

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;
    // Triangle mesh data
    GLMesh gMesh;
    GLMesh gCubeMesh;
    GLMesh gPlaneMesh;
    GLMesh gCylinderMesh;
    // Texture id
    GLuint gTextureId;
    GLuint gTextureId2;
    GLuint gTextureId3;
    GLuint gTextureId4;
    GLuint gTextureId5;
    GLuint gTextureId6;
    GLuint gTextureId7;
    GLuint gTextureId8;
    // Shader program
    GLuint gProgramId;

    // camera
    Camera gCamera(glm::vec3(-2.0f, 3.0f, 11.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

    //Boolean for handling perspective change
    bool perspectiveProjection = true;

}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void UCreateMesh(GLMesh& mesh);
void UCreateCubeMesh(GLMesh& mesh);
void UCreatePlaneMesh(GLMesh& mesh);
void UCreateCylinderMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);


/* Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; //VAP position 0 for vertex position data
layout(location = 1) in vec3 normal; //VAP position 1 for normals
layout(location = 2) in vec2 textureCoordinate;

out vec3 vertexNormal; //for outgoing normals to fragment shader
out vec3 vertexFragmentPos; //For outgoing color/pixels to fragmentshader
out vec2 vertexTextureCoordinate;


//Global variables for the transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexTextureCoordinate = textureCoordinate;
    vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)
    vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
}
);


/* Cube Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSL(440,

    struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // For outgoing cube color to the GPU

uniform vec3 viewPosition;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform Material material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 vertexNormal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 vertexNormal, vec3 vertexFragmentPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 vertexNormal, vec3 vertexFragmentPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(vertexNormal);
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: calculate a point light
    result += CalcPointLight(pointLight, norm, vertexFragmentPos, viewDir);
    // phase 3: spot light (note: commented this out for the time being)
    //result += CalcSpotLight(spotLight, norm, vertexFragmentPos, viewDir);

    fragmentColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 vertexNormal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(vertexNormal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, vertexNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vertexTextureCoordinate));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vertexTextureCoordinate));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vertexTextureCoordinate));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 vertexNormal, vec3 vertexFragmentPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - vertexFragmentPos);
    // diffuse shading
    float diff = max(dot(vertexNormal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, vertexNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - vertexFragmentPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vertexTextureCoordinate));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vertexTextureCoordinate));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vertexTextureCoordinate));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 vertexNormal, vec3 vertexFragmentPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - vertexFragmentPos);
    // diffuse shading
    float diff = max(dot(vertexNormal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, vertexNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - vertexFragmentPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vertexTextureCoordinate));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vertexTextureCoordinate));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vertexTextureCoordinate));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
);


// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}


int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the mesh
    UCreateMesh(gMesh); // Calls the function to create the Vertex Buffer Object
    // Create the mesh
    UCreateCubeMesh(gCubeMesh);
    // Create the mesh
    UCreatePlaneMesh(gPlaneMesh);
    // Create the mesh
    UCreateCylinderMesh(gCylinderMesh);

    // Create the shader program
    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;

    // Load texture
    const char* texFilename = "../OpenGLSample/WishingYouTheBest.jpg";
    if (!UCreateTexture(texFilename, gTextureId))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // Load texture2
    const char* texFilename2 = "../OpenGLSample/WoodTexture.jpg";
    if (!UCreateTexture(texFilename2, gTextureId2))
    {
        cout << "Failed to load texture " << texFilename2 << endl;
        return EXIT_FAILURE;
    }
    // Load texture3
    const char* texFilename3 = "../OpenGLSample/PlaneBase1.jpg";
    if (!UCreateTexture(texFilename3, gTextureId3))
    {
        cout << "Failed to load texture " << texFilename3 << endl;
        return EXIT_FAILURE;
    }
    // Load texture4
    const char* texFilename4 = "../OpenGLSample/PaperTowel.jpg";
    if (!UCreateTexture(texFilename4, gTextureId4))
    {
        cout << "Failed to load texture " << texFilename4 << endl;
        return EXIT_FAILURE;
    }
    // Load texture5
    const char* texFilename5 = "../OpenGLSample/Hole.jpg";
    if (!UCreateTexture(texFilename5, gTextureId5))
    {
        cout << "Failed to load texture " << texFilename5 << endl;
        return EXIT_FAILURE;
    }
    // Load texture6
    const char* texFilename6 = "../OpenGLSample/BronzeKey1.jpg";
    if (!UCreateTexture(texFilename6, gTextureId6))
    {
        cout << "Failed to load texture " << texFilename6 << endl;
        return EXIT_FAILURE;
    }
    // Load texture7
    const char* texFilename7 = "../OpenGLSample/Silver.jpg";
    if (!UCreateTexture(texFilename7, gTextureId7))
    {
        cout << "Failed to load texture " << texFilename7 << endl;
        return EXIT_FAILURE;
    }
    // Load texture8
    const char* texFilename8 = "../OpenGLSample/LampShade.jpg";
    if (!UCreateTexture(texFilename8, gTextureId8))
    {
        cout << "Failed to load texture " << texFilename8 << endl;
        return EXIT_FAILURE;
    }
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gProgramId);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gProgramId, "uTexture"), 0);

    GLint material_diffuseLoc = glGetUniformLocation(gProgramId, "material.diffuse");
    GLint material_specularLoc = glGetUniformLocation(gProgramId, "material.specular");

    glUniform1i(material_diffuseLoc, 0);
    glUniform1i(material_specularLoc, 1);

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        // per-frame timing
        // --------------------
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // input
        // -----
        UProcessInput(gWindow);

        // Render this frame
        URender();

        glfwPollEvents();
    }

    // Release mesh data
    UDestroyMesh(gMesh);
    // Release mesh data
    UDestroyMesh(gCubeMesh);
    // Release mesh data
    UDestroyMesh(gPlaneMesh);
    // Release mesh data
    UDestroyMesh(gCylinderMesh);

    // Release texture
    UDestroyTexture(gTextureId);
    // Release texture
    UDestroyTexture(gTextureId2);
    // Release texture
    UDestroyTexture(gTextureId3);
    // Release texture
    UDestroyTexture(gTextureId4);
    // Release texture
    UDestroyTexture(gTextureId5);
    // Release texture
    UDestroyTexture(gTextureId6);
    // Release texture
    UDestroyTexture(gTextureId7);
    // Release texture
    UDestroyTexture(gTextureId8);

    // Release shader program
    UDestroyShaderProgram(gProgramId);

    // Release shader programs
    UDestroyShaderProgram(gProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    static const float cameraSpeed = 2.5f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        gCamera.ProcessKeyboard(UP, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.ProcessKeyboard(DOWN, gDeltaTime);

    //Perspective Change
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        perspectiveProjection = true;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        perspectiveProjection = false;

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}

// glfw: handle mouse button events
// --------------------------------
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
    break;

    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }
}


// Functioned called to render a frame
void URender()
{
    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera position for the shader to use
    GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    // set the material shininess
    GLint material_shininessLoc = glGetUniformLocation(gProgramId, "material.shininess");
    glUniform1f(material_shininessLoc, 32.0f);

    // set up the directional light for the scene
    GLint dirLight_directionLoc = glGetUniformLocation(gProgramId, "dirLight.direction");
    GLint dirLight_ambientLoc = glGetUniformLocation(gProgramId, "dirLight.ambient");
    GLint dirLight_diffuseLoc = glGetUniformLocation(gProgramId, "dirLight.diffuse");
    GLint dirLight_specularLoc = glGetUniformLocation(gProgramId, "dirLight.specular");

    // pass direction and light data to the shader program's corresponding uniforms
    glUniform3f(dirLight_directionLoc, -0.2f, -1.0f, -0.3f);
    glUniform3f(dirLight_ambientLoc, 0.65f, 0.65f, 0.65f);
    glUniform3f(dirLight_diffuseLoc, 0.29f, 0.29f, 0.29f);
    glUniform3f(dirLight_specularLoc, 0.8f, 0.8f, 0.8f);

    // set up the point light for the scene
    GLint pointLight_positionLoc = glGetUniformLocation(gProgramId, "pointLight.position");
    GLint pointLight_ambientLoc = glGetUniformLocation(gProgramId, "pointLight.ambient");
    GLint pointLight_diffuseLoc = glGetUniformLocation(gProgramId, "pointLight.diffuse");
    GLint pointLight_specularLoc = glGetUniformLocation(gProgramId, "pointLight.specular");
    GLint pointLight_constantLoc = glGetUniformLocation(gProgramId, "pointLight.constant");
    GLint pointLight_linearLoc = glGetUniformLocation(gProgramId, "pointLight.linear");
    GLint pointLight_quadraticLoc = glGetUniformLocation(gProgramId, "pointLight.quadratic");

    // pass point light data to the shader program's corresponding uniforms
    glUniform3f(pointLight_positionLoc, -8.0f, 3.0f, 3.5f);
    glUniform3f(pointLight_ambientLoc, 0.4f, 0.4f, 0.4f);
    glUniform3f(pointLight_diffuseLoc, 0.98f, 0.68f, 0.36f);
    glUniform3f(pointLight_specularLoc, 0.8f, 0.8f, 0.8f);
    glUniform1f(pointLight_constantLoc, 0.5f);
    glUniform1f(pointLight_linearLoc, 0.09f);
    glUniform1f(pointLight_quadraticLoc, 0.032f);

    //glBindVertexArray(gMesh.vao);
    //glUseProgram(gProgramId);


    // 1. Scale the object - Photo
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, 1.0f, 0.1f));
    // 2. Rotates shape by 0 degrees in the y axis
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-26.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. Place object on top of the origin
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 0.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();

    // Creates a perspective projection
    glm::mat4 projection;
    if (perspectiveProjection) {
        projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }
    else {
        float orthoSize = 6.0f;
        projection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.1f, 100.0f);
    }


    // Set the shader to be used
    glUseProgram(gProgramId);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projLoc = glGetUniformLocation(gProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    ///////////////////////////////////////////////////
//	ALL CUBE CREATIONS HERE  (Scaling for photo is above this though)
///////////////////////////////////////////////////

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gCubeMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureId);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - BOTTOM Frame
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 0.1f, 0.2f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-26.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.25f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;


    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureId2);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Left Frame
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 1.15f, 0.2f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-26.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.48f, 0.0f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Right Frame
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 1.15f, 0.2f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-26.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.7f, 0.48f, 0.0f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Top Frame
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 0.1f, 0.2f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-26.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -0.25f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Back of Frame
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, 1.1f, 0.1f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-26.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.46f, -0.15f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Picture Frame Stand
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.8f, 0.1f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.25f, -0.3f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureId6);

    // 1. Scale the object - Key Base
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.27f, 0.02f, 0.2f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -0.07f, 0.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Key Top
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f, 0.02f, 0.05f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-2.95f, -0.07f, 0.4f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Key Stem
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.02f, 0.4f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-3.2f, -0.07f, 0.738f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Key Tooth 1
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.015f, 0.08f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-3.192f, -0.07f, 0.9f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Key Tooth 2
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.08f, 0.015f, 0.08f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-3.15f, -0.07f, 0.82f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Key Tooth 3
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.06f, 0.015f, 0.08f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-3.14f, -0.07f, 0.74f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);

    // 1. Scale the object - Key Tooth 3
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.09f, 0.015f, 0.12f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-3.1f, -0.07f, 0.63f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gCubeMesh.nVertices);



    ///////////////////////////////////////////////////
//	ALL PLANE CREATIONS HERE
///////////////////////////////////////////////////
// 
    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gPlaneMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureId3);

    // 1. Scale the object - Table Plane
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 5.0f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, -0.58f, 0.0f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gPlaneMesh.nVertices);

    ///////////////////////////////////////////////////
//	ALL CYLINDER CREATIONS HERE
///////////////////////////////////////////////////


    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gCylinderMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureId5);

    // 1. Scale the object - Paper Towel Roll
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 2.5f, 0.6f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -0.08f, 0.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    // Draws the triangles Bottom
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    //Draws the triangles Top
    glDrawArrays(GL_TRIANGLE_FAN, 37, 36);
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureId4);
    //Draws the triangles Sides
    glDrawArrays(GL_TRIANGLE_STRIP, 74, gCylinderMesh.nVertices);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureId7);

    // 1. Scale the object - Lamp Base
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.5f, 0.6f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-5.5f, -0.08f, 0.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    // Draws the triangles Bottom
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    //Draws the triangles Top
    glDrawArrays(GL_TRIANGLE_FAN, 37, 36);
    //Draws the triangles Sides
    glDrawArrays(GL_TRIANGLE_STRIP, 74, gCylinderMesh.nVertices);

    // 1. Scale the object - Lamp Stem
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 4.0f, 0.1f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-5.5f, -0.08f, 0.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    // Draws the triangles Bottom
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    //Draws the triangles Top
    glDrawArrays(GL_TRIANGLE_FAN, 37, 36);
    //Draws the triangles Sides
    glDrawArrays(GL_TRIANGLE_STRIP, 74, gCylinderMesh.nVertices);

    // 1. Scale the object - Lamp Base +1
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.55f, 0.38f, 0.55f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-5.5f, 0.58f, 0.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    // Draws the triangles Bottom
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    //Draws the triangles Top
    glDrawArrays(GL_TRIANGLE_FAN, 37, 36);
    //Draws the triangles Sides
    glDrawArrays(GL_TRIANGLE_STRIP, 74, gCylinderMesh.nVertices);

    // 1. Scale the object - Lamp Base +2
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.38f, 0.5f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-5.5f, 1.08f, 0.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    // Draws the triangles Bottom
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    //Draws the triangles Top
    glDrawArrays(GL_TRIANGLE_FAN, 37, 36);
    //Draws the triangles Sides
    glDrawArrays(GL_TRIANGLE_STRIP, 74, gCylinderMesh.nVertices);

    // 1. Scale the object - Lamp Base +3
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.45f, 0.38f, 0.45f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-5.5f, 1.58f, 0.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    // Draws the triangles Bottom
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    //Draws the triangles Top
    glDrawArrays(GL_TRIANGLE_FAN, 37, 36);
    //Draws the triangles Sides
    glDrawArrays(GL_TRIANGLE_STRIP, 74, gCylinderMesh.nVertices);

    // 1. Scale the object - Lamp Base +4
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.35f, 0.28f, 0.35f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-5.5f, 2.08f, 0.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    // Draws the triangles Bottom
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    //Draws the triangles Top
    glDrawArrays(GL_TRIANGLE_FAN, 37, 36);
    //Draws the triangles Sides
    glDrawArrays(GL_TRIANGLE_STRIP, 74, gCylinderMesh.nVertices);

    // 1. Scale the object - Lamp Shade
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.9f, 1.38f, 0.9f));
    // 2. Rotates shape by 0 degrees in the y axis
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. Place object on top of the origin
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(-5.5f, 2.58f, 0.5f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    // Draws the triangles Bottom
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    //Draws the triangles Top
    glDrawArrays(GL_TRIANGLE_FAN, 37, 36);
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureId8);
    //Draws the triangles Sides
    glDrawArrays(GL_TRIANGLE_STRIP, 74, gCylinderMesh.nVertices);

    // Deactivate the Vertex Array Object and shader program
    glBindVertexArray(0);
    glUseProgram(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}


// Implements the UCreateMesh function
void UCreateMesh(GLMesh& mesh)
{
    // Vertex data
    GLfloat verts[] = {
        //Positions         //Normals             //Texture Coordinates
        //Base (Bottom)
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  //1
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 1.0f,  //2
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,  //3
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,  //4
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f,  //5
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  //6

        //Left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  //7 (1)
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  //8 (5)
         0.0f, 0.75f, 0.0f,  -1.0f,  0.0f,  0.0f,  0.5f, 1.0f,  //9 (apex)

         //Back
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f,  //10 (5)
         0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f,  //11 (3)
         0.0f, 0.75f, 0.0f,  0.0f,  0.0f, 1.0f,  0.5f, 1.0f, //12 (apex)

         //Right
         0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  //13 (2)
         0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  //14 (3)
         0.0f, 0.75f, 0.0f,  1.0f,  0.0f,  0.0f,  0.5f, 1.0f, //15 (apex)

         //Front
         -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,  0.0f, 0.0f, //16 (1)
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,  1.0f, 0.0f,  //17 (2)
         0.0f, 0.75f, 0.0f,   0.0f,  0.0f,  -1.0f,  0.5f, 1.0f, //18 (apex)
    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

// Implements the UCreateCubeMesh function
void UCreateCubeMesh(GLMesh& mesh)
{
    // Position and Color data
    GLfloat verts[] = {
        //Positions          //Normals
        // ------------------------------------------------------
        //Back Face          //Negative Z Normal  Texture Coords.
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

       //Front Face         //Positive Z Normal
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

      //Left Face          //Negative X Normal
     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     //Right Face         //Positive X Normal
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     //Bottom Face        //Negative Y Normal
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    //Top Face           //Positive Y Normal
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

// Implements the UCreateMesh function
void UCreatePlaneMesh(GLMesh& mesh)
{
    // Position and Color data
    GLfloat verts[] = {
        //Positions          //Normals
        // -----------------------------------------------------     

        //Top Face           //Positive Z Normal  Texture Coords
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

void UCreateCylinderMesh(GLMesh& mesh)
{
    GLfloat verts[] = {
        // cylinder bottom		// texture coords
        1.0f, 0.0f, 0.0f,		0.5,  0.0,                //0
        .98f, 0.0f, -0.17f,		0.6,  0.017,              //1
        .94f, 0.0f, -0.34f,		0.68, 0.04,               //2
        .87f, 0.0f, -0.5f,		0.77, 0.08,               //3
        .77f, 0.0f, -0.64f,		0.83, 0.13,               //4
        .64f, 0.0f, -0.77f,		0.88, 0.17, //5
        .5f, 0.0f, -0.87f,		0.93, 0.25, //6
        .34f, 0.0f, -0.94f,		0.97, 0.33, //7
        .17f, 0.0f, -0.98f,		0.99, 0.41, //8
        0.0f, 0.0f, -1.0f,		1.0,0.5,
        -.17f, 0.0f, -0.98f,	0.99, 0.41,
        -.34f, 0.0f, -0.94f,	0.97, 0.33,
        -.5f, 0.0f, -0.87f,		0.93, 0.25,
        -.64f, 0.0f, -0.77f,	0.88, 0.17,
        -.77f, 0.0f, -0.64f,	0.83, 0.13,
        -.87f, 0.0f, -0.5f,		0.77, 0.08,
        -.94f, 0.0f, -0.34f,	0.68, 0.04,
        -.98f, 0.0f, -0.17f,	0.6, 0.017,
        -1.0f, 0.0f, 0.0f,		0.5, 1.0,
        -.98f, 0.0f, 0.17f,		0.41, 0.99,
        -.94f, 0.0f, 0.34f,		0.33, 0.97, //20
        -.87f, 0.0f, 0.5f,		0.25, 0.93,
        -.77f, 0.0f, 0.64f,		0.17, 0.88,
        -.64f, 0.0f, 0.77f,		0.13, 0.83,
        -.5f, 0.0f, 0.87f,		0.08, 0.77,
        -.34f, 0.0f, 0.94f,		0.04, 0.68,
        -.17f, 0.0f, 0.98f,		0.017, 0.6,
        0.0f, 0.0f, 1.0f,		0.0, 0.5,
        .17f, 0.0f, 0.98f,		0.017, 0.41,
        .34f, 0.0f, 0.94f,		0.04, 0.33,
        .5f, 0.0f, 0.87f,		0.08, 0.25, //30
        .64f, 0.0f, 0.77f,		0.13, 0.17,
        .77f, 0.0f, 0.64f,		0.17, 0.13,
        .87f, 0.0f, 0.5f,		0.25, 0.08,
        .94f, 0.0f, 0.34f,		0.33, 0.04,
        .98f, 0.0f, 0.17f,		0.41, 0.017,
        1.0f, 0.0f, 0.0f,		0.5,  0.0,  //36

        // cylinder top			// texture coords
        1.0f, 1.0f, 0.0f,		0.5,  0.0,  //37
        .98f, 1.0f, -0.17f,		0.6,  0.017,
        .94f, 1.0f, -0.34f,		0.68, 0.04,
        .87f, 1.0f, -0.5f,		0.77, 0.08, //40
        .77f, 1.0f, -0.64f,		0.83, 0.13,
        .64f, 1.0f, -0.77f,		0.88, 0.17,
        .5f, 1.0f, -0.87f,		0.93, 0.25,
        .34f, 1.0f, -0.94f,		0.97, 0.33,
        .17f, 1.0f, -0.98f,		0.99, 0.41,
        0.0f, 1.0f, -1.0f,		1.0,  0.5,
        -.17f, 1.0f, -0.98f,	0.99, 0.41,
        -.34f, 1.0f, -0.94f,	0.97, 0.33,
        -.5f, 1.0f, -0.87f,		0.93, 0.25,
        -.64f, 1.0f, -0.77f,	0.88, 0.17, //50
        -.77f, 1.0f, -0.64f,	0.83, 0.13,
        -.87f, 1.0f, -0.5f,		0.77, 0.08,
        -.94f, 1.0f, -0.34f,	0.68, 0.04,
        -.98f, 1.0f, -0.17f,	0.6, 0.017,
        -1.0f, 1.0f, 0.0f,		0.5, 1.0,
        -.98f, 1.0f, 0.17f,		0.41, 0.99,
        -.94f, 1.0f, 0.34f,		0.33, 0.97,
        -.87f, 1.0f, 0.5f,		0.25, 0.93,
        -.77f, 1.0f, 0.64f,		0.17, 0.88,
        -.64f, 1.0f, 0.77f,		0.13, 0.83, //60
        -.5f, 1.0f, 0.87f,		0.08, 0.77,
        -.34f, 1.0f, 0.94f,		0.04, 0.68,
        -.17f, 1.0f, 0.98f,		0.017, 0.6,
        0.0f, 1.0f, 1.0f,		0.0, 0.5,
        .17f, 1.0f, 0.98f,		0.017, 0.41,
        .34f, 1.0f, 0.94f,		0.04, 0.33,
        .5f, 1.0f, 0.87f,		0.08, 0.25,
        .64f, 1.0f, 0.77f,		0.13, 0.17,
        .77f, 1.0f, 0.64f,		0.17, 0.13,
        .87f, 1.0f, 0.5f,		0.25, 0.08, //70
        .94f, 1.0f, 0.34f,		0.33, 0.04,
        .98f, 1.0f, 0.17f,		0.41, 0.017,
        1.0f, 1.0f, 0.0f,		0.5,  0.0,  //73

        // cylinder body		// texture coords
        1.0f, 1.0f, 0.0f,		0.0,1.0,    //74
        1.0f, 0.0f, 0.0f,		0.0,0.0,
        .98f, 0.0f, -0.17f,		0.027,0.0,
        1.0f, 1.0f, 0.0f,		0.0,1.0,
        .98f, 1.0f, -0.17f,		0.027,1.0,
        .98f, 0.0f, -0.17f,		0.027,0.0,
        .94f, 0.0f, -0.34f,		0.054,0.0,
        .98f, 1.0f, -0.17f,		0.027,1.0,
        .94f, 1.0f, -0.34f,		0.054,1.0,
        .94f, 0.0f, -0.34f,		0.054,0.0,
        .87f, 0.0f, -0.5f,		0.081,0.0,
        .94f, 1.0f, -0.34f,		0.054,1.0,
        .87f, 1.0f, -0.5f,		0.081,1.0,
        .87f, 0.0f, -0.5f,		0.081,0.0,
        .77f, 0.0f, -0.64f,		0.108,0.0,
        .87f, 1.0f, -0.5f,		0.081,1.0,
        .77f, 1.0f, -0.64f,		0.108,1.0,
        .77f, 0.0f, -0.64f,		0.108,0.0,
        .64f, 0.0f, -0.77f,		0.135,0.0,
        .77f, 1.0f, -0.64f,		0.108,1.0,
        .64f, 1.0f, -0.77f,		0.135,1.0,
        .64f, 0.0f, -0.77f,		0.135,0.0,
        .5f, 0.0f, -0.87f,		0.162,0.0,
        .64f, 1.0f, -0.77f,		0.135, 1.0,
        .5f, 1.0f, -0.87f,		0.162, 1.0,
        .5f, 0.0f, -0.87f,		0.162, 0.0,
        .34f, 0.0f, -0.94f,		0.189, 0.0,
        .5f, 1.0f, -0.87f,		0.162, 1.0,
        .34f, 1.0f, -0.94f,		0.189, 1.0,
        .34f, 0.0f, -0.94f,		0.189, 0.0,
        .17f, 0.0f, -0.98f,		0.216, 0.0,
        .34f, 1.0f, -0.94f,		0.189, 1.0,
        .17f, 1.0f, -0.98f,		0.216, 1.0,
        .17f, 0.0f, -0.98f,		0.216, 0.0,
        0.0f, 0.0f, -1.0f,		0.243, 0.0,
        .17f, 1.0f, -0.98f,		0.216, 1.0,
        0.0f, 1.0f, -1.0f,		0.243, 1.0,
        0.0f, 0.0f, -1.0f,		0.243, 0.0,
        -.17f, 0.0f, -0.98f,	0.270, 0.0,
        0.0f, 1.0f, -1.0f,		0.243, 1.0,
        -.17f, 1.0f, -0.98f,	0.270, 1.0,
        -.17f, 0.0f, -0.98f,	0.270, 0.0,
        -.34f, 0.0f, -0.94f,	0.297, 0.0,
        -.17f, 1.0f, -0.98f,	0.270, 1.0,
        -.34f, 1.0f, -0.94f,	0.297, 1.0,
        -.34f, 0.0f, -0.94f,	0.297, 0.0,
        -.5f, 0.0f, -0.87f,		0.324, 0.0,
        -.34f, 1.0f, -0.94f,	0.297, 1.0,
        -.5f, 1.0f, -0.87f,		0.324, 1.0,
        -.5f, 0.0f, -0.87f,		0.324, 0.0,
        -.64f, 0.0f, -0.77f,	0.351, 0.0,
        -.5f, 1.0f, -0.87f,		0.324, 1.0,
        -.64f, 1.0f, -0.77f,	0.351, 1.0,
        -.64f, 0.0f, -0.77f,	0.351, 0.0,
        -.77f, 0.0f, -0.64f,	0.378, 0.0,
        -.64f, 1.0f, -0.77f,	0.351, 1.0,
        -.77f, 1.0f, -0.64f,	0.378, 1.0,
        -.77f, 0.0f, -0.64f,	0.378, 0.0,
        -.87f, 0.0f, -0.5f,		0.405, 0.0,
        -.77f, 1.0f, -0.64f,	0.378, 1.0,
        -.87f, 1.0f, -0.5f,		0.405, 1.0,
        -.87f, 0.0f, -0.5f,		0.405, 0.0,
        -.94f, 0.0f, -0.34f,	0.432, 0.0,
        -.87f, 1.0f, -0.5f,		0.405, 1.0,
        -.94f, 1.0f, -0.34f,	0.432, 1.0,
        -.94f, 0.0f, -0.34f,	0.432, 0.0,
        -.98f, 0.0f, -0.17f,	0.459, 0.0,
        -.94f, 1.0f, -0.34f,	0.432, 1.0,
        -.98f, 1.0f, -0.17f,	0.459, 1.0,
        -.98f, 0.0f, -0.17f,	0.459, 0.0,
        -1.0f, 0.0f, 0.0f,		0.486, 0.0,
        -.98f, 1.0f, -0.17f,	0.459, 1.0,
        -1.0f, 1.0f, 0.0f,		0.486, 1.0,
        -1.0f, 0.0f, 0.0f,		0.486, 0.0,
        -.98f, 0.0f, 0.17f,		0.513, 0.0,
        -1.0f, 1.0f, 0.0f,		0.486, 1.0,
        -.98f, 1.0f, 0.17f,		0.513, 1.0,
        -.98f, 0.0f, 0.17f,		0.513, 0.0,
        -.94f, 0.0f, 0.34f,		0.54, 0.0,
        -.98f, 1.0f, 0.17f,		0.513, 1.0,
        -.94f, 1.0f, 0.34f,		0.54, 1.0,
        -.94f, 0.0f, 0.34f,		0.54, 0.0,
        -.87f, 0.0f, 0.5f,		0.567, 0.0,
        -.94f, 1.0f, 0.34f,		0.54, 1.0,
        -.87f, 1.0f, 0.5f,		0.567, 1.0,
        -.87f, 0.0f, 0.5f,		0.567, 0.0,
        -.77f, 0.0f, 0.64f,		0.594, 0.0,
        -.87f, 1.0f, 0.5f,		0.567, 1.0,
        -.77f, 1.0f, 0.64f,		0.594, 1.0,
        -.77f, 0.0f, 0.64f,		0.594, 0.0,
        -.64f, 0.0f, 0.77f,		0.621, 0.0,
        -.77f, 1.0f, 0.64f,		0.594, 1.0,
        -.64f, 1.0f, 0.77f,		0.621, 1.0,
        -.64f, 0.0f, 0.77f,		0.621, 0.0,
        -.5f, 0.0f, 0.87f,		0.648, 0.0,
        -.64f, 1.0f, 0.77f,		0.621, 1.0,
        -.5f, 1.0f, 0.87f,		0.648, 1.0,
        -.5f, 0.0f, 0.87f,		0.648, 0.0,
        -.34f, 0.0f, 0.94f,		0.675, 0.0,
        -.5f, 1.0f, 0.87f,		0.648, 1.0,
        -.34f, 1.0f, 0.94f,		0.675, 1.0,
        -.34f, 0.0f, 0.94f,		0.675, 0.0,
        -.17f, 0.0f, 0.98f,		0.702, 0.0,
        -.34f, 1.0f, 0.94f,		0.675, 1.0,
        -.17f, 1.0f, 0.98f,		0.702, 1.0,
        -.17f, 0.0f, 0.98f,		0.702, 0.0,
        0.0f, 0.0f, 1.0f,		0.729, 0.0,
        -.17f, 1.0f, 0.98f,		0.702, 1.0,
        0.0f, 1.0f, 1.0f,		0.729, 1.0,
        0.0f, 0.0f, 1.0f,		0.729, 0.0,
        .17f, 0.0f, 0.98f,		0.756, 0.0,
        0.0f, 1.0f, 1.0f,		0.729, 1.0,
        .17f, 1.0f, 0.98f,		0.756, 1.0,
        .17f, 0.0f, 0.98f,		0.756, 0.0,
        .34f, 0.0f, 0.94f,		0.783, 0.0,
        .17f, 1.0f, 0.98f,		0.756, 1.0,
        .34f, 1.0f, 0.94f,		0.783, 1.0,
        .34f, 0.0f, 0.94f,		0.783, 0.0,
        .5f, 0.0f, 0.87f,		0.810, 0.0,
        .34f, 1.0f, 0.94f,		0.783, 1.0,
        .5f, 1.0f, 0.87f,		0.810, 1.0,
        .5f, 0.0f, 0.87f,		0.810, 0.0,
        .64f, 0.0f, 0.77f,		0.837, 0.0,
        .5f, 1.0f, 0.87f,		0.810, 1.0,
        .64f, 1.0f, 0.77f,		0.837, 1.0,
        .64f, 0.0f, 0.77f,		0.837, 0.0,
        .77f, 0.0f, 0.64f,		0.864, 0.0,
        .64f, 1.0f, 0.77f,		0.837, 1.0,
        .77f, 1.0f, 0.64f,		0.864, 1.0,
        .77f, 0.0f, 0.64f,		0.864, 0.0,
        .87f, 0.0f, 0.5f,		0.891, 0.0,
        .77f, 1.0f, 0.64f,		0.864, 1.0,
        .87f, 1.0f, 0.5f,		0.891, 1.0,
        .87f, 0.0f, 0.5f,		0.891, 0.0,
        .94f, 0.0f, 0.34f,		0.918, 0.0,
        .87f, 1.0f, 0.5f,		0.891, 1.0,
        .94f, 1.0f, 0.34f,		0.918, 1.0,
        .94f, 0.0f, 0.34f,		0.918, 0.0,
        .98f, 0.0f, 0.17f,		0.945, 0.0,
        .94f, 1.0f, 0.34f,		0.918, 0.0,
        .98f, 1.0f, 0.17f,		0.945, 1.0,
        .98f, 0.0f, 0.17f,		0.945, 0.0,
        1.0f, 0.0f, 0.0f,		1.0, 0.0,
        .98f, 1.0f, 0.17f,		0.945, 1.0,
        1.0f, 1.0f, 0.0f,		1.0, 1.0,
        1.0f, 0.0f, 0.0f,		1.0, 0.0
    };

    // total float values per each type
    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerUV = 2;

    // store vertex and index count
    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerUV));

    // Create VAO
    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create VBO
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV);

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(2);
}


void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
}


/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}


void UDestroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}


// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}


void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}

