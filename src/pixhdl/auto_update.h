#ifndef __AUTO_UPDATE_H__
#define __AUTO_UPDATE_H__

#include "dbg.h"
#include <stdio.h>
#include <stdlib.h>


#define VERSION_WIDTH 6
#define LATEST_VERSION "curl -s https://github.com/kokkonisd/pixhdl/releases/latest | grep -Eo \"/tag/(.+)>\" | head -c 11 | tail -c 5"

#define UPDATE_COMMAND "TMPDWN=$(curl -s https://api.github.com/repos/kokkonisd/pixhdl/releases/latest | grep \"tag_name\" | awk '{print \"https://github.com/kokkonisd/pixhdl/archive/\" substr($2, 2, length($2)-3) \"/pixhdl-\" substr($2, 3, length($2)-4) \".tar.gz\"}') && curl -L -s $TMPDWN -o /tmp/pixhdl_latest.tar.gz && tar xvzf /tmp/pixhdl_latest.tar.gz -C /tmp/ && cd /tmp/pixhdl-* && make clean && make uninstall && make && make install && cd .. && rm -rf pixhdl-* pixhdl_latest*"


int update ();


#endif
