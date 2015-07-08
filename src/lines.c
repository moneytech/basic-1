// -- Lines BASIC program storage

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lines.h"

static char* __memory;
static size_t __memory_size;

/*
  static void
_terminate(line* l)
{
  char* p = (char*) l;
  p += sizeof(line) + l->length;
  line* end = (line*) p;
  end->number = 0;
  end->length = 0;
}
*/

  static line*
_next(line* l)
{
  char* p = (char*) l;
  p += sizeof(line) + l->length;
  return (line*) p;
}

  static bool
_is_end(line* l)
{
  return l && l->number == 0 && l->length == 0;
}

  void
lines_init(char *memory, size_t memory_size)
{
  __memory = memory;
  __memory_size = memory_size;
 
  // Signal end 
  line* l = (line*) __memory;
  l->number = 0;
  l->length = 0;
}

  bool
lines_store(uint16_t number, char* contents)
{
  // support insert

  /*
  Find line that is to be insert after. That line has a line number < insert and the next line has a >
  */

  char *p = __memory;
  line* l = (line*) p;
  while ( ! _is_end( l ) )
  {
    line* next = _next( l );

    if ( l->number < number && next->number > number )
    {
      // We need to insert
      printf("insert %d\n", number);
    }

    l = next;
  }
  
  l->number = number;
  l->length = strlen(contents) + 1; // Length is offset to next line
  strcpy( &(l->contents), contents );
 
  line* end = _next( l );
  end->number = 0;
  end->length = 0;
 
  return true;
}

  bool
lines_delete(uint16_t number)
{
  // delete first line
  line* l = (line*) __memory;
  l->number = 0;
  l->length = 0;
  strcpy( &(l->contents), "" );

  return true;
}

  void
lines_list(lines_list_cb out)
{
  char *p = __memory;
  
  line* l = (line*) p;
  while( ! _is_end( l ) )
  {
    out(l->number, &(l->contents) );
    l = _next( l );
  }
}
