#include "SpriteAnimation.h"
#include <nlohmann/json.hpp>
#include "FileSystem.h"

using namespace nlohmann;

SpriteAnimation::SpriteAnimation(const std::string& filepath) :
    currentFrame(nullptr),
    currentSprite(std::make_unique<Sprite>(filepath)),
    frameIndex(0)
{
    json j = json::parse(bacon::fs::readText(filepath).value_or(""));
    if (!j.contains("frames") || !j["frames"].is_array())
    {
        SDL_LogError(0, "no frames, no animation");
        return;
    }

    currentSprite->filepath = "Sprites/" + j["meta"]["image"].get<std::string>();

    for (auto& jFrame : j["frames"])
    {
        auto frame = std::make_unique<SpriteFrame>();
        frame->src.x = jFrame["frame"]["x"];
        frame->src.y = jFrame["frame"]["y"];
        frame->src.w = jFrame["frame"]["w"];
        frame->src.h = jFrame["frame"]["h"];
        frame->duration = jFrame["duration"];

        frames.push_back(std::move(frame));
    }

    if (j["meta"].contains("frameTags") && j["meta"]["frameTags"].is_array())
    {
        for (auto& jTag : j["meta"]["frameTags"])
        {
            std::string name = jTag["name"];
            int startFrame = jTag["from"];
            int endFrame = jTag["to"];
            cycles.emplace(name, std::vector<SpriteFrame*>{});
            for (int i = startFrame; i <= endFrame; ++i)
            {
                cycles[name].push_back(frames[i].get());
            }
        }
        setCycle(j["meta"]["frameTags"][0]["name"]);
    }
    else
    {
        cycles.emplace("", std::vector<SpriteFrame*>{});
        for (int i = 0; i < frames.size();++i)
        {
            cycles[""].push_back(frames[i].get());
        }
        setCycle("");
    }
}

void SpriteAnimation::setCycle(const std::string& cycle)
{
    if (cycles.find(cycle) == cycles.end())
    {
        SDL_LogError(0, "no such cycle %s", cycle.c_str());
        return;
    }

    currentCycle = cycle;
    frameIndex = 0;
    currentFrame = cycles[cycle][0];
    currentSprite->sourceRect = currentFrame->src;
    hold = currentFrame->duration;
}

void SpriteAnimation::update(Uint64 deltaMillis)
{
    if (frames.empty()) return;

    hold -= deltaMillis;
    if (hold <= 0)
    {
        ++frameIndex;
        if (frameIndex >= cycles[currentCycle].size())
        {
            frameIndex = 0;
        }
        currentFrame = cycles[currentCycle][frameIndex];
        currentSprite->sourceRect = currentFrame->src;
        hold = currentFrame->duration;
    }
}
