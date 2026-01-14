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

#include <rfb/PointerSettings.h>
#include <rfb/ServerCore.h>
#include <rfb/LogWriter.h>
#include <rfb/UserConfig.h>

using namespace rfb;

static LogWriter vlog("PointerSettings");

PointerSettings::PointerSettings()
{
  // Initialize all settings to safe defaults
  acceptPointerEvents = true;           // Pointer input enabled by default
  PointerAllowLeftClick = true;    // Left click allowed by default
  PointerAllowMiddleClick = true;  // Middle click allowed by default
  PointerAllowRightClick = true;   // Right click allowed by default
  PointerAllowScroll = true;       // Scroll allowed by default
  PointerAllowOtherButtons = true; // Extended buttons allowed by default
}

int PointerSettings::filterButtonMask(int buttonMask) const
{
  int filteredMask = buttonMask;

  // Button mask bits (X11 standard):
  // Bit 0 (0x01): Left button
  // Bit 1 (0x02): Middle button
  // Bit 2 (0x04): Right button
  // Bit 3 (0x08): Scroll up (button 4)
  // Bit 4 (0x10): Scroll down (button 5)

  if (!PointerAllowLeftClick) {
    filteredMask &= ~0x01;
  }
  if (!PointerAllowMiddleClick) {
    filteredMask &= ~0x02;
  }
  if (!PointerAllowRightClick) {
    filteredMask &= ~0x04;
  }
  if (!PointerAllowScroll) {
    // Clear scroll buttons (buttons 4 and 5)
    filteredMask &= ~0x18;
  }
  if (!PointerAllowOtherButtons) {
    // filter only known button.
    filteredMask &= (0x01|0x02|0x04|0x18);
  }

  return filteredMask;
}

void PointerSettings::loadFromUserConfig(const UserConfig& config)
{
  acceptPointerEvents = config.acceptPointerEvents;
  PointerAllowLeftClick = config.PointerAllowLeftClick;
  PointerAllowMiddleClick = config.PointerAllowMiddleClick;
  PointerAllowRightClick = config.PointerAllowRightClick;
  PointerAllowScroll = config.PointerAllowScroll;
  PointerAllowOtherButtons = config.PointerAllowOtherButtons;
}
