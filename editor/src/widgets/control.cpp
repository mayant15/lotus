#include <widgets.h>
#include <IconsForkAwesome.h>
#include <lotus/ecs/EventManager.h>

namespace Editor::Widgets
{
    enum class EPlayState
    {
        PLAYING, PAUSED, STOPPED
    };

    void Control()
    {
        static bool show = true;
        static EPlayState state = EPlayState::STOPPED;

        ImGui::Begin("Controls", &show);
        auto& em = Lotus::EventManager::Get();

        if (ImGui::Button(ICON_FK_PLAY " Play") && state != EPlayState::PLAYING)
        {
            // Start scene
            em.Dispatch(Lotus::SimulationBeginEvent {});
            state = EPlayState::PLAYING;
        }

        ImGui::SameLine();

        if (ImGui::Button(ICON_FK_PAUSE " Pause") && state == EPlayState::PLAYING)
        {
            // Pause scene
            em.Dispatch(Lotus::SimulationPauseEvent {});
            state = EPlayState::PAUSED;
        }

        ImGui::SameLine();

        if (ImGui::Button(ICON_FK_STOP " Stop") && state != EPlayState::STOPPED)
        {
            // Stop scene
            em.Dispatch(Lotus::SimulationEndEvent {});
            state = EPlayState::STOPPED;
        }

        ImGui::End();
    }
}
