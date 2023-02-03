#include "CommonHeaders.hh"
#include "TileGroup.hh"
#include "Components/EntityManager.hh"
#include "Components/TransformComponent.hh"
#include "Components/SpriteComponent.hh"
#include "Components/AnimatorComponent.hh"
#include "Components/AudioListenerComponent.hh"
#include "Components/Entity.hh"
#include "GUI/Button.hh"

EntityManager entityManager;

TextObject* textObj1{new TextObject(ASSETS_FONT_ARCADECLASSIC, 14, sf::Color::White, sf::Text::Bold)};

sf::Clock* gameClock{new sf::Clock()};
float deltaTime{};

uint32 flags{};
    //flags += b2Draw::e_aabbBit;
    //flags += b2Draw::e_shapeBit;
    //flags += b2Draw::e_centerOfMassBit;
    //flags += b2Draw::e_pairBit;
    //flags += b2Draw::e_jointBit;

Game::Game()
{
  window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
  event = new sf::Event();
  gravity = new b2Vec2(0.f, 0.f);
  world = new b2World(*gravity);
  drawPhysics = new DrawPhysics(window);


  contactEventManager = new ContactEventManager();
}

Game::~Game()
{
}

void Game::Initialize()
{
  flags += b2Draw::e_shapeBit;
  world->SetDebugDraw(drawPhysics);
  drawPhysics->SetFlags(flags);
  world->SetContactListener(contactEventManager);

  textObj1->SetTextStr("Hello game engine");
  MainLoop();
}

void Game::UpdatePhysics()
{
  world->ClearForces();
  world->Step(deltaTime, 8, 8);
}

void Game::Update()
{
  deltaTime = gameClock->getElapsedTime().asSeconds();
  gameClock->restart();

  entityManager.Update(deltaTime);
}

void Game::MainLoop()
{
  while (window->isOpen())
  {
    while(window->pollEvent(*event))
    {
      if(event->type == sf::Event::Closed)
      {
        window->close();
      }
    }

    UpdatePhysics();
    Update();
    Render();
  }
  Destroy();
}

void Game::Render()
{
  window->clear(sf::Color::Black);

  window->draw(*textObj1->GetText());
  entityManager.Render(*window);
  if(debugPhysics)
  {
    world->DebugDraw();
  }
  window->display();
}

void Game::Destroy()
{
  delete window;
  delete event;
}