#pragma once

#define C1_GWORLD_PATTERN "48 8B 1D ? ? ? ? 48 85 DB 74 3B 41"
#define C1_GOBJECT_PATTERN "49 63 C8 48 8D 14 40 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1"
#define C1_FMEMORYFREE_PATTERN "48 85 C9 74 2E 53"
#define C1_GETOBJNAME_PATTERN "40 53 48 83 EC 20 48 8B D9 48 85 D2 75 30 33 C0 48 89 01 48 C7 41 08 ? ? ? ? E8 ? ? ? ? 48 8B 0B 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B C3 48 83 C4 20 5B C3 48"
#define C1_GETFIRSTPLAYERCONTROLLER_PATTERN "83 B9 ? ? ? ? ? 7E ? 48 8B 89 ? ? ? ? E9"
#define C1_PROCESSEVENT_PATTERN "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A"
#define C1_STATICCONSTRUCTOBJECTINTERNAL_PATTERN "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ?  48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 8B A5 ? ? ? ? 48 8D 05"
#define C1_SPRINT_PATCH_PATTERN "88 91 ? ? ? ? 48 85 DB 0F 84"