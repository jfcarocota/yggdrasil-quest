#pragma once
#include<SFML/Graphics.hpp>
#include "Components/Component.hh"
#include "Components/TransformComponent.hh"
#include<string>

class TextComponent: public Component
{
private:
  TransformComponent* transform;
  sf::Font font{sf::Font()};
  sf::Text text{};
  std::string fontUrl;
  int size{};
  sf::Color color;
  std::string textStr{};
public:
  TextComponent(std::string fontUrl, int size, sf::Color color, sf::Uint32 style);
  void SetTextStr(std::string textStr);
  ~TextComponent();
  void Initialize() override;
  void Render(sf::RenderWindow& window) override;

};