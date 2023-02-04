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
enum GAME_STATE
{
  MAIN,
  GAME,
  ROLLING,
  WIN,
  GAME_OVER
};

GAME_STATE gameState;

uint32 flags{};
    //flags += b2Draw::e_aabbBit;
    //flags += b2Draw::e_shapeBit;
    //flags += b2Draw::e_centerOfMassBit;
    //flags += b2Draw::e_pairBit;
    //flags += b2Draw::e_jointBit;

/**************************GAMEPLAY_ASSETS********************************/
Entity* border{};
Entity* walls{};
Entity* btnDice{};

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
    walls->AddComponent<TransformComponent>(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f, 160.f, 120.f, 3.66f);
    walls->AddComponent<SpriteComponent>(ASSETS_SPRITES_WALLS, 2, 1);

    btnDice = &entityManager.AddEntity("dice");
    btnDice->AddComponent<TransformComponent>(WINDOW_WIDTH * 0.5f + 300.f, WINDOW_HEIGHT * 0.5f + 230.f, 184.f, 198.2f, 0.5f);
    btnDice->AddComponent<SpriteComponent>(ASSETS_SPRITES_DICE, 0, 0);
    btnDice->AddComponent<AnimatorComponent>();
    AnimatorComponent* diceAniamtor = btnDice->GetComponent<AnimatorComponent>();
    diceAniamtor->AddAnimation("idle", AnimationClip("assets/animations/dice/idle.json"));
    diceAniamtor->AddAnimation("1", AnimationClip("assets/animations/dice/1.json"));
    diceAniamtor->AddAnimation("2", AnimationClip("assets/animations/dice/2.json"));
    diceAniamtor->AddAnimation("3", AnimationClip("assets/animations/dice/3.json"));
    diceAniamtor->AddAnimation("4", AnimationClip("assets/animations/dice/4.json"));
    diceAniamtor->AddAnimation("5", AnimationClip("assets/animations/dice/5.json"));
    diceAniamtor->AddAnimation("6", AnimationClip("assets/animations/dice/6.json"));
    diceAniamtor->AddAnimation("7", AnimationClip("assets/animations/dice/7.json"));
    diceAniamtor->AddAnimation("8", AnimationClip("assets/animations/dice/8.json"));
    diceAniamtor->AddAnimation("9", AnimationClip("assets/animations/dice/9.json"));
    diceAniamtor->AddAnimation("10", AnimationClip("assets/animations/dice/10.json"));
    diceAniamtor->AddAnimation("11", AnimationClip("assets/animations/dice/11.json"));
    diceAniamtor->AddAnimation("12", AnimationClip("assets/animations/dice/12.json"));
    diceAniamtor->AddAnimation("13", AnimationClip("assets/animations/dice/13.json"));
    diceAniamtor->AddAnimation("14", AnimationClip("assets/animations/dice/14.json"));
    diceAniamtor->AddAnimation("15", AnimationClip("assets/animations/dice/15.json"));
    diceAniamtor->AddAnimation("16", AnimationClip("assets/animations/dice/16.json"));
    diceAniamtor->AddAnimation("17", AnimationClip("assets/animations/dice/17.json"));
    diceAniamtor->AddAnimation("18", AnimationClip("assets/animations/dice/18.json"));
    diceAniamtor->AddAnimation("19", AnimationClip("assets/animations/dice/19.json"));
    diceAniamtor->AddAnimation("20", AnimationClip("assets/animations/dice/20.json"));

    diceAniamtor->AddAnimation("rol", AnimationClip("assets/animations/dice/rol.json"));
    btnDice->AddComponent<Button>(0.f, sf::Color::Transparent, sf::Color::Transparent, [=](){
      if(gameState == GAME_STATE::GAME)
      {
        gameState = GAME_STATE::ROLLING;
        std::cout << "rolling dice" << std::endl;
        diceAniamtor->Play("rol");
        int rol = 1 + (std::rand() % 20);
        std::cout << rol << std::endl;
      }
    });

    border = &entityManager.AddEntity("border");
    border->AddComponent<TransformComponent>(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f, 96.f, 96.f, 9.f);
    border->AddComponent<SpriteComponent>(ASSETS_SPRITES_UI, 0, 0);

    gameState = GAME_STATE::GAME;
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