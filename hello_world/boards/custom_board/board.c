#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <stdio.h>

static int board_custom_board_init(void)
{
    printf("Board Initialized");
    return 0;
}

SYS_INIT(board_custom_board_init, POST_KERNEL, 0);