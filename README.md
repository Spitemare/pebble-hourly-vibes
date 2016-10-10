# pebble-hourly-vibes

pebble-hourly-vibes does the hard work of vibing every hour, on the hour.

# Installation

```
pebble package pebble-hourly-vibes
```

You must be using a new-style project; install the latest pebble tool and SDK and run `pebble convert-project` on your app if you aren't.

# Usage

```c
// This is not a complete example, but should demonstrate the basic usage

#include <pebble-hourly-vibes/hourly-vibes.h>

...

static void init(void) {
  hourly_vibes_init();
#ifdef PBL_HEALTH
  hourly_vibes_enable_health(true);
#endif
  ...
}

static void deinit(void) {
  hourly_vibes_deinit();
  ...
}
```

After calling `hourly_vibes_init()` you may set additional options such as a custom vibe pattern or using Pebble Health.

# pebble-hourly-vibes API

| Method | Description |
|--------|---------|
| `void hourly_vibes_init(void)` | Initialize everything by subscribing to the TickTimerService |
| `void hourly_vibes_deinit(void)` | Deinitialize everything: unsubscribe from services, free memory, etc. |
| `void hourly_vibes_set_enabled(bool enabled)` | Enable or disable the hourly vibes |
| `void hourly_vibes_set_pattern(VibePattern pattern)` | Set a custom VibePattern. By default `vibes_short_pulse()` is used |
| `void hourly_vibes_enable_health(bool enable)` | Register/unregister with Pebble Health |

# Note on Pebble Health

On platforms that support Pebble Health, pebble-hourly-vibes can subscribe to health events and detect when the user is asleep. Vibes will happen only if the user is awake. This avoids awkward vibes at 0200 that could awaken a user. To enable this, just call `hourly_vibes_enable_health(true)` sometime after calling `hourly_vibes_init()`.

Pebble Health integration is turned off by default so apps and watchfaces that don't want to integrate with Pebble Health won't get the nag screen thrown up by the watch if the user doesn't have Pebble Health enabled.
