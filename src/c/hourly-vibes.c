#include <pebble.h>
#include <pebble-events/pebble-events.h>
#include "hourly-vibes.h"

#ifdef PBL_HEALTH
static bool s_sleeping;
static EventHandle s_health_event_handle;
#endif

static uint32_t *s_segments;
static uint32_t s_num_segments;

static bool s_enabled = true;
static EventHandle s_tick_timer_event_handle;

static void prv_vibe(void) {
    if (quiet_time_is_active()) return;

    if (s_segments != NULL) {
        VibePattern pattern = {
            .durations = s_segments,
            .num_segments = s_num_segments
        };
        vibes_enqueue_custom_pattern(pattern);
    } else {
        vibes_short_pulse();
    }
}

static void prv_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
#ifdef PBL_HEALTH
    if (s_sleeping) return;
#endif

    if (s_enabled && (units_changed & HOUR_UNIT) != 0) prv_vibe();
}

#ifdef PBL_HEALTH
static void prv_health_event_handler(HealthEventType event, void *context) {
    if (event == HealthEventSignificantUpdate) {
        prv_health_event_handler(HealthEventSleepUpdate, context);
    } else if (event == HealthEventSleepUpdate || (event == HealthEventMovementUpdate && s_sleeping)) {
        HealthActivityMask mask = health_service_peek_current_activities();
        s_sleeping = (mask & HealthActivitySleep) || (mask & HealthActivityRestfulSleep);
    }
}
#endif

void hourly_vibes_init(void) {
    s_tick_timer_event_handle = events_tick_timer_service_subscribe(HOUR_UNIT, prv_tick_handler);
}

void hourly_vibes_deinit(void) {
    if (s_tick_timer_event_handle != NULL) {
        events_tick_timer_service_unsubscribe(s_tick_timer_event_handle);
        s_tick_timer_event_handle = NULL;
    }

    if (s_segments != NULL) {
        free(s_segments);
        s_segments = NULL;
        s_num_segments = 0;
    }

#ifdef PBL_HEALTH
    if (s_health_event_handle != NULL) {
        events_health_service_events_unsubscribe(s_health_event_handle);
        s_health_event_handle = NULL;
    }
#endif
}

void hourly_vibes_set_enabled(bool enabled) {
    s_enabled = enabled;
}

void hourly_vibes_set_pattern(VibePattern pattern) {
    if (s_segments != NULL) {
        free(s_segments);
        s_segments = NULL;
        s_num_segments = 0;
    }

    if (pattern.num_segments > 0) {
        s_segments = malloc(pattern.num_segments * sizeof(uint32_t));
        memcpy(s_segments, pattern.durations, pattern.num_segments * sizeof(uint32_t));
        s_num_segments = pattern.num_segments;
    }
}

#ifdef PBL_HEALTH
void hourly_vibes_enable_health(bool enable) {
    if (enable && s_health_event_handle == NULL) {
        s_health_event_handle = events_health_service_events_subscribe(prv_health_event_handler, NULL);
    } else if (!enable) {
        if (s_health_event_handle != NULL) {
            events_health_service_events_unsubscribe(s_health_event_handle);
            s_health_event_handle = NULL;
        }
    }
}
#endif
