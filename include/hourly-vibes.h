#pragma once
#include <pebble.h>

void hourly_vibes_init(void);
void hourly_vibes_deinit(void);
void hourly_vibes_set_enabled(bool enabled);
void hourly_vibes_set_pattern(VibePattern pattern);
#ifdef PBL_HEALTH
void hourly_vibes_enable_health(bool enable);
#endif
