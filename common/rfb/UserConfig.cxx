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

#include "ServerCore.h"
#include <rfb/UserConfig.h>
#include <rfb/ServerCore.h>
#include <rfb/LogWriter.h>
#include <yaml-cpp/yaml.h>
#include <sys/stat.h>

using namespace rfb;

static LogWriter vlog("UserConfig");

std::string UserConfig::getConfigPath(const char* username, const char* kasmpasswdDir)
{
  std::string basePath(kasmpasswdDir);
  size_t lastSlash = basePath.find_last_of("/");
  if (lastSlash != std::string::npos) {
    basePath = basePath.substr(0, lastSlash);
  }
  std::ostringstream oss;
  oss << basePath << "/.kasmpasswd.d/" << username << ".yaml";
  return oss.str();
}

UserConfig::UserConfig()
{
  // Initialize all integers to 0
  DLP_ClipSendMax = 0;
  DLP_ClipAcceptMax = 0;
  DLP_ClipDelay = 0;
  DLP_KeyRateLimit = 0;

  // Initialize all bools to false
  DLP_RegionAllowClick = false;
  DLP_RegionAllowRelease = false;
  KeyboardAllowOnlyKeysInRemap = false;
  PointerAllowLeftClick = false;
  PointerAllowMiddleClick = false;
  PointerAllowRightClick = false;
  PointerAllowScroll = false;
  PointerAllowOtherButtons = false;

  acceptPointerEvents = false;
  acceptKeyEvents = false;
  // Strings are initialized to empty by default
}

void UserConfig::loadFromFileOrGlobalDefaults(const char* username, const char* kasmpasswdDir)
{
  loadGlobalDefaults();

  if (loadFromFile(username, kasmpasswdDir)) {
    vlog.info("Loaded per-user config for %s", username);
  }
}

bool UserConfig::loadFromFile(const char* username, const char* kasmpasswdDir)
{
  if (!username || !username[0] || !kasmpasswdDir || !kasmpasswdDir[0]) {
    vlog.debug("No username or kasmpasswd directory provided");
    return false;
  }

  // Try user-specific config first, then default
  std::string configPath = getConfigPath(username, kasmpasswdDir);
  struct stat st;
  if (stat(configPath.c_str(), &st) != 0) {
	vlog.debug("No per-user config found for %s", username);
	return false;
  }

  try {
    YAML::Node config = YAML::LoadFile(configPath);
    vlog.info("Loading user config from: %s", configPath.c_str());

    // Parse DLP section
    if (config["data_loss_prevention"]) {
      YAML::Node dlp = config["data_loss_prevention"];

      // Visual region settings
      if (dlp["visible_region"]) {
        YAML::Node region = dlp["visible_region"];
        // Build DLP_Region from top, left, right, bottom properties
        if (region["left"] && region["top"] && region["right"] && region["bottom"]) {
          std::string left = region["left"].as<std::string>();
          std::string top = region["top"].as<std::string>();
          std::string right = region["right"].as<std::string>();
          std::string bottom = region["bottom"].as<std::string>();
          DLP_Region = left + "," + top + "," + right + "," + bottom;
        }
        if(region["concealed_region"]){
          YAML::Node concealedRegion = region["concealed_region"];
          if (concealedRegion["allow_click_down"])
            DLP_RegionAllowClick = concealedRegion["allow_click_down"].as<bool>();
          if (concealedRegion["allow_click_release"])
            DLP_RegionAllowRelease = concealedRegion["allow_click_release"].as<bool>();
        }
      }

      if(dlp["keyboard"]){
        YAML::Node keyboard = dlp["keyboard"];
        if (keyboard["enabled"])
          acceptKeyEvents = keyboard["enabled"].as<bool>();
        if (keyboard["rate_limit"])
		  DLP_KeyRateLimit = keyboard["rate_limit"].as<int>(0);
      }

      // Clipboard settings
      if (dlp["clipboard"]) {
        YAML::Node clipboard = dlp["clipboard"];
        if (clipboard["server_to_client"]) {
		  if(clipboard["server_to_client"]["size"])
			DLP_ClipSendMax = clipboard["server_to_client"]["size"].as<int>(0);
		}
		if (clipboard["client_to_server"]) {
		  if(clipboard["client_to_server"]["size"])
			DLP_ClipAcceptMax = clipboard["client_to_server"]["size"].as<int>(0);
		}
        if (clipboard["delay_between_operations"])
          DLP_ClipDelay = clipboard["delay_between_operations"].as<int>(0);
      }
    }

    // Parse keyboard section
    if (config["keyboard"]) {
      YAML::Node keyboard = config["keyboard"];
      if (keyboard["remap_keys"]) {
        YAML::Node remapNode = keyboard["remap_keys"];
        if (remapNode.IsSequence()) {
          // Handle as YAML array
          RemapKeys.clear();
          for (size_t i = 0; i < remapNode.size(); i++) {
            if (i > 0) RemapKeys += ",";
            RemapKeys += remapNode[i].as<std::string>();
          }
        } else {
          // Handle as scalar string
          RemapKeys = remapNode.as<std::string>();
        }
      }
      if (keyboard["allow_only_keys_in_remap"])
        KeyboardAllowOnlyKeysInRemap = keyboard["allow_only_keys_in_remap"].as<bool>();
    }

    // Parse pointer section
    if (config["pointer"]) {
      YAML::Node pointer = config["pointer"];
	  if (pointer["enabled"])
        acceptPointerEvents = pointer["enabled"].as<bool>();
      if (pointer["allow_left_click"])
        PointerAllowLeftClick = pointer["allow_left_click"].as<bool>();
      if (pointer["allow_middle_click"])
        PointerAllowMiddleClick = pointer["allow_middle_click"].as<bool>();
      if (pointer["allow_right_click"])
        PointerAllowRightClick = pointer["allow_right_click"].as<bool>();
      if (pointer["allow_scroll"])
        PointerAllowScroll = pointer["allow_scroll"].as<bool>();
      if (pointer["allow_other_buttons"])
        PointerAllowOtherButtons = pointer["allow_other_buttons"].as<bool>();
    }

    vlog.info("Successfully loaded user config");
    return true;
  } catch (const YAML::Exception& e) {
    vlog.error("Failed to parse config %s: %s", configPath.c_str(), e.what());
    return false;
  }
}

void UserConfig::loadGlobalDefaults()
{
  vlog.info("Loading global default settings");

  // Load from Server:: global parameters
  DLP_Region = (const char*) Server::DLP_Region;
  DLP_RegionAllowClick = Server::DLP_RegionAllowClick;
  DLP_RegionAllowRelease = Server::DLP_RegionAllowRelease;
  DLP_ClipSendMax = Server::DLP_ClipSendMax;
  DLP_ClipAcceptMax = Server::DLP_ClipAcceptMax;
  DLP_ClipDelay = Server::DLP_ClipDelay;
  DLP_KeyRateLimit = Server::DLP_KeyRateLimit;
  KeyboardAllowOnlyKeysInRemap = Server::KeyboardAllowOnlyKeysInRemap;
  PointerAllowLeftClick = Server::PointerAllowLeftClick;
  PointerAllowMiddleClick = Server::PointerAllowMiddleClick;
  PointerAllowRightClick = Server::PointerAllowRightClick;
  PointerAllowScroll = Server::PointerAllowScroll;
  PointerAllowOtherButtons = Server::PointerAllowOtherButtons;

  // KeyboardRemapKeys is not set from globals - handled separately via KeyRemapper
  RemapKeys = "";

  acceptPointerEvents = Server::acceptPointerEvents;
  acceptKeyEvents = Server::acceptKeyEvents;
}
