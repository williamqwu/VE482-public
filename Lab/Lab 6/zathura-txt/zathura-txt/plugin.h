#ifndef PLUGIN_H
#define PLUGIN_H

#include <stdbool.h>
#include <zathura/plugin-api.h>
#include <cairo.h>

/**
 * Open a txt document
 *
 * @param document Zathura document
 * @return ZATHURA_ERROR_OK if no error occurred otherwise see
 *   zathura_error_t
 */
GIRARA_HIDDEN zathura_error_t txt_document_open(zathura_document_t* document);

/**
 * Closes and frees the internal document structure
 *
 * @param document Zathura document
 * @return ZATHURA_ERROR_OK if no error occurred otherwise see
 *   zathura_error_t
 */
GIRARA_HIDDEN zathura_error_t txt_document_free(zathura_document_t* document, void* txt_document);

/**
 * Initializes the page with the needed values
 *
 * @param page The page object
 * @return ZATHURA_ERROR_OK when no error occurred, otherwise see
 *    zathura_error_t
 */
GIRARA_HIDDEN zathura_error_t txt_page_init(zathura_page_t* page);

/**
 * Frees a PostScript page
 *
 * @param page Page
 * @return ZATHURA_ERROR_OK if no error occurred otherwise see
 *   zathura_error_t
 */
GIRARA_HIDDEN zathura_error_t txt_page_clear(zathura_page_t* page, void* txt_page);

/**
 * Renders a page onto a cairo object
 *
 * @param page Page
 * @param cairo Cairo object
 * @param printing Set to true if page should be rendered for printing
 * @return ZATHURA_ERROR_OK if no error occurred otherwise see
 *   zathura_error_t
 */
GIRARA_HIDDEN zathura_error_t txt_page_render_cairo(zathura_page_t* page, void* txt_page, cairo_t* cairo, bool printing);

#endif // PLUGIN_H
