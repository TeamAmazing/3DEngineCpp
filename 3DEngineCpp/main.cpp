#include "3DEngine.h"
#include "freeLook.h"
#include "freeMove.h"

class TestGame : public Game
{
public:
	TestGame() {}

	virtual void Init();
protected:
private:
	TestGame(const TestGame& other) {}
	void operator=(const TestGame& other) {}
};

void TestGame::Init()
{
	GameObject* planeObject = new GameObject();
	GameObject* planeObject2 = new GameObject();
	GameObject* pointLightObject = new GameObject();
	GameObject* spotLightObject = new GameObject();
	GameObject* directionalLightObject = new GameObject();
	GameObject* camera = new GameObject();
	camera->AddComponent(new Camera(Matrix4f().InitPerspective(ToRadians(70.0f), Window::GetAspect(), 0.1f, 1000.0f)));
	camera->AddComponent(new FreeLook());
	camera->GetTransform().SetPos(Vector3f(0, 3, 0));

	planeObject->AddComponent(new MeshRenderer(new Mesh("./res/models/plane4.obj"), new Material(new Texture("bricks.jpg"), 0.5f, 4,
		new Texture("bricks_normal.jpg"),
		new Texture("bricks_disp.png"), 0.03f, -0.5f)));
	planeObject->GetTransform().SetPos(Vector3f(0, -1, 5));
	planeObject->GetTransform().SetScale(4.0f);

	planeObject2->AddComponent(new MeshRenderer(new Mesh("./res/models/plane4.obj"), new Material(new Texture("bricks.jpg"), 0.5f, 4,
		new Texture("bricks_normal.jpg"),
		new Texture("bricks_disp.png"), 0.03f, -0.5f)));
	planeObject2->GetTransform().SetPos(Vector3f(20, 20, 10));
	planeObject2->GetTransform().SetRot(Quaternion(Vector3f(0, 0, 1), ToRadians(90)));
	planeObject2->GetTransform().SetScale(4.0f);

	pointLightObject->AddComponent(new PointLight(Vector3f(0, 1, 0), 0.4f, Attenuation(0, 0, 0.01)));
	pointLightObject->GetTransform().SetPos(Vector3f(10, 0, 7));

	spotLightObject->AddComponent(new SpotLight(Vector3f(0, 1, 1), 0.4f, Attenuation(0, 0, 0.002f), ToRadians(91.1f),8,1));
	//spotLightObject->GetTransform().SetRot(Quaternion(Vector3f(0, 1, 0), ToRadians(90.0f)));
	spotLightObject->GetTransform().Rotate(Quaternion(Vector3f(0, 1, 0), ToRadians(90.0f))); 
	spotLightObject->GetTransform().Rotate(Quaternion(Vector3f(1, 0, 0), ToRadians(-60.0f)));
	spotLightObject->GetTransform().SetPos((Vector3f(10, 1, 5)));

	directionalLightObject->AddComponent(new DirectionalLight(Vector3f(1, 1, 1), 0.4f, 10));

	AddToScene(planeObject);
	AddToScene(planeObject2);
	AddToScene(pointLightObject);
	AddToScene(spotLightObject);
	AddToScene(directionalLightObject);
	planeObject->AddChild(camera);

	directionalLightObject->GetTransform().SetRot(Quaternion(Vector3f(1, 0, 0), ToRadians(-45)));

	GameObject* box = new GameObject();
	box->AddComponent(new MeshRenderer(new Mesh("./res/models/cube.obj"), new Material(new Texture("Planks.jpg"), 1, 8, new Texture("Planks.jpg"),
		new Texture("Planks.jpg"), 0.04f, -1.0f)));

	box->GetTransform().SetPos(Vector3f(14, 0, 5));
	box->GetTransform().SetRot(Quaternion(Vector3f(0, 1, 0), ToRadians(30.0f)));
	AddToScene(box);
	box->AddComponent(new FreeMove());

}

int main()
{
	TestGame game;
	CoreEngine engine(800, 600, 60, &game);
	engine.CreateWindow("3D Game Engine");
	engine.Start();

	return 0;
}
