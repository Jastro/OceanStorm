#ifndef EVENTS_H
#define EVENTS_H

extern int[MaxEvents] event_happened;

void initialize_events();
int has_event_happened(int event_id);
void mark_event_as_happened(int event_id);

#endif