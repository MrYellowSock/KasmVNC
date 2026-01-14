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

#ifndef __RFB_USERCONFIG_H__
#define __RFB_USERCONFIG_H__

#include <string>

namespace rfb {

  /**
   * UserConfig - Centralized per-user configuration loader
   *
   * This class loads all per-user configuration from YAML files and provides
   * a centralized configuration object that DLPSettings, KeyboardSettings,
   * and PointerSettings can query.
   *
   * Configuration loading precedence:
   * 1. Per-user YAML: ~/.kasmpasswd.d/username.yaml
   * 3. Global server parameters (Server::DLP_*)
   *
   * Usage:
   *   UserConfig config;
   *   config.loadFromFileOrGlobalDefaults(username, kasmpasswdDir);
   *   dlpSettings.loadFromUserConfig(config);
   */
  class UserConfig {
  public:
    // DLP - Clipboard settings
    int DLP_ClipSendMax;
	// Note : Unlike DLP_ClipSendMax, this value cannot be higher that the global server's setting.
    int DLP_ClipAcceptMax;
    int DLP_ClipDelay;

    // DLP - Visual region settings
    std::string DLP_Region;
    bool DLP_RegionAllowClick;
    bool DLP_RegionAllowRelease;

    // Keyboard settings
    int DLP_KeyRateLimit;
    std::string RemapKeys;  // Comma-separated format
    bool KeyboardAllowOnlyKeysInRemap;

    // Pointer settings
    bool PointerAllowLeftClick;
    bool PointerAllowMiddleClick;
    bool PointerAllowRightClick;
    bool PointerAllowScroll;
    bool PointerAllowOtherButtons;

	int acceptPointerEvents;
	int acceptKeyEvents;

    /**
     * Constructor - Initializes all settings to empty/default values
     */
    UserConfig();

    void loadFromFileOrGlobalDefaults(const char* username, const char* kasmpasswdDir);

  private:
    /**
     * Load configuration from YAML file
     *
     * @param username The username to load config for
     * @param kasmpasswdDir The directory containing kasmpasswd and .d/ configs
     * @return true if YAML was parsed successfully, false on error
     */
    bool loadFromFile(const char* username, const char* kasmpasswdDir);

    /**
     * Load configuration from global Server:: parameters
     *
     * Called when YAML loading fails or file doesn't exist.
     */
    void loadGlobalDefaults();

    /**
     * Get the configuration file path for a user
     *
     * @param username The username to get config path for
     * @param kasmpasswdDir The base directory (e.g., path to kasmpasswd file)
     * @return Full path to the user's YAML config file
     */
    static std::string getConfigPath(const char* username, const char* kasmpasswdDir);
  };

} // namespace rfb

#endif // __RFB_USERCONFIG_H__
