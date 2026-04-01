# State Machine Design

States:
- IDLE
- LEFT
- RIGHT
- HAZARD

Events:
- EVT_LEFT
- EVT_RIGHT
- EVT_BOTH

Each state transition is deterministic and handled in IndicatorTask.
