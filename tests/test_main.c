#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

static int exit_code(const char *command) {
    int status = system(command);
    if (status == -1 || !WIFEXITED(status)) {
        return -1;
    }
    return WEXITSTATUS(status);
}

int main(void) {
    assert(exit_code("./app 0 >/tmp/sky-load-accept.json") == 0);
    assert(exit_code("./app 849 1000 85 >/tmp/sky-load-below.json") == 0);
    assert(exit_code("./app 850 1000 85 >/tmp/sky-load-shed.json") == 3);
    assert(exit_code("./app 1001 1000 85 >/dev/null 2>&1") == 2);
    assert(exit_code("./app nope >/dev/null 2>&1") == 2);
    assert(exit_code("./app 10 0 85 >/dev/null 2>&1") == 2);
    assert(exit_code("./app 10 1000 101 >/dev/null 2>&1") == 2);
    puts("load shedder contract tests passed");
    return 0;
}
