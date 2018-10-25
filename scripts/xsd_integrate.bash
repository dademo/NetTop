#!/bin/bash

function errecho() {
	echo $@ >&2
}

function genBody() {
	for xsd_dir in examples/xml/*xsd; do
		if [ -d "${xsd_dir}" ]; then
			echo "// dir: ${xsd_dir}"
			errecho "dir: ${xsd_dir}"
			for fileName in "${xsd_dir}"/*; do
				echo "// file: ${fileName}"
				errecho "file: ${fileName}"
				file=$(cat "${fileName}" | sed -e 's/$/\\/g; s/"/\\"/g')
				var=$(echo "${fileName}" | sed -E 's/.*\///g; s/(\.|-)/_/g;')
				echo -e "${var} = \"\\\\\n${file}\";\n";
			done
		fi
	done
}

function genFile() {
	# Header
	echo '#ifndef XSD_DATA_H'
	echo '#define XSD_DATA_H'
	echo

	# BODY
	genBody

	echo
	# Footer
	echo '#endif'
}

function main() {
	genFile
}

main
