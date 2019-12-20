#include "Precompiled.h"
#include "Shader.h"
#include "WindowGroup.h"
#include "Font.h"
#include "HeightMap.h"
#include "Terrain.h"
#include "StaticMesh.h"
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
	
	std::shared_ptr<WindowGroup> group;
	std::shared_ptr<HeightMap> heightMap;
	std::shared_ptr<Terrain> terrain;
	std::shared_ptr<SkinnedMesh> mesh;
	std::shared_ptr<AxisObject> axis;

	std::shared_ptr<Camera> camera;
	std::shared_ptr<Mouse> mouse;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	double wheel;
	bool mouseFirstMoved;
	float mDeltaTime;

	const int screenW = 1024;
	const int screenH = 768;

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


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		group = make_shared<WindowGroup>();
		group->Initialize();

		mouse = make_shared<Mouse>(window);
		
		terrain = make_shared<Terrain>();
		terrain->Initialize(glm::ivec2(100,100));
		camera = make_shared<Camera>();
		camera->Initialize(screenW, screenH);

		axis = make_shared<AxisObject>();
		
		mesh = make_shared<SkinnedMesh>();
		mesh->LoadModel("Assets/Model/magician.X");
		
	}

	void InitGLFW()
	{
		// GLFW �ʱ�ȭ
		if (!glfwInit())
		{
			cerr << "ERROR : init GLFW\n";
			return exit(-1);
		}

		// GLFW ����
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// window ����
		window = glfwCreateWindow(screenW, screenH, "ShaderToy Viewer", NULL, NULL);

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
		// GLEW �ʱ�ȭ
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
		static const GLfloat black[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		static const GLfloat one = 1.0f;
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);

		terrain->Render(camera);
		axis->Render(camera);
		mesh->BoneTransform();
		mesh->RenderModel(camera);

		group->Render(camera);
	}
};

int main(int argc, char* argv[])
{
	shared_ptr<Game> game = make_shared<Game>();
	game->Initialize();

	game->RunLoop();
	glfwTerminate();

	return 0;
}