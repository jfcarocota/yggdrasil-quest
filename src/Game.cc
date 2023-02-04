#include "CommonHeaders.hh"
#include "TileGroup.hh"
#include "Components/EntityManager.hh"
#include "Components/TransformComponent.hh"
#include "Components/SpriteComponent.hh"
#include "Components/AnimatorComponent.hh"
#include "Components/AudioListenerComponent.hh"
#include "Components/Entity.hh"
#include "GUI/TextComponent.hh"
#include "GUI/Button.hh"

EntityManager entityManager;

sf::Clock* gameClock{new sf::Clock()};
float deltaTime{};
sf::Color renderColor{sf::Color()};
/*enum GAME_STATE
{
  MAIN,
  GAME,
  WIN,
  GAME_OVER
};

GAME_STATE gameState;*/

uint32 flags{};
    //flags += b2Draw::e_aabbBit;
    //flags += b2Draw::e_shapeBit;
    //flags += b2Draw::e_centerOfMassBit;
    //flags += b2Draw::e_pairBit;
    //flags += b2Draw::e_jointBit;

/**************************GAMEPLAY_ASSETS********************************/
Entity* border{};
Entity* walls{};

Game::Game()
{
  window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
  event = new sf::Event();
  gravity = new b2Vec2(0.f, 0.f);
  world = new b2World(*gravity);
  drawPhysics = new DrawPhysics(window);
  renderColor = sf::Color::White;

  auto* audioMainTitle{&entityManager.AddEntity("main-audio")};
  audioMainTitle->AddComponent<AudioListenerComponent>();
  auto* audMainTitleComp = audioMainTitle->GetComponent<AudioListenerComponent>();
  audMainTitleComp->SetAudioClip(new AudioClip(ASSETS_AUDIO_MAIN_MENU));
  audMainTitleComp->SetLoop(true);
  audMainTitleComp->Play();

/**************************************SCENE_MAIN_MENU_STARTS********************************************/

  //Cover image
  auto* coverTree{&entityManager.AddEntity("cover")};
  coverTree->AddComponent<TransformComponent>(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f, 800.f, 600.f, 1.f);
  coverTree->AddComponent<SpriteComponent>("assets/cover.png", 0, 0);

  //Title
  auto* titleText{&entityManager.AddEntity("title")};
  titleText->AddComponent<TransformComponent>(WINDOW_WIDTH * 0.5f - 300.f, WINDOW_HEIGHT * 0.5f - 100.f, 1.f, 1.f, 1.f);
  titleText->AddComponent<TextComponent>(ASSETS_FONT_ANCIENT, 60.f, sf::Color(255, 140, 0), sf::Style::None);
  auto& tittleTextComp{*titleText->GetComponent<TextComponent>()};
  tittleTextComp.SetTextStr("YGGDRASIL QUEST");

  //Play button
  auto* buttonPlay{&entityManager.AddEntity("btn_play")};
  buttonPlay->AddComponent<TransformComponent>(WINDOW_WIDTH * 0.5f - 50.f, WINDOW_HEIGHT * 0.5f, 100.f, 50.f, 1.f);
  buttonPlay->AddComponent<Button>(0.f, sf::Color::White, sf::Color::Transparent, [=](){
    std::cout << "start game" << std::endl;
    //clean scene
    coverTree->Destroy();
    titleText->Destroy();
    buttonPlay->Destroy();
    audMainTitleComp->SetAudioClip(new AudioClip(ASSETS_AUDIO_GAMEPLAY));
    audMainTitleComp->SetLoop(true);
    renderColor = sf::Color::Black;
    audMainTitleComp->Play();

    //create scene 2

    walls = &entityManager.AddEntity("walls");
    walls->AddComponent<TransformComponent>(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f, 160.f, 120.f, 2.66f);
    walls->AddComponent<SpriteComponent>(ASSETS_SPRITES_WALLS, 2, 1);

    border = &entityManager.AddEntity("border");
    border->AddComponent<TransformComponent>(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f, 96.f, 96.f, 9.f);
    border->AddComponent<SpriteComponent>(ASSETS_SPRITES_UI, 0, 0);
  });
  auto& btnPlayComp{*buttonPlay->GetComponent<Button>()};
  btnPlayComp.SetTexture("assets/GUI/button.png");
  buttonPlay->AddComponent<TextComponent>(ASSETS_FONT_ARCADECLASSIC, 15.f, sf::Color::Yellow, sf::Style::Default);
  auto& bntplayTxtComp = *buttonPlay->GetComponent<TextComponent>();
  bntplayTxtComp.SetTextStr("Start");

  /**************************************SCENE_DUNGEON_STARTS********************************************/

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
  window->clear(renderColor);

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