#ifndef XSD_DATA_H_INCLUDED
#define XSD_DATA_H_INCLUDED
#define GENERATION_DATE "2018-10-27 14:44:22"

// dir: ../examples/xml/master-xsd
// file: ../examples/xml/master-xsd/config.xsd
const char config_xsd[] = "\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
\
    <xsd:annotation>\
        <xsd:documentation xml:lang=\"fr\">\
            Définitions globales, s'appliquant à tous les types de messages pour l'application NetTop\
        </xsd:documentation>\
    </xsd:annotation>\
\
    <xsd:element name=\"message\" type=\"messageType\" />\
\
\
\
    <xsd:complexType name=\"messageType\">\
        <xsd:sequence>\
            <xsd:element name=\"action\" type=\"actionType\"/>\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"actionType\">\
        <xsd:sequence>\
            <xsd:any minOccurs=\"0\" maxOccurs=\"unbounded\" processContents=\"skip\" />\
        </xsd:sequence>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"config\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
</xsd:schema>\n";

// file: ../examples/xml/master-xsd/httpRequest-query.xsd
const char httpRequest_query_xsd[] = "\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
\
    <xsd:annotation>\
        <xsd:documentation xml:lang=\"fr\">\
            Définitions globales, s'appliquant à tous les types de messages pour l'application NetTop\
        </xsd:documentation>\
    </xsd:annotation>\
\
  <xsd:element name=\"message\" type=\"messageType\" />\
\
\
\
    <xsd:complexType name=\"messageType\">\
        <xsd:sequence>\
            <xsd:element name=\"action\" type=\"actionType\"/>\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"actionType\">\
        <xsd:sequence>\
            <xsd:element name=\"query\" type=\"queryType\" minOccurs=\"1\" maxOccurs=\"1\"/>\
        </xsd:sequence>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"httpRequest-query\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"queryType\">\
        <xsd:all>\
            <xsd:element name=\"path\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\" />\
            <xsd:element name=\"headers\" type=\"headersType\" minOccurs=\"1\" maxOccurs=\"1\" />\
            <xsd:element name=\"post-args\" type=\"postArgsType\" minOccurs=\"0\" maxOccurs=\"1\" />\
        </xsd:all>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"GET\"/>\
                    <xsd:enumeration value=\"POST\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"headersType\">\
        <xsd:all>\
            <xsd:element name=\"Accept\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\" />\
            <xsd:element name=\"Accept-Charset\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\" />\
            <xsd:element name=\"Accept-Language\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\" />\
            <xsd:element name=\"User-Agent\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\" />\
        </xsd:all>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"postArgsType\">\
        <xsd:sequence>\
            <xsd:element name=\"arg\" type=\"postPostArgType\" minOccurs=\"0\" maxOccurs=\"unbounded\" />\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"postPostArgType\">\
        <xsd:simpleContent>\
            <xsd:extension base=\"xsd:string\">\
                <xsd:attribute name=\"name\" type=\"xsd:string\" />\
            </xsd:extension>\
        </xsd:simpleContent>\
    </xsd:complexType>\
    \
</xsd:schema>\n";

// file: ../examples/xml/master-xsd/sqlquery-error.xsd
const char sqlquery_error_xsd[] = "\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
\
    <xsd:annotation>\
        <xsd:documentation xml:lang=\"fr\">\
            Définitions globales, s'appliquant à tous les types de messages pour l'application NetTop\
        </xsd:documentation>\
    </xsd:annotation>\
\
  <xsd:element name=\"message\" type=\"messageType\" />\
\
\
\
    <xsd:complexType name=\"messageType\">\
        <xsd:sequence>\
            <xsd:element name=\"action\" type=\"actionType\"/>\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"actionType\">\
        <xsd:sequence>\
            <xsd:element name=\"query\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\"/>\
            <xsd:element name=\"error\" type=\"errorType\" minOccurs=\"1\" maxOccurs=\"1\"/>\
        </xsd:sequence>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"sqlquery-error\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"errorType\">\
        <xsd:simpleContent>\
            <xsd:extension base=\"xsd:string\">\
                <xsd:attribute name=\"code\" type=\"xsd:unsignedInt\" />\
            </xsd:extension>\
        </xsd:simpleContent>\
    </xsd:complexType>\
\
</xsd:schema>\n";

// file: ../examples/xml/master-xsd/sqlquery-modif-result.xsd
const char sqlquery_modif_result_xsd[] = "\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
\
    <xsd:annotation>\
        <xsd:documentation xml:lang=\"fr\">\
            Définitions globales, s'appliquant à tous les types de messages pour l'application NetTop\
        </xsd:documentation>\
    </xsd:annotation>\
\
\
\
    <xsd:complexType name=\"SQLITE_BASE\">\
        <xsd:simpleContent>\
            <xsd:extension base=\"xsd:string\">\
                <xsd:attribute name=\"columnId\" type=\"xsd:unsignedInt\" />\
                <xsd:attribute name=\"type\">\
                    <xsd:simpleType>\
                        <xsd:restriction base=\"xsd:string\">\
                            <xsd:enumeration value=\"NULL\"/>\
                            <xsd:enumeration value=\"INTEGER\"/>\
                            <xsd:enumeration value=\"REAL\"/>\
                            <xsd:enumeration value=\"TEXT\"/>\
                            <xsd:enumeration value=\"BLOB\"/>\
                        </xsd:restriction>\
                    </xsd:simpleType>\
                </xsd:attribute>\
                <xsd:attribute name=\"name\" type=\"xsd:string\" />\
            </xsd:extension>\
        </xsd:simpleContent>\
    </xsd:complexType>\
    \
\
\
  <xsd:element name=\"message\" type=\"messageType\" />\
\
\
\
    <xsd:complexType name=\"messageType\">\
        <xsd:all>\
            <xsd:element name=\"action\" type=\"actionType\"/>\
        </xsd:all>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"actionType\">\
        <xsd:sequence>\
            <xsd:element name=\"query\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\"/>\
            <xsd:element name=\"answer\" type=\"answerType\" minOccurs=\"1\" maxOccurs=\"1\"/>\
        </xsd:sequence>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"sqlquery-modif-result\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"answerType\">\
        <xsd:all>\
            <xsd:element name=\"changes\" type=\"xsd:unsignedInt\" minOccurs=\"1\" maxOccurs=\"1\" />\
        </xsd:all>\
    </xsd:complexType>\
\
</xsd:schema>\n";

// file: ../examples/xml/master-xsd/sqlquery-select-result.xsd
const char sqlquery_select_result_xsd[] = "\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
\
    <xsd:annotation>\
        <xsd:documentation xml:lang=\"fr\">\
            Définitions globales, s'appliquant à tous les types de messages pour l'application NetTop\
        </xsd:documentation>\
    </xsd:annotation>\
\
\
\
    <xsd:complexType name=\"SQLITE_BASE\">\
        <xsd:simpleContent>\
            <xsd:extension base=\"xsd:string\">\
                <xsd:attribute name=\"columnId\" type=\"xsd:unsignedInt\" />\
                <xsd:attribute name=\"type\">\
                    <xsd:simpleType>\
                        <xsd:restriction base=\"xsd:string\">\
                            <xsd:enumeration value=\"NULL\"/>\
                            <xsd:enumeration value=\"INTEGER\"/>\
                            <xsd:enumeration value=\"REAL\"/>\
                            <xsd:enumeration value=\"TEXT\"/>\
                            <xsd:enumeration value=\"BLOB\"/>\
                        </xsd:restriction>\
                    </xsd:simpleType>\
                </xsd:attribute>\
                <xsd:attribute name=\"name\" type=\"xsd:string\" />\
            </xsd:extension>\
        </xsd:simpleContent>\
    </xsd:complexType>\
    \
\
\
  <xsd:element name=\"message\" type=\"messageType\" />\
  \
\
    <xsd:complexType name=\"messageType\">\
        <xsd:sequence>\
            <xsd:element name=\"action\" type=\"actionType\"/>\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"actionType\">\
        <xsd:sequence>\
            <xsd:element name=\"query\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\"/>\
            <xsd:element name=\"answer\" type=\"answerType\" minOccurs=\"1\" maxOccurs=\"1\"/>\
        </xsd:sequence>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"sqlquery-select-result\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"answerType\">\
        <xsd:sequence>\
            <xsd:element name=\"row\" type=\"rowType\" minOccurs=\"0\" maxOccurs=\"unbounded\" />\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"rowType\">\
        <xsd:sequence>\
            <xsd:element name=\"value\" type=\"row_valueType\" minOccurs=\"0\" maxOccurs=\"unbounded\" />\
        </xsd:sequence>\
    </xsd:complexType>\
\
    \
    <xsd:complexType name=\"row_valueType\">\
        <xsd:simpleContent>\
            <xsd:extension base=\"SQLITE_BASE\" />\
        </xsd:simpleContent>\
    </xsd:complexType>\
\
</xsd:schema>\n";

// dir: ../examples/xml/module-xsd
// file: ../examples/xml/module-xsd/config-query.xsd
const char config_query_xsd[] = "\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
\
    <xsd:annotation>\
        <xsd:documentation xml:lang=\"fr\">\
            Définitions globales, s'appliquant à tous les types de messages pour l'application NetTop\
        </xsd:documentation>\
    </xsd:annotation>\
\
    <xsd:element name=\"message\" type=\"messageType\" />\
\
\
\
    <xsd:complexType name=\"messageType\">\
        <xsd:sequence>\
            <xsd:element name=\"action\" type=\"actionType\"/>\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"actionType\">\
        <xsd:sequence>\
            <xsd:element name=\"configQueryName\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"unbounded\"/>\
        </xsd:sequence>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"config-query\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
</xsd:schema>\n";

// file: ../examples/xml/module-xsd/httpRequest-answer.xsd
const char httpRequest_answer_xsd[] = "\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
\
    <xsd:annotation>\
        <xsd:documentation xml:lang=\"fr\">\
            Définitions globales, s'appliquant à tous les types de messages pour l'application NetTop\
        </xsd:documentation>\
    </xsd:annotation>\
\
    <xsd:element name=\"message\" type=\"messageType\" />\
\
\
\
    <xsd:complexType name=\"messageType\">\
        <xsd:sequence>\
            <xsd:element name=\"action\" type=\"actionType\"/>\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"actionType\">\
        <xsd:sequence>\
            <xsd:element name=\"answer\" type=\"answerType\" minOccurs=\"1\" maxOccurs=\"1\"/>\
        </xsd:sequence>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"httpRequest-answer\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
    <xsd:complexType name=\"answerType\">\
        <xsd:all>\
            <xsd:element name=\"code\" type=\"xsd:unsignedInt\" minOccurs=\"1\" maxOccurs=\"1\"/>\
            <xsd:element name=\"headers\" type=\"headersType\" minOccurs=\"1\" maxOccurs=\"1\"/>\
            <xsd:element name=\"body\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\"/>\
        </xsd:all>\
    </xsd:complexType>\
\
    <xsd:complexType name=\"headersType\">\
        <xsd:all>\
            <xsd:element name=\"Content-Type\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\" />\
            <xsd:element name=\"Content-Language\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\" />\
        </xsd:all>\
    </xsd:complexType>\
\
</xsd:schema>\n";

// file: ../examples/xml/module-xsd/log.xsd
const char log_xsd[] = "\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
\
    <xsd:annotation>\
        <xsd:documentation xml:lang=\"fr\">\
            Définitions globales, s'appliquant à tous les types de messages pour l'application NetTop\
        </xsd:documentation>\
    </xsd:annotation>\
\
    <xsd:element name=\"message\" type=\"messageType\" />\
\
\
\
    <xsd:complexType name=\"messageType\">\
        <xsd:sequence>\
            <xsd:element name=\"action\" type=\"actionType\"/>\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"actionType\">\
        <xsd:sequence>\
            <xsd:element name=\"logLevel\" type=\"logLevelType\" minOccurs=\"1\" maxOccurs=\"1\"/>\
            <xsd:element name=\"text\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\"/>\
        </xsd:sequence>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"log\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
    <xsd:simpleType name=\"logLevelType\">\
        <xsd:restriction base=\"xsd:string\">\
            <xsd:enumeration value=\"DEBUG\" />\
            <xsd:enumeration value=\"INFO\" />\
            <xsd:enumeration value=\"NOTICE\" />\
            <xsd:enumeration value=\"WARNING\" />\
            <xsd:enumeration value=\"ERROR\" />\
        </xsd:restriction>\
    </xsd:simpleType>\
\
</xsd:schema>\n";

// file: ../examples/xml/module-xsd/sqlquery-modif-query.xsd
const char sqlquery_modif_query_xsd[] = "\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
\
    <xsd:annotation>\
        <xsd:documentation xml:lang=\"fr\">\
            Définitions globales, s'appliquant à tous les types de messages pour l'application NetTop\
        </xsd:documentation>\
    </xsd:annotation>\
\
    <xsd:element name=\"message\" type=\"messageType\" />\
\
\
\
    <xsd:complexType name=\"messageType\">\
        <xsd:sequence>\
            <xsd:element name=\"action\" type=\"actionType\"/>\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"actionType\">\
        <xsd:sequence>\
            <xsd:element name=\"query\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\"/>\
        </xsd:sequence>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"sqlquery-modif-query\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
</xsd:schema>\n";

// file: ../examples/xml/module-xsd/sqlquery-select-query.xsd
const char sqlquery_select_query_xsd[] = "\
<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
\
    <xsd:annotation>\
        <xsd:documentation xml:lang=\"fr\">\
            Définitions globales, s'appliquant à tous les types de messages pour l'application NetTop\
        </xsd:documentation>\
    </xsd:annotation>\
\
    <xsd:element name=\"message\" type=\"messageType\" />\
\
\
\
    <xsd:complexType name=\"messageType\">\
        <xsd:sequence>\
            <xsd:element name=\"action\" type=\"actionType\"/>\
        </xsd:sequence>\
    </xsd:complexType>\
\
\
    <xsd:complexType name=\"actionType\">\
        <xsd:sequence>\
            <xsd:element name=\"query\" type=\"xsd:string\" minOccurs=\"1\" maxOccurs=\"1\"/>\
        </xsd:sequence>\
        <xsd:attribute name=\"type\">\
            <xsd:simpleType>\
                <xsd:restriction base=\"xsd:string\">\
                    <xsd:enumeration value=\"sqlquery-select-query\"/>\
                </xsd:restriction>\
            </xsd:simpleType>\
        </xsd:attribute>\
    </xsd:complexType>\
\
</xsd:schema>\n";


#endif
