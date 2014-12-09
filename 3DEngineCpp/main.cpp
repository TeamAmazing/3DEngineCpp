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
	GameObject* pointLightObject = new GameObject();
	GameObject* spotLightObject = new GameObject();
	GameObject* directionalLightObject = new GameObject();

	planeObject->AddComponent(new MeshRenderer(new Mesh("./res/models/plane4.obj"), new Material(new Texture("bricks.jpg"), 0.5f, 4,
		new Texture("bricks_normal.jpg"),
		new Texture("bricks_disp.png"), 0.03f, -0.5f)));
	planeObject->GetTransform().SetPos(Vector3f(0, -1, 5));
	planeObject->GetTransform().SetScale(4.0f);

	pointLightObject->AddComponent(new PointLight(Vector3f(0, 1, 0), 0.4f, Attenuation(0, 0, 0.01)));
	pointLightObject->GetTransform().SetPos(Vector3f(10, 0, 7));

	spotLightObject->AddComponent(new SpotLight(Vector3f(0, 1, 1), 0.4f, Attenuation(0, 0, 0.02f), ToRadians(91.1f),8,1));
	//spotLightObject->GetTransform().SetRot(Quaternion(Vector3f(0, 1, 0), ToRadians(90.0f)));
	spotLightObject->GetTransform().Rotate(Quaternion(Vector3f(0, 1, 0), ToRadians(90.0f))); 
	spotLightObject->GetTransform().Rotate(Quaternion(Vector3f(1, 0, 0), ToRadians(-60.0f)));
	spotLightObject->GetTransform().SetPos((Vector3f(10, 1, 5)));

	directionalLightObject->AddComponent(new DirectionalLight(Vector3f(1, 1, 1), 0.4f, 10));

	GameObject* testMesh1 = new GameObject();
	GameObject* testMesh2 = new GameObject();

	testMesh1->AddComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks2.jpg"), 1, 8,
		new Texture("bricks2_normal.png"),
		new Texture("bricks2_disp.jpg"), 0.04f, -1.0f)));
	testMesh2->AddComponent(new MeshRenderer(new Mesh("./res/models/plane3.obj"), new Material(new Texture("bricks2.jpg"), 1, 8,
		new Texture("bricks2_normal.png"),
		new Texture("bricks2_disp.jpg"), 0.04f, -1.0f)));

	testMesh1->GetTransform().SetPos(Vector3f(0, 2, 0));
	testMesh1->GetTransform().SetRot(Quaternion(Vector3f(0, 1, 0), 0.4f));
	testMesh1->GetTransform().SetScale(1.0f);

	testMesh2->GetTransform().SetPos(Vector3f(0, 0, 25));

	testMesh1->AddChild(testMesh2);

	AddToScene(planeObject);
	AddToScene(pointLightObject);
	AddToScene(spotLightObject);
	AddToScene(directionalLightObject);
	AddToScene(testMesh1);
	testMesh2->AddChild((new GameObject())
		->AddComponent(new Camera(Matrix4f().InitPerspective(ToRadians(70.0f), Window::GetAspect(), 0.1f, 1000.0f)))
		->AddComponent(new FreeLook())
		->AddComponent(new FreeMove()));

	directionalLightObject->GetTransform().SetRot(Quaternion(Vector3f(1, 0, 0), ToRadians(-45)));

	GameObject* box = new GameObject();
	box->AddComponent(new MeshRenderer(new Mesh("./res/models/cube.obj"), new Material(new Texture("Planks.jpg"), 1, 8, new Texture("Planks.jpg"),
		new Texture("Planks.jpg"), 0.04f, -1.0f)));

	box->GetTransform().SetPos(Vector3f(14, 0, 5));
	box->GetTransform().SetRot(Quaternion(Vector3f(0, 1, 0), ToRadians(30.0f)));
	AddToScene(box);

}

int main()
{
	TestGame game;
	CoreEngine engine(800, 600, 60, &game);
	engine.CreateWindow("3D Game Engine");
	engine.Start();

	return 0;
}
