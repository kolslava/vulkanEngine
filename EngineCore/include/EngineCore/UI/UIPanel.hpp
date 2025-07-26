#pragma once

/**
 * @class UIPanel
 * @brief An abstract base class for all UI panels in the editor.
 *
 * This class defines the common interface that all UI panels must implement.
 * The Application class will iterate over a list of UIPanel pointers and call
 * OnImGuiRender() for each one every frame.
 */
class UIPanel
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~UIPanel() = default;

    /**
     * @brief Pure virtual function to render the panel's UI.
     *
     * Derived classes must implement this function to draw their specific
     * user interface using ImGui commands.
     */
    virtual void OnImGuiRender() = 0;
};
