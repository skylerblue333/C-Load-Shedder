#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_CAPACITY 1000UL
#define DEFAULT_THRESHOLD_PERCENT 85UL

typedef struct {
    unsigned long capacity;
    unsigned long threshold_percent;
    unsigned long accepted;
    unsigned long shed;
} load_shedder_t;

typedef enum {
    LOAD_ACCEPT = 0,
    LOAD_SHED = 1,
    LOAD_INVALID = 2
} load_decision_t;

static int parse_ulong(const char *value, unsigned long *out) {
    char *end = NULL;
    unsigned long parsed;

    if (value == NULL || *value == '\0' || out == NULL) {
        return -1;
    }

    errno = 0;
    parsed = strtoul(value, &end, 10);
    if (errno != 0 || end == value || *end != '\0') {
        return -1;
    }

    *out = parsed;
    return 0;
}

static int init_shedder(load_shedder_t *shedder, unsigned long capacity,
                        unsigned long threshold_percent) {
    if (shedder == NULL || capacity == 0 || threshold_percent == 0 || threshold_percent > 100) {
        return -1;
    }

    shedder->capacity = capacity;
    shedder->threshold_percent = threshold_percent;
    shedder->accepted = 0;
    shedder->shed = 0;
    return 0;
}

static load_decision_t decide(load_shedder_t *shedder, unsigned long in_flight) {
    unsigned long long lhs;
    unsigned long long rhs;

    if (shedder == NULL || in_flight > shedder->capacity) {
        return LOAD_INVALID;
    }

    lhs = (unsigned long long)in_flight * 100ULL;
    rhs = (unsigned long long)shedder->capacity * shedder->threshold_percent;

    if (lhs >= rhs) {
        shedder->shed++;
        return LOAD_SHED;
    }

    shedder->accepted++;
    return LOAD_ACCEPT;
}

static const char *decision_name(load_decision_t decision) {
    switch (decision) {
        case LOAD_ACCEPT:
            return "accept";
        case LOAD_SHED:
            return "shed";
        default:
            return "invalid";
    }
}

int main(int argc, char **argv) {
    load_shedder_t shedder;
    unsigned long capacity = DEFAULT_CAPACITY;
    unsigned long threshold = DEFAULT_THRESHOLD_PERCENT;
    unsigned long in_flight;
    load_decision_t decision;

    if (argc < 2 || argc > 4) {
        fprintf(stderr, "usage: %s <in_flight> [capacity] [threshold_percent]\n", argv[0]);
        return 2;
    }

    if (parse_ulong(argv[1], &in_flight) != 0 ||
        (argc >= 3 && parse_ulong(argv[2], &capacity) != 0) ||
        (argc == 4 && parse_ulong(argv[3], &threshold) != 0) ||
        init_shedder(&shedder, capacity, threshold) != 0) {
        fprintf(stderr, "invalid numeric configuration\n");
        return 2;
    }

    decision = decide(&shedder, in_flight);
    if (decision == LOAD_INVALID) {
        fprintf(stderr, "in_flight cannot exceed capacity\n");
        return 2;
    }

    printf("{\"decision\":\"%s\",\"in_flight\":%lu,\"capacity\":%lu,\"threshold_percent\":%lu}\n",
           decision_name(decision), in_flight, shedder.capacity, shedder.threshold_percent);
    return decision == LOAD_SHED ? 3 : 0;
}
