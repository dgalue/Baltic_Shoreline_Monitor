# Integration Test

A minimal integration test initializes Meshtastic and the custom FreeRTOS tasks.
The test runs for about ten seconds and prints queue levels while feeding the
watchdog. Any assertion failure or missing final message indicates a problem.

## Expected Serial Output
```
[TST] tick 0 q_audio=1 q_events=0 q_log=0
[TST] tick 1 q_audio=2 q_events=1 q_log=0
...
[TST] tick 9 q_audio=10 q_events=2 q_log=0
[TST] integration test complete
```

Build and flash with PlatformIO:
```bash
pio test -e seeed-xiao-s3 -f integration_test -t upload
```

