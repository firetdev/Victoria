#pragma once

#include "component.hpp"

#include <SFML/Audio.hpp>
#include <string>
#include <memory>
#include <iostream>

class AudioPlayer : public Component {
private:
    std::unique_ptr<sf::Sound> sound;
    std::shared_ptr<sf::SoundBuffer> buffer;

public:
    float volume = 100.0f;
    float pitch = 1.0f;
    bool looping = false;

    AudioPlayer(const std::string& filepath)
        : sound(nullptr), buffer(nullptr)
    {
        if (!Load(filepath))
            std::cout << "AudioPlayer failed to initialize: " << filepath << std::endl;
    }

    bool Load(const std::string& filepath) {
        buffer = std::make_shared<sf::SoundBuffer>();

        if (!buffer->loadFromFile(filepath))
            return false;

        sound = std::make_unique<sf::Sound>(*buffer);
        
        ApplyProperties();
        return true;
    }

    void ApplyProperties() {
        if (sound) {
            sound->setVolume(volume);
            sound->setPitch(pitch);
            sound->setLooping(looping);
        }
    }

    void Play() {
        if (sound) {
            ApplyProperties();
            sound->play();
        }
    }

    void Stop() {
        if (sound) sound->stop();
    }

    bool IsPlaying() const {
        return (sound) ? (sound->getStatus() == sf::Sound::Status::Playing) : false;
    }
};
