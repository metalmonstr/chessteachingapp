//#include "congratulations.h"
//#include "ui_congratulations.h"

//congratulations::congratulations(QWidget *parent) :
//    QWidget(parent),
//    World(b2Vec2(0.f, -1.5f))
//    , ui(new Ui::congratulations)
//{
//    ui->setupUi(this);

//    CreateGround(World, 350.f, 400.f);
//    //CreateGround(World, 350.f, 0.f);
//    CreateWall(World, 0.f, 200.f);
//    CreateWall(World, 700.f, 200.f);

//    // Size the texture
//    texture.create(700, 400);

//    makeBox = 0;

//    timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, &congratulations::renderTexture);
//    timer->start(1);
//}

//congratulations::~congratulations()
//{
//    delete ui;
//}

//void congratulations::on_pushButton_clicked()
//{
//    close();
//}

//void congratulations::CreateGround(b2World& World, float X, float Y)
//{
//    const float SCALE = 30.f;

//    b2BodyDef BodyDef;
//    BodyDef.position = b2Vec2(X/30.f, Y/30.f);
//    BodyDef.type = b2_staticBody;
//    b2Body* Body = World.CreateBody(&BodyDef);

//    b2PolygonShape Shape;
//    Shape.SetAsBox((700.f/2)/SCALE, (55.f/2)/SCALE); // Creates a box shape. Divide your desired width and height by 2.
//    b2FixtureDef FixtureDef;
//    FixtureDef.density = 0.f;  // Sets the density of the body
//    FixtureDef.shape = &Shape; // Sets the shape
//    Body->CreateFixture(&FixtureDef); // Apply the fixture definition
//}

//void congratulations::CreateWall(b2World& World, float X, float Y)
//{
//    const float SCALE = 30.f;

//    b2BodyDef BodyDef;
//    BodyDef.position = b2Vec2(X/30.f, Y/30.f);
//    BodyDef.type = b2_staticBody;
//    b2Body* Body = World.CreateBody(&BodyDef);

//    b2PolygonShape Shape;
//    Shape.SetAsBox((55.f/2)/SCALE, (400.f/2)/SCALE); // Creates a box shape. Divide your desired width and height by 2.
//    b2FixtureDef FixtureDef;
//    FixtureDef.density = 0.f;  // Sets the density of the body
//    FixtureDef.shape = &Shape; // Sets the shape
//    Body->CreateFixture(&FixtureDef); // Apply the fixture definition
//}

//void congratulations::CreateBox(b2World& World, int MouseX, int MouseY)
//{
//    const float SCALE = 30.f;

//    b2BodyDef BodyDef;
//    BodyDef.position = b2Vec2(MouseX/SCALE, MouseY/SCALE);
//    BodyDef.type = b2_dynamicBody;
//    b2Body* Body = World.CreateBody(&BodyDef);

//    Body->SetAngularVelocity(10);
//    b2PolygonShape Shape;
//    Shape.SetAsBox((5.f)/SCALE, (5.f)/SCALE);
//    b2FixtureDef FixtureDef;
//    FixtureDef.density = 1.f;
//    FixtureDef.friction = 0.7f;
//    FixtureDef.shape = &Shape;
//    Body->CreateFixture(&FixtureDef);
//}

//void congratulations::renderTexture() {
//    const float SCALE = 30.f;

//    sf::Texture BoxTexture;
//    BoxTexture.loadFromFile("../A8EducationApplication/check-box.png");
//    BoxTexture.setSmooth(true);

//    if(makeBox == 25) {
//        randBox();
//        makeBox = 0;
//    }
//    else {
//        makeBox++;
//    }

//    /** Simulate the world */
//    World.Step(1/60.f, 8, 3);

//    texture.clear(sf::Color::White);
//    for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
//    {
//        if(BodyIterator->GetPosition().y < 0) {
//            World.DestroyBody(BodyIterator);
//        }

//        if (BodyIterator->GetType() == b2_dynamicBody)
//        {
//            sf::Sprite Sprite;
//            Sprite.setTexture(BoxTexture);
//            Sprite.setScale((1.5f)/SCALE, (1.5f)/SCALE);
//            Sprite.setColor(randColor());
//            Sprite.setOrigin(16.f, 16.f);
//            Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
//            Sprite.setRotation(BodyIterator->GetAngle() * 180/b2_pi);
//            texture.draw(Sprite);
//        }
//    }


//    // Set to a QImage
//    sf::Texture rt = texture.getTexture();
//    sf::Image irt = rt.copyToImage();
//    const uint8_t *pp = irt.getPixelsPtr();

//    QImage qi(pp, 700, 400, QImage::Format_ARGB32);
//    qi = qi.rgbSwapped();

//    ui->label->setPixmap(QPixmap::fromImage(qi));
//}

//void congratulations::randBox() {
//    int random = rand() % 14 + 1;
//    CreateBox(World, (random*50), 350);
//}

//sf::Color congratulations::randColor()
//{
//   int random = rand() % 5;
//   if(random == 0)
//   {
//       return sf::Color::Red;
//   }
//   else if(random == 1)
//   {
//       return sf::Color::Blue;
//   }
//   else if(random == 2)
//   {
//       return sf::Color::Green;
//   }
//   else if(random == 3)
//   {
//       return sf::Color::Yellow;
//   }
//   else
//   {
//       return sf::Color::Magenta;
//   }
//}
