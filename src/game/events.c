// En events.c
#include "events.h"
#include "../utils/definitions.h"

int[MaxEvents] event_happened;

void initialize_events() {
    // Inicializar todos los eventos como no ocurridos
    for(int i = 0; i < MaxEvents; i++) {
        event_happened[i] = 0;
    }
}

int has_event_happened(int event_id) {
    return event_happened[event_id];
}

void mark_event_as_happened(int event_id) {
    event_happened[event_id] = 1;
}