/* Copyright (C) 2015-2021, Wazuh Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef REGISTRY_H
#define REGISTRY_H

#ifdef WIN32

#include "../syscheck.h"

/**
 * @brief Retrieves the configuration associated with a given registry element.
 *
 * @param key A string holding the full path to the registry element.
 * @param arch An integer specifying the bit count of the register element, must be ARCH_32BIT or ARCH_64BIT.
 * @return A pointer to the associated registry configuration, NULL on error or if no valid configuration was found.
 */
registry *fim_registry_configuration(const char *key, int arch);

/**
 * @brief Free all memory associated with a registry.
 *
 * @param data A fim_entry object to be free'd.
 */
void fim_registry_free_entry(fim_entry *entry);

/**
 * @brief Main scheduled algorithm for registry scan
 */
void fim_registry_scan();

/**
 * @brief Create a cJSON object holding the attributes associated with a fim_registry_value_data according to its
 * configuration.
 *
 * @param data A fim_registry_value_data object holding the value attributes to be tranlated.
 * @param configuration The configuration associated with the registry value.
 * @return A pointer to a cJSON object the translated value attributes.
 */
cJSON *fim_registry_value_attributes_json(const fim_registry_value_data *data, const registry *configuration);

/**
 * @brief Create a cJSON object holding the attributes associated with a fim_registry_key according to its
 * configuration.
 *
 * @param data A fim_registry_key object holding the key attributes to be tranlated.
 * @param configuration The configuration associated with the registry key.
 * @return A pointer to a cJSON object the translated key attributes.
 */
cJSON *fim_registry_key_attributes_json(const fim_registry_key *data, const registry *configuration);

/**
 * @brief Check and trigger a FIM event on a registry.
 *
 * @param new New data aquired from the actual registry entry.
 * @param saved Registry information retrieved from the FIM DB.
 * @param configuration Configuration associated with the given registry.
 * @param mode FIM event mode which caused the event.
 * @param event_type Added, modifed or deleted event.
 * @param w_evt Whodata information associated with the current event.
 * @param diff A string holding the difference between the original and new value of the registry.
 * @return A cJSON object holding the generated event, NULL on error.
 */
cJSON *fim_registry_event(const fim_entry *new,
                          const fim_entry *saved,
                          const registry *configuration,
                          fim_event_mode mode,
                          unsigned int event_type,
                          whodata_evt *w_evt,
                          const char *diff);

#endif

#endif
