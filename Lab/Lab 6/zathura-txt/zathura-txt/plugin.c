#include "plugin.h"
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <stdlib.h>
#include <cairo.h>

static const char* get_extension(const char* path);

typedef struct txt_page_s {
  char** data;
}txt_page_t;

typedef struct txt_document_s {
  struct txt_page_s* pages; /**< List of metadata structs */
}txt_document_t;

ZATHURA_PLUGIN_REGISTER_WITH_FUNCTIONS(
  "txt",
  VERSION_MAJOR, VERSION_MINOR, VERSION_REV,
  ZATHURA_PLUGIN_FUNCTIONS({
    .document_open            = txt_document_open,
    .document_free            = txt_document_free,
    .page_init                = txt_page_init,
    .page_clear               = txt_page_clear,
    .page_render_cairo        = txt_page_render_cairo,
  }),
  ZATHURA_PLUGIN_MIMETYPES({
    "text/plain"
  })
)

/* ============= MEMBER FUNC. ============== */

zathura_error_t txt_document_open(zathura_document_t* document){
  zathura_error_t error = ZATHURA_ERROR_OK;
  txt_document_t *txt_document = g_malloc0(sizeof(txt_document_t));

  const char* path = zathura_document_get_path(document);
  FILE *fin= fopen(path, "r");

  txt_document->pages = (txt_page_t *)malloc(sizeof(txt_page_t));
  txt_document->pages[0].data = malloc(40*sizeof(char*));
  char tmpData[256];

  fgets(tmpData,256,fin);
  txt_document->pages[0].data[0] = strdup(tmpData);
  fclose(fin);

  zathura_document_set_number_of_pages(document, 1);
  zathura_document_set_data(document, txt_document);
  return error;
}

zathura_error_t txt_document_free(zathura_document_t* document, void* txt_document){
  txt_document_t *txt_documentt = txt_document;
  free(txt_documentt->pages);
  g_free(txt_documentt);
  return ZATHURA_ERROR_OK;
}

zathura_error_t txt_page_render_cairo(zathura_page_t* page, void* txt_page, cairo_t* cairo, bool printing){
  txt_page_t* txt_pagee = txt_page;
  zathura_document_t* document = zathura_page_get_document(page);
  cairo_move_to(cairo, 20, 30);
  cairo_show_text(cairo, txt_pagee->data[0]);
  return ZATHURA_ERROR_OK;
}

zathura_error_t txt_page_init(zathura_page_t* page){
  zathura_document_t* document = zathura_page_get_document(page);
  txt_document_t* txt_document = zathura_document_get_data(document);
  zathura_page_set_width(page, 500);
  zathura_page_set_height(page, 500);
  zathura_page_set_data(page, txt_document->pages);
  return ZATHURA_ERROR_OK;
}

zathura_error_t txt_page_clear(zathura_page_t* page, void* txt_page){
  txt_page_t* txt_pagee = txt_page;
  free(txt_pagee->data);
  return ZATHURA_ERROR_OK;
}

/* ============= STATIC ============= */

static const char*
get_extension(const char* path)
{
  if (path == NULL) {
    return NULL;
  }

  unsigned int i = strlen(path);
  for (; i > 0; i--) {
    if (*(path + i) != '.') {
      continue;
    } else {
      break;
    }
  }

  if (i == 0) {
    return NULL;
  }

  return path + i + 1;
}
