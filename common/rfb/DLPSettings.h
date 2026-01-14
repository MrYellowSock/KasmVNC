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

#ifndef __RFB_DLPSETTINGS_H__
#define __RFB_DLPSETTINGS_H__

#include <string>
#include <rdr/types.h>

namespace rfb {

  class UserConfig;  // Forward declaration

  /**
   * DLPSettings - Per-user Data Loss Prevention configuration
   *
   */
  class DLPSettings {
  public:
    // Visual Region Settings
    bool regionEnabled;           // Is visual region restriction enabled?
    bool regionPercents;          // Are coordinates in percentages (vs absolute pixels)?
    int regionX1, regionY1;       // Top-left corner of allowed region
    int regionX2, regionY2;       // Bottom-right corner of allowed region
    rdr::U16 regionPcX1, regionPcY1;  // Percentage coordinates (0-10000 for 0-100.00%)
    rdr::U16 regionPcX2, regionPcY2;
    bool regionAllowClick;        // Allow pointer clicks outside region?
    bool regionAllowRelease;      // Allow pointer releases outside region?

    // Clipboard Settings
    int DLP_ClipSendMax;              // Max bytes to send to client (server->client)
    int DLP_ClipAcceptMax;            // Max bytes to accept from client (client->server)
    int DLP_ClipDelay;                // Min milliseconds between clipboard operations

    /**
     * Constructor - Initializes all settings to safe defaults
     */
    DLPSettings();


    /**
     * Load settings from UserConfig object
     *
     * @param config The UserConfig object containing loaded settings
     *
     * This method copies values from the UserConfig object into this DLPSettings instance.
     * It should be called after UserConfig has loaded from YAML or global defaults.
     */
    void loadFromUserConfig(const UserConfig& config);

    /**
     * Parse a DLP_Region string and populate region settings
     *
     * @param regionStr Region string in format "x1,y1,x2,y2" (absolute or percent)
     *
     * Examples:
     *   "100,50,800,600"        - Absolute pixel coordinates
     *   "10%,10%,90%,90%"       - Percentage coordinates
     *
     * Sets regionEnabled, regionPercents, and coordinate fields.
     * Logs error and exits if format is invalid (for compatibility with original behavior).
     */
    void parseRegion(const char* regionStr);

    /**
     * Translate region coordinates to absolute pixels
     *
     * @param x1, y1, x2, y2 Output parameters for absolute pixel coordinates
     * @param fbWidth Width of the framebuffer in pixels
     * @param fbHeight Height of the framebuffer in pixels
     *
     * Converts percentage-based coordinates to absolute pixels based on framebuffer size.
     * Clamps values to framebuffer boundaries.
     */
    void translateRegion(rdr::U16 &x1, rdr::U16 &y1, rdr::U16 &x2, rdr::U16 &y2,
                        int fbWidth, int fbHeight) const;

  };

} // namespace rfb

#endif // __RFB_DLPSETTINGS_H__
