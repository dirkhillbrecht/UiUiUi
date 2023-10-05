// SPDX-License-Identifier: BSD-2-Clause
// (C) 2022 Dirk Hillbrecht

#pragma once

#include "Arduino.h"
#include <U8g2lib.h>

#include "UIArea.h"
#include "UIWidget.h"
#include "UIParent.h"

/** Representation of the display onto which UiUiUi renders its user interface.
 *
 * This is always the topmost class of a UiUiUi interface.
 * It gets gets one widget as parameter which gets the whole screen to render itself upon.
 * Note that UIDisplay itself is NOT an UIWidget, but a UIParent so it can be informed
 * directly if anything has to be rendered at all.
 *
 * UIDisplay converts any actual rendered area into tiles and sends only the changed tiles
 * to the display. It is even possible to restrict the number of tiles sent at once - in this case,
 * render() must be called regulary by some background task.
 */
class UIDisplay: public UIParent {

  public:

    /** Setup the UIDisplay onto the given U8g2 instance and with the given root widget.
     *
     * If doSendBuffer is true, UIDisplay sends always the complete buffer to the display
     * if any changes happened during rendering. Use this if updates of areas do not work
     * on your display. Normally, this should be false.
     *
     * In event-driven setups you may want to restrict the number of operations (and therefore the time spent)
     * in render(). To allow this, you can give a maximum number of tiles sent to the display
     * in the render() call where that actual rendering took place and the maximum number of tiles
     * sent in subsequent calls to render().
     *
     * Note that UIDisplay will in follow cycles _always_ send at least one complete row or column of tiles
     * to the display. So, if you specify "2" as maximum number of tiles but the area to send is "5x3" tiles,
     * actually three tiles will be sent. So, on a 128x64 display, the "maximum minimum" amount
     * of tiles sent at once will always be 8 (64/8).
     *
     * @param root The root widget which will get the whole display to render on.
     */
    UIDisplay(UIWidget* root);

    /** Initialize the whole user interface, layout the widgets.
     *
     * Initializing the UI normally implies the initial rendering.
     * This can be suppressed by setting "render" to false. Then, however the sketch has to
     * call render(&display,true) itself.
     *
     * @param display U8g2 display to work on
     * @param enable If true, explicitly enables the display, false if not. Default is true.
     * @param render If true, performs initial rendering, otherwise not. Default is true.
     */
    void init(U8G2* display,bool enable=true,bool render=true);

    /** Sets or changes the number of tiles to be sent to the display in one chunk.
     *
     * Set this to a small value to prevent longer pauses of execution due to communication
     * with the display, especially if it has a slow serial connection.
     *
     * These values may be changed during sketch execution, even during a running rendering process.
     * They are effective immediately.
     *
     * If this method is not called or if it is called without parameters, the maximum number
     * of tiles is set to "unlimited" (0xffff). In this case, all needed tiles are sent to
     * the screen immediately after rendering. This may need several 100 milliseconds.
     *
     * @param maxFirstUpdateTiles Maximum number of tiles updated at once together with rendering
     * @param maxFollowUpdateTiles Maximum number of tiles updated at once if no rendering happens
     */
    void setUpdateTiles(uint16_t maxFirstUpdateTiles=0xffff,uint16_t maxFollowUpdateTiles=0xffff);

    /** Enable the UI: It presents itself on screen, display is turned on.
     *
     * @param display U8g2 display to work on
     * @param render If true, renders the display explicitly on enabling, if false, not. Default is false.
     * @param force If true, forces rendering to a complete re-render, otherwise normal rendering.
     *   Default is false
     */
    void enable(U8G2* display,bool render=false,bool force=false);

    /** Disables the UI: No rendering and display is switched to powersaving mode.
     *
     * Usually, the display will be switched off after this call while its memory contents are preserved.
     * If the display is disabled this way, render() will not perform any actions and will be very fast.
     * That's important if the interface is updated asynchronously in the background.
     *
     * @param display U8g2 display to render on
     */
    void disable(U8G2* display);

    /** Deactivates the UI: It will be shown but no rendering will be performed.
     *
     * Calling this method disables the render() process completely.
     * Calling render() will return in almost no time.
     * The display will simply stay frozen until activate() is called.
     *
     * Calling this method makes sense is rendering is performed by a background task
     * which is executed regulary and there are time-critical processes ongoing which
     * prevent a normal render() run.
     *
     * If you draw the UI synchronously, i.e. by directly calling render() in loop() or so,
     * it often makes more sense to simply not call render() during such processes.
     */
    void deactivate();

    /** Activates the UI: Rendering will be performed if render() is called.
     *
     * This is the counterpart to deactivate(). Rendering of the UI will resume.
     * Note that this method does *not* call render() internally.
     *
     * This method, as deactivate(), will normally be used in asynchronous setups
     * where render() is called regulary in the background. After activate(), the
     * next render() call will be performed anyway and resume the usual interface updates.
     */
    void activate();

    /** Called by the root widget to indicate that rendering must be performed. */
    void childNeedsRendering(UIWidget *child);

    /** Render the interface, update everything that has changed since the last render() call.
     *
     * Rendering is performed by calling the render() method of every widget in the UI
     * which is currently visible (UICards being the the widget group which can render
     * a widget invisible).
     *
     * @param display U8g2 display to render upon
     * @param force Flag whether a complete rendering should take place (true) or
     *   only the parts are updated which need a redraw (false). Default is false.
     */
    void render(U8G2* display,bool force=false);

    /** Returns whether the UI is updating the display due to some earlier call to render().
     *
     * This method only makes sense if the number of updated tiles is limited and display
     * updates could be split into several calls of render(). Then, this method returns true
     * if such delayed updates are still vacant. If not, it returns false. This also means
     * that calls to render() will return almost immediately. */
    bool isUpdatingDisplay();

  private:

    /** The root UI widget */
    UIWidget* root;

    /** Flag whether the UI is currently enabled, i.e. it renders itself. */
    bool enabled;

    /** Flag whether the UI is active, i.e. calls to render() will update the UI. */
    bool active;

    /** Flag if actual rendering must be performed. */
    bool renderingNeeded;

    /** Maximum number of tiles which are updated in a display update cycle with actual rendering */
    uint16_t maxFirstUpdateTiles;

    /** Maximum number of tiles which are updated in a display update cycle without actual rendering */
    uint16_t maxFollowUpdateTiles;

    /** Dimension IN TILES to actually update */
    UIArea updateTiles;

    /** Dimension IN TILES to update after the current update tiles have been finished */
    UIArea furtherUpdateTiles;

    /** Update tiles on the display.
     *
     * Tiles to be updated are stored in the updateTiles field.
     *
     * @param maxUpdateTiles Maximum number of tiles to update at once (at least one row or column)
     */
    void doUpdateTiles(U8G2 *display,uint16_t maxUpdateTiles);
	
	
	/** Calculates the tiles on the display that correspond to the pixels in the given area.
	 *
	 * Each tile is 8 pixels tall and 8 pixels wide.
	 * Rotation of the display IS taken into account.
	 */
	void computeTileAreaFromPixelArea(U8G2 *display, UIArea * pixelArea, UIArea * tileArea);

};

// end of file
