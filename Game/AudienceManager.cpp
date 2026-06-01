#include "stdafx.h"
#include "AudienceManager.h"

namespace
{
    struct AudienceData
    {
        Vector3 position;
        float rotationY;
    };

    AudienceData audienceData[] =
    {
       
        //{{-250,0,600}, 150.0f},
        //{{-200,0,640}, 160.0f},
        {{-150,0,680}, 160.0f},
        {{-100,0,720}, 170.0f},
        {{-50,0,760}, 170.0f},
        {{0,0,800}, 180.0f},
        {{50,0,760}, -170.0f},
        {{100,0,720}, -170.0f},
        {{150,0,680}, -160.0f},
        //{{200,0,640}, -160.0f},
        //{{250,0,600}, -150.0f},
        
        //ここまで


    };
}

AudienceManager::AudienceManager()
{
}

AudienceManager::~AudienceManager()
{
}

bool AudienceManager::Start()
{
    const int audienceNum =
        sizeof(audienceData) /
        sizeof(AudienceData);

    for (int i = 0; i < audienceNum; i++)
    {
        Audience* audience =
            NewGO<Audience>(0);

        audience->SetPosition(
            audienceData[i].position);

        Quaternion rot;
        rot.SetRotationY(
            Math::DegToRad(
                audienceData[i].rotationY));

        audience->SetRotation(rot);

        m_audienceList.push_back(
            audience);
    }

    return true;
}

void AudienceManager::Update()
{
}
