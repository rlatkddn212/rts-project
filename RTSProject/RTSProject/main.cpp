#include "Precompiled.h"
#include "Shader.h"
#include "WindowGroup.h"
#include "Font.h"
#include "GameStage.h"
#include "SkinnedMesh.h"
#include "AxisObject.h"
#include "CameraRay.h"
#include "EffectResourcePool.h"
#include "ObjectResourcePool.h"
#include "UIResourcePool.h"
#include "RenderManager.h"

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
		srand(time(0));

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return;
		}

		mGameState = EGameplay;
		InitGLFW();
		InitGLEW();

		CreateCallbacks();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetWindowUserPointer(window, this);

		if (TTF_Init() != 0)
		{
			SDL_Log("Failed to initialize SDL_ttf");
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		
		EffectResourcePool::GetInstance()->Initialize();
		RenderManager::GetInstance()->Initialize();
		ObjectResourcePool::GetInstance()->Initialize();
		//UIResourcePool::GetInstance()->Initialize();

		group = make_shared<GameStage>();
		group->Initialize(window, screenW, screenH);
	}

	void Terminate()
	{
		glfwTerminate();
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
		window = glfwCreateWindow(screenW, screenH, "RTS", NULL, NULL);

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

	void CreateCallbacks()
	{
		glfwSetKeyCallback(window, HandleKeys);
		glfwSetCursorPosCallback(window, HandleMouse);
		glfwSetMouseButtonCallback(window, HandleMouseButton);
		glfwSetScrollCallback(window, HandleScrollWheel);
	}

	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
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

		theWindow->group->PressKey(theWindow->keys);
	}

	static void HandleMouse(GLFWwindow* window, double xPos, double yPos)
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

		theWindow->group->CursorPos(xPos, yPos);
	}

	static void HandleMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		Game* theWindow = static_cast<Game*>(glfwGetWindowUserPointer(window));
		theWindow->group->MouseButton(button, action);
	}

	static void HandleScrollWheel(GLFWwindow* window, double xPos, double yPos)
	{
		Game* theWindow = static_cast<Game*>(glfwGetWindowUserPointer(window));
		theWindow->wheel = yPos;

		theWindow->group->MouseWheel(yPos);
	}

	void RunLoop()
	{
		while (mGameState != EQuit)
		{
			static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			static const GLfloat one = 1.0f;
			glClearBufferfv(GL_COLOR, 0, black);
			glClearBufferfv(GL_DEPTH, 0, &one);

			GenerateOutput();

			// swap buffer
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void ProcessInput()
	{

	}
	atomic<unsigned> updateFrame = 0;
	void UpdateGame()
	{
		//printf("FPS : %f\n", 1000.0f / (float)(SDL_GetTicks() - mTicksCount));
		//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
			;
		while (mGameState != EQuit)
		{
			while (updateFrame == RenderManager::GetInstance()->GetFrame())
			{
				;
			}

			float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
			mTicksCount = SDL_GetTicks();

			//if (deltaTime > 0.05f)
			//	deltaTime = 0.05f;

			group->Update(deltaTime);
			group->AddRender();

			updateFrame++;
		}
	}

	void GenerateOutput()
	{
		while (updateFrame != RenderManager::GetInstance()->GetFrame())
			;
		RenderManager::GetInstance()->Render();
	}
};

int main(int argc, char* argv[])
{
	shared_ptr<Game> game = make_shared<Game>();
	game->Initialize();

	thread update(&Game::UpdateGame, game);
	game->RunLoop();
	update.join();
	game->Terminate();

	return 0;
}