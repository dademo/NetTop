#!/bin/bash

for xsdFile in ../examples/xml/{master,module}-xsd/*; do
	xmlFile=$(echo "${xsdFile}" | sed -e 's/\.xsd$/\.xml/; s/-xsd/-xml/');

	xmllint --schema "${xsdFile}" "${xmlFile}" --noout;
done

xmllint --schema ../examples/xml/conf.xsd ../conf.xml --noout
