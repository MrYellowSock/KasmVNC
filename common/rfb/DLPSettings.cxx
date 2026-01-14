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

#include <rfb/DLPSettings.h>
#include <rfb/UserConfig.h>
#include <rfb/ServerCore.h>
#include <rfb/LogWriter.h>
#include <stdlib.h>
#include <string.h>

using namespace rfb;

static LogWriter vlog("DLPSettings");

DLPSettings::DLPSettings()
{
  // Initialize all settings to safe defaults (DLP disabled by default)

  // Visual Region - disabled by default
  regionEnabled = false;
  regionPercents = false;
  regionX1 = 0;
  regionY1 = 0;
  regionX2 = 0;
  regionY2 = 0;
  regionPcX1 = 0;
  regionPcY1 = 0;
  regionPcX2 = 0;
  regionPcY2 = 0;
  regionAllowClick = false;
  regionAllowRelease = false;

  // Clipboard - no limits by default
  DLP_ClipSendMax = 0;        // 0 = unlimited
  DLP_ClipAcceptMax = 0;      // 0 = unlimited
  DLP_ClipDelay = 0;          // 0 = no delay
}

void DLPSettings::loadFromUserConfig(const UserConfig& config)
{
  // Load all DLP settings from UserConfig
  DLP_ClipSendMax = config.DLP_ClipSendMax;
  DLP_ClipAcceptMax = config.DLP_ClipAcceptMax;
  DLP_ClipDelay = config.DLP_ClipDelay;

  // Parse visual region if provided
  if (!config.DLP_Region.empty()) {
    parseRegion(config.DLP_Region.c_str());
  }
  regionAllowClick = config.DLP_RegionAllowClick;
  regionAllowRelease = config.DLP_RegionAllowRelease;
}

static void mixedPercentages() {
  vlog.error("Mixing percentages and absolute values in DLP_Region is not allowed");
  exit(1);
}

static void parseRegionPart(const bool percents, rdr::U16 &pcdest, int &dest,
                            char **inptr) {
  char *nextptr, *ptr;
  ptr = *inptr;
  int val = strtol(ptr, &nextptr, 10);
  if (!*ptr || ptr == nextptr) {
    vlog.error("Invalid value for DLP_Region");
    exit(1);
  }
  ptr = nextptr;
  if (*ptr == '%') {
    if (!percents)
      mixedPercentages();
    pcdest = val;

    if (val < 0 || val > 100) {
      vlog.error("Percent must be 0-100");
      exit(1);
    }

    ptr++;
  } else if (percents) {
    mixedPercentages();
  }
  dest = val;

  for (; *ptr && *ptr == ','; ptr++);

  *inptr = ptr;
}

void DLPSettings::parseRegion(const char* regionStr)
{
  if (!regionStr || !regionStr[0]) {
    regionEnabled = false;
    return;
  }

  unsigned len = strlen(regionStr);
  unsigned i;
  unsigned commas = 0;
  int val;
  char *ptr, *nextptr;

  for (i = 0; i < len; i++) {
    if (regionStr[i] == ',')
      commas++;
  }

  if (commas != 3) {
    vlog.error("DLP_Region must contain four values");
    exit(1);
  }

  ptr = (char *) regionStr;

  val = strtol(ptr, &nextptr, 10);
  if (!*ptr || ptr == nextptr) {
    vlog.error("Invalid value for DLP_Region");
    exit(1);
  }
  ptr = nextptr;
  if (*ptr == '%') {
    regionPercents = true;
    regionPcX1 = val;
    ptr++;
  }
  regionX1 = val;

  for (; *ptr && *ptr == ','; ptr++);

  parseRegionPart(regionPercents, regionPcY1, regionY1, &ptr);
  parseRegionPart(regionPercents, regionPcX2, regionX2, &ptr);
  parseRegionPart(regionPercents, regionPcY2, regionY2, &ptr);

  // Validity checks
  if (!regionPercents) {
    if (regionX1 > 0 && regionX2 > 0 && regionX2 <= regionX1) {
      vlog.error("DLP_Region x2 must be > x1");
      exit(1);
    }
    if (regionY1 > 0 && regionY2 > 0 && regionY2 <= regionY1) {
      vlog.error("DLP_Region y2 must be > y1");
      exit(1);
    }
  }

  regionEnabled = true;
}

void DLPSettings::translateRegion(rdr::U16 &x1, rdr::U16 &y1, rdr::U16 &x2, rdr::U16 &y2,
                                  int fbWidth, int fbHeight) const
{
  if (regionPercents) {
    x1 = regionPcX1 ? regionPcX1 * fbWidth / 100 : 0;
    y1 = regionPcY1 ? regionPcY1 * fbHeight / 100 : 0;
    x2 = regionPcX2 ? (100 - regionPcX2) * fbWidth / 100 : fbWidth;
    y2 = regionPcY2 ? (100 - regionPcY2) * fbHeight / 100 : fbHeight;
  } else {
    x1 = abs(regionX1);
    y1 = abs(regionY1);
    x2 = fbWidth;
    y2 = fbHeight;

    if (regionX2 < 0)
      x2 += regionX2;
    else if (regionX2 > 0)
      x2 = regionX2;

    if (regionY2 < 0)
      y2 += regionY2;
    else if (regionY2 > 0)
      y2 = regionY2;
  }

  if (y2 > fbHeight)
    y2 = fbHeight - 1;
  if (x2 > fbWidth)
    x2 = fbWidth - 1;
}
