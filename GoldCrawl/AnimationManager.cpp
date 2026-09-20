#include "AnimationManager.h"

AnimationManager& AnimationManager::only()
{
    static AnimationManager instance;
    return instance;
}

void AnimationManager::addOneShot(const std::string& filepath, SDL_Rect bounds)
{
    auto spriteAnimation = std::make_shared<SpriteAnimation>(filepath, "", false);
    auto animation = std::make_shared<Animation>(spriteAnimation, bounds);
    animations.push_back(animation);
}

void AnimationManager::update(Uint64 delta)
{
    // https://en.cppreference.com/cpp/container/vector/erase2
    std::erase_if(animations, [](auto& anim) { return anim->animation->isComplete(); });

    for (auto& animation : animations)
    {
        animation->animation->update(delta);
    }
}

void AnimationManager::render(const Renderer& renderer, Vector mapOffset) const
{
    for (auto& animation : animations)
    {
        SDL_Rect bounds{
            animation->bounds.x - mapOffset.x,
            animation->bounds.y - mapOffset.y,
            animation->bounds.w,
            animation->bounds.h
        };
        renderer.drawSprite(animation->animation->getSprite(), bounds);
    }
}
