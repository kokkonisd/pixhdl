#include "auto_update.h"


int update (const char * current_version)
{
    // File pointer to read output of popen() (to get latest version)
    FILE *fp;
    // Character array to store the latest version number
    char latest_version[VERSION_WIDTH];
    // Return value of the system() call
    int ret = 0;

    log_info("Checking for updates...");

    // Try to get the latest version via curl
    fp = popen(LATEST_VERSION, "r");
    // Check if popen() call was successful
    check(fp, "Couldn't get latest version. Check your internet connection.");
    // Get the latest version number
    fgets(latest_version, VERSION_WIDTH, fp);
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
        log_info("Downloading and installing version v%s of pixhdl...", latest_version);

        // Launch the update command
        ret = system(UPDATE_COMMAND);
        // Check that the update command was successful
        check(ret == 0, "Error downloading version v%s.", latest_version);

        log_info("Done! Version v%s of pixhdl is now installed.", latest_version);
    }

    // Check that pclose() is successful
    check(pclose(fp) != -1, "Couldn't complete update.");

    return 0;

error:
    if (fp) pclose(fp);
    return 1;
}
