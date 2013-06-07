/***
 * Simple engine for creating rudimentary PDFs
 * Doesn't support anything too fancy (use a proper PDF library
 * in that case)
 */

#ifndef PDF_GEN_H
#define PDF_GEN_H

#include <stdint.h>

#define PDF_RGB(r,g,b) ((((r) & 0xff) << 16) | (((g) & 0xff) << 8) | (((b) & 0xff)))
#define PDF_RED PDF_RGB(0xff, 0, 0)
#define PDF_GREEN PDF_RGB(0, 0xff, 0)

/**
 * @defgroup subsystem Simple PDF Generation
 * Allows for quick generation of simple PDF documents.
 * This is useful for producing easily printed output from C code, where advanced
 * formatting is not required
 *
 * Note: All coordinates/sizes are in points (1/72 of an inch)
 * All coordinates are based on 0,0 being the bottom left of the page
 *
 * @par PDF library example:
 * @code
#include "pdfgen.h"
 ...
struct pdf_doc *pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT);
pdf_append_page(pdf);
pdf_add_text(pdf, NULL, "This is text", 12, 50, 20);
pdf_add_line(pdf, NULL, 50, 24, 150, 24);
pdf_save(pdf, "output.pdf");
pdf_destroy(pdf);
 * @endcode
 */

struct pdf_doc;
struct pdf_object;

struct pdf_info {
    char creator[64];
    char producer[64];
    char title[64];
    char author[64];
    char subject[64];
    char date[64];
};

/**
 * Convert a value in inches into a number of points.
 * Always returns an integer value
 */
#define PDF_INCH_TO_POINT(inch) ((int)((inch) * 72 + 0.5))

/**
 * Convert a value in milli-meters into a number of points.
 * Always returns an integer value
 */
#define PDF_MM_TO_POINT(mm) ((int)((mm) * 72 / 25.4 + 0.5))

/**
 * Point width of a standard US-Letter page
 */
#define PDF_LETTER_WIDTH PDF_INCH_TO_POINT(8.5)

/**
 * Point height of a standard US-Letter page
 */
#define PDF_LETTER_HEIGHT PDF_INCH_TO_POINT(11)

/**
 * Point width of a standard A4 page
 */
#define PDF_A4_WIDTH PDF_MM_TO_POINT(210)

/**
 * Point height of a standard A4 page
 */
#define PDF_A4_HEIGHT PDF_MM_TO_POINT(297)

/**
 * Create a new PDF object, with the given page
 * width/height
 * @param width Width of the page
 * @param height Height of the page
 * @param info Optional information to be put into the PDF header
 * @return PDF document object, or NULL on failure
 */
struct pdf_doc *pdf_create(int width, int height, struct pdf_info *info);

/**
 * Destroy the pdf object, and all of its associated memory
 */
void pdf_destroy(struct pdf_doc *pdf);

/**
 * Retrieve the error message if any operation fails
 * @param pdf pdf document to retrieve error message from
 * @param errval optional pointer to an integer to be set to the error code
 * @return NULL if no error message, string description of error otherwise
 */
const char *pdf_get_err(struct pdf_doc *pdf, int *errval);

/**
 * Sets the font to use in the output. Default value is Times-Roman if
 * this function is not called
 * @param pdf PDF document to update font on
 * @param font New font to use. This must be one of the standard PDF fonts:
 *  Courier, Courier-Bold, Courier-BoldOblique, Courier-Oblique,
 *  Helvetica, Helvetica-Bold, Helvetica-BoldOblique, Helvetica-Oblique,
 *  Times-Roman, Times-Bold, Times-Italic, Times-BoldItalic,
 *  Symbol or ZapfDingbats
 * @return < 0 on failure, 0 on success
 */
int pdf_set_font(struct pdf_doc *pdf, const char *font);

/**
 * Retrieves a PDF document height
 */
int pdf_height(struct pdf_doc *pdf);

/**
 * Retrieves a PDF document width
 */
int pdf_width(struct pdf_doc *pdf);

/**
 * Add a new page to the given pdf
 * @return new page object
 */
struct pdf_object *pdf_append_page(struct pdf_doc *pdf);

/**
 * Save the given pdf document to the supplied filename
 */
int pdf_save(struct pdf_doc *pdf, const char *filename);

/**
 * Add a text string to the document
 * @param pdf PDF document to add to
 * @param page Page to add object to (NULL => most recently added page)
 * @param text String to display
 * @param size Point size of the font
 * @param xoff X location to put it in
 * @param yoff Y location to put it in
 * @return 0 on success, < 0 on failure
 */
int pdf_add_text(struct pdf_doc *pdf, struct pdf_object *page,
        const char *text, int size, int xoff, int yoff, uint32_t colour);

/**
 * Add a line to the document
 * @param pdf PDF document to add to
 * @param page Page to add object to (NULL => most recently added page)
 * @param x1 X offset of start of line
 * @param y1 Y offset of start of line
 * @param x2 X offset of end of line
 * @param y2 Y offset of end of line
 * @param width Width of the line
 * @return 0 on success, < 0 on failure
 */
int pdf_add_line(struct pdf_doc *pdf, struct pdf_object *page,
    int x1, int y1, int x2, int y2, int width, uint32_t colour);

/**
 * Add an outline rectangle to the document
 * @param pdf PDF document to add to
 * @param page Page to add object to (NULL => most recently added page)
 * @param x X offset to start rectangle at
 * @param y Y offset to start rectangle at
 * @param width Width of rectangle
 * @param height Height of rectangle
 * @param border_width Width of rectangle border
 * @return 0 on succss, < 0 on failure
 */
int pdf_add_rectangle(struct pdf_doc *pdf, struct pdf_object *page,
    int x, int y, int width, int height, int border_width, uint32_t colour);

int pdf_add_filled_rectangle(struct pdf_doc *pdf, struct pdf_object *page,
    int x, int y, int width, int height, int border_width,
    uint32_t colour);

/**
 * Add a bookmark to the document
 *
 * Note: At the moment all bookmarks are at the top level -
 * there is no support for the full bookmark tree structure yet.
 * @param pdf PDF document to add bookmark to
 * @param page Page to jump to for bookmark
               (or NULL for the most recently added page)
 * @param name String to associate with the bookmark
 * @return < 0 on failure, new bookmark id on success
 */
int pdf_add_bookmark(struct pdf_doc *pdf, struct pdf_object *page,
        const char *name);

#endif
