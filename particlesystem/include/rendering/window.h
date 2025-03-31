#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string_view>
#include <span>

/// This namespace contains objects that are helper functions that support the rendering
/// of general ui elements and graphics primitives.
namespace rendering {

class Window {
public:
    Window(std::string_view title, int width, int height);
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;
    ~Window();

    void setTitle(std::string_view title);

    // Returns a time value in seconds since window was created
    double time() const;

    // Returns bool which signifies if the user has requested to close the window
    bool shouldClose() const;

    // Returns the current size of the window in pixels
    glm::vec2 size() const;

    // Sets up the initial frame state and must be called in the beginning of the frame. It
    // will poll and respond to all the keyboard and mouse input and set up the user interface
    // that can be called during the frame.
    void beginFrame();

    // Finalizes the current frame and swaps the front and back buffers for double buffering.
    void endFrame();

    // Clear the window with specific color, each channel is in range [0,1]
    void clear(glm::vec4 color);

    // Draws points on screen
    void drawPoint(glm::vec2 pos, float radius, glm::vec4 color);
    void drawPoints(std::span<const glm::vec2> pos, std::span<const float> radius,
                    std::span<const glm::vec4> color);

    // Draws points by extracting fields from packed data.
    // The stride represents the number of bytes between each sphere primitive.
    void drawPoints(const glm::vec2* pos, const float* radius, const glm::vec4* color, size_t count,
                    size_t stride_in_bytes = 0);

    // UI
    void beginGuiWindow(std::string_view label);
    void endGuiWindow();

    /**
     * * Draws simple text to the gui
     *
     * If the user changed the value for the slider, the return value will be
     * \c true and \p value contains the changed value. If the user
     * did not interact with the slider, the value passed to \p value will not be
     * modified.
     *
     * \pre \p label must not be a nullptr
     * \pre \p label must not be empty
     * \pre \p minValue must be smaller than \p maxValue
     *
     * \param value The in/out parameter for the value displayed by the slider
     * \return \c true if the user interacted with the slider, \c false otherwise
     */
    void text(std::string_view text);
    void text(std::string_view text, glm::vec4 color);

    /**
     * * Draws a slider with the provided \p label and the minimum value of \p minValue
     * and maximum value of \p maxValue.
     *
     * If the user changed the value for the slider, the return value will be
     * \c true and \p value contains the changed value. If the user
     * did not interact with the slider, the value passed to \p value will not be
     * modified.
     *
     * \pre \p label must not be a nullptr
     * \pre \p label must not be empty
     * \pre \p minValue must be smaller than \p maxValue
     *
     * \param value The in/out parameter for the value displayed by the slider
     * \return \c true if the user interacted with the slider, \c false otherwise
     */

    bool sliderFloat(std::string_view label, float& value, float min, float max);
    bool sliderVec2(std::string_view label, glm::vec2& value, float min, float max);
    bool sliderInt(std::string_view label, int& value, int min, int max);

    /**
     * Draws a color picker into the UI. If the user changed the color, the return
     * value will be \c true and \p r, \p g, and \p b contain the color components of the
     * new color. If the user did not interact with the picker, the passed color will not
     * be modified.
     *
     * \pre \p label must not be a nullptr
     * \pre \p label must not be empty
     * \param color The in/out parameter for the color, all components (r,g,b) operate in the range
     * [0, 1] \return \c true if the user picked a new color, \c false otherwise
     */
    bool colorPicker(std::string_view label, glm::vec4& color);

    /**
     * Draws a new button with the provided \p label. If the user pressed the button, this function
     * returns \c true, otherwise, \c false is returned.
     *
     * \param label The text that is written inside the button
     * \pre \p label must not be a nullptr
     * \pre \p label must not be empty
     *
     * \return \c true if the user pressed the button, \c false otherwise
     */
    bool button(std::string_view label);

    /**
     * Draws a checkbox with the provided \p label. If the user modifies the value through
     * interaction, this function returns \c true, otherwise, \c false is returned.
     *
     * \param label The text that is written next to the checkbox
     * \pre \p label must not be a nullptr
     * \pre \p label must not be empty
     *
     * \return \c true if the user pressed the checkbox, \c false otherwise
     */
    bool checkbox(std::string_view label, bool& value);

    // Horizontal line separator
    void separator();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}  // namespace rendering
