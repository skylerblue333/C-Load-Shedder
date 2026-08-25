# Sky Load Shedder

**Status: engineering beta.** A small deterministic C admission-control primitive for deciding whether work should be accepted or shed as in-flight load approaches configured capacity.

## Behavior

```bash
./app <in_flight> [capacity] [threshold_percent]
```

Defaults: capacity `1000`, threshold `85%`.

- Below threshold: JSON decision `accept`, exit code `0`.
- At or above threshold: JSON decision `shed`, exit code `3`.
- Invalid configuration or load above declared capacity: exit code `2`.

Example:

```bash
./app 850 1000 85
# {"decision":"shed","in_flight":850,"capacity":1000,"threshold_percent":85}
```

## Verification

CI enforces C11, `-Wall -Wextra -Wpedantic -Werror`, contract tests, AddressSanitizer/UBSan execution, and an independent CMake build. The container packages a static binary into a `scratch` image and runs as UID/GID 65532.

## Scope limitations

This is a local decision primitive. It does **not** measure live CPU/memory, proxy requests, coordinate distributed nodes, implement queues/backpressure, provide adaptive thresholds, authenticate callers, expose a network service, or claim production capacity/SLA figures.

## SKYCOIN4444 integration

Use the binary or decision logic behind a verified gateway/work-queue integration where a simple deterministic saturation threshold is useful. Real load signals and operational policy remain deployment responsibilities.
