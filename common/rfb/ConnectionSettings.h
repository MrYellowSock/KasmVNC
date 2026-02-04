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

#ifndef __RFB_CONNECTIONSETTINGS_H__
#define __RFB_CONNECTIONSETTINGS_H__

namespace rfb {
  class UserConfig;  // Forward declaration

  /**
   * ConnectionSettings - Per-user connection sharing configuration
   */
  class ConnectionSettings {
  public:
    bool neverShared;
    bool disconnectClients;

    /**
     * Constructor - Initializes all settings to safe defaults
     */
    ConnectionSettings();

    /**
     * Load settings from UserConfig object
     *
     * @param config The UserConfig object containing loaded settings
     */
    void loadFromUserConfig(const UserConfig& config);
  };

} // namespace rfb

#endif // __RFB_CONNECTIONSETTINGS_H__
