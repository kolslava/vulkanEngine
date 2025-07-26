#pragma once
#include "EngineCore/UI/UIPanel.hpp"

// Forward declaration of Application to avoid including the full header.
class Application; 

/**
 * @class MainMenuPanel
 * @brief A UI panel that renders the main menu bar for the application.
 *
 * This panel typically includes menus like "File", "Edit", etc., with options
 * that can trigger application-level actions.
 */
class MainMenuPanel : public UIPanel
{
public:
    /**
     * @brief Constructs a MainMenuPanel.
     * @param app A pointer to the main Application instance, used to trigger
     *            actions like closing the application.
     */
    MainMenuPanel(Application* app);

    /**
     * @brief Renders the main menu bar using ImGui.
     */
    void OnImGuiRender() override;

private:
    /// @brief A pointer to the Application instance.
    Application* m_app;
};
