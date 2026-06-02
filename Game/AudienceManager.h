#pragma once

#include "Game.h"
#include "Audience.h"
#include <vector>

class AudienceManager : public IGameObject
{
public:
    AudienceManager();
    ~AudienceManager();

    bool Start();
    void Update();

private:
    std::vector<Audience*> m_audienceList;


};
