#include "auto_update.h"


int update (const char * current_version)
{
    // File pointer to read output of popen() (to get latest version)
    FILE *fp;
    // Character array to store the latest version number
    char latest_version[VERSION_WIDTH];
    // Return value of the system() call
    int ret = 0;
    // Choice (y/n) of the user if an update is found
    char choice = 0;
    // Char to flush input (getchar())
    char flush_input = 0;

    log_info("Checking for updates...");

    // Try to get the latest version via curl
    fp = popen(LATEST_VERSION, "r");
    // Check if popen() call was successful
    check(fp, "Couldn't get latest version. Check your internet connection.");
    // Get the latest version number
    fgets(latest_version, VERSION_WIDTH, fp);
    // Check that pclose() is successful
    check(pclose(fp) != -1, "Couldn't complete update.");
    // Check that the version number is valid
    check(strlen(latest_version), "Couldn't get latest version. Check your internet connection.");

    if (strcmp(latest_version, current_version) == 0) {
        // Aliases is up to date
        log_info("pixhdl is up to date (v%s).", current_version);
    } else if (strcmp(latest_version, current_version) < 0) {
        // Local version is ahead of latest stable release
        log_info("Your version of pixhdl (v%s) is ahead of the latest stable release (v%s).", current_version, latest_version);
    } else {
        // Newer stable version found and should be installed
        log_info("Newer stable version of pixhdl found: v%s", latest_version);
        // Ask the user if they really want to install the newer version
        log_info("Do you want to install v%s of pixhdl? (Current version: v%s)", latest_version, current_version);

        do {
            // Prompt
            printf("[yY/nN] > ");
            // Get character from user
            choice = getchar();

            // If the choice is yes
            if (choice == 'y' || choice == 'Y') {
                // Download the newer version
                log_info("Downloading and installing version v%s of pixhdl...", latest_version);
                // Launch the update command
                ret = system(UPDATE_COMMAND);
                // Check that the update command was successful
                check(ret == 0, "Error downloading version v%s.", latest_version);
                // Inform the user that the installation is complete
                log_info("Done! Version v%s of pixhdl is now installed.", latest_version);
            // If the choice is no
            } else if (choice == 'n' || choice == 'N') {
                // Abort the update
                log_info("Aborting update.");
            // If the choice is something else
            } else {
                // Inform the user that they have to enter the correct input
                log_info("Please enter y/Y (yes) to continue with the update, or n/N (no) to abort it.");
            }

            // Flush input to get rid of eventual characters that are still on the buffer
            while ((flush_input = getchar()) != '\n' && flush_input != EOF) { }
        // While the choice is invalid, loop
        } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');
    }

    return 0;

error:
    if (fp) pclose(fp);
    return 1;
}
