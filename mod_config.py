"""
mod_config.py — Mod configuration for 'gz' (tpgz).

This file is imported by configure.py to determine which custom objects
to compile and link into the DOL, and how to name the output ISO.
"""

# Display name used in ISO filename: <iso_prefix>-<version>.iso
iso_prefix = "tpgz"

# Mod source objects to compile (paths relative to src/).
mod_objects = [
    "gz/gz_utility_notification.cpp",
    "gz/gz.cpp",
    "gz/gz_capture.cpp",
    "gz/gz_textbox.cpp",
    "gz/gz_utility_keyboard.cpp",
    "gz/gz_save_callbacks.cpp",
    "gz/gz_menu.cpp",
    "gz/gz_menu_settings.cpp",
    "gz/gz_utility_confirm.cpp",
    "gz/gz_setup_wizard.cpp",
    "gz/gz_menu_credits.cpp",
    "gz/gz_menu_framework.cpp",
    "gz/gz_menu_tools.cpp",
    "gz/gz_menu_practice.cpp",
    "gz/gz_menu_memory.cpp",
    "gz/gz_menu_cheats.cpp",
    "gz/gz_menu_flags.cpp",
    "gz/gz_menu_heaps.cpp",
    "gz/gz_menu_inventory.cpp",
    "gz/gz_menu_inventory_ring.cpp",
    "gz/gz_menu_inventory_pause.cpp",
    "gz/gz_menu_inventory_pause_submenu.cpp",
    "gz/gz_menu_warp.cpp",
    "gz/gz_menu_main.cpp",
    "gz/gz_menu_scene.cpp",
    "gz/gz_warp_preview.cpp",
    "gz/gz_manager_practice.cpp",
    "gz/gz_manager_cheats.cpp",
    "gz/gz_manager_tools.cpp",
    "gz/gz_manager_scene.cpp",
    "gz/gz_utility_world_text.cpp",
    "gz/gz_utility_draw.cpp",
    "gz/gz_utility_misc.cpp",
]

# Additional dependencies from the decomp that the mod needs but aren't
# already linked.
extra_objects = [
    "dolphin/card/CARDDelete.c",
]

# Objects already compiled as part of the vanilla decomp that need to be
# force-linked (added to DOL link order) but NOT re-compiled.
# These are appended in link_order_callback but not in the lib objects list.
extra_link_objects = [
    "dolphin/gx/GXDraw.c",
    "d/d_debug_pad.cpp",
    "d/d_debug_viewer.cpp",
]

# Umbra platform abstraction layer (always included).
umbra_objects = [
    "umbra/umbra_platform.cpp",
    "umbra/umbra_nintendont.cpp",
    "umbra/umbra_storage.cpp",
    "umbra/umbra_net.cpp",
    "umbra/umbra_gdb.cpp",
    "umbra/umbra_usb.cpp",
]
