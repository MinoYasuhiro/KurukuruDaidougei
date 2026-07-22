#pragma once

enum class LoadingState
{
    ResetPlayer,
    ResetCamera,
    ResetSpawner,
    ResetCoinBox,

    ResetCircle,
    ResetArrow,
    ResetCoinEffect,
    ResetConfetti,
    ResetMissEffect,

    Finish
};

class LoadingManager : public IGameObject
{
public:
    bool Start();
    void Update();

    float GetProgress() const
    {
        return m_progress;
    }


    bool IsFinished() const
    {
        return m_isFinished;
    }

private:
    void UpdateProgress(int currentStep);
    void SetProgress( int currentStep,int totalStep );
private:
    LoadingState m_state;

    float m_progress = 0.0f;

    bool m_isFinished = false;
};