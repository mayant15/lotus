#pragma once

#include <imgui/imgui.h>
#include <utils.h>

#include <lotus/ecs/EventManager.h>
#include <lotus/ILifecycle.h>
#include <lotus/scene/SceneManager.h>

/**
 * Place common ones here that don't need much logic to work
 */
namespace Editor::Panel
{
    ImVec2 viewportDims {1280.0f, 720.0f};

    void MainDockSpace()
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    }

    void DemoWindow()
    {
        ImGui::ShowDemoWindow();

    }

    void Viewport(unsigned int tex, float ux, float uy)
    {
        static bool show = true;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});

        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoScrollbar;
        ImGui::Begin("Viewport", &show, flags);

        // Render last texture, update window size

//        ImGui::Image((ImTextureID) tex, viewportDims);

        // NOTE: The viewport setup is kind of convoluted. The color buffer size always stays constant.
        // When the viewport size changes, the portion of the color buffer that is rendered to changes, while maintaining
        // the aspect ratio. This portion is controlled through glViewport, and the logic is in the renderer. See
        // Lotus::Renderer::SetViewport and Lotus::Renderer::OnUpdate
        //
        // Then, Lotus::Renderer::GetViewportUV returns the portion of the color buffer that is rendered to, normalized
        // to be a fraction, i.e, if [0, 1]x[0, 1] is the color buffer, then [0, ux]x[0, uy] is the rendered region.
        // This is then passed to imgui as the UVs to map to the window, essentially "cropping" the texture.
        // TODO: Is this faster than blitting to another texture? Idk, I'll probably have to create a new texture to blit
        //   to every frame in order to account for the changing viewport size? That's why I implemented it this way
        ImGui::GetWindowDrawList()->AddImage(
                (void *)tex,
                ImVec2(ImGui::GetCursorScreenPos()),
                ImVec2(ImGui::GetCursorScreenPos().x + viewportDims.x,
                       ImGui::GetCursorScreenPos().y + viewportDims.y), ImVec2(0, uy), ImVec2(ux, 0));

        // Update viewport dims to render the next frame
        viewportDims = ImGui::GetContentRegionAvail();

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void MainMenu(float fps)
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Scene"))
                {
                    // TODO: Do this somewhere else
                    std::optional<std::string> filepath = OpenFileDialog();
                    if (filepath.has_value())
                    {
                        Lotus::SceneManager::LoadScene(filepath.value());
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z"))
                {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
                {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X"))
                {}
                if (ImGui::MenuItem("Copy", "CTRL+C"))
                {}
                if (ImGui::MenuItem("Paste", "CTRL+V"))
                {}
                ImGui::EndMenu();
            }

            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("FPS: %.1f", fps);

            ImGui::EndMainMenuBar();
        }
    }

    struct MyLog
    {
        ImGuiTextBuffer Buf;
        ImGuiTextFilter Filter;
        ImVector<int> LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
        bool AutoScroll;  // Keep scrolling if already at the bottom.

        MyLog()
        {
            AutoScroll = true;
            Clear();
        }

        void Clear()
        {
            Buf.clear();
            LineOffsets.clear();
            LineOffsets.push_back(0);
        }

        void AddLog(const char* fmt, ...)

        IM_FMTARGS(2)
        {
            int old_size = Buf.size();
            va_list args;
                    va_start(args, fmt);
            Buf.appendfv(fmt, args);
                    va_end(args);
            for (int new_size = Buf.size(); old_size < new_size; old_size++)
                if (Buf[old_size] == '\n')
                    LineOffsets.push_back(old_size + 1);
        }

        void Draw(const char* title, bool* p_open = NULL)
        {
            if (!ImGui::Begin(title, p_open))
            {
                ImGui::End();
                return;
            }

            // Options menu
            if (ImGui::BeginPopup("Options"))
            {
                ImGui::Checkbox("Auto-scroll", &AutoScroll);
                ImGui::EndPopup();
            }

            // Main window
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            bool clear = ImGui::Button("Clear");
            ImGui::SameLine();
            bool copy = ImGui::Button("Copy");
            ImGui::SameLine();
            Filter.Draw("Filter", -100.0f);

            ImGui::Separator();
            ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            if (clear)
                Clear();
            if (copy)
                ImGui::LogToClipboard();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = Buf.begin();
            const char* buf_end = Buf.end();
            if (Filter.IsActive())
            {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have a random access on the result on our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1)
                                                                            : buf_end;
                    if (Filter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                // The simplest and easy way to display the entire buffer:
                //   ImGui::TextUnformatted(buf_begin, buf_end);
                // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                // within the visible area.
                // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                // on your side is recommended. Using ImGuiListClipper requires
                // - A) random access into your data
                // - B) items all being the  same height,
                // both of which we can handle since we an array pointing to the beginning of each line of text.
                // When using the filter (in the block of code above) we don't have random access into the data to display
                // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                // it possible (and would be recommended if you want to search through tens of thousands of entries).
                ImGuiListClipper clipper;
                clipper.Begin(LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1)
                                                                                : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::EndChild();
            ImGui::End();
        }
    };

    void Log(bool* p_open)
    {
        static MyLog log;

        // For the demo: add a debug button _BEFORE_ the normal log window contents
        // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
        // Most of the contents of the window will be added by the log.Draw() call.
        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin("Log", p_open);
        if (ImGui::SmallButton("[Debug] Add 5 entries"))
        {
            static int counter = 0;
            const char* categories[3] = {"info", "warn", "error"};
            const char* words[] = {"Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate",
                                   "Nincompoop", "Pauciloquent"};
            for (int n = 0; n < 5; n++)
            {
                const char* category = categories[counter % IM_ARRAYSIZE(categories)];
                const char* word = words[counter % IM_ARRAYSIZE(words)];
                log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                           ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
                counter++;
            }
        }
        ImGui::End();

        // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
        log.Draw("Log", p_open);
    }
}
