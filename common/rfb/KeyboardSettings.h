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

#ifndef __RFB_KEYBOARDSETTINGS_H__
#define __RFB_KEYBOARDSETTINGS_H__

#include <string>

namespace rfb {
  class UserConfig;  // Forward declaration

  /**
   * KeyboardSettings - Per-user keyboard configuration
   */
  class KeyboardSettings {
  public:
    // Rate limiting
    int DLP_KeyRateLimit;              // Max keys per second (0 = unlimited)
	bool acceptKeyEvents;

    // Key remapping
    std::string RemapKeys;      // Comma-separated remap string: "0x22->0x40,0x24<>0x41"
    bool KeyboardAllowOnlyKeysInRemap;  // If true, only allow keys that are in the remap table

    /**
     * Constructor - Initializes all settings to safe defaults
     */
    KeyboardSettings();
    
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

#endif // __RFB_KEYBOARDSETTINGS_H__
