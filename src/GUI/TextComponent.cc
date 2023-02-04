#include "GUI/TextComponent.hh"
#include "Components/EntityManager.hh"

TextComponent::TextComponent(std::string fontUrl, int size, sf::Color color, sf::Uint32 style)
{
  this->fontUrl = fontUrl;
  this->size = size;
  this->color = color;

  font.loadFromFile(fontUrl);
  text = sf::Text(fontUrl, font, size);
  text.setFillColor(color);
  text.setStyle(style);
}

TextComponent::~TextComponent()
{
}

void TextComponent::SetTextStr(std::string textStr)
{
  text.setString(textStr);
}

void TextComponent::Initialize()
{
  transform = owner->GetComponent<TransformComponent>();
  text.setPosition(transform->GetPosition() + sf::Vector2f(10.f, 10.f));
  text.setOrigin(transform->GetWidth() / 2, transform->GetHeight() / 2);
}

void TextComponent::Render(sf::RenderWindow& window)
{
  window.draw(text);
}