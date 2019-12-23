#pragma once

// aligned (* bpp) to 64
#define OVERLAY_WIDTH_BASE    320

// aligned to 128
#define OVERLAY_HEIGHT_BASE   384

/* - 1:  smaller overlay, saves RAM
 * - 2:  bigger overlay, more readable on handheld
 */
#define OVERLAY_SCALE     1

#define OVERLAY_WIDTH     OVERLAY_SCALE * OVERLAY_WIDTH_BASE
#define OVERLAY_HEIGHT    OVERLAY_SCALE * OVERLAY_HEIGHT_BASE
