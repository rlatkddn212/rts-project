#include "Precompiled.h"
#include "Shader.h"
#include "WindowGroup.h"
#include "Font.h"
#include "HeightMap.h"
#include "Terrain.h"
#include "Model.h"
#include "Camera.h"
#include "Mouse.h"
#include "SkinnedMesh.h"
#include "AxisObject.h"
#include "Ray.h"

using namespace std;

class Game
{
	enum GameState
	{
		EGameplay,
		EPaused,
		EQuit
	};

	GLFWwindow* window;
	Uint32 mTicksCount;
	GameState mGameState;
	
	WindowGroup* group;
	HeightMap* heightMap;
	Terrain* terrain;
	Camera* camera;

	SkinnedMesh* mesh;
	AxisObject* axis;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	double wheel;
	bool mouseFirstMoved;
	float mDeltaTime;

	Mouse* mouse;

public:

	void Initialize()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return ;
		}

		mGameState = EGameplay;
		InitGLFW();
		InitGLEW();
		
		createCallbacks();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetWindowUserPointer(window, this);

		if (TTF_Init() != 0)
		{
			SDL_Log("Failed to initialize SDL_ttf");
		}

		group = new WindowGroup();
		group->Initialize();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		mouse = new Mouse(window);
		
		terrain = new Terrain();
		terrain->Initialize(glm::ivec2(100,100));
		camera = new Camera();
		camera->Initialize();

		axis = new AxisObject();
		
		mesh = new SkinnedMesh();
		mesh->LoadModel("magician.X");
		
	}

	void InitGLFW()
	{
		// GLFW 초기화
		if (!glfwInit())
		{
			cerr << "ERROR : init GLFW\n";
			return exit(-1);
		}

		// GLFW 설정
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// window 생성
		window = glfwCreateWindow(1024, 768, "ShaderToy Viewer", NULL, NULL);

		if (window == NULL)
		{
			cerr << "Error : open GLFW\n";
			glfwTerminate();
			return exit(-1);
		}

		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	}

	void InitGLEW()
	{
		// GLEW 초기화
		glfwMakeContextCurrent(window);
		glewExperimental = true;

		if (glewInit() != GLEW_OK)
		{
			cerr << "ERROR : init GLEW\n";
			return exit(-1);
		}
	}

	void createCallbacks()
	{
		glfwSetKeyCallback(window, handleKeys);
		glfwSetCursorPosCallback(window, handleMouse);
		glfwSetMouseButtonCallback(window, handleMouseButton);
		glfwSetScrollCallback(window, handleScrollWheel);
	}

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
	{
		Game* theWindow = static_cast<Game*>(glfwGetWindowUserPointer(window));
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
			{
				theWindow->keys[key] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				theWindow->keys[key] = false;
			}
		}
	}

	static void handleMouse(GLFWwindow* window, double xPos, double yPos)
	{
		Game* theWindow = static_cast<Game*>(glfwGetWindowUserPointer(window));
		if (theWindow->mouseFirstMoved)
		{
			theWindow->lastX = xPos;
			theWindow->lastY = yPos;
			theWindow->mouseFirstMoved = false;
		}

		theWindow->xChange = xPos - theWindow->lastX;
		theWindow->yChange = theWindow->lastY - yPos;

		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
	}

	static void handleMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{

		}
	}

	static void handleScrollWheel(GLFWwindow* window, double xPos, double yPos)
	{
		Game* theWindow = static_cast<Game*>(glfwGetWindowUserPointer(window));
		theWindow->wheel = yPos;
	}

	void RunLoop()
	{
		while (mGameState != EQuit)
		{
			ProcessInput();
			UpdateGame();
			GenerateOutput();

			// swap buffer
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void ProcessInput()
	{
		mouse->Update(lastX, lastY, wheel);
		camera->ProcessInput(keys, mouse);

		Ray ray(camera, mouse);
		mesh->Intersect(ray);
		wheel = 0.0;
	}

	void UpdateGame()
	{
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
			;

		float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
		if (deltaTime > 0.05f)
		{
			deltaTime = 0.05f;
		}

		mDeltaTime = deltaTime;
		camera->Update(deltaTime);
		mesh->Update(deltaTime);

	}

	void GenerateOutput()
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		static const GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);

		terrain->Render(camera);
		axis->Render(camera);
		mesh->BoneTransform();
		mesh->RenderModel(camera);
	}
};

int main(int argc, char* argv[])
{
	Game* game = new Game();
	game->Initialize();

	game->RunLoop();
	glfwTerminate();

	return 0;
}