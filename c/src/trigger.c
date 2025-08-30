// trigger.c
#include "trigger.h"
#include <stdio.h>
#include <time.h>

const char* TRIGGER_FILE = "access_trigger.lock";

int is_intrusion_detected() {
    FILE *f = fopen(TRIGGER_FILE, "rb");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}

void set_intrusion_trigger() {
    FILE *f = fopen(TRIGGER_FILE, "wb");
    if (f) {
        time_t now = time(NULL);
        fprintf(f, "INTRUSION: %s", ctime(&now));
        fclose(f);
    }
}

void clear_intrusion_trigger() {
    remove(TRIGGER_FILE);
}