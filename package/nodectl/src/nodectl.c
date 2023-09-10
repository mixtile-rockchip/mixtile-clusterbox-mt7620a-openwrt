#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "log.h"

int EN_PIN[] = {499, 498, 496, 497};
int POWER_ON_PIN[] = {508, 509, 511, 510};
int RESET_PIN[] = {504, 505, 507, 506};
int PRZ_PIN[] = {503, 502, 500, 501};
int TTY_USB[] = {1, 2, 3, 0};
int DEV_BUS_INDEX[] = {6, 5, 3, 4};

int *arrays[] = {EN_PIN, POWER_ON_PIN, RESET_PIN, PRZ_PIN};
int array_sizes[] = {4, 4, 4, 4};
const char *directions[] = {"out", "out", "out", "in"};
int subcommand = -1;

enum Subcommands {
    SUB_LIST,
    SUB_POWERON,
    SUB_POWEROFF,
    SUB_REBOOT,
    SUB_FLASH,
    SUB_CONSOLE,
    SUB_RESCAN,
};

void usage() {
    const char *usage_text =
        "Usage: nodectl commands:\n"
        "   nodectl list\n"
        "   nodectl poweron (--all|-n N)\n"
        "   nodectl reboot (--all|-n N)\n"
        "   nodectl flash (--all|-n N) -f /path/firmware.img\n"
        "   nodectl console -n N\n"
        "   nodectl rescan\n"
        "\n";

    fputs(usage_text, stdout);
}

int init_pin() {
    int pin, i;
    char filepath[50];
    FILE *fp;

    for (int arr_idx = 0; arr_idx < 4; arr_idx++) {
        int *array = arrays[arr_idx];
        const char *direction = directions[arr_idx];

        for (i = 0; i < array_sizes[arr_idx]; i++) {
            pin = array[i];
            snprintf(filepath, 50, "/sys/class/gpio/gpio%d", pin);
            if ((fp = fopen(filepath, "r")) == NULL) {
                fp = fopen("/sys/class/gpio/export", "w");
                fprintf(fp, "%d\n", pin);
                fclose(fp);

                snprintf(filepath, 50, "/sys/class/gpio/gpio%d/direction", pin);
                fp = fopen(filepath, "w");
                fprintf(fp, "%s\n", direction);
                fclose(fp);
            }
        }
    }
    return 0;
}

int set_pin(int pin) {
    FILE *fp;
    char filepath[50];
    snprintf(filepath, 50, "/sys/class/gpio/gpio%d/value", pin);
    if ((fp = fopen(filepath, "w")) != NULL) {
        fprintf(fp, "1\n");
        fclose(fp);
    } else {
        Log_d("set value gpio%d failed.", pin);
        return 1;
    }
    return 0;
}

int reset_pin(int pin) {
    FILE *fp;
    char filepath[50];
    snprintf(filepath, 50, "/sys/class/gpio/gpio%d/value", pin);
    if ((fp = fopen(filepath, "w")) != NULL) {
        fprintf(fp, "0\n");
        fclose(fp);
    } else {
        Log_d("reset value gpio%d failed.", pin);
        return 1;
    }
    return 0;
}

void set_en(int arg1) {
    if (arg1 >= 1 && arg1 <= 4) {
        set_pin(POWER_ON_PIN[arg1 - 1]);
        set_pin(RESET_PIN[arg1 - 1]);
        set_pin(EN_PIN[arg1 - 1]);
    } else {
        Log_d("Please make sure NO.x is correct 1-4.\n");
    }
}

void reset_en(int arg1) {
    if (arg1 >= 1 && arg1 <= 4) {
        reset_pin(EN_PIN[arg1 - 1]);
    } else {
        Log_d("Please make sure NO.x is correct 1-4.\n");
    }
}

int node_list() {
    char hint[120];
    char command[50];
    sprintf(hint, "echo \"If no device is found, run the rescan command and then the list command to view the device.\"");
    sprintf(command, "lspci | grep Rockchip");
    system(hint);
    system(command);
    return 0;
}

int node_rescan() {
    char command[50];
    sprintf(command, "echo 1 > /sys/bus/pci/rescan");
    system(command);
    return 0;
}

void node_power_all_on() {
    for (int index = 1; index <= 4; index++) {
        set_en(index);
    }
}

void node_power_all_off() {
    for (int index = 1; index <= 4; index++) {
        reset_en(index);
    }
}

int node_poweron(int argc, char *argv[]) {
    Log_d("node_poweron.");
    if (argc < 2) {
        return 1;
    }

    if (strcmp(argv[1], "--all") == 0) {
        node_power_all_on();
    } else if (argc >= 3 && strcmp(argv[1], "-n") == 0) {
        if (atoi(argv[2]) >= 1 && atoi(argv[2]) <= 4) {
            set_en(atoi(argv[2]));
        } else {
            printf("The num must be 1 - 4.\n");
        }
    } else {
        return 1;
    }
    return 0;
}

int node_poweroff(int argc, char *argv[]) {
    Log_d("node_poweroff.");
    if (argc < 2) {
        return 1;
    }

    if (strcmp(argv[1], "--all") == 0) {
        node_power_all_off();
    } else if (argc >= 3 && strcmp(argv[1], "-n") == 0) {
        if (atoi(argv[2]) >= 1 && atoi(argv[2]) <= 4) {
            reset_en(atoi(argv[2]));
        } else {
            printf("The num must be 1 - 4\n");
        }
    } else {
        return 1;
    }
    return 0;
}

int node_reboot(int argc, char *argv[]) {
    Log_d("node_reboot.");
    node_poweroff(argc, argv);
    sleep(1);
    node_poweron(argc, argv);
}

int node_console(int argc, char *argv[]) {
    Log_d("node_console.");
    char command[50];
    if (argc < 3) {
        return 1;
    }

    if (strcmp(argv[1], "-n") == 0) {
        if (atoi(argv[2]) >= 1 && atoi(argv[2]) <= 4) {
            Log_d("picocom -b 1500000 /dev/ttyCH343USB%d.", TTY_USB[atoi(argv[2]) - 1]);
            sprintf(command, "picocom -b 1500000 /dev/ttyCH343USB%d", TTY_USB[atoi(argv[2]) - 1]);
            system(command);
            return 0;
        } else {
            printf("The num must be 1 - 4\n");
            return 1;
        }
    }
    return 0;
}

int node_flash(int argc, char *argv[]) {
    printf("The function is under active development, please stay tuned.\n");
    return 0;
}

int main(int argc, char *argv[]) {

    int status = 0;
    init_pin();
    if (argc < 2) {
        usage();
        return 1;
    }
    if (strcmp(argv[1], "list") == 0) {
        subcommand = SUB_LIST;
    } else if (strcmp(argv[1], "poweron") == 0) {
        subcommand = SUB_POWERON;
    } else if (strcmp(argv[1], "poweroff") == 0) {
        subcommand = SUB_POWEROFF;
    } else if (strcmp(argv[1], "reboot") == 0) {
        subcommand = SUB_REBOOT;
    } else if (strcmp(argv[1], "flash") == 0) {
        subcommand = SUB_FLASH;
    } else if (strcmp(argv[1], "console") == 0) {
        subcommand = SUB_CONSOLE;
    } else if (strcmp(argv[1], "rescan") == 0) {
        subcommand = SUB_RESCAN;
    } else {
        usage();
        return 1;
    }

    switch (subcommand) {

        case SUB_LIST:
            status = node_list();
            break;

        case SUB_POWERON:
            status = node_poweron(argc - 1, &argv[1]);
            break;

        case SUB_POWEROFF:
            status = node_poweroff(argc - 1, &argv[1]);
            break;

        case SUB_REBOOT:
            status = node_reboot(argc - 1, &argv[1]);
            break;

        case SUB_FLASH:
            status = node_flash(argc - 1, &argv[1]);
            break;

        case SUB_CONSOLE:
            status = node_console(argc - 1, &argv[1]);
            break;

        case SUB_RESCAN:
            status = node_rescan(argc - 1, &argv[1]);
            break;

        default:
            usage();
            break;
    }
    if (status) {
        usage();
    }

    return 0;
}
