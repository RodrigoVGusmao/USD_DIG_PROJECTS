#define GLEW_STATIC

//c++
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

//graphics
#include<SDL.h>
#include<SDL_image.h>
#include<glew.h>
#include<SDL_opengl.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

//constants
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define DEBUG_WINDOW_WIDTH 320
#define DEBUG_WINDOW_HEIGHT 320
#define PI 3.14159

/**
things to correct:
create some classes to make code easier to read
cleanup program before closing it
**/

using namespace std;

struct key {
    float speed;
    bool key_W;
    bool key_S;
    bool key_D;
    bool key_A;
    bool key_SPACEBAR;
    bool key_CONTROL;
};

struct position {
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float pitch;
    float yaw;
    float roll;
};

struct mouse {
    float sensitivity;
    int pos_X;
    int pos_Y;
    int pos_X_old;
    int pos_Y_old;
};

struct Material {
    unsigned int diffuse;
    unsigned int specular;
    unsigned int shininess;
};
struct DirectionLight {
    unsigned int direction;
    unsigned int ambient;
    unsigned int diffuse;
    unsigned int specular;
};
struct PointLight {
    unsigned int position;
    unsigned int linear;
    unsigned int quadratic;
    unsigned int ambient;
    unsigned int diffuse;
    unsigned int specular;
};
struct SpotLight {
    unsigned int position;
    unsigned int direction;
    unsigned int linear;
    unsigned int quadratic;
    unsigned int cutOff;
    unsigned int outerCutOff;
    unsigned int ambient;
    unsigned int diffuse;
    unsigned int specular;
};

bool init(SDL_Window** window)
{
    SDL_GLContext context;
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cerr << "couldn't initialize SDL: " << SDL_GetError() << endl;
        return true;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    *window = SDL_CreateWindow("learnOpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        cerr << "couldn't create SDL window: " << SDL_GetError() << endl;
        return true;
    }

    if(IMG_Init(imgFlags) != imgFlags)
    {
        cerr << "couldn't initialize SDL_Image: " << IMG_GetError() << endl;
        return true;
    }

    if(SDL_CaptureMouse(SDL_TRUE) < 0)
    {
        clog << "couldn't set SDL_WINDOW_MOUSE_CAPTURE flag: " << SDL_GetError() << endl;
        cin.get();
    }

    context = SDL_GL_CreateContext(*window);
    if(context == NULL)
    {
        cerr << "couldn't create SDL context: " << SDL_GetError() << endl;
        return true;
    }

    if(SDL_GL_SetSwapInterval(1) < 0)
    {
        clog << "couldn't set Vsync: " << SDL_GetError() << endl;
        cin.get();
    }

    SDL_WarpMouseInWindow(*window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    return false;
}

bool initGlew()
{
    glewExperimental= GL_TRUE;
    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
        cerr << "couldn't initialize GLEW: " << glewGetErrorString(error) << endl;
        return true;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    return false;
}

bool shaderCompiler(unsigned int shader, const GLchar* shaderSource)
{
    int success;
    char infoLog[512];

    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success != 1)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cerr << endl << endl << "couldn't compile shader:\n" << shaderSource << endl << endl << "error:" << infoLog << endl;
        return true;
    }

    return false;
}

bool linkShaderProgram(unsigned int shaderProgram, unsigned int vertexShader, unsigned int fragmentShader)
{
    int success;
    char infoLog[512];

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(success != 1)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cerr << "couldn't link shader program:\n" << infoLog << endl;
        return true;
    }

    return false;
}

bool openFile(string fileName, string* fileContent)
{
    stringstream fileStream;
    ifstream file;

    cout << endl << "loading " << fileName.c_str() << "..." << endl;

    file.open(fileName.c_str());

    if(file.is_open() != true)
    {
        cerr << "couldn't open " << fileName.c_str() << endl;
        return true;
    }

    fileStream << file.rdbuf();

    file.close();

    *fileContent = fileStream.str();

    cout << fileStream.str() << endl;
    cout << "string address: " << fileContent << endl;

    return false;
}

bool loadTextures(string path, unsigned int *texture)
{
    SDL_Surface *loadedImage = IMG_Load(path.c_str());
    if(loadedImage == NULL)
    {
        cerr << "couldn't load " << path.c_str() << ":" << IMG_GetError() << endl;
        return true;
    }

    glGenTextures(1, texture);

    glBindTexture(GL_TEXTURE_2D, *texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, loadedImage->w, loadedImage->h, 0, GL_RGB, GL_UNSIGNED_BYTE, loadedImage->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(loadedImage);
    loadedImage = NULL;

    return false;
}

void createObject(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO, float vertices[], unsigned int verticesSize, unsigned int indices[], unsigned int indicesSize)
{
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    //glGenBuffers(1, EBO);

    glBindVertexArray(*VAO);
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
            glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

        /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);*/

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
            glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void mouse(position *camPosition, mouse *mouseAttr)
{
    camPosition->yaw += (mouseAttr->pos_X - mouseAttr->pos_X_old) * mouseAttr->sensitivity;
    camPosition->pitch += (mouseAttr->pos_Y_old - mouseAttr->pos_Y) * mouseAttr->sensitivity;

    /*GLfloat xoffset = mouseX - WINDOW_WIDTH/2;
    GLfloat yoffset = WINDOW_HEIGHT/2 - mouseY;

    camPosition->yaw += xoffset * sensibility;
    camPosition->pitch += yoffset * sensibility;*/

    if(camPosition->yaw > 360.0f)
    {
        camPosition->yaw -= 360.0f;
    }
    if(camPosition->yaw < -360.0f)
    {
        camPosition->yaw += 360.0f;
    }
    if(camPosition->pitch > 89.0f)
    {
        camPosition->pitch = 89.0f;
    }
    if(camPosition->pitch < -89.0f)
    {
        camPosition->pitch = -89.0f;
    }

    camPosition->cameraFront.x = cos(glm::radians(camPosition->pitch)) * cos(glm::radians(camPosition->yaw));
    camPosition->cameraFront.y = sin(glm::radians(camPosition->pitch));
    camPosition->cameraFront.z = cos(glm::radians(camPosition->pitch)) * sin(glm::radians(camPosition->yaw));
    camPosition->cameraFront = glm::normalize(camPosition->cameraFront);
}

void inputModifier(glm::mat4 *view, key keyStates, position *camPosition)
{
    float finalSpeed = keyStates.speed;
    float yPos = camPosition->cameraPos.y;

    if((keyStates.key_W == true || keyStates.key_S == true) && (keyStates.key_D == true || keyStates.key_A == true))
    {
        finalSpeed /= 2.0f;
    }
    if(keyStates.key_W == true)
    {
        camPosition->cameraPos += camPosition->cameraFront * finalSpeed;
        camPosition->cameraPos.y = yPos;
    }
    if(keyStates.key_S == true)
    {
        camPosition->cameraPos -= camPosition->cameraFront * finalSpeed;
        camPosition->cameraPos.y = yPos;
    }
    if(keyStates.key_D == true)
    {
        camPosition->cameraPos += glm::normalize(glm::cross(camPosition->cameraFront, camPosition->cameraUp)) * finalSpeed;
        camPosition->cameraPos.y = yPos;
    }
    if(keyStates.key_A == true)
    {
        camPosition->cameraPos -= glm::normalize(glm::cross(camPosition->cameraFront, camPosition->cameraUp)) * finalSpeed;
        camPosition->cameraPos.y = yPos;
    }
    if(keyStates.key_SPACEBAR == true)
    {
        camPosition->cameraPos += camPosition->cameraUp * finalSpeed;
    }
    if(keyStates.key_CONTROL == true)
    {
        camPosition->cameraPos -= camPosition->cameraUp * finalSpeed;
    }

    *view = glm::lookAt(camPosition->cameraPos, camPosition->cameraPos + camPosition->cameraFront, camPosition->cameraUp);
}

int main(int argc, char* argv[])
{
    struct key keyStates;
    struct position camPosition;
    struct mouse mouseAttr;

    struct Material object;
    struct DirectionLight dirLight;
    struct PointLight light[4];
    struct SpotLight flashlight;

    bool quit = false;

    SDL_Event event;
    SDL_Window *window;

    float currentFPS;

    unsigned int oldTime;
    unsigned int VBO, toyVAO, lightVAO, EBO, objectVertexShader, objectFragmentShader, lightVertexShader, lightFragmentShader, objectShaderProgram, lightShaderProgram;
    unsigned int lightPvmLoc, objectPvmLoc, lightColorLoc, modelLoc, viewPositionLoc;
    unsigned int crate, crateSpec, crateEmission;

    string vertexShaderSource, fragmentShaderSource, vertexShaderSource2, fragmentShaderSource2;

    float vertices[] = {
        //positions           //Normal             //texCoords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::mat4 model(1);
    glm::mat4 view(1);
    glm::mat4 projection(1);
    glm::mat4 MVP(1);

    //glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.32f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    if(init(&window) == true)
    {
        return 1;
    }
    if(initGlew() == true)
    {
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    if(openFile("GLSL/objectVSS.txt", &vertexShaderSource) == true)
    {
        return 1;
    }
    if(openFile("GLSL/objectFSS.txt", &fragmentShaderSource) == true)
    {
        return 1;
    }
    if(openFile("GLSL/lightFSS.txt", &fragmentShaderSource2) == true)
    {
        return 1;
    }
    if(openFile("GLSL/lightVSS.txt", &vertexShaderSource2) == true)
    {
        return 1;
    }

    if(loadTextures("textures/container2.jpg", &crate) == true)
    {
        return 1;
    }
    if(loadTextures("textures/container2_specular.jpg", &crateSpec) == true)
    {
        return 1;
    }
    if(loadTextures("textures/container2_emission.jpg", &crateEmission) == true)
    {
        return 1;
    }

    objectVertexShader = glCreateShader(GL_VERTEX_SHADER);
    if(shaderCompiler(objectVertexShader, vertexShaderSource.c_str()) == true)
    {
        return 1;
    }
    objectFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if(shaderCompiler(objectFragmentShader, fragmentShaderSource.c_str()) == true)
    {
        return 1;
    }
    lightFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if(shaderCompiler(lightFragmentShader, fragmentShaderSource2.c_str()) == true)
    {
        return 1;
    }
    lightVertexShader = glCreateShader(GL_VERTEX_SHADER);
    if(shaderCompiler(lightVertexShader, vertexShaderSource2.c_str()) == true)
    {
        return 1;
    }
    objectShaderProgram = glCreateProgram();
    if(linkShaderProgram(objectShaderProgram, objectVertexShader, objectFragmentShader) == true)
    {
        return 1;
    }
    lightShaderProgram = glCreateProgram();
    if(linkShaderProgram(lightShaderProgram, lightVertexShader, lightFragmentShader) == true)
    {
        return 1;
    }

    createObject(&toyVAO, &VBO, &EBO, vertices, sizeof(vertices), NULL, 0);
    createObject(&lightVAO, &VBO, &EBO, vertices, sizeof(vertices), NULL, 0);

    //inirializing variables
    modelLoc = glGetUniformLocation(objectShaderProgram, "model");
    lightPvmLoc = glGetUniformLocation(lightShaderProgram, "MVP");
    objectPvmLoc = glGetUniformLocation(objectShaderProgram, "MVP");
    lightColorLoc = glGetUniformLocation(lightShaderProgram, "color");
    viewPositionLoc = glGetUniformLocation(objectShaderProgram, "viewPos");

    //Material
    object.diffuse = glGetUniformLocation(objectShaderProgram, "obj.diffuse");
    object.specular = glGetUniformLocation(objectShaderProgram, "obj.specular");
    object.shininess = glGetUniformLocation(objectShaderProgram, "obj.shininess");

    //directional light
    dirLight.direction = glGetUniformLocation(objectShaderProgram, "dir.direction");
    dirLight.ambient = glGetUniformLocation(objectShaderProgram, "dir.ambient");
    dirLight.diffuse = glGetUniformLocation(objectShaderProgram, "dir.diffuse");
    dirLight.specular = glGetUniformLocation(objectShaderProgram, "dir.specular");

    //point light 0
    light[0].position = glGetUniformLocation(objectShaderProgram, "point[0].position");
    light[0].linear = glGetUniformLocation(objectShaderProgram, "point[0].linear");
    light[0].quadratic = glGetUniformLocation(objectShaderProgram, "point[0].quadratic");
    light[0].ambient = glGetUniformLocation(objectShaderProgram, "point[0].ambient");
    light[0].diffuse = glGetUniformLocation(objectShaderProgram, "point[0].diffuse");
    light[0].specular = glGetUniformLocation(objectShaderProgram, "point[0].specular");

    //point light 1
    light[1].position = glGetUniformLocation(objectShaderProgram, "point[1].position");
    light[1].linear = glGetUniformLocation(objectShaderProgram, "point[1].linear");
    light[1].quadratic = glGetUniformLocation(objectShaderProgram, "point[1].quadratic");
    light[1].ambient = glGetUniformLocation(objectShaderProgram, "point[1].ambient");
    light[1].diffuse = glGetUniformLocation(objectShaderProgram, "point[1].diffuse");
    light[1].specular = glGetUniformLocation(objectShaderProgram, "point[1].specular");

    //point light 2
    light[2].position = glGetUniformLocation(objectShaderProgram, "point[2].position");
    light[2].linear = glGetUniformLocation(objectShaderProgram, "point[2].linear");
    light[2].quadratic = glGetUniformLocation(objectShaderProgram, "point[2].quadratic");
    light[2].ambient = glGetUniformLocation(objectShaderProgram, "point[2].ambient");
    light[2].diffuse = glGetUniformLocation(objectShaderProgram, "point[2].diffuse");
    light[2].specular = glGetUniformLocation(objectShaderProgram, "point[2].specular");

    //point light 3
    light[3].position = glGetUniformLocation(objectShaderProgram, "point[3].position");
    light[3].linear = glGetUniformLocation(objectShaderProgram, "point[3].linear");
    light[3].quadratic = glGetUniformLocation(objectShaderProgram, "point[3].quadratic");
    light[3].ambient = glGetUniformLocation(objectShaderProgram, "point[3].ambient");
    light[3].diffuse = glGetUniformLocation(objectShaderProgram, "point[3].diffuse");
    light[3].specular = glGetUniformLocation(objectShaderProgram, "point[3].specular");

    //spot light
    flashlight.position = glGetUniformLocation(objectShaderProgram, "spot.position");
    flashlight.direction = glGetUniformLocation(objectShaderProgram, "spot.direction");
    flashlight.cutOff = glGetUniformLocation(objectShaderProgram, "spot.cutOff");
    flashlight.outerCutOff = glGetUniformLocation(objectShaderProgram, "spot.outerCutOff");
    flashlight.linear = glGetUniformLocation(objectShaderProgram, "spot.linear");
    flashlight.quadratic = glGetUniformLocation(objectShaderProgram, "spot.quadratic");
    flashlight.ambient = glGetUniformLocation(objectShaderProgram, "spot.ambient");
    flashlight.diffuse = glGetUniformLocation(objectShaderProgram, "spot.diffuse");
    flashlight.specular = glGetUniformLocation(objectShaderProgram, "spot.specular");

    keyStates.key_A = false;
    keyStates.key_S = false;
    keyStates.key_D = false;
    keyStates.key_W = false;
    keyStates.key_SPACEBAR = false;
    keyStates.key_CONTROL = false;
    camPosition.cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    camPosition.cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
    camPosition.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    camPosition.pitch = 0.0f;
    camPosition.yaw = 0.0f;
    camPosition.roll = 0.0f;
    mouseAttr.pos_X = WINDOW_WIDTH/2;
    mouseAttr.pos_X_old = WINDOW_WIDTH/2;
    mouseAttr.pos_Y = WINDOW_HEIGHT/2;
    mouseAttr.pos_Y_old = WINDOW_HEIGHT/2;
    mouseAttr.sensitivity = 0.05f;

    oldTime = SDL_GetTicks();

    while(quit != true)
    {
        currentFPS = 1000.0f/(SDL_GetTicks()-oldTime);
        oldTime = SDL_GetTicks();
        SDL_Delay(1);

        keyStates.speed = 12.0f/currentFPS;

        cout << "FPS: " << currentFPS << endl;

        while(SDL_PollEvent(&event) != 0)
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        case SDLK_w:
                            keyStates.key_W = true;
                            break;

                        case SDLK_s:
                            keyStates.key_S = true;
                            break;

                        case SDLK_d:
                            keyStates.key_D = true;
                            break;

                        case SDLK_a:
                            keyStates.key_A = true;
                            break;

                        case SDLK_SPACE:
                            keyStates.key_SPACEBAR = true;
                            break;

                        case SDLK_LCTRL:
                            keyStates.key_CONTROL = true;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                            keyStates.key_W = false;
                            break;

                        case SDLK_s:
                            keyStates.key_S = false;
                            break;

                        case SDLK_d:
                            keyStates.key_D = false;
                            break;

                        case SDLK_a:
                            keyStates.key_A = false;
                            break;

                        case SDLK_SPACE:
                            keyStates.key_SPACEBAR = false;
                            break;

                        case SDLK_LCTRL:
                            keyStates.key_CONTROL = false;
                            break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    SDL_GetMouseState(&mouseAttr.pos_X, &mouseAttr.pos_Y);
                    mouse(&camPosition, &mouseAttr);
                    SDL_WarpMouseInWindow(window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
                    break;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //calculations
        //objectPos.x = 10*sin(SDL_GetTicks()/currentFPS/1000000 * 180/PI);
        //objectPos.y = 0.5f + cos(SDL_GetTicks() * 180/PI)/2;
        //objectPos.z = 10*cos(SDL_GetTicks()/currentFPS/1000000 * 180/PI);

        //rendering objects

        inputModifier(&view, keyStates, &camPosition);

        projection = glm::mat4(1);
        projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100.0f);

        MVP = glm::mat4(1);
        MVP = projection * view;

        glUseProgram(objectShaderProgram);
            //vertex shader
            glUniformMatrix4fv(objectPvmLoc, 1, GL_FALSE, glm::value_ptr(MVP));

            //material
            glUniform1i(object.diffuse, 0);
            glUniform1i(object.specular, 1);
            glUniform1f(object.shininess, 32.0f);

            //misc
            glUniform3f(viewPositionLoc, camPosition.cameraPos.x, camPosition.cameraPos.y, camPosition.cameraPos.z);

            //directional light
            glUniform3f(dirLight.direction, -0.2f, -1.0f, -0.3f);
            glUniform3f(dirLight.ambient, 0.05f, 0.05f, 0.05f);
            glUniform3f(dirLight.diffuse, 0.4f, 0.4f, 0.4f);
            glUniform3f(dirLight.specular, 0.5f, 0.5f, 0.5f);

            //point light 0
            glUniform3f(light[0].position, pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
            glUniform1f(light[0].linear, 0.09f);
            glUniform1f(light[0].quadratic, 0.032f);
            glUniform3f(light[0].ambient, 0.05f, 0.05f, 0.05f);
            glUniform3f(light[0].diffuse, 0.8f, 0.8f, 0.8f);
            glUniform3f(light[0].specular, 1.0f, 1.0f, 1.0f);

            //point light 1
            glUniform3f(light[1].position, pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
            glUniform1f(light[1].linear, 0.09f);
            glUniform1f(light[1].quadratic, 0.032f);
            glUniform3f(light[1].ambient, 0.05f, 0.05f, 0.05f);
            glUniform3f(light[1].diffuse, 0.8f, 0.8f, 0.8f);
            glUniform3f(light[1].specular, 1.0f, 1.0f, 1.0f);

            //point light 2
            glUniform3f(light[2].position, pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
            glUniform1f(light[2].linear, 0.09f);
            glUniform1f(light[2].quadratic, 0.032f);
            glUniform3f(light[2].ambient, 0.05f, 0.05f, 0.05f);
            glUniform3f(light[2].diffuse, 0.8f, 0.8f, 0.8f);
            glUniform3f(light[2].specular, 1.0f, 1.0f, 1.0f);

            //point light 3
            glUniform3f(light[3].position, pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
            glUniform1f(light[3].linear, 0.09f);
            glUniform1f(light[3].quadratic, 0.032f);
            glUniform3f(light[3].ambient, 0.05f, 0.05f, 0.05f);
            glUniform3f(light[3].diffuse, 0.8f, 0.8f, 0.8f);
            glUniform3f(light[3].specular, 1.0f, 1.0f, 1.0f);

            //spot light
            glUniform3f(flashlight.position, camPosition.cameraPos.x, camPosition.cameraPos.y, camPosition.cameraPos.z);
            glUniform3f(flashlight.direction, camPosition.cameraFront.x, camPosition.cameraFront.y, camPosition.cameraFront.z);
            glUniform1f(flashlight.cutOff, glm::cos(glm::radians(12.5f)));
            glUniform1f(flashlight.outerCutOff, glm::cos(glm::radians(15.0f)));
            glUniform1f(flashlight.linear, 0.09f);
            glUniform1f(flashlight.quadratic, 0.032f);
            glUniform3f(flashlight.ambient, 0.0f, 0.0f, 0.0f);
            glUniform3f(flashlight.diffuse, 1.0f, 1.0f, 1.0f);
            glUniform3f(flashlight.specular, 1.0f, 1.0f, 1.0f);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, crate);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, crateSpec);

            for(int i = 0; i < 10; i++)
            {
                model = glm::mat4(1);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                glBindVertexArray(toyVAO);
                    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
            }

        for(int i = 0; i < 4; i++)
        {
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));

            MVP = glm::mat4(1);
            MVP = projection * view * model;

            //rendering light
            glUseProgram(lightShaderProgram);
                glUniformMatrix4fv(lightPvmLoc, 1, GL_FALSE, glm::value_ptr(MVP));

                glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);

                glBindVertexArray(lightVAO);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    exit(EXIT_SUCCESS);

    return 0;
}
