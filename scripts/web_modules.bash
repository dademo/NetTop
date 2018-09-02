#!/bin/bash


index=0
mod_list[0]="";


function errecho() {
    echo -e "-- [web_modules] ${@}" 1>&2
}


cat << EOF
#ifndef WEB_CONFIG_H_INCLUDED
#define WEB_CONFIG_H_INCLUDED

#include "../engine/router/router.h"

EOF


errecho "Scanning for web modules..."
for mod_file in src/web/mod_*.c; do
    mod=$(echo "${mod_file}" | sed -e 's/src\/web\///; s/mod_//; s/.c$//')

    errecho "Found module \e[1m${mod}\e[0m"

    mod_list[${index}]="${mod_file}"

    ((index++))
done

cat << EOF

/* Adding the different mod files */
EOF

for mod_file in ${mod_list[@]}; do
    mod_file_header="$(echo "${mod_file}" | sed -e 's/src\/web\///; s/.c$/.h/')";
    echo -e "#include \"${mod_file_header}\""
done

cat << EOF

/* A function that load all the modules */
void load_all_modules(struct router_conf* conf) {
EOF

for mod_file in ${mod_list[@]}; do
    mod="$(echo "${mod_file}" | sed -e 's/src\/web\///; s/.c$//; s/mod_//')";
    echo -e "\tmodule_load_${mod}(conf);"
done

cat << EOF
}
EOF

errecho "Scan done"

cat << EOF
#endif
EOF
