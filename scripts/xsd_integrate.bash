#!/bin/bash

function errecho() {
	echo $@ >&2
}

function genBody() {
	for xsd_dir in ../examples/xml/*xsd; do
		if [ -d "${xsd_dir}" ]; then
			echo "// dir: ${xsd_dir}"
			errecho "dir: ${xsd_dir}"
			for fileName in "${xsd_dir}"/*; do
				echo "// file: ${fileName}"
				errecho "file: ${fileName}"
				file=$(cat "${fileName}" | sed -e 's/$/\\/g; s/"/\\"/g')
				var=$(echo "${fileName}" | sed -E 's/.*\///g; s/(\.|-)/_/g;')
				pre_var=$(echo ${xsd_dir} | sed -E 's/.*\///g; s/-.*//g')
				echo -e "static const char const globalData_${pre_var}_${var}[] = \"\\\\\n${file}\n\";\n";
			done
		fi
	done
}

function genFile() {
	# Header
	echo '#ifndef XSD_DATA_H_INCLUDED'
	echo '#define XSD_DATA_H_INCLUDED'
	echo "#define GENERATION_DATE \"$(date +'%Y-%m-%d %H:%M:%S')\""
	echo

	# BODY
	genBody

	echo
	# Footer
	echo '#endif'
}

function main() {
	genFile > ../src/engine/ipc/xsd_data.h
}

main
