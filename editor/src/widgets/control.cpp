#include <widgets.h>
#include <IconsForkAwesome.h>

namespace Editor::Widgets
{
    void Control()
    {
        static bool show = true;
        ImGui::Begin("Controls", &show);
        ImGui::Button(ICON_FK_PLAY " Play");
        ImGui::SameLine();
        ImGui::Button(ICON_FK_PAUSE " Pause");
        ImGui::End();
    }
}
