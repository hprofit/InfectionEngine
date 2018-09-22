

class PhyTest : public GameStateManager
{
private:
	class AmmoRound : public physics::CollisionSphere
	{
	public:
		AmmoRound()
		{
			body = new physics::RigidBody;
			setState();
		}
		~AmmoRound()
		{
			delete body;
		}

		void setState();
	};

  class Box : public physics::CollisionBox
  {
  public:
    Box()
    {
      body = new physics::RigidBody;
      //setState(0);
    }

    ~Box()
    {
      delete body;
    }

    /** Sets the box to a specific location. */
    void setState(real x, real y,real z);
  };

  // object
	AmmoRound bullet;
	TransformComponent* pTransComp; // for bullet
  physics::CollisionPlane plane;
  Box box[3];
  TransformComponent* cubeTransComp[3];


  // collision info
  const static unsigned maxContacts = 256;
  physics::Contact contacts[maxContacts];
  physics::CollisionData cData;
  physics::ContactResolver resolver = physics::ContactResolver(maxContacts * 8);


	void updateObjects();
  void generateContacts();
public:
	PhyTest(int i = 0) {};
	void PhyTest_Setup();
	void PhyTest_Update();
};

