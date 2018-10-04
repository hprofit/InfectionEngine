

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

class PhyFractureTest : public GameStateManager
{
public:
  class Block : public physics::CollisionBox
  {
  public:
    bool exists;

    Block():
      exists(false)
    {
      body = new physics::RigidBody();
    }

    ~Block()
    {
      delete body;
    }

    void calculateMassProperties(real invDensity);
    void setState(const Vector3D &position,
      const Quaternion &orientation,
      const Vector3D &extents,
      const Vector3D &velocity);

    void divideBlock(const physics::Contact& contact,
      Block* target, Block* blocks);

  };
  const int MAX_BLOCKS = 9;
  Block blocks[9];
  bool hit;
  bool ball_active;
  physics::CollisionSphere ball;
  unsigned fracture_contact;

  // object
  TransformComponent* pTransComp; // for bullet
  physics::CollisionPlane plane;
  TransformComponent* cubeTransComp[9];

  // collision info
  const static unsigned maxContacts = 256;
  physics::Contact contacts[maxContacts];
  physics::CollisionData cData;
  physics::ContactResolver resolver = physics::ContactResolver(maxContacts * 8);

  void PhyTest_Setup();
  void SetBall();
  void SetBox();
  void generateContacts();

  PhyFractureTest()
  {
    cData.contactArray = contacts;
    SetBall();
    SetBox();
    cData.contactCount = 0;
  };

  void update();
  


};

