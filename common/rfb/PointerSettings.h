/* Copyright (C) 2025 Kasm
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */

#ifndef __RFB_POINTERSETTINGS_H__
#define __RFB_POINTERSETTINGS_H__

#include <string>

namespace rfb {

  class UserConfig;  // Forward declaration

  /**
   * PointerSettings - Per-user pointer/mouse configuration
   *
   */
  class PointerSettings {
  public:
    // Enable/disable pointer input
    bool acceptPointerEvents;               // Whether pointer input is enabled for this user

    // Button controls
    bool PointerAllowLeftClick;        // Allow left mouse button (button 1)
    bool PointerAllowMiddleClick;      // Allow middle mouse button (button 2)
    bool PointerAllowRightClick;       // Allow right mouse button (button 3)
    bool PointerAllowScroll;           // Allow scroll wheel (buttons 4-5 and scrollX/scrollY)

    /**
     * Filter the button mask based on allowed buttons
     * Returns the filtered button mask
     */
    int filterButtonMask(int buttonMask) const;

    /**
     * Constructor - Initializes all settings to safe defaults
     */
    PointerSettings();
    
    /**
     * Load settings from UserConfig object
     *
     * @param config The UserConfig object containing loaded settings
     *
     * This method copies values from the UserConfig object into this DLPSettings instance.
     * It should be called after UserConfig has loaded from YAML or global defaults.
     */
    void loadFromUserConfig(const UserConfig& config);
  };

} // namespace rfb

#endif // __RFB_POINTERSETTINGS_H__
