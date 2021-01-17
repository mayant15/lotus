#include "ShaderHotReload.h"

#include <lotus/ecs/EventManager.h>
#include <lotus/resources/Shader.h>
#include <lotus/debug.h>
#include <lotus/Input.h>

namespace Lotus
{
    void onPressR(const KeyboardEvent& event)
    {
        if (Input::GetKeyPressed(L_KEY_R))
        {
            GET(EventManager).Dispatch(ShaderModifyEvent {});
        }
    }

    void reloadShaders(const ShaderModifyEvent& event)
    {
        using namespace Lotus;
        LOG_INFO("Reloading shaders");

        // TODO: Reload only the updated shader
        ForEachAsset<Shader>([](Handle<Shader> shader) {
            shader->Reload();
        });
    }

    void ShaderHotReloadInit()
    {
        auto& em = GET(EventManager);
        em.Bind<ShaderModifyEvent, reloadShaders>();

        // TODO: Watch filesystem for changes
        // https://docs.microsoft.com/en-us/windows/win32/fileio/obtaining-directory-change-notifications?redirectedfrom=MSDN
        em.Bind<KeyboardEvent, onPressR>();
    }
}
