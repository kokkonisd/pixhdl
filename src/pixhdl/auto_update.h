/**
 * @file auto_update.h
 * @author Dimitri Kokkonis ([\@kokkonisd](https://github.com/kokkonisd))
 * @date 4 Aug 2019
 * @brief Auto-update functionality for the _pixhdl_ project.
 */


#ifndef __AUTO_UPDATE_H__
#define __AUTO_UPDATE_H__

#include "dbg.h"
#include <stdio.h>
#include <stdlib.h>

/** Width (in chars) of the version number, which is in the format of _X.Y.Z_ (for example, 'version 1.2.3'). */
#define VERSION_WIDTH 6

/** Command to get the latest version of _pixhdl_ from the _releases_ page of its official GitHub repo. */
#define LATEST_VERSION "curl -s https://github.com/kokkonisd/pixhdl/releases/latest | grep -Eo \"/tag/(.+)>\" | head -c 11 | tail -c 5"

/**
 * @brief Command to update _pixhdl_ to its latest version.
 *
 * This command gets the latest version of _pixhdl_, downloads it in `tmp/`, then runs:
 * @code
 * $ make clean
 * $ sudo make uninstall
 * $ make
 * $ sudo make install
 * @endcode
 * It finally removes the downloaded files, and the installation is complete.
 */
#define UPDATE_COMMAND "TMPDWN=$(curl -s https://api.github.com/repos/kokkonisd/pixhdl/releases/latest | grep \"tag_name\" | awk '{print \"https://github.com/kokkonisd/pixhdl/archive/\" substr($2, 2, length($2)-3) \"/pixhdl-\" substr($2, 3, length($2)-4) \".tar.gz\"}') && curl -L -s $TMPDWN -o /tmp/pixhdl_latest.tar.gz && tar xvzf /tmp/pixhdl_latest.tar.gz -C /tmp/ && cd /tmp/pixhdl-* && make clean && sudo make uninstall && make && sudo make install && cd .. && rm -rf pixhdl-* pixhdl_latest*"


/**
 * @brief Check for an update and install newer version.
 *
 * This function compares the current version with the latest version currently available
 * on the official GitHub repo of _pixhdl_. If a newer version is available, it asks the
 * user for confirmation, then downloads and installs the newer version.
 *
 * @return (`int`): 1 if successful, 0 otherwise.
 */
int update ();


#endif
