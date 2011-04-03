/*
 * Copyright (C) 2011 Mark Hills <mark@pogo.org.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

#define _GNU_SOURCE /* strcasestr(), strdupa() */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "listing.h"

#define BLOCK 1024
#define MAX_WORDS 32
#define SEPARATOR ' '

/*
 * Initialise a record listing
 */

void listing_init(struct listing_t *ls)
{
    ls->record = NULL;
    ls->size = 0;
    ls->entries = 0;
}

/*
 * Deallocate resources associated with this listing
 *
 * The listing does not allocate records itself, so it is not
 * responsible for deallocating them.
 */

void listing_clear(struct listing_t *ls)
{
    if (ls->record != NULL)
        free(ls->record);
}

/*
 * Blank the listing so it contains no entries
 *
 * We don't de-allocate memory, but this gives us an advantage where
 * listing re-use is of similar size.
 */

void listing_blank(struct listing_t *ls)
{
    ls->entries = 0;
}

/*
 * Enlarge the storage space of the listing to at least the target
 * size
 *
 * Return: 0 on success or -1 on memory allocation failure
 * Post: size of listing is greater than or equal to target
 */

static int enlarge(struct listing_t *ls, size_t target)
{
    size_t p;
    struct record_t **ln;

    if (target <= ls->size)
        return 0;

    p = target + BLOCK - 1; /* pre-allocate additional entries */

    ln = realloc(ls->record, sizeof(struct record_t*) * p);
    if (ln == NULL) {
        perror("realloc");
        return -1;
    }

    ls->record = ln;
    ls->size = p;
    return 0;
}

/*
 * Add a record to the listing
 *
 * Return: 0 on success or -1 on memory allocation failure
 */

int listing_add(struct listing_t *ls, struct record_t *lr)
{
    if (enlarge(ls, ls->entries + 1) == -1)
        return -1;

    ls->record[ls->entries++] = lr;
    return 0;
}

/*
 * Remove a record from the listing
 *
 * Return: 0 on success or -1 if record was not found in given crate
 */

int listing_remove(struct listing_t *ls, struct record_t *lr)
{
    int n, pos = -1;

    /* search and remove given record; we don't care about possible double entries.
     * so if we found the entry skip the rest */
    for (n = 0; n < ls->entries; n++) {
        if (ls->record[n] == lr) {
            ls->record[n] = NULL;
            pos = n;
            break;
        }
    }

    /* record not in crate! */
    if (pos == -1)
        return -1;

    ls->entries--;

    /* resort list */
    for (n = pos; n < ls->entries; n++)
        ls->record[n] = ls->record[n + 1];

    return 0;
}

/*
 * Standard comparison function between two records
 */

static int record_cmp(const struct record_t *a, const struct record_t *b)
{
    int r;

    r = strcmp(a->artist, b->artist);
    if (r < 0)
        return -1;
    else if (r > 0)
        return 1;

    r = strcmp(a->title, b->title);
    if (r < 0)
        return -1;
    else if (r > 0)
        return 1;

    return strcmp(a->pathname, b->pathname);
}

/*
 * Comparison function, see qsort(3)
 */

static int qcompar(const void *a, const void *b)
{
    return record_cmp(*(struct record_t **)a, *(struct record_t **)b);
}

/*
 * Post: listing is sorted
 */

void listing_sort(struct listing_t *ls)
{
    qsort(ls->record, ls->entries, sizeof(struct record_t*), qcompar);
}

/*
 * Check if a record matches the given string. This function is the
 * definitive code which defines what constitutes a 'match'.
 *
 * Return: true if this is a match, otherwise false
 */

static bool record_match(struct record_t *re, const char *match)
{
    if (strcasestr(re->artist, match) != NULL)
        return true;
    if (strcasestr(re->title, match) != NULL)
        return true;
    return false;
}

/*
 * Check for a match against all the strings in a given
 * NULL-terminated array
 *
 * Return: true if the given record matches, otherwise false
 */

static bool record_match_all(struct record_t *re, char **matches)
{
    while (*matches != NULL) {
        if (!record_match(re, *matches))
            return false;
        matches++;
    }
    return true;
}

/*
 * Copy the source listing
 *
 * Return: 0 on success or -1 on memory allocation failure
 * Post: on failure, dest is valid but incomplete
 */

int listing_copy(const struct listing_t *src, struct listing_t *dest)
{
    int n;

    listing_blank(dest);

    for (n = 0; n < src->entries; n++) {
	if (listing_add(dest, src->record[n]) != 0)
	    return -1;
    }

    return 0;
}

/*
 * Find entries from the source listing with match the given string
 *
 * Copy the subset of the source listing which matches the given
 * string into the destination. This function defines what constitutes
 * a match.
 *
 * Return: 0 on success, or -1 on memory allocation failure
 * Post: on failure, dest is valid but incomplete
 */

int listing_match(struct listing_t *src, struct listing_t *dest,
		  const char *match)
{
    int n;
    char *buf, *words[MAX_WORDS];
    struct record_t *re;

    fprintf(stderr, "Matching '%s'\n", match);

    buf = strdupa(match);
    n = 0;
    for (;;) {
        char *s;

        if (n == MAX_WORDS - 1) {
            fputs("Ignoring excessive words in match string.\n", stderr);
            break;
        }

        words[n] = buf;
        n++;

        s = strchr(buf, SEPARATOR);
        if (s == NULL)
            break;
        *s = '\0';
        buf = s + 1; /* skip separator */
    }
    words[n] = NULL; /* terminate list */

    listing_blank(dest);

    for (n = 0; n < src->entries; n++) {
        re = src->record[n];

        if (record_match_all(re, words)) {
            if (listing_add(dest, re) == -1)
                return -1;
        }
    }

    return 0;
}

/*
 * Binary search of sorted listing
 *
 * We implement our own binary search rather than using the bsearch()
 * from stdlib.h, because we need to know the position to insert to if
 * the item is not found.
 *
 * Pre: base is sorted
 * Return: position of match >= item
 * Post: on exact match, *found is true
 */

static size_t bin_search(struct record_t **base, size_t n,
                         struct record_t *item, bool *found)
{
    int r;
    size_t mid;

    /* Return the first entry ordered after this one */

    if (n == 0) {
        *found = false;
        return 0;
    }

    mid = n / 2;
    r = record_cmp(item, base[mid]);

    if (r < 0)
        return bin_search(base, mid, item, found);
    if (r > 0)
        return mid + 1 + bin_search(base + mid + 1, n - mid - 1, item, found);

    *found = true;
    return mid;
}

/*
 * Insert or re-use an entry in a sorted listing
 *
 * Pre: listing is sorted
 * Return: pointer to item, or existing entry; NULL if out of memory
 * Post: listing is sorted and contains item or a matching item
 */

struct record_t* listing_insert(struct listing_t *ls, struct record_t *item)
{
    bool found;
    size_t z;

    z = bin_search(ls->record, ls->entries, item, &found);
    if (found)
        return ls->record[z];

    /* Insert the new item */

    if (enlarge(ls, ls->entries + 1) == -1)
        return NULL;

    memmove(ls->record + z + 1, ls->record + z,
            sizeof(struct record_t*) * (ls->entries - z));
    ls->record[z] = item;
    ls->entries++;

    return item;
}

/*
 * Debug the content of a listing to standard error
 */

void listing_debug(struct listing_t *ls)
{
    int n;

    for (n = 0; n < ls->entries; n++)
        fprintf(stderr, "%d: %s\n", n, ls->record[n]->pathname);
}
