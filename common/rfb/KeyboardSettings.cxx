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

#include <rfb/KeyboardSettings.h>
#include <rfb/ServerCore.h>
#include <rfb/LogWriter.h>
#include <rfb/UserConfig.h>

using namespace rfb;

static LogWriter vlog("KeyboardSettings");

KeyboardSettings::KeyboardSettings()
{
  // Initialize all settings to safe defaults
  DLP_KeyRateLimit = 0;      // 0 = unlimited
  RemapKeys = "";     // empty = no remapping
  acceptKeyEvents = false;
}

void KeyboardSettings::loadFromUserConfig(const UserConfig& config)
{
  DLP_KeyRateLimit = config.DLP_KeyRateLimit;
  RemapKeys = config.RemapKeys;
  acceptKeyEvents = config.acceptKeyEvents;
}
