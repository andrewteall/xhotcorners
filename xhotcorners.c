#include <X11/Xlib.h>
#include <assert.h>  //assert()
#include <malloc.h>  //malloc
#include <signal.h>  //signal() & SIGINT
#include <stdlib.h>  //getenv()
#include <string.h>
#include <unistd.h>  //usleep()

static volatile int keepRunning = 1;

void intHandler(int sig) {
    keepRunning = 0;
}

static int _XlibErrorHandler(Display *display, XErrorEvent *event) {
    fprintf(stderr, "An error occured detecting the mouse position\n");
    return True;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, intHandler);
    
    if (argc == 2 && strcmp(argv[1], "--version") == 0) {
        printf("xhotcorners Beta\n");    
    } else {
        
        int number_of_screens;
        Bool result;
        Window *root_windows;
        Window window_returned;
        int root_x, root_y;
        int win_x, win_y;
        unsigned int mask_return;

        Display *display = XOpenDisplay(NULL);
        assert(display);

        XSetErrorHandler(_XlibErrorHandler);
        number_of_screens = XScreenCount(display);
        root_windows = (Window *)malloc(sizeof(Window) * number_of_screens);
        int i;
        for (i = 0; i < number_of_screens; i++) {
            root_windows[i] = XRootWindow(display, i);
        }
        FILE *configFile;

        //Default commands to blank
        char *tlCommand = "", *trCommand = "", *blCommand = "", *brCommand = "";

        //Disable all Hotcorners by default
        int topLeftHotcornerEnabled = 0, topRightHotcornerEnabled = 0,
            bottomLeftHotcornerEnabled = 0, bottomRightHotcornerEnabled = 0;

        //Get Display Size
        int scr = 0;
        int width = DisplayWidth(display, scr);
        int height = DisplayHeight(display, scr);

        //Build rcFilePath string
        char *basepath;
        char *configFilePath;

        if (argc == 3 && strcmp(argv[1], "-C") == 0) {
            printf("Using Custom Config File Path\n");
            basepath = getenv("PWD");
            configFilePath = (char *)malloc(strlen(basepath) + strlen(argv[2]) + 1);
            configFilePath[0] = '\0';
            strcat(configFilePath, basepath);
            strcat(configFilePath, "/");
            strcat(configFilePath, argv[2]);
        } else {
            basepath = getenv("HOME");
            configFilePath = (char *)malloc(strlen(basepath) + 36);
            configFilePath[0] = '\0';
            strcat(configFilePath, basepath);
            strcat(configFilePath, "/.config/xhotcorners/xhotcorners.rc");
        }


        //Read config File and Store values in vairables
        configFile = fopen(configFilePath, "r");
        printf("Sucessfully Opened Config File\n");

        if (configFile != NULL) {
            char line[256];
            int result, lastOc, firstOc, commandLen;
            /*****************************************************************
         * Top Left Hot Corner                                            *
         *****************************************************************/
            for (i = 0; fgets(line, sizeof line, configFile) != NULL; i++) {
                if ((result = strncmp(line, "tlenabled", 9)) == 0) {
                    if (line[10] == '1') {
                        topLeftHotcornerEnabled = 1;
                        break;
                    }
                    break;
                }
            }
            for (i = 0; fgets(line, sizeof line, configFile) != NULL; i++) {
                if ((result = strncmp(line, "tlcommand", 9)) == 0) {
                    lastOc = strrchr(line, '"') - line;
                    firstOc = strchr(line, '"') - line;
                    commandLen = lastOc - firstOc - 1;
                    tlCommand = (char *)malloc(commandLen + 1);
                    strncpy(tlCommand, line + firstOc + 1, commandLen);
                    tlCommand[commandLen] = '\0';
                    break;
                }
            }
            /*****************************************************************
         * Top Right Hot Corner                                           *
         *****************************************************************/
            for (i = 0; fgets(line, sizeof line, configFile) != NULL; i++) {
                if ((result = strncmp(line, "trenabled", 9)) == 0) {
                    if (line[10] == '1') {
                        topRightHotcornerEnabled = 1;
                        break;
                    }
                    break;
                }
            }

            for (i = 0; fgets(line, sizeof line, configFile) != NULL; i++) {
                if ((result = strncmp(line, "trcommand", 9)) == 0) {
                    lastOc = strrchr(line, '"') - line;
                    firstOc = strchr(line, '"') - line;
                    commandLen = lastOc - firstOc - 1;
                    trCommand = (char *)malloc(commandLen + 1);
                    strncpy(trCommand, line + firstOc + 1, commandLen);
                    trCommand[commandLen] = '\0';
                    break;
                }
            }
            /*****************************************************************
         * Bottom Left Hot Corner                                         *
         *****************************************************************/
            for (i = 0; fgets(line, sizeof line, configFile) != NULL; i++) {
                if ((result = strncmp(line, "blenabled", 9)) == 0) {
                    if (line[10] == '1') {
                        bottomLeftHotcornerEnabled = 1;
                        break;
                    }
                    break;
                }
            }

            for (i = 0; fgets(line, sizeof line, configFile) != NULL; i++) {
                if ((result = strncmp(line, "blcommand", 9)) == 0) {
                    lastOc = strrchr(line, '"') - line;
                    firstOc = strchr(line, '"') - line;
                    commandLen = lastOc - firstOc - 1;
                    blCommand = (char *)malloc(commandLen + 1);
                    strncpy(blCommand, line + firstOc + 1, commandLen);
                    blCommand[commandLen] = '\0';
                    break;
                }
            }
            /*****************************************************************
         * Bottom Right Hot Corner                                        *
         *****************************************************************/
            for (i = 0; fgets(line, sizeof line, configFile) != NULL; i++) {
                if ((result = strncmp(line, "brenabled", 9)) == 0) {
                    if (line[10] == '1') {
                        bottomRightHotcornerEnabled = 1;
                        break;
                    }
                    break;
                }
            }
            for (i = 0; fgets(line, sizeof line, configFile) != NULL; i++) {
                if ((result = strncmp(line, "brcommand", 9)) == 0) {
                    lastOc = strrchr(line, '"') - line;
                    firstOc = strchr(line, '"') - line;
                    commandLen = lastOc - firstOc - 1;
                    brCommand = (char *)malloc(commandLen + 1);
                    strncpy(brCommand, line + firstOc + 1, commandLen);
                    brCommand[commandLen] = '\0';
                    break;
                }
            }
        } else {
            perror(configFilePath);
        }
        fclose(configFile);
        free(configFilePath);

        /*****************************************************************
      * Start Program Loop                                             *
      *****************************************************************/
        int commandDelay = 500000;
        int loopDelay = 50000;
        while (keepRunning) {
            for (i = 0; i < number_of_screens; i++) {
                result = XQueryPointer(display, root_windows[i], &window_returned,
                                       &window_returned, &root_x, &root_y, &win_x, &win_y, &mask_return);
                if (result == True)
                    break;
            }

            if (result != True) {
                fprintf(stderr, "No mouse found.\n");
                return -1;
            }

            if (topLeftHotcornerEnabled == 1)
                if (root_x == 0 && root_y == 0) {
                    system(tlCommand);
                    usleep(commandDelay);
                }

            if (topRightHotcornerEnabled == 1)
                if (root_x == width - 1 && root_y == 0) {
                    system(trCommand);
                    usleep(commandDelay);
                }

            if (bottomRightHotcornerEnabled == 1)
                if (root_x == width - 1 && root_y == height - 1) {
                    system(brCommand);
                    usleep(commandDelay);
                }

            if (bottomLeftHotcornerEnabled == 1)
                if (root_x == 0 && root_y == height - 1) {
                    system(blCommand);
                    usleep(commandDelay);
                }
            usleep(loopDelay);
        }
        /*****************************************************************
      * End Program Loop                                               *
      *****************************************************************/
        free(tlCommand);
        free(trCommand);
        free(blCommand);
        free(brCommand);
        free(root_windows);
        (void)XCloseDisplay(display);
    }
    return 0;
}
