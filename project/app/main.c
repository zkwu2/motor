
#include "robot.h"

int32_t app_main(robot_state_t *self, int32_t event)
{
    switch(event)
    {
        case EVE_STAT_ENTER:
            robot_state_show_enter();  
            break;
        case EVE_STAT_EXIT:
            break;
        case EVE_STAT_BACK:
            // robot_state_back(NULL);
            break;
        default:
            return ROBOT_STATE_REV_SKIP;
    }
    return ROBOT_STATE_REV_DEEL;
}

// void *print_hello(void *arg) 
// {
//     logi("Hello from thread!\n");
//     return NULL;
// }

void robot_main(void)
{
    // hw_system_init();
    robot_init();
    // pthread_t thread_id;
    // pthread_create(&thread_id, NULL, print_hello, NULL);
    robot_run(app_main);
    while(1)
    {
		os_thread_sleep(40);
    }
}

int32_t main(void)
{
    robot_main(); //会一直停在这个函数里面

    return 0;
}