/* @(#) $Id$ */

/* Copyright (C) 2003-2008 Third Brigade, Inc.
 * All right reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 3) as published by the FSF - Free Software
 * Foundation
 */

/* Functions to handle the configuration files
 */


#include "shared.h"
#include "agentlessd-config.h"


int Read_CAgentless(XML_NODE node, void *config, void *config2)
{
    int i = 0,j = 0,s = 0;

    /* XML definitions */
    char *xml_lessd_server = "host";
    char *xml_lessd_port = "port";
    char *xml_lessd_type = "type";
    char *xml_lessd_frequency = "frequency";
    char *xml_lessd_state = "state";
    char *xml_lessd_command = "run_command";
    char *xml_lessd_options = "arguments";


    agentlessd_config *lessd_config = (agentlessd_config *)config;

     
    /* Getting any configured entry. */
    if(lessd_config->entries)
    {
        while(lessd_config->entries[s])
            s++;
    }

    
    /* Allocating the memory for the config. */
    os_realloc(lessd_config->entries, (s + 2) * sizeof(agentlessd_entries *), 
               lessd_config->entries);
    os_calloc(1, sizeof(agentlessd_entries), lessd_config->entries[s]);
    lessd_config->entries[s + 1] = NULL;


    /* Zeroing the elements. */
    lessd_config->entries[s]->server = NULL;
    lessd_config->entries[s]->command = NULL;
    lessd_config->entries[s]->options = "";
    lessd_config->entries[s]->type = NULL;
    lessd_config->entries[s]->frequency = 86400;
    lessd_config->entries[s]->state = 0;
    lessd_config->entries[s]->current_state = 0;
    lessd_config->entries[s]->port = 0;
    lessd_config->entries[s]->error_flag = 0;

    
    /* Reading the XML. */
    while(node[i])
    {
        if(!node[i]->element)
        {
            merror(XML_ELEMNULL, ARGV0);
            return(OS_INVALID);
        }
        else if(!node[i]->content)
        {
            merror(XML_VALUENULL, ARGV0, node[i]->element);
            return(OS_INVALID);
        }
        else if(strcmp(node[i]->element, xml_lessd_frequency) == 0)
        {
            if(!OS_StrIsNum(node[i]->content))
            {
                merror(XML_VALUEERR,ARGV0,node[i]->element,node[i]->content);
                return(OS_INVALID);
            }

            lessd_config->entries[s]->frequency = atoi(node[i]->content);
        }
        else if(strcmp(node[i]->element, xml_lessd_port) == 0)
        {
            if(!OS_StrIsNum(node[i]->content))
            {
                merror(XML_VALUEERR,ARGV0,node[i]->element,node[i]->content);
                return(OS_INVALID);
            }

            lessd_config->entries[s]->port = atoi(node[i]->content);
        }
        else if(strcmp(node[i]->element, xml_lessd_server) == 0)
        {
            /* Getting any configured entry. */
            j = 0;
            if(lessd_config->entries[s]->server)
            {
                while(lessd_config->entries[s]->server[j])
                    j++;
            }

            os_realloc(lessd_config->entries[s]->server, (j + 2) * 
                       sizeof(char *), 
                       lessd_config->entries[s]->server);
            os_strdup(node[i]->content, lessd_config->entries[s]->server[j]);
            lessd_config->entries[s]->server[j + 1] = NULL;
        }
        else if(strcmp(node[i]->element, xml_lessd_type) == 0)
        {
            char script_path[1024 +1];

            script_path[1024] = '\0';
            snprintf(script_path, 1024, "%s/%s", AGENTLESSDIRPATH,
                                         node[i]->content);
            
            if(File_DateofChange(script_path) <= 0)
            {
                merror("%s: ERROR: Unable to find '%s' at '%s'.",
                       ARGV0, node[i]->content, AGENTLESSDIRPATH); 
                merror(XML_VALUEERR,ARGV0, node[i]->element, node[i]->content);
                return(OS_INVALID);
            }
            os_strdup(node[i]->content, lessd_config->entries[s]->type);
        }
        else if(strcmp(node[i]->element, xml_lessd_command) == 0)
        {
            os_strdup(node[i]->content, lessd_config->entries[s]->command);
        }
        else if(strcmp(node[i]->element, xml_lessd_options) == 0)
        {
            os_strdup(node[i]->content, lessd_config->entries[s]->options);
        }
        else if(strcmp(node[i]->element, xml_lessd_state) == 0)
        {
            if(strcmp(node[i]->content, "periodic") == 0)
            {
                lessd_config->entries[s]->state |= LESSD_STATE_PERIODIC;
            }
            else if(strcmp(node[i]->content, "stay_connected") == 0)
            {
                lessd_config->entries[s]->state |= LESSD_STATE_CONNECTED;
            }
            else if(strcmp(node[i]->content, "periodic_diff") == 0)
            {
                lessd_config->entries[s]->state |= LESSD_STATE_PERIODIC;
                lessd_config->entries[s]->state |= LESSD_STATE_DIFF;
            }
            else
            {
                merror(XML_VALUEERR,ARGV0,node[i]->element,node[i]->content);
                return(OS_INVALID);
            }
        }
        else
        {
            merror(XML_INVELEM, ARGV0, node[i]->element);
            return(OS_INVALID);
        }
        i++;
    }


    /* We must have at least one entry set */
    if(!lessd_config->entries[s]->server ||
       !lessd_config->entries[s]->state ||
       !lessd_config->entries[s]->type)
    {
        merror(XML_INV_MISSOPTS, ARGV0);
        return(OS_INVALID);
    }
       
    
    if((lessd_config->entries[s]->state == LESSD_STATE_PERIODIC) &&
       !lessd_config->entries[s]->frequency)
    {
        merror(XML_INV_MISSFREQ, ARGV0);
        return(OS_INVALID);
    }

    return(0);
}


/* EOF */
