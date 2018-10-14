#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "config.h"
#include "../tools/log.h"

int addConfigTarget(char *config_key, struct conf_target_ipc target, struct arr_config_target *config_target)
{
    struct config_target *tmp_config_target;

    config_target->target = realloc(config_target->target, (config_target->length + 1) * sizeof(struct config_target));

    if (config_target->target == NULL)
    {
        fprintf(stderr, "addConfigTarget: Unable to malloc (%d Bytes)\n", (config_target->length + 1) * sizeof(struct config_target));
        return 1;
    }

    tmp_config_target = config_target->target + (config_target->length + 1);

    strcpy(tmp_config_target->config_key, config_key);

    tmp_config_target->target = target;

    config_target->length++;

    return 0;
}

int delConfigTarget(struct arr_config_target *all_config_target, struct config_target config_target)
{
    struct config_target *tmp_config_target;
    struct config_target *new_config_target;
    for (int i = 0; i < all_config_target->length; i++)
    {
        tmp_config_target = all_config_target->target + i;
        if (strcmp(tmp_config_target->config_key, config_target.config_key) == 0)
        {
            new_config_target = malloc((all_config_target->length - 1) * sizeof(struct config_target));

            if (new_config_target == NULL)
            {
                fprintf(stderr, "delConfigTarget: Unable to malloc (%d Bytes)\n", (all_config_target->length - 1) * sizeof(struct config_target));
                return 1;
            }

            /* before */
            memcpy(
                new_config_target,
                all_config_target->target,
                i * sizeof(struct config_target));
            /* after */
            memcpy(
                new_config_target + i,
                all_config_target->target + (i + 1),
                (all_config_target->length - i - 1) * sizeof(struct config_target));

            free(all_config_target->target);
            all_config_target->length--;
        }
    }
    return 0;
}

void cleanConfig_target(struct arr_config_target all_config_target)
{
    struct config_target *targetIPC;
    for (int i = 0; i < all_config_target.length; i++)
    {
        targetIPC = all_config_target.target + i;
        free(targetIPC->config_key);
    }
    free(all_config_target.target);
}

struct conf_target_ipc *getIPCTarget(struct arr_config_target all_config_target, char *config_key)
{
    struct config_target *tmp_config_target;

    for (int i = 0; i < all_config_target.length; i++)
    {
        tmp_config_target = all_config_target.target + i;
        if (strcmp(tmp_config_target->config_key, config_key) == 0)
        {
            return &(tmp_config_target->target);
        }
    }

    return 0;
}

void readConf(char *confPath)
{
    /* Checking for file existance */
    struct stat confFileStat;
    int _stat = stat(confPath, &confFileStat);
    if (_stat != 0)
    {
        fprintf(stderr, "readConf: Unable to read file \n");
        perror("readConf: ");
        return;
    }

    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlNodePtr currChild;

    doc = xmlParseFile(confPath);

    if (doc == NULL)
    {
        fprintf(stderr, "Document not parsed successfully. \n");
        return;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return;
    }

    /*
    if (xmlStrcmp(cur->name, (const xmlChar *)"story"))
    {
        fprintf(stderr, "document of the wrong type, root node != story");
        xmlFreeDoc(doc);
        return;
    }
    */
    /*
    printf("Root node: %s\n", cur->name);

    for (currChild = cur->xmlChildrenNode; currChild; currChild = currChild->next)
    {
        if (currChild->type == XML_ELEMENT_NODE)
        {
            printf("node type: Element, name: %s\n", currChild->name);
        }
        else if (currChild->type == XML_TEXT_NODE)
        {
            printf("node type: Text, name: %s\n", currChild->name);
        }
        printf("content: %s\n", currChild->content);
    }
*/
    getModuleDocument(doc, "auth");
    getModuleDocument(doc, "system");

    xmlFreeDoc(doc);
    return;
}

char *getModuleDocument(xmlDocPtr doc, char *moduleName)
{
    xmlDocPtr newDoc;
    xmlNodePtr newDocBase;
    xmlNodePtr base;
    xmlNodePtr currModule;
    xmlChar *xmlbuff;
    int buffersize;

    base = xmlDocGetRootElement(doc);

    for (currModule = base->xmlChildrenNode; currModule; currModule = currModule->next)
    {
        if (currModule->type == XML_ELEMENT_NODE && xmlStrcmp(currModule->name, (const xmlChar *)moduleName) == 0)
        {
            /* Copying the whole content */
            newDoc = xmlNewDoc("1.0");
            newDocBase = xmlNewNode(NULL, moduleName);
            xmlDocSetRootElement(newDoc, newDocBase);
            xmlAddChildList(newDocBase, xmlCopyNodeList(currModule->children));
            xmlDocDumpFormatMemory(newDoc, &xmlbuff, &buffersize, 1);
            printf("%s", (char *)xmlbuff);

            /*
            * Free associated memory.
            */
            xmlFree(xmlbuff);
            xmlFreeDoc(newDoc);
        }
    }

    return NULL;
}