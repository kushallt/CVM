#pragma once

#include "commons.h"
#include "memory.h"
#include <string>
enum class hashState
{
    Empty,
    Full,
    Deleted,
};
struct Entry
{
    uint32_t hashValue = 0;
    ObjString *key;
    Value value = NIL_VAL;
    hashState state = hashState::Empty;
};

class HashTable
{
public:
    Entry *entries;
    int count;
    int capacity;

    void init_table()
    {
        entries = NULL;
        count = 0;
        capacity = 0;
    }
    uint32_t hash_function(ObjString *key)
    {
        uint32_t hash = 2166136261u;
        for (int i = 0; i < key->chars.size(); i++)
        {
            hash ^= (uint8_t)key->chars[i];
            hash *= 16777619;
        }
        return hash;
    }
    void insert(ObjString *key, Value value)
    {
        if (count + 1 >= capacity * (0.75))
        {
            int oldcap = capacity;
            grow_capacity();
            Entry *oldEntries = entries;
            entries = new Entry[capacity];
            for (int i = 0; i < oldcap; i++)
            {
                if (oldEntries[i].state == hashState::Full)
                {
                    insert(oldEntries[i].key, oldEntries[i].value); // recomputes index
                }
            }

            delete[] oldEntries;
        }
        uint32_t hash = hash_function(key);
        int index = hash & (capacity - 1);
        for (;;)
        {
            if (entries[index].state == hashState::Deleted || entries[index].state == hashState::Empty)
            {
                entries[index] = Entry{.hashValue = hash, .key = key, .value = value, .state = hashState::Full};
                return;
            }
            if (index + 1 < capacity)
                index++;
            else
                index = 0;
        }
    }
    int search(ObjString *key)
    {
        uint32_t hash = hash_function(key);
        int index = hash & (capacity - 1);
        for (int i = 0; i < capacity; i++)
        {
            if (entries[index].state == hashState::Empty)
            {
                return -1;
            }
            if (entries[index].key->chars == key->chars)
            {
                return index;
            }
            if (index + 1 < capacity)
                index++;
            else
                index = 0;
        }
        return -1;
    }
    void delete_Entry(ObjString *key)
    {
        int index = search(key);
        if (index != -1)
        {
            entries[index] = Entry{.state = hashState::Deleted};
        }
    }
    void free_table()
    {
        FREE_ARRAY(Entry, entries, count);
        init_table();
    }
    void grow_capacity()
    {
        capacity = capacity < 8 ? 8 : capacity * 2;
    }
};