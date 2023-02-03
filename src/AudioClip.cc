#include "AudioClip.hh"

AudioClip::AudioClip()
{

}
AudioClip::AudioClip(const char* audioUrl)
{
  sound = new sf::Sound();
  this->audioUrl = audioUrl;
}
void AudioClip::SetVolume(float volume)
{
  sound->setVolume(volume);
}
void AudioClip::SetLoop(bool isLoop)
{
  sound->setLoop(isLoop);
}

AudioClip::~AudioClip()
{
}

void AudioClip::Play(sf::SoundBuffer& buffer)
{
  sound->setBuffer(buffer);
  buffer.loadFromFile(audioUrl);
  sound->play();
}