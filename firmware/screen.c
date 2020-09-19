/*
 Screen layer for ugui

 Copyright 2019, S. Kevin Hester, kevinh@geeksville.com

 (MIT License)
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include "screen.h"
// #include "lcd.h"
// #include "ugui.h"
// #include "fonts.h"
// #include "state.h"
// #include "mainscreen.h"
// #include "utils.h"

// volatile bool g_graphs_ui_update[3] = { false, false, false };

// variables_t g_vars[VARS_SIZE];
// #ifndef SW102
// GraphVars g_graphVars[VARS_SIZE];
// GraphData g_graphData[VARS_SIZE][3];
// #endif

// extern UG_GUI gui;

// // If true, the scroll position changed and force a complete redraw
// // FIXME - heading shouldn't be redrawn
// // FIXME - currently limited to one scrollable per screen
// static bool forceScrollableRelayout;

// // If the user is editing an editable, this will be it
// static Field *curActiveEditable = NULL, *g_curCustomizingField = NULL, *g_CustomizingGraphXAxis = NULL;

// volatile bool g_changeXAxisTrigger = false;
// volatile uint8_t g_xAxisReferenceScale = 0;

// #define MAX_SCROLLABLE_DEPTH 3 // How deep can we nest scrollables in our stack

// static Field *scrollableStack[MAX_SCROLLABLE_DEPTH];
// int scrollableStackPtr = 0; // Points to where to push the next entry (so if zero, stack is empty)

// // Returns true if we decided to draw something
// typedef bool (*FieldRenderFn)(FieldLayout *layout);

// static const FieldRenderFn renderers[];

// /// If true blink changed to be true or false this tick and we should redraw anything that is animated
// static bool blinkChanged;
// static bool blinkOn;

// static uint32_t screenUpdateCounter;

// void update_battery_power_usage_label(void);

// /**
//  * Used to map from FieldVariant enums to rendering functions
//  */

// static Screen *curScreen;
// static bool screenDirty;

// bool graphNeedUpdate = false;

// #ifdef SW102
// #define HEADING_FONT FONT_5X12
// #else
// #define HEADING_FONT TITLE_TEXT_FONT

// #endif

// #define SCROLLABLE_FONT CONFIGURATIONS_TEXT_FONT

// const UG_FONT *editable_label_font = &SMALL_TEXT_FONT;
// const UG_FONT *editable_value_font = &SMALL_TEXT_FONT;
// const UG_FONT *editable_units_font = &SMALL_TEXT_FONT;

// // We can optionally render by filling all with black and then drawing text with a transparent background
// // This is useful on very small screens (SW102) where we might want the text to overlap.  However, this
// // approach causes flickering on non memory resident framebuffers (850C)
// #ifdef SW102
// #define EDITABLE_BLANKALL true
// #else
// #define EDITABLE_BLANKALL false
// #endif

// // The default is for editables to be two rows tall, with the data value on the second row
// // define this as 1 if you want them to be one row tall (because you have a wide enough screen)
// #ifndef EDITABLE_NUM_ROWS
// #define EDITABLE_NUM_ROWS 2
// #endif

// // The default is C_WHITE, redefine if you want something else
// #ifndef EDITABLE_CURSOR_COLOR
// #define EDITABLE_CURSOR_COLOR       C_WHITE
// #endif

// static void putAligned(FieldLayout *layout, AlignmentX alignx,
// 		AlignmentY aligny, int insetx, int insety, const UG_FONT *font,
// 		const char *str);

// static UG_COLOR getBackColor(const FieldLayout *layout) {
// 	switch (layout->color) {
// 	case ColorInvert:
// 		return C_WHITE;

// 	case ColorHeading:
// 		return HEADING_BACKGROUND;

// 	case ColorNormal:
// 	default:
// 		return C_BLACK;
// 	}
// }

// static UG_COLOR getForeColor(const FieldLayout *layout) {
// 	switch (layout->color) {
// 	case ColorInvert:
// 		return C_BLACK;

// 	case ColorError:
// 		return C_ERROR;

// 	case ColorWarning:
// 		return C_WARNING;

// 	case ColorNormal:
// 	case ColorHeading:
// 	default:
// 		return C_WHITE;
// 	}
// }

// static void autoTextHeight(FieldLayout *layout) {
// 	// Allow developer to use this shorthand for one row high text fields
// 	if (layout->height == -1) {
// 		assert(layout->font); // you must specify a font to use this feature
// 		layout->height = layout->font->char_height;
// 	}
// }

// bool renderDrawTextCommon(FieldLayout *layout, const char *msg) {
// 	autoTextHeight(layout);
// 	UG_S16 height = layout->height;

// 	UG_COLOR back = getBackColor(layout);
// 	UG_SetForecolor(getForeColor(layout));

// 	// ug fonts include no blank space at the beginning, so we always include one col of padding
// 	UG_FillFrame(layout->x, layout->y, layout->x + layout->width - 1,
// 			layout->y + height - 1, back);
//   UG_SetBackcolor(back);
// 	if (!layout->field->rw->blink || blinkOn) // if we are supposed to blink do that
// 		putAligned(layout, layout->align_x, AlignTop, layout->inset_x,
// 				layout->inset_y, layout->font, msg);
// 	return true;
// }

// static bool renderDrawTextRW(FieldLayout *layout) {
// 	return renderDrawTextCommon(layout, layout->field->rw->drawTextPtr.msg);
// }

// static bool renderDrawTextRO(FieldLayout *layout) {
// 	return renderDrawTextCommon(layout, layout->field->drawTextPtr.msg);
// }

// static bool renderFill(FieldLayout *layout) {
// 	assert(layout->width >= 1);
// 	assert(layout->height >= 1);

// 	UG_FillFrame(layout->x, layout->y, layout->x + layout->width - 1,
// 			layout->y + layout->height - 1, getForeColor(layout));
// 	return true;
// }

// static bool renderMesh(FieldLayout *layout) {
// 	assert(layout->width >= 1);
// 	assert(layout->height >= 1);

// 	UG_DrawMesh(layout->x, layout->y, layout->x + layout->width - 1,
// 			layout->y + layout->height - 1, getForeColor(layout));
// 	return true;
// }

// static void drawSelectionMarkerForced(FieldLayout *layout) {
//   // Only consider doing this on items that might be animated
//   // we size the cursor to be slightly shorter than the box it is in

//   //  && !curActiveEditable - old code when editing don't blink the selection cursor
//   if (layout->field && layout->field->rw->is_selected) {
//     UG_FontSelect(&FONT_CURSORS);
//     UG_PutChar('0', layout->x + layout->width - FONT_CURSORS.char_width, // draw on ride side of line
//         layout->y + (layout->height - FONT_CURSORS.char_height) / 2, // draw centered vertially within the box
//         layout->field->rw->is_selected && blinkOn ? EDITABLE_CURSOR_COLOR : getBackColor(ColorNormal),
//             C_TRANSPARENT);
//   }
// }

// /**
//  * If we are selected, highlight this item with a bar to the left (on color screens possibly draw a small
//  * color pointer or at least color the line something nice.
//  */
// static void drawSelectionMarker(FieldLayout *layout) {
// 	// Only consider doing this on items that might be animated
// 	// we size the cursor to be slightly shorter than the box it is in
//   Field *field = layout->field;
//   if(!field)
//     return; // Couldn't have cursor

// 	//  if the field is currently selected we need to blink the cursor

//   // also if the field is marked as dirty, we render just in case the reason it is dirty is that is was previously selected
//   // FIXME - is_selected was probably a mistake, refactor to remove all these nasty field type checks
//   bool couldHaveCursor = field->rw->dirty && ((field->variant == FieldEditable && !field->editable.read_only) || field->variant == FieldScrollable);

// 	if (field->rw->is_selected || couldHaveCursor) {
// 		drawSelectionMarkerForced(layout);
// 	}
// }

// /**
//  * If we have a border on this layout, drawit
//  */
// static void drawBorder(FieldLayout *layout) {
// 	UG_COLOR color = getForeColor(layout);
// 	int fatness = (layout->border & BorderFat) ? Yby64(1) : 1;

// 	if (layout->border & BorderTop)
// 		UG_DrawLine(layout->x, layout->y, layout->x + layout->width - 1,
// 				layout->y, color); // top

// 	if (layout->border & BorderBottom)
// 		UG_FillFrame(layout->x, layout->y + layout->height - fatness,
// 				layout->x + layout->width - 1, layout->y + layout->height - 1,
// 				color); // bottom

// 	if (layout->border & BorderLeft)
// 		UG_DrawLine(layout->x, layout->y, layout->x,
// 				layout->y + layout->height - 1, color); // left

// 	if (layout->border & BorderRight)
// 		UG_DrawLine(layout->x + layout->width - 1, layout->y,
// 				layout->x + layout->width - 1, layout->y + layout->height - 1,
// 				color); // right
// }

// const Coord screenWidth = SCREEN_WIDTH, screenHeight = SCREEN_HEIGHT; // FIXME, for larger devices allow screen objcts to nest inside other screens

// // True while the user is holding down the m key and but not trying to edit anything
// // We use a static so we can detect when state changes
// // We do all this calculation only once in the main render loop, so that all fields change at once
// static bool oldForceLabels;
// static bool forceLabels;

// // If our currently rendered field is nested beneath a customizable, it is good to track the parent field
// Field *parentCustomizable;

// /// If we are redirecting to another field, we return the final field
// static Field* getField(const FieldLayout *layout) {
// 	Field *field = layout->field;
// 	assert(field);

// 	if (field->variant == FieldCustomizable) {
// 		assert(
// 				field && field->customizable.selector
// 						&& field->customizable.choices);

// 		parentCustomizable = field;
// 		field = field->customizable.choices[*field->customizable.selector];
// 	} else
// 		parentCustomizable = NULL;

// 	return field;
// }

// /// Should we redraw this field this tick? We always render dirty items, or items that might need to show blink animations
// static bool needsRender(Field *field) {
//   if (field->rw->visibility == FieldNotVisible)
//     return false;

// 	if (field->rw->visibility == FieldTransitionNotVisible ||
// 	    field->rw->visibility == FieldTransitionVisible)
// 	  return true;

//   if (field->rw->dirty)
// 		return true;

// 	if (blinkChanged) {
// 		if (field->rw->blink)
// 			return true; // this field is doing a blink animation and it is time for that to update

// 		if (field && field->rw->is_selected)
// 			return true; // we also do a blink animation for our selection cursor
// 	}

// 	if (field->variant == FieldEditable)
// 		return true; // Editables are smart enough to do their own rendering shortcuts based on cached values

// 	return false;
// }

// /**
//  * Render a specified field using the specified layout.  Usually field is just layout->field, but not always (renderCustomizable
//  * redirects to a different field for rendering)
//  *
//  */
// static bool renderField(FieldLayout *layout, Field *field) {
// 	return renderers[field->variant](layout);
// }

// const bool renderLayouts(FieldLayout *layouts, bool forceRender) {
// 	bool didDraw = false; // we only render to hardware if something changed

// 	Coord maxy = 0;

// 	bool didChangeForceLabels = false; // if we did label force/unforce we need to remember for the next render
// 	bool mpressed = SCREENFN_FORCE_LABELS;

// 	// For each field if that field is dirty (or the screen is) redraw it
// 	for (FieldLayout *layout = layouts; layout->field; layout++) {
// 		Field *field = getField(layout); // we might be redirecting

// 		if (forceRender) // tell the field it must redraw itself
// 			field->rw->dirty = true;

// 		if (field->variant == FieldEditable &&
// 		    field->rw->visibility == FieldVisible) {
// 			// If this field normally doesn't show the label, but M is pressed now, show it
// 			forceLabels = mpressed && layout->label_align_x == AlignHidden && field->rw->visibility == FieldVisible;
// 			didChangeForceLabels = true;
// 		}

// 		// We always render dirty items, or items that might need to show blink animations
// 		if (needsRender(field)) {
// 			if (layout->width == 0)
// 				layout->width = screenWidth - layout->x;

// 			if (layout->height == 0)
// 				layout->height = screenHeight - layout->y;

// 			// if user specified width in terms of characters, change it to pixels
// 			if (layout->width < 0) {
// 			  if (field->variant != FieldCustom)
// 			    assert(layout->font); // you must specify a font to use this feature
// 				layout->width = -layout->width
// 						* (layout->font->char_width + gui.char_h_space);
// 			}

// 			// a y <0 means, start just below the previous lowest point on the screen, -1 is immediately below, -2 has one blank line, -3 etc...
// 			if (layout->y < 0)
// 				layout->y = maxy + -layout->y - 1;

// 			didDraw |= renderField(layout, field);

// //			assert(layout->height != -1); // by the time we reach here this must be set

// 			// After the renderer has run, cache the highest Y we have seen (for entries that have y = -1 for auto assignment)
// 			if (layout->y + layout->height > maxy)
// 				maxy = layout->y + layout->height;

// 			drawSelectionMarker(layout);
// 			drawBorder(layout);
// 		}
// 	}

// 	// We clear the dirty bits in a separate pass because multiple layouts on the screen might share the same field
// 	for (const FieldLayout *layout = layouts; layout->field; layout++) {
// 		getField(layout)->rw->dirty = false; // we call getField because we might be redirecting
// 	}

// 	if (didChangeForceLabels)
// 		oldForceLabels = forceLabels;

// 	return didDraw;
// }

// // Return the scrollable we are currently showing the user, or NULL if none
// // The (currently only one allowed per screen) scrollable that is currently being shown to the user.
// // if the scrollable changes, we'll need to regenerate the entire render
// static Field* getActiveScrollable() {
// 	return scrollableStackPtr ? scrollableStack[scrollableStackPtr - 1] : NULL;
// }

// /**
//  * The user just clicked on a scrollable entry, descend down into it
//  */
// static void enterScrollable(Field *f) {
// 	assert(scrollableStackPtr < MAX_SCROLLABLE_DEPTH);
// 	scrollableStack[scrollableStackPtr++] = f;

// 	// We always set blink for scrollables, because they contain child items that might need to blink
// 	f->rw->blink = true;

// 	// NOTE: Only the root scrollable is ever checked for 'dirty' by the main screen renderer,
// 	// so that's the one we set
// 	scrollableStack[0]->rw->dirty = true;

// 	forceScrollableRelayout = true;

//   // clean the full screen
//   screenDirty = true;
// }

// /**
//  * The user just clicked to exit a scrollable entry, ascend to the entry above us or if we are the top
//  * go back to the main screen
//  *
//  * @return true if we just selected a new scrollable
//  */
// static bool exitScrollable() {
// 	assert(scrollableStackPtr > 0);
// 	scrollableStackPtr--;

// 	Field *f = getActiveScrollable();
// 	if (f) {
// 		// Parent was a scrollable, show it
// 		f->rw->dirty = true;
// 		forceScrollableRelayout = true;

// 	  // clean the full screen
// 	  screenDirty = true;
// 	} else {
// 		// otherwise we just leave the screen
// 	  showNextScreen();
// 	}

//   return true;
// }

// #define SCROLLABLE_VPAD 4 // extra space between each row (for visual appearance)
// #define SCROLLABLE_ROW_HEIGHT (SCROLLABLE_VPAD + 16) // for planning purposes - might be larger at runtime
// #define MAX_SCROLLABLE_ROWS (SCREEN_HEIGHT / SCROLLABLE_ROW_HEIGHT) // Max number of rows we can show on one screen (including header)

// static int maxRowsPerScreen;

// static Field heading = FIELD_DRAWTEXT_RW();
// static Field label = FIELD_DRAWTEXT_RW();
// static Field blankRow = FIELD_FILL; // Used to fill with blank space if necessary FIELD_FILL

// static bool renderActiveScrollable(FieldLayout *layout, Field *field) {
// 	const Coord rowHeight = EDITABLE_NUM_ROWS
// 			* (SCROLLABLE_FONT.char_height + gui.char_v_space) + SCROLLABLE_VPAD;
// 	maxRowsPerScreen = SCREEN_HEIGHT / rowHeight; // might be less than MAX_SCROLLABLE_ROWS

// 	Field *scrollable = getActiveScrollable();
// 	bool weAreExpanded = scrollable == field;

// 	assert(rowHeight >= SCROLLABLE_ROW_HEIGHT); // Make sure we we don't violate our array sizes

// 	// If we are expanded show our heading and the current visible child elements
// 	// Otherwise just show our label so that the user might select us to expand
// 	if (weAreExpanded) {
// 		static FieldLayout rows[MAX_SCROLLABLE_ROWS + 1]; // Used to layout each of the currently visible rows + heading + end of rows marker

// 		if (forceScrollableRelayout) {
// 			bool hasMoreRows = true; // Once we reach an invalid row we stop rendering and instead fill with blank space

// 			forceScrollableRelayout = false;
// 			int i = 0;
// 			for (; i < maxRowsPerScreen && hasMoreRows; i++) {
// 				FieldLayout *r = rows + i;

// 				r->x = layout->x;
// 				r->width = layout->width;
// 				r->border = BorderNone;

// 				if (i == 0) { // heading
// 					heading.rw->dirty = true; // Force the heading to be redrawn even if we aren't chaning the string
// 					fieldPrintf(&heading, "%s", field->scrollable.label);
// 					r->field = &heading;
// 					r->color = ColorHeading;
// 					r->border = HEADING_BORDER;
// 					r->font = &HEADING_FONT;

// 					r->y = layout->y;
// 					r->height = r->font->char_height + gui.char_v_space
// 							+ SCROLLABLE_VPAD;
// 				} else {
// 					r->y = rows[i - 1].y + rows[i - 1].height;
// 					r->label_align_y = EDITABLE_NUM_ROWS == 1 ? AlignCenter : AlignTop;
// 					r->label_align_x = AlignLeft;
// 					r->align_x = AlignRight;
// 					r->inset_x = FONT_CURSORS.char_width; // move the value all the way to the right (but leave room for the cursor)

// 					// visible menu rows, starting with where the user has scrolled to
// 					const int entryNum = field->rw->scrollable.first + i - 1;

// 					// Get the next entry
// 					Field *entry = &field->scrollable.entries[entryNum];

// 					if (entry->variant == FieldEnd)
// 						hasMoreRows = false; // This will short circuit all future processing

// 					// if the current row is valid, render that, otherwise render blank space
// 					if (hasMoreRows) {
// 	          r->height = rowHeight; // all data rows are the same height
// 						entry->rw->dirty = true; // Force it to be redrawn

// 						r->field = entry;
// 						entry->rw->is_selected = (entryNum
// 								== field->rw->scrollable.selected);
// 						entry->rw->blink = entry->rw->is_selected;
// 						r->color = ColorNormal; // force color because r points to a static object that holds previous color
// 					} else {
// 						r->field = &blankRow;

// 						// For this blank row at the end resize it to fill the rest of vertical space
// 						r->height = rowHeight * (maxRowsPerScreen - i);

// 						r->color = ColorInvert; // black box for empty slots at end
// 					}

// 					r->field->rw->dirty = true; // Force rerender
// 				} // end data row handling
// 			} // end looping over all valid rows
// 			rows[i].field = NULL; // mark end of array (for rendering)
// 		} // end forceRelayout

// 		// draw (or redraw if necessary) our current set of visible rows
// 		return renderLayouts(rows, false);
// 	} else {
// 		static FieldLayout rows[1 + 1]; // Used to layout each our single row + end of rows marker

// 		// Just draw our label (not highlighted) - show selection bar if necessary
// 		FieldLayout *r = &rows[0];

// 		r->x = layout->x;
// 		r->y = layout->y;
// 		r->width = layout->width;
// 		r->height = layout->height;
// 		r->border = BorderNone;

// 		fieldPrintf(&label, "%s", field->scrollable.label);
// 		r->field = &label;
// 		r->color = ColorNormal;
// 		r->font = &SCROLLABLE_FONT;

// 		// If we are inside a scrollable and selected, blink
// 		if (scrollable)
// 			label.rw->is_selected =
// 					field
// 							== &scrollable->scrollable.entries[scrollable->rw->scrollable.selected];
// 		else
// 			label.rw->is_selected = false;
// 		// label.blink = label.is_selected; // we no longer need to set blink if is_selected is set

// 		rows[1].field = NULL; // mark end of array (for rendering)

// 		// draw (or redraw if necessary) our current set of visible rows
// 		return renderLayouts(rows, false);
// 	}
// }

// static bool renderScrollable(FieldLayout *layout) {
// 	if (!getActiveScrollable()) // we are the first scrollable on this screen, use us to init the stack
// 		enterScrollable(layout->field);

// 	// If we are being asked to render the root scrollable, instead we want to substitute the deepest scrollable
// 	// in the stack
// 	Field *field = layout->field;
// 	if (scrollableStack[0] == field)
// 		field = getActiveScrollable();

// 	return renderActiveScrollable(layout, field);
// }

// Set to true if we should automatically convert kph -> mph and km -> mi
bool screenConvertMiles = false;

// Set to true if we should automatically convert C -> F
bool screenConvertFarenheit = false;

// Set to true if we should automatically convert kg -> lb
bool screenConvertPounds = false;

// // Get the numeric value of an editable number, properly handling different possible byte encodings
// // if withConversion, convert from SI units if necessary
// static int32_t getEditableNumber(Field *field, bool withConversion) {
//   assert(field->variant == FieldEditable);

//   int32_t num;
//   switch (field->editable.size) {
//   case 1:
//     num = *(uint8_t*) field->editable.target;
//     break;
//   case 2:
//     num = *(int16_t*) field->editable.target;
//     break;
//   case 4:
//     num = *(int32_t*) field->editable.target;
//     break;
//   default:
//     assert(0);
//     return 0;
//   }

//   if (withConversion) {
//     const char *units = field->editable.number.units;
//     if (screenConvertMiles
//         && (strcasecmp(units, "kph") == 0
//             || strcasecmp(units, "km") == 0))
//       num = (num * 100) / 161; // div by 1.609 for km->mi

//     if (screenConvertFarenheit && strcmp(units, "C") == 0)
//       num = 32 + (num * 9) / 5;

//     if (screenConvertPounds && strcmp(units, "kg") == 0)
//       num = (num * 220) / 100;
//   }

//   return num;
// }

// // Convert from SI to Imperial
// int32_t convertUnits(int32_t val, ConvertUnitsType type) {

//   switch (type) {
//     case ConvertToImperial_speed:
//       val = (val * 100) / 161; // div by 1.609 for km->mi
//       break;

//     case ConvertFromImperial_speed:
//       val = (val * 161) / 100; // mul by 1.609 for km->mi
//       break;

//     case ConvertToImperial_temperature:
//       val = 32 + (val * 9) / 5;
//       break;

//     case ConvertFromImperial_temperature:
//       val = ((val * 100) / 180) - 32;
//       break;

//     case ConvertToImperial_mass:
//       val = (val * 220) / 100;
//       break;

//     case ConvertFromImperial_mass:
//       val = (val * 100) / 220;
//       break;
//   }

//   return val;
// }

// int32_t convertToImperialIfNeeded(Field *field, int32_t num) {

//   const char *units = field->editable.number.units;
//   if (screenConvertMiles
//       && (strcasecmp(units, "kph") == 0
//           || strcasecmp(units, "km") == 0))
//     num = convertUnits(num, ConvertToImperial_speed);

//   if (screenConvertFarenheit && strcmp(units, "C") == 0)
//     num = convertUnits(num, ConvertToImperial_temperature);

//   if (screenConvertPounds && strcmp(units, "kg") == 0)
//     num = convertUnits(num, ConvertToImperial_mass);

//   return num;
// }

// int32_t convertFromImperialIfNeeded(Field *field, int32_t num) {

//   const char *units = field->editable.number.units;
//   if (screenConvertMiles
//       && (strcasecmp(units, "kph") == 0
//           || strcasecmp(units, "km") == 0))
//     num = convertUnits(num, ConvertFromImperial_speed);

//   if (screenConvertFarenheit && strcmp(units, "C") == 0)
//     num = convertUnits(num, ConvertFromImperial_temperature);

//   if (screenConvertPounds && strcmp(units, "kg") == 0)
//     num = convertUnits(num, ConvertFromImperial_mass);

//   return num;
// }

// // Set the numeric value of an editable number, properly handling different possible byte encodings
// static void setEditableNumber(Field *field, uint32_t v, bool withConversion) {
// 	if (withConversion) {
// 		const char *units = field->editable.number.units;
// 		if (screenConvertMiles
// 				&& (strcasecmp(units, "kph") == 0
// 						|| strcasecmp(units, "km") == 0))
//       v = convertUnits(v, ConvertFromImperial_speed);

// 		if (screenConvertFarenheit && strcmp(units, "C") == 0)
// 		  v = convertUnits(v, ConvertFromImperial_temperature);

//     if (screenConvertPounds && strcmp(units, "kg") == 0)
//       v = convertUnits(v, ConvertFromImperial_mass);
// 	}

// 	switch (field->editable.size) {
// 	case 1:
// 		*(uint8_t*) field->editable.target = (uint8_t) v;
// 		break;
// 	case 2:
// 		*(uint16_t*) field->editable.target = (uint16_t) v;
// 		break;
// 	case 4:
// 		*(uint32_t*) field->editable.target = (uint32_t) v;
// 		break;
// 	default:
// 		assert(0);
// 	}
// }

// static int countEnumOptions(Field *s) {
// 	const char **e = s->editable.editEnum.options;

// 	int n = 0;
// 	while (*e++)
// 		n++;

// 	return n;
// }

// /**
//  * We only convert to displayed units once, when the user starts editing, so that when the user increments/decrements they see the value change
//  * by the expected amount.  We convert back into SI units when the user stops editing.
//  */
// int32_t curEditableValueConverted;

// /**
//  * increment/decrement an editable
//  */
// static void changeEditable(bool increment, bool x10) {
// 	Field *f = curActiveEditable;
// 	assert(f);

// 	int v = curEditableValueConverted;

// 	switch (f->editable.typ) {
// 	case EditUInt: {
// 		int step = f->editable.number.inc_step;

// 		if (step == 0)
// 			step = 1;

// 		if (x10)
// 		  step *= 10;

// 		v += step * (increment ? 1 : -1);
// 		if (v < f->editable.number.min_value) // loop around
// 			v = f->editable.number.max_value;
// 		else if (v > f->editable.number.max_value)
// 			v = f->editable.number.min_value;
// 		break;
// 	}
// 	case EditEnum: {
// 		int numOpts = countEnumOptions(f);
// 		v += increment ? 1 : -1;
// 		if (v < 0) // loop around
// 			v = numOpts - 1;
// 		else if (v >= numOpts)
// 			v = 0;
// 		break;
// 	}
// 	default:
// 		assert(0);
// 		break;
// 	}

// 	curEditableValueConverted = v;
// }

// /// Return a human readable name for the units of this field (converting from SI if necessary)
// static const char* getUnits(Field *field) {
// 	const char *units = field->editable.number.units;

// 	if (screenConvertMiles) {
// 		if (strcasecmp(units, "kph") == 0)
// 			return "mph";

// 		if (strcasecmp(units, "km") == 0)
// 			return "mi";
// 	}

// 	if (screenConvertFarenheit) {
// 		if (strcmp(units, "C") == 0)
// 			return "F";
// 	}

//   if (screenConvertPounds) {
//     if (strcmp(units, "kg") == 0)
//       return "lb";
//   }

// 	return units;
// }

// /// Given an editible extract its value as a string (max len MAX_FIELD_LEN)
// static void getEditableString(Field *field, int32_t num, char *outbuf) {
// 	switch (field->editable.typ) {
// 	case ReadOnlyStr:
// 		// NOTE: We ignore the passed in number (it will be garbage anyways) and instead just return the string
// 		strncpy(outbuf, field->editable.target, MAX_FIELD_LEN);
// 		break;

// 	case EditUInt: {
// 		// properly handle div_digits
// 		int divd = field->editable.number.div_digits;
// 		if (divd == 0)
// 			snprintf(outbuf, MAX_FIELD_LEN, "%lu", num);
// 		else {
// 			int div = 1;
// 			while (divd--)
// 				div *= 10; // pwrs of 10

// 			if (field->editable.number.hide_fraction)
// 				snprintf(outbuf, MAX_FIELD_LEN, "%ld", num / div);
// 			else
// 				snprintf(outbuf, MAX_FIELD_LEN, "%ld.%0*lu", num / div,
// 						field->editable.number.div_digits, num % div);
// 		}
// 		break;
// 	}
// 	case EditEnum:
// 		strncpy(outbuf, field->editable.editEnum.options[num], MAX_FIELD_LEN);
// 		break;
// 	default:
// 		assert(0);
// 		break;
// 	}
// }

// // Sometimes we want to know where we just draw a string, so I have this FIXME ugly hack here
// static int renderedStrX, renderedStrY;

// // Center justify a string on a line of specified width
// static void putStringCentered(int x, int y, int width, const UG_FONT *font,
// 		const char *str) {
//   int maxchars = strlen(str);

//     // Note: we don't need char_h_space for the last char in the string, because the printing won't be adding that pad space
// 	UG_S16 strwidth = (font->char_width + gui.char_h_space) * maxchars - gui.char_h_space;

// 	if (strwidth > width) { // if string is too long, trim it to fit (to prevent wrapping to next row)
// 	  maxchars = width / (font->char_width + gui.char_h_space);

// 	  assert(maxchars > 0);
// 	}

// 	if (strwidth < width)
// 		x += (width - strwidth) / 2; // if we have extra space put half of it before the string

// 	UG_FontSelect(font);
// 	UG_PutString_with_length(x, y, (char*) str, maxchars);
// 	renderedStrX = x;
// 	renderedStrY = y;
// }

// // right justify a string (printing it to the left of X and Y)
// static void putStringRight(int x, int y, const UG_FONT *font, const char *str) {
// 	UG_S16 strwidth = (font->char_width + gui.char_h_space) * strlen(str);

// 	x -= strwidth;

// 	UG_FontSelect(font);
// 	UG_PutString(x, y, (char*) str);
// 	renderedStrX = x;
// 	renderedStrY = y;
// }

// static void putStringLeft(int x, int y, const UG_FONT *font, const char *str) {
// 	UG_FontSelect(font);
// 	UG_PutString(x, y, (char*) str);
// 	renderedStrX = x;
// 	renderedStrY = y;
// }

// /**
//  * Draw a string in a field, respecting font, alignment and optional insets.
//  *
//  * Insets will be from the left/top if using align left/top/center, otherwise they will be from the right.
//  */
// static void putAligned(FieldLayout *layout, AlignmentX alignx,
// 		AlignmentY aligny, int insetx, int insety, const UG_FONT *font,
// 		const char *str) {
// 	assert(font); // dynamic font selection not yet supported

// 	// First find the y position
// 	int y = layout->y;
// 	switch (aligny) {
// 	case AlignTop:
// 		y += insety;
// 		break;
// 	case AlignBottom:
// 		y += layout->height - (insety + font->char_height);
// 		break;
// 	case AlignCenter:
// 		y += insety + layout->height / 2 - font->char_height / 2;
// 		break;
// 	default:
// 		assert(0);
// 	}

// 	// Now print the string at the proper x positon
// 	switch (alignx) {
// 	case AlignHidden:
// 		return; // Don't draw at all
// 	case AlignLeft:
// 		putStringLeft(layout->x + insetx, y, font, str);
// 		break;
// 	case AlignRight:
// 		putStringRight(layout->x + layout->width - insetx, y, font, str);
// 		break;
// 	case AlignCenter:
// 		putStringCentered(layout->x + insetx, y, layout->width, font, str);
// 		break;
// 	default:
// 		assert(0);
// 	}
// }

// /// Update this readonly editable with a string value.  Important: the original field target must be pointing to a WRITABLE array, not a const string.
// void updateReadOnlyStr(Field *field, const char *str) {
// 	assert(field->editable.typ == ReadOnlyStr); // Make sure the field is being used as provisioned
// 	strcpy(field->editable.target, str);
// 	field->rw->dirty = true;
// }

// void updateReadOnlyLabelStr(Field *field, const char *str) {
//   strcpy(field->editable.label, str);
//   field->rw->dirty = true;
// }

// UG_COLOR getEditableColor(Field *f, int32_t val) {
//   int32_t warn_threshold = f->rw->editable.number.warn_threshold;
//   int32_t error_threshold = f->rw->editable.number.error_threshold;
//   UG_COLOR color = C_WHITE;

//   // in the case of invalid thresholds
//   if (warn_threshold == -1 || error_threshold == -1) {
//     return C_WHITE;
//   } else {
//     warn_threshold = convertToImperialIfNeeded(f, warn_threshold);
//     error_threshold = convertToImperialIfNeeded(f, error_threshold);
//   }

//   // find if thresholds are inverted, like in the case of battery voltage
//   bool threshold_inverted = false;
//   if (error_threshold < warn_threshold) {
//     threshold_inverted = true;
//     int temp = error_threshold;
//     error_threshold = warn_threshold;
//     warn_threshold = temp;
//   }

//   bool threshold_invalid = true;
//   if (warn_threshold != -1 ||
//       error_threshold != -1 ||
//       f->rw->editable.number.auto_thresholds != FIELD_THRESHOLD_DISABLED)
//     threshold_invalid = false;

//   int threshold_delta = (error_threshold - warn_threshold) / 2;

//   if (threshold_inverted == false) {
//     // white zone
//     if (val < (warn_threshold - threshold_delta) ||
//         threshold_invalid != 0) {
//       color = C_WHITE;
//     // transition zone from white to yellow
//     } else if (val >= (warn_threshold - threshold_delta) &&
//         val < (warn_threshold)) {
//       // calculate color, linear transition from blue to yellow (RGB565)
//       int color_blue = map(val, // our actual input
//                       warn_threshold - threshold_delta , // start at 0
//                       warn_threshold, // max transition value
//                       0x1f, // min output value
//                       0); // max output value: 5 bits for blue color

//       color = color_blue |
//           0x3f << 5 | // 6 green bits
//           0x1f << 11; // 5 red bits

//     // yellow zone
//     } else if (val >= (warn_threshold) &&
//                val < (error_threshold - threshold_delta)) {
//       color = GRAPH_COLOR_WARN;
//     // transition zone from yellow to red
//     } else if (val >= (error_threshold - threshold_delta) &&
//               val < (error_threshold)) {
//       // calculate color, linear transition from blue to yellow (RGB565)
//       color = map(val, // our actual input
//                      error_threshold - threshold_delta, // start at 0
//                      error_threshold, // max transition value
//                      0x3f, // min output value: 6 bits for green color
//                      0); // max output value

//       color = (color << 5) | // 6 green bits
//          (0x1f << 11); // 5 red bits all enable
//       // red zone
//     } else if (val >= (error_threshold)) {
//       color = GRAPH_COLOR_ERROR;
//     }
//   } else {
//     // red zone
//     if (val <= (warn_threshold)) {
//       color = GRAPH_COLOR_ERROR;

//     // transition zone from red to yellow
//     } else if (val > warn_threshold &&
//              val <= (warn_threshold + threshold_delta)) {
//       // calculate color, linear transition from blue to yellow (RGB565)
//       color = map(val, // our actual input
//                  warn_threshold,
//                  warn_threshold + threshold_delta,
//                  0,
//                  0x3f);

//       color = (color << 5) | // 6 green bits
//           (0x1f << 11); // 5 red bits all enable

//     // yellow zone
//     } else if (val > (warn_threshold + threshold_delta) &&
//                val <= error_threshold) {
//       color = GRAPH_COLOR_WARN;

//     // transition zone from yellow to white
//     } else if (val > error_threshold &&
//                val <= (error_threshold + threshold_delta)) {
//         // calculate color, linear transition from blue to yellow (RGB565)
//         int color_blue = map(val, // our actual input
//                         error_threshold + threshold_delta , // start at 0
//                         error_threshold, // max transition value
//                         0, // min output value
//                         0x1f); // max output value: 5 bits for blue color

//         color = color_blue |
//             0x3f << 5 | // 6 green bits
//             0x1f << 11; // 5 red bits

//       // white zone
//     } else if (val > (error_threshold + threshold_delta) ||
//           threshold_invalid != 0) {
//       color = C_WHITE;
//     }
//   }

//   return color;
// }

// /**
//  * This render operator is smart enough to do its own dirty managment.  If you set dirty, it will definitely redraw.  Otherwise it will check the actual data bytes
//  * of what we are trying to render and if the same as last time, it will decide to not draw.
//  */
// static bool renderEditable(FieldLayout *layout) {
// 	Field *field = getField(layout);
// 	UG_S16 width = layout->width;
// 	bool isActive = curActiveEditable == field; // are we being edited right now?
// 	bool isCustomizing = g_curCustomizingField
// 			&& g_curCustomizingField == parentCustomizable;
// 	bool dirty = field->rw->dirty;
// 	bool showLabel = layout->label_align_x != AlignHidden;
// 	bool layoutShowUnits = layout->show_units == Show;
// 	bool showLabelAtTop = layout->label_align_y == AlignTop;

// 	// see if we should fill our entire box with blankspace
// 	if (field->rw->visibility == FieldTransitionNotVisible) {
//     UG_FillFrame(layout->x, layout->y, layout->x + width - 1,
//         layout->y + layout->height - 1, getBackColor(layout));

//     field->rw->visibility = FieldNotVisible;
//     return true;
// 	}

// 	const UG_FONT *font = layout->font ? layout->font : editable_value_font;

// 	bool isTwoRows = showLabel && (EDITABLE_NUM_ROWS == 2);

// 	// a rough approximation of the offset for descenders (i.e. the bottom parts of chars like g and j)
// 	// * now that we have inset_y, remove descender_y - I don't think it is needed anymore rows: https://github.com/OpenSource-EBike-firmware/Color_LCD/commit/020b195a4d5ffa3a226aeaed955d634c40b3cf7f#r34886687
// 	// int descender_y = (font->char_height / 8);

// 	if (layout->height == -1) // We should autoset
// 		layout->height = (
// 				(isTwoRows || showLabelAtTop) ?
// 						editable_label_font->char_height : 0)
// 				+ font->char_height;

// 	UG_S16 height = layout->height;

// 	UG_COLOR back = getBackColor(layout), fore = getForeColor(layout);
// 	UG_SetForecolor(fore);

// 	// If we are blinking right now, that's a good place to poll our buttons so that the user can press and hold to change a series of values
// 	if (isActive && blinkChanged && !field->editable.read_only) {
//     static uint8_t cnt_up = 0;
//     static uint8_t cnt_down = 0;

// 		if (buttons_get_up_state()) {
//       if (cnt_up < 255)
//         cnt_up++;

// 			changeEditable(true, (cnt_up > 10) ? true : false);
// 		}
// 		else {
//       cnt_up = 0;
// 		}

// 		if (buttons_get_down_state()) {
//       if (cnt_down < 255)
//         cnt_down++;

// 			changeEditable(false, (cnt_down > 10) ? true : false);
// 		}
//     else {
//       cnt_down = 0;
//     }
// 	}

// 	// Get the value we are trying to show (it might be a num or an enum)
// 	// If we are actively editing, we are careful to show only the cached editable value
// 	int32_t num =
// 			isActive ?
// 					curEditableValueConverted : getEditableNumber(field, true);

// 	// If we are customizing this value, we don't check for changes of the value because that causes glitches in the display with extra
// 	// redraws
// 	bool valueChanged = (num != layout->old_editable && !isCustomizing) ||
// 	    field->rw->visibility == FieldTransitionVisible;

// 	// update right now this state as it is not used later then here
// 	if (field->rw->visibility == FieldTransitionVisible)
// 	  field->rw->visibility = FieldVisible;

// 	char valuestr[MAX_FIELD_LEN];

// 	// Do we need to handle a blink transition right now?
// 	bool needBlink = blinkChanged
// 			&& (isActive || field->rw->is_selected || isCustomizing);

//   // If not dirty, labels didn't change and we aren't animating then exit
//   bool forceLabelsChanged = forceLabels != oldForceLabels;

// 	// If the value numerically changed, see if it also changed as a string (much more expensive)
// 	bool showValue = !forceLabels && (valueChanged || dirty || needBlink || forceLabelsChanged); // default to not drawing the value
// 	if (showValue) {
// 		char oldvaluestr[MAX_FIELD_LEN];
// 		getEditableString(field, layout->old_editable, oldvaluestr);

// 		layout->old_editable = num;

// 		getEditableString(field, num, valuestr);
// 		if ((strlen(valuestr) != strlen(oldvaluestr)) ||
// 		    forceLabelsChanged)
// 			dirty = true; // Force a complete redraw (because alignment of str in field might have changed and we don't want to leave turds on the screen
// 	}

// 	bool thresholds_color = field->rw->editable.number.auto_thresholds != FIELD_THRESHOLD_DISABLED;
// 	// see if color should fade depending on thresholds
// 	UG_COLOR color;
// 	if (thresholds_color)
//     // calculate color, linear transition from white to yellow or red (RGB565)
//     color = getEditableColor(field, num);
// 	else
// 	  color = C_WHITE;

//   // if color changed, then make dirty
//   if (color != field->rw->editable.number.previous_color) {
//       field->rw->editable.number.previous_color = color;
//     dirty = true;
//   }

// 	if (!dirty && !valueChanged && !forceLabelsChanged && !needBlink)
// 		return false; // We didn't actually change so don't try to draw anything

// 	// fill our entire box with blankspace (if we must)
// 	bool blankAll = EDITABLE_BLANKALL || forceLabelsChanged || dirty
// 			|| (isCustomizing && needBlink);
// 	if (blankAll)
// 		UG_FillFrame(layout->x, layout->y, layout->x + width - 1,
// 				layout->y + height - 1, back);

// 	UG_SetBackcolor(blankAll ? C_TRANSPARENT : C_BLACK); // we just cleared the background ourself, from now on allow fonts to overlap
// 	UG_SetForecolor(fore);

// 	// If the user is trying to customize a field, we blink the field contents.
// 	// If this field is already showing a label it looks better to blink by alternating the field contents with blackspace
// 	// If this field is not showing a label we need to instead alternate field name with blackspace
// 	// by forcing showOnlyLabel
// 	bool showOnlyLabel = forceLabels; // the common case, just cares about our global

// 	if (isCustomizing && needBlink) {
// 		if (!showLabel)  // the label is hidden for this field, so we alternate between the label name and blackspace
// 			showOnlyLabel = true;

// 		if (!blinkOn)
// 			return true; // Just show black background this time
// 	}

// 	// Show the label in the middle of the box (and nothing else)
// 	// We show this if the user has pressed the key to see all of the field names (useful on tiny screen devices)
// 	// or the user is currently customizing a field - in which case we blink alternating the field name and the contents
// 	if (showOnlyLabel) {
// 		putStringCentered(layout->x,
// 				layout->y + (height - editable_label_font->char_height) / 2,
// 				width, editable_label_font, field->editable.label);

// 		return true;
// 	}

// 	// Show the label (if showing the conventional way - i.e. small and off to the top left.
// 	if (showLabel) {
// 		UG_SetBackcolor(C_TRANSPARENT); // always draw labels with transparency, because they might slightly overlap the border
// 		UG_SetForecolor(LABEL_COLOR);

// 		int label_inset_x = 0, label_inset_y = 0; // Move to be a public constant or even a LayoutField member if useful

// 		putAligned(layout, layout->label_align_x, layout->label_align_y,
// 				label_inset_x, label_inset_y, editable_label_font,
// 				field->editable.label);
// 	}

// 	UG_SetBackcolor(blankAll ? C_TRANSPARENT : C_BLACK); // we just cleared the background ourself, from now on allow fonts to overlap
// 	UG_SetForecolor(fore);

// 	// draw editable value
// 	if (showValue) {
// 		UG_FontSelect(font);

// 		int y = layout->inset_y; // used as an inset
// 		int x = layout->inset_x;

// 		AlignmentY align_y = layout->align_y;
// 		// @casainho did you need to customized the vertical alignment of data fields?  If so, I don't think you intended to use unit_align_y
// 		// which was only supposed to be for the units field.  I've added an align_y property you can use instead (it defaults to center)

// 		if (showLabel) {
// 			if (!showLabelAtTop) {
// 				if (isTwoRows) // put the value on the second line (if the screen is narrow)
// 					y += editable_label_font->char_height;
// 			} else {
// 				y += (editable_label_font->char_height); // put value just beneath label
// 				align_y = AlignTop;
// 			}
// 		}

// 	  UG_SetForecolor(color);
// 		putAligned(layout, layout->align_x, align_y, x, y, font, valuestr);

// 		// Blinking underline cursor when editing, just below value and drawing to the right edge of the box
// 		if (isActive) {
// 			UG_S16 cursorY = renderedStrY + font->char_height + 1;
// 			UG_DrawLine(renderedStrX - 1, cursorY, layout->x + width, cursorY,
// 					blinkOn ? EDITABLE_CURSOR_COLOR : back);
// 		}
// 	}

// 	// Put units in bottom right (unless we are showing the label)
// 	bool showUnits = layoutShowUnits && field->editable.typ == EditUInt && !showLabel;
// 	if (showUnits) {
// 		const char *units = getUnits(field);
// 		int ulen = strlen(units);
// 		if (ulen) {
// 			int unit_inset_x = 0, unit_inset_y = 0; // Move to be a public constant or even a LayoutField member if useful

// 			if (layout->unit_align_x == AlignDefault) { // If unspecified (normally should be AlignRight) pick rational defaults
// 				layout->unit_align_x = AlignRight;
// 				layout->unit_align_y = AlignBottom;
// 			}
// 			putAligned(layout, layout->unit_align_x, layout->unit_align_y,
// 					unit_inset_x, unit_inset_y, editable_units_font, units);
// 		}
// 	}

// 	return true;
// }

// static bool renderCustom(FieldLayout *layout) {
// 	assert(layout->field->custom.render);
// 	return (*layout->field->custom.render)(layout);
// }

// static int graphX, // upper left of graph
// 		graphY, // upper left of graph,
// 		graphWidth, // total draw area width
// 		graphHeight, // total draw area height
// 		graphXmin, // x loc of 0,0 position
// 		graphXmax, // x loc of rightmost data point
// 		graphYmin, // y loc of 0,0 position (for min value)
// 		graphYmax, // y loc of max value
// 		graphLabelY; // y loc of the label for field name

// // Clear our box completely if needed
// static void graphClear(Field *field) {
// 	UG_SetForecolor(GRAPH_COLOR_ACCENT);
// 	UG_SetBackcolor(GRAPH_COLOR_BACKGROUND);

// 	if (field->rw->dirty) {
// 		// clear all
// 		UG_FillFrame(graphX, graphY, graphX + graphWidth - 1,
// 				graphY + graphHeight, GRAPH_COLOR_BACKGROUND);
// 	}
// }

// // Draw our axis lines and min/max numbers
// static void graphLabelAxisCustomize(Field *field) {
//   uint8_t x_axis_scale_config = field->rw->graph.x_axis_scale_config;

//   if (blinkOn == false) {
//     putStringRight(SCREEN_WIDTH, graphYmin - 10 - 2, &GRAPH_XAXIS_FONT, "  ");
//   } else {
//     UG_SetForecolor(LABEL_COLOR);
//     UG_SetForecolor(LABEL_COLOR);

//     // x axis scale
//     switch (x_axis_scale_config) {
//       case 0:
//         putStringRight(SCREEN_WIDTH - 1, graphYmin - 10 - 2, &GRAPH_XAXIS_FONT, "15");
//         break;

//       case 1:
//         putStringRight(SCREEN_WIDTH, graphYmin - 10 - 2, &GRAPH_XAXIS_FONT, "1h");
//         break;

//       case 2:
//         putStringRight(SCREEN_WIDTH, graphYmin - 10 - 2, &GRAPH_XAXIS_FONT, "4h");
//         break;

//       case 3:
//       default:
//         putStringRight(SCREEN_WIDTH, graphYmin - 10 - 2, &GRAPH_XAXIS_FONT, "au");
//         break;
//     }
//   }

//   if (g_changeXAxisTrigger) {
//     switch (x_axis_scale_config) {
//       default:
//       case GRAPH_X_AXIS_SCALE_15M:
//       case GRAPH_X_AXIS_SCALE_1H:
//       case GRAPH_X_AXIS_SCALE_4H:
//         field->rw->graph.x_axis_scale = x_axis_scale_config;
//         break;

//       // use reference scale for the auto mode
//       // bits of g_xAxisReferenceScale are set dependind on reference the scale
//       case GRAPH_X_AXIS_SCALE_AUTO:
//         if (g_xAxisReferenceScale & 2)
//           field->rw->graph.x_axis_scale = GRAPH_X_AXIS_SCALE_4H;
//         else if (g_xAxisReferenceScale & 1)
//           field->rw->graph.x_axis_scale = GRAPH_X_AXIS_SCALE_1H;
//         break;
//     }
//   }

//   // draw max value
//   // x axis scale
//   static int32_t max_val_pre = INT32_MAX;
//   static int32_t min_val_pre = INT32_MIN;
//   bool draw_y_label_max = true;
//   bool draw_y_label_min = true;
//   uint8_t x_axis_scale;
//   x_axis_scale = field->rw->graph.x_axis_scale;
//   Field *source = field->graph.source;
//   GraphData *graph = field->rw->graph.data[x_axis_scale];
//   if(!graph) // no data yet
//     return;

//   // the graph would be an horizontal line at bottom, so, put Y axis labels specific if value is 0 or higher
//   if (graph->min_val == graph->max_val) {
//     if (graph->min_val > 0)
//       draw_y_label_min = false;
//     else
//       draw_y_label_max = false;
//   }

//   char valstr[MAX_FIELD_LEN];

//   // draw if value changed or dirty
//   if((graph->max_val != max_val_pre ||
//       blinkChanged ||
//       g_changeXAxisTrigger) &&
//       draw_y_label_max) {
//     max_val_pre = graph->max_val;

//     if (graph->max_val != INT32_MIN) {
//       getEditableString(source, graph->max_val, valstr);
//       putStringRight((GRAPH_MAXVAL_FONT.char_width * 4) + 4,
//                      graphYmax, &GRAPH_MAXVAL_FONT, valstr);
//     }
//   }

//   // draw if value changed or dirty
//   if((graph->min_val != min_val_pre ||
//       blinkChanged ||
//       g_changeXAxisTrigger) &&
//       draw_y_label_min) {
//     min_val_pre = graph->min_val;

//     if (graph->min_val != INT32_MAX) {
//       getEditableString(source, graph->min_val, valstr);
//       putStringRight((GRAPH_MAXVAL_FONT.char_width * 4) + 4,
//                      graphYmin - GRAPH_MAXVAL_FONT.char_height,
//           &GRAPH_MAXVAL_FONT, valstr);
//     }
//   }
// }

// // Draw our axis lines and min/max numbers
// static void graphLabelAxis(Field *field) {
//   static int32_t max_val_pre = INT32_MAX;
//   static int32_t min_val_pre = INT32_MIN;
//   bool draw_y_label_max = true;
//   bool draw_y_label_min = true;

// 	// Only need to draw labels and axis if dirty
// 	Field *source = field->graph.source;
// 	if (field->rw->dirty ||
// 	    g_graphs_ui_update[0] == true ||
//       g_graphs_ui_update[1] == true ||
//       g_graphs_ui_update[2] == true) {
// 		UG_SetForecolor(LABEL_COLOR);
// 		putStringCentered(graphX, graphLabelY, graphWidth, &GRAPH_LABEL_FONT,
// 				source->editable.label);
// 		UG_SetForecolor(LABEL_COLOR);

// 		// vertical axis line
//     UG_DrawLine(graphXmin, graphYmin, graphXmin, graphYmax,
//     GRAPH_COLOR_AXIS);

// 		// horiz axis line
//     UG_DrawLine(graphXmin, graphYmin, graphXmin + GRAPH_MAX_POINTS, graphYmin, C_DIM_GRAY);

// 		// x axis scale
//     uint8_t x_axis_scale;
//     x_axis_scale = field->rw->graph.x_axis_scale;
//     if (g_CustomizingGraphXAxis == NULL) {
//       switch (x_axis_scale) {
//         default:
//         case 0:
//           putStringRight(SCREEN_WIDTH - 1, graphYmin - 10 - 2, &GRAPH_XAXIS_FONT, "15");
//           break;

//         case 1:
//           putStringRight(SCREEN_WIDTH, graphYmin - 10 - 2, &GRAPH_XAXIS_FONT, "1h");
//           break;

//         case 2:
//           putStringRight(SCREEN_WIDTH, graphYmin - 10 - 2, &GRAPH_XAXIS_FONT, "4h");
//           break;
//       }
//     }

//     // draw max value
//     GraphData *graph = field->rw->graph.data[x_axis_scale];
//     if(!graph) // no data yet
//       return;

//     // the graph would be an horizontal line at bottom, so, put Y axis labels specific if value is 0 or higher
//     if (graph->min_val == graph->max_val) {
//       if (graph->min_val > 0)
//         draw_y_label_min = false;
//       else
//         draw_y_label_max = false;
//     }

//     char valstr[MAX_FIELD_LEN];

//     // draw if value changed or dirty
//     if((graph->max_val != max_val_pre ||
//         field->rw->dirty) &&
//         draw_y_label_max) {
//       max_val_pre = graph->max_val;

//       if (graph->max_val != INT32_MIN) {
//         int32_t i32_value = convertToImperialIfNeeded(source, graph->max_val);
//         getEditableString(source, i32_value, valstr);
//         putStringRight((GRAPH_MAXVAL_FONT.char_width * 4) + 4,
//                        graphYmax, &GRAPH_MAXVAL_FONT, valstr);
//       }
//     }

//     // draw if value changed or dirty
//     if((graph->min_val != min_val_pre ||
//         field->rw->dirty) &&
//         draw_y_label_min) {
//       min_val_pre = graph->min_val;

//       if (graph->min_val != INT32_MAX) {
//         int32_t i32_value = convertToImperialIfNeeded(source, graph->min_val);
//         getEditableString(source, i32_value, valstr);
//         putStringRight((GRAPH_MAXVAL_FONT.char_width * 4) + 4,
//                        graphYmin - GRAPH_MAXVAL_FONT.char_height,
//             &GRAPH_MAXVAL_FONT, valstr);
//       }
//     }
// 	}
// }

// // Linear  interpolated between the min/max values to generate a y coordinate for plotting a particular value x
// static inline int32_t graphScaleY(GraphData *graph, int32_t x) {
// 	return map(x,
// 	    graph->min_val,
// 	    graph->max_val,
// 	    0,
// 	    graphYmin - graphYmax);
// }

// static void graphDrawPoints(Field *field) {
//   static bool end_valid_overflow = 0;
//   uint8_t x_axis_scale = field->rw->graph.x_axis_scale;
// 	GraphData *graph = field->rw->graph.data[x_axis_scale];
//   Field *source = field->graph.source;
// 	if(!graph)
// 		return; // no data yet

// 	int ptr = graph->start_valid;
// 	if (ptr == graph->end_valid)
// 		return; // ring buffer is empty

// 	uint32_t end_valid = graph->end_valid;
// 	if (end_valid == 0)
// 	  end_valid_overflow = true;

// 	if (end_valid_overflow)
// 	  end_valid = GRAPH_MAX_POINTS;

//   // first, erase the full points draw area
//   UG_FillFrame(graphXmin + 1, graphYmin - 1, graphXmin + GRAPH_MAX_POINTS,
//                graphYmax, GRAPH_COLOR_BACKGROUND);

// 	int x = graphXmin; // the vertical axis line

// 	// user may had used imperial units
// 	int warn_threshold = convertToImperialIfNeeded(source, field->graph.source->rw->editable.number.warn_threshold);
// 	int error_threshold = convertToImperialIfNeeded(source, field->graph.source->rw->editable.number.error_threshold);

// 	// find if thresholds are inverted, like in the case of battery voltage
// 	bool threshold_inverted = false;
// 	if (error_threshold < warn_threshold) {
//     threshold_inverted = true;
//     int temp = error_threshold;
//     error_threshold = warn_threshold;
//     warn_threshold = temp;
// 	}

// 	bool threshold_invalid = true;
// 	if (warn_threshold != -1 ||
// 	    error_threshold != -1 ||
// 	    field->graph.source->rw->editable.number.auto_thresholds != FIELD_THRESHOLD_DISABLED)
// 	  threshold_invalid = false;

// 	int threshold_delta = (error_threshold - warn_threshold) / 2;

// 	static int y_previous;
// 	do {
// 		x++; // drawing a new vertical line now
// 		int val = graph->points[ptr];
// 		int y = graphScaleY(graph, val);

//     // the graph would be an horizontal line at bottom, but so force line to be max value
//     if (graph->min_val == graph->max_val &&
//         graph->min_val > 0 &&
//         (graph->end_valid > 2)) // ignore very first value as graph->min_val == graph->max_val would always be true
//       y = graphYmin - graphYmax;

// 		int y_contour;
// 		int y_line;
// 		int y_temp;

//     // force first line to not be full white
//     if(x == (graphXmin + 1)) {
//       y_previous = y;
//     }

//     // calculate contour
//     y_temp = y;
//     if(y >= y_previous) {
//       y_contour = y - y_previous;
//       y_line = y_previous;
//     } else {
//       y_contour = y_previous - y;
//       y_line = y;
//     }
//     y_previous = y_temp;

//     if (threshold_inverted == false) {
//       // blue zone
//       if (val < (warn_threshold - threshold_delta) ||
//           graph->min_val == graph->max_val ||
//           threshold_invalid != 0) {
//         UG_DrawLine(x, graphYmin, x, graphYmin - y_line, GRAPH_COLOR_NORMAL);
//         UG_DrawLine(x, graphYmin - y_line, x, graphYmin - y_line - y_contour, GRAPH_COLOR_ACCENT);
//       // transition zone from blue to yellow
//       } else if (val >= (warn_threshold - threshold_delta) &&
//           val < (warn_threshold)) {
//         // calculate color, linear transition from blue to yellow (RGB565)
//         int color_yellow = map(val, // our actual input
//                         warn_threshold - threshold_delta, // start at 0
//                         warn_threshold, // max transition value
//                         0, // min output value
//                         0x3f); // max output value: 6 bits for green color

//         int color_blue = map(val, // our actual input
//                         warn_threshold - threshold_delta , // start at 0
//                         warn_threshold, // max transition value
//                         0x1f, // min output value
//                         0); // max output value: 5 bits for blue color

//         int color = color_blue |
//             (color_yellow << 5) | // 6 green bits
//             ((color_yellow / 2) << 11); // 5 red bits

//         UG_DrawLine(x, graphYmin, x, graphYmin - y_line, color);
//         UG_DrawLine(x, graphYmin - y_line, x, graphYmin - y_line - y_contour, GRAPH_COLOR_ACCENT);
//       // yellow zone
//       } else if (val >= (warn_threshold) &&
//                  val < (error_threshold - threshold_delta)) {
//         UG_DrawLine(x, graphYmin, x, graphYmin - y_line, GRAPH_COLOR_WARN);
//         UG_DrawLine(x, graphYmin - y_line, x, graphYmin - y_line - y_contour, GRAPH_COLOR_ACCENT);
//       // transition zone from yellow to red
//       } else if (val >= (error_threshold - threshold_delta) &&
//                 val < (error_threshold)) {
//         // calculate color, linear transition from blue to yellow (RGB565)
//         int color = map(val, // our actual input
//                        error_threshold - threshold_delta, // start at 0
//                        error_threshold, // max transition value
//                        0x3f, // min output value: 6 bits for green color
//                        0); // max output value

//         color = (color << 5) | // 6 green bits
//            (0x1f << 11); // 5 red bits all enable

//         UG_DrawLine(x, graphYmin, x, graphYmin - y_line, color);
//         UG_DrawLine(x, graphYmin - y_line, x, graphYmin - y_line - y_contour, GRAPH_COLOR_ACCENT);
//         // red zone
//       } else if (val >= (error_threshold)) {
//         UG_DrawLine(x, graphYmin, x, graphYmin - y_line, GRAPH_COLOR_ERROR);
//         UG_DrawLine(x, graphYmin - y_line, x, graphYmin - y_line - y_contour, GRAPH_COLOR_ACCENT);
//       }
//     } else {
//       // red zone
//       if (val <= (warn_threshold)) {
//         UG_DrawLine(x, graphYmin, x, graphYmin - y_line, GRAPH_COLOR_ERROR);
//         UG_DrawLine(x, graphYmin - y_line, x, graphYmin - y_line - y_contour, GRAPH_COLOR_ACCENT);

//       // transition zone from red to yellow
//       } else if (val > warn_threshold &&
//                val <= (warn_threshold + threshold_delta)) {
//       // calculate color, linear transition from blue to yellow (RGB565)
//       int color = map(val, // our actual input
//                      warn_threshold,
//                      warn_threshold + threshold_delta,
//                      0,
//                      0x3f);

//       color = (color << 5) | // 6 green bits
//          (0x1f << 11); // 5 red bits all enable

//       UG_DrawLine(x, graphYmin, x, graphYmin - y_line, color);
//       UG_DrawLine(x, graphYmin - y_line, x, graphYmin - y_line - y_contour, GRAPH_COLOR_ACCENT);

//       // yellow zone
//       } else if (val > (warn_threshold + threshold_delta) &&
//                  val <= error_threshold) {
//         UG_DrawLine(x, graphYmin, x, graphYmin - y_line, GRAPH_COLOR_WARN);
//         UG_DrawLine(x, graphYmin - y_line, x, graphYmin - y_line - y_contour, GRAPH_COLOR_ACCENT);

//       // transition zone from yellow to blue
//       } else if (val > error_threshold &&
//                  val <= (error_threshold + threshold_delta)) {
//         // calculate color, linear transition from blue to yellow (RGB565)
//         int color_yellow = map(val, // our actual input
//                         error_threshold,
//                         error_threshold + threshold_delta,
//                         0x3f, // min output value
//                         0); // max output value: 6 bits for green color

//         int color_blue = map(val, // our actual input
//                         error_threshold,
//                         error_threshold + threshold_delta,
//                         0, // min output value
//                         0x1f); // max output value: 5 bits for blue color

//         int color = color_blue |
//             (color_yellow << 5) | // 6 green bits
//             ((color_yellow / 2) << 11); // 5 red bits

//         UG_DrawLine(x, graphYmin, x, graphYmin - y_line, color);
//         UG_DrawLine(x, graphYmin - y_line, x, graphYmin - y_line - y_contour, GRAPH_COLOR_ACCENT);

//         // blue zone
//       } else if (val > (error_threshold + threshold_delta) ||
//             graph->min_val == graph->max_val ||
//             threshold_invalid != 0) {
//         UG_DrawLine(x, graphYmin, x, graphYmin - y_line, GRAPH_COLOR_NORMAL);
//         UG_DrawLine(x, graphYmin - y_line, x, graphYmin - y_line - y_contour, GRAPH_COLOR_ACCENT);
//       }
//     }

//     ptr = (ptr + 1) % GRAPH_MAX_POINTS; // increment and wrap
// 	} while (ptr != graph->end_valid); // we just did the last entry?
// }

// /**
//  * Our graphs are invoked for rendering once each blink interval, but most of the time we opt to do nothing.
//  */
// static bool renderGraph(FieldLayout *layout) {
// 	Field *field = getField(layout);

//   if (g_CustomizingGraphXAxis)
//     graphLabelAxisCustomize(field);

// 	bool isCustomizing = g_curCustomizingField
// 			&& g_curCustomizingField == parentCustomizable;
// 	bool needBlink = (blinkChanged && isCustomizing) || g_changeXAxisTrigger;

// 	if (needBlink)
// 		field->rw->dirty = true; // Force a complete redraw if blink changed

//   // If we are not dirty and we don't need an update, just return
// 	if ((!(g_graphs_ui_update[0] == true ||
// 	    g_graphs_ui_update[1] == true ||
// 	    g_graphs_ui_update[2] == true)) &&
// 	        !field->rw->dirty)
// 		return false;

// 	Field *source = field->graph.source;
// 	assert(source);

// 	// Set axis coordinates
// 	int axisdigits = 4;
// 	int axiswidth = axisdigits
// 			* (GRAPH_MAXVAL_FONT.char_width + gui.char_h_space);
// 	graphX = layout->x; // upper left of graph
// 	graphY = layout->y; // upper left of graph,
// 	graphWidth = layout->width; // total draw area width
// 	graphHeight = layout->height; // total draw area height
// 	graphXmin = graphX + 1 + axiswidth; // x loc of 0,0 position
// 	graphXmax = graphX + graphWidth - 1; // x loc of rightmost data point
// 	graphYmin = graphY + graphHeight - 1; // y loc of 0,0 position (for min value)
// 	graphYmax = graphY + GRAPH_LABEL_FONT.char_height + GRAPH_GRAPH_LABEL_OFFSET; // y loc of max value
// 	graphLabelY = graphY; // y loc of the label for field name

// 	// limit max x based on the number of points we might have (so each point gets its own column
// 	if (graphXmin + GRAPH_MAX_POINTS < graphXmax)
// 		graphXmax = graphXmin + GRAPH_MAX_POINTS;

// 	// clear only if needed
// 	graphClear(field);

// 	if(needBlink && !blinkOn)
// 		return true; // If we are supposed to be blinking return before we actually draw the graph contents

// 	graphLabelAxis(field);
// 	graphDrawPoints(field);

// 	g_changeXAxisTrigger = false;
//   g_graphs_ui_update[0] = false;
//   g_graphs_ui_update[1] = false;
//   g_graphs_ui_update[2] = false;

// 	return true;
// }

// static bool renderCustomizable(FieldLayout *layout) {
// 	// Delegate rendering to the field which is actually selected
// 	Field *field = getField(layout);

// 	return renderField(layout, field);
// }

// static bool renderEnd(FieldLayout *layout) {
// 	assert(0); // This should never be called I think
// 	return true;
// }

// static const FieldRenderFn renderers[] = { renderDrawTextRW, renderDrawTextRO,
// 		renderFill, renderMesh, renderScrollable, renderEditable, renderCustom,
// 		renderGraph, renderCustomizable, renderEnd };

// // If we are showing a scrollable redraw it
// static void forceScrollableRender() {
// 	Field *active = getActiveScrollable();
// 	if (active) {
// 		scrollableStack[0]->rw->dirty = true; // the gui thread only looks in the root scrollable to find dirty
// 		forceScrollableRelayout = true;
// 	}
// }

// // Mark a new editable as active (and that it now wants to be animated)
// static void setActiveEditable(Field *clicked) {
// 	if (curActiveEditable) {
// 		curActiveEditable->rw->blink = false;

// 		// callback onSetEditable
// 	  void (*onSetEditable)(uint32_t) = curActiveEditable->editable.number.onSetEditable;
// 		if (onSetEditable)
// 		  onSetEditable(curEditableValueConverted);

// 		// Save any changed value
// 		setEditableNumber(curActiveEditable, curEditableValueConverted, true);
// 	}

// 	curActiveEditable = clicked;

// 	if (clicked) {
// 		clicked->rw->dirty = true; // force redraw with highlighting
// 		clicked->rw->blink = true;

// 		// get initial value for editing
// 		curEditableValueConverted = getEditableNumber(clicked, true);
// 	}

// 	forceScrollableRender(); // FIXME, I'm not sure if this is really required
// }

// // Returns true if we've handled the event (and therefore it should be cleared)
// static bool onPressEditable(buttons_events_t events) {
// 	bool handled = false;
// 	Field *s = curActiveEditable;

// 	if (events & UP_CLICK) {
// 		// Note: we mark that we've handled this 'event' (so that other subsystems don't think they should) but really, we have already
// 		// been calling changeEditable in our render function, where we check only on blinkChanged, so that users can press and hold to
// 		// change values.
// 		handled = true;
// 	}

// 	if (events & DOWN_CLICK) {
// 		handled = true;
// 	}

// // Mark that we are no longer editing - click pwr button to exit
// 	if (events & SCREENCLICK_STOP_EDIT) {
// 		setActiveEditable(NULL);

// 		handled = true;
// 	}

// 	if (handled) {
// 		s->rw->dirty = true; // redraw our position

// 		// If we are inside a scrollable, tell the GUI that scrollable also needs to be redrawn
// 		Field *scrollable = getActiveScrollable();
// 		if (scrollable) {
// 			scrollableStack[0]->rw->dirty = true; // we just changed something, make sure we get a chance to be redrawn
// 		}
// 	}

// 	return handled;
// }

// int countEntries(Field *s) {
// 	Field *e = s->scrollable.entries;

// 	int n = 0;
// 	while (e && e->variant != FieldEnd) {
// 		n++;
// 		e++;
// 	}

// 	return n;
// }

// // Returns true if we've handled the event (and therefore it should be cleared)
// // if first or selected changed, mark our scrollable as dirty (so child editables can be drawn)
// static bool onPressScrollable(buttons_events_t events) {
// 	bool handled = false;
// 	Field *s = getActiveScrollable();

// 	if (!s)
// 		return false; // no scrollable is active

// 	Field *curActive = &s->scrollable.entries[s->rw->scrollable.selected];

//   if (events & (UP_CLICK | DOWN_CLICK)) {
//     // Before we move away, mark the current item as dirty, so it will be redrawn (prevent leaving blinking arrow turds on the screen)
//     curActive->rw->dirty = true;

//     // Go to previous
//     if(events & UP_CLICK) {
//       if (s->rw->scrollable.selected >= 1) {
//         s->rw->scrollable.selected--;
//       }

//       if (s->rw->scrollable.selected < s->rw->scrollable.first) // we need to scroll the whole list up some
//         s->rw->scrollable.first = s->rw->scrollable.selected;
//     }

//     // Go to next
//     if (events & DOWN_CLICK) {
//       int numEntries = countEntries(s);

//       if (s->rw->scrollable.selected < numEntries - 1) {
//         s->rw->scrollable.selected++;
//       }

//       int numDataRows = maxRowsPerScreen - 1;
//       int lastVisibleRow = s->rw->scrollable.first + numDataRows - 1;
//       if (s->rw->scrollable.selected > lastVisibleRow) // we need to scroll the whole list down some
//         s->rw->scrollable.first = s->rw->scrollable.selected - numDataRows + 1;
//     }

//     forceScrollableRender();
//     handled = true;
//   }

// // If we aren't already editing anything, start now (note: we will only be called if some active editable
// // hasn't already handled this button
// 	if (events & SCREENCLICK_START_EDIT && !curActiveEditable) {
// 		switch (curActive->variant) {
// 		case FieldEditable:
// 			if (!curActive->editable.read_only) { // only start editing non read only fields
// 				setActiveEditable(curActive);
// 				handled = true;
// 			}
// 			break;

// 		case FieldScrollable:
// 			enterScrollable(curActive);
// 			handled = true;
// 			break;

// 		default:
// 			break;
// 		}
// 	}

// // click power button to exit out of menus
// 	if (!handled && (events & SCREENCLICK_EXIT_SCROLLABLE)) {
// 		handled = exitScrollable(); // if we were top scrollable don't claim we handled this press (let rest of app do it)
// 	}

// 	return handled;
// }

// /**
//  * For the current screen.  Select the next customizable field on the screen (or nothing if there are not suitable
//  * fields).  If there isn't a current customizable field, select the first candidate.
//  */
// #define CUSTOMIZABLE_FIELDS_MAX_NUMBER 5

// static void selectNextCustomizableField(bool increase) {
// 	Field *customizableFields[CUSTOMIZABLE_FIELDS_MAX_NUMBER];
// 	uint8_t customizableFieldsCounter = 0;
// 	static uint8_t customizableFieldIndex;

//   // put all pointers on array at NULL
//   memset(&customizableFields, 0, sizeof(customizableFields));

//   FieldLayout *layout = curScreen->fields;
//   while (layout->field) {
//     Field *field = layout->field;

//     // let´s find customizable fields only
//     if (field->variant == FieldCustomizable &&
//         customizableFieldsCounter < CUSTOMIZABLE_FIELDS_MAX_NUMBER) {
//       customizableFields[customizableFieldsCounter++] = field;
//     }

//     layout++;
//   }

// 	// increment with wrap
// 	if (increase) {
//     // Force the field we are leaving to get redrawn (to not leave turds around)
//     if (g_curCustomizingField) {
//       g_curCustomizingField->customizable.choices[*g_curCustomizingField->customizable.selector]->rw->dirty = true;
//     }

//     customizableFieldIndex = (customizableFieldIndex + 1) % customizableFieldsCounter;
// 	}

// 	g_curCustomizingField = customizableFields[customizableFieldIndex];
// }

// /**
//  * For the currently customizing field, advance the target to the next possible choice for the sort of data to show.
//  */
// static void changeCurrentCustomizableField(uint8_t ui8_direction) {
// 	Field *s = g_curCustomizingField;
// 	assert(s && s->variant == FieldCustomizable);
// 	uint8_t i = *s->customizable.selector;
// 	uint8_t i_max;

// 	// find number of customized fields
//   for (i_max = 0; s->customizable.choices[i_max] != 0; i_max++)
//     ;

// 	if (ui8_direction) {
//     if (!s->customizable.choices[++i]) // we fell off the end, loop around
//       i = 0;
// 	} else {
// 	  if (i == 0)
// 	    i = (i_max - 1);
// 	  else
// 	    i--;
// 	}

// 	*s->customizable.selector = i;
// }

// static void changeXAxis(uint8_t ui8_direction) {
//   if (ui8_direction) {
//     g_CustomizingGraphXAxis->rw->graph.x_axis_scale_config =
//         (g_CustomizingGraphXAxis->rw->graph.x_axis_scale_config + 1) % 4;
//   } else {
//     if (g_CustomizingGraphXAxis->rw->graph.x_axis_scale_config > 0)
//       g_CustomizingGraphXAxis->rw->graph.x_axis_scale_config--;
//     else
//       g_CustomizingGraphXAxis->rw->graph.x_axis_scale_config = 3;
//   }

//   g_changeXAxisTrigger = true;
// }

// // Returns true if we've handled the event (and therefore it should be cleared)
// // if first or selected changed, mark our scrollable as dirty (so child editables can be drawn)
// static bool onPressCustomizing(buttons_events_t events) {
//   static Field *g_curCustomizingFieldBackup = NULL;

// 	// If we aren't already editing anything, start now (note: we will only be called if some active editable
// 	// hasn't already handled this button
// 	if (!g_curCustomizingField && (events & SCREENCLICK_START_CUSTOMIZING) &&
// 	    g_CustomizingGraphXAxis == NULL) {
// 		selectNextCustomizableField(false); // start to customize previous field
// 		return true;
// 	}

// 	if (!g_curCustomizingField &&  // If we don't now have a customizing field, don't consider any other buttons
// 	    g_CustomizingGraphXAxis == NULL)
// 		return false;

// 	// Change the current customizable field to show the next possible value
// 	if (events & UP_CLICK) {
// 	  if (g_CustomizingGraphXAxis == NULL)
// 	    changeCurrentCustomizableField(1);
// 	  else
// 	    changeXAxis(1);

// 		return true;
// 	}

//   if (events & DOWN_CLICK) {
//       if (g_CustomizingGraphXAxis == NULL)
//         changeCurrentCustomizableField(0);
//       else
//         changeXAxis(0);
//     return true;
//   }

// 	// Go to next customizable field
// 	if (events & ONOFF_CLICK &&
// 	    g_CustomizingGraphXAxis == NULL) {
// 		selectNextCustomizableField(true); // customize next field
// 		return true;
// 	}

//   // Customize the X axis
//   if (events & UPDOWN_LONG_CLICK) {
//     if (g_CustomizingGraphXAxis) {
//       g_curCustomizingField = g_curCustomizingFieldBackup;
//       g_CustomizingGraphXAxis->rw->dirty = true;
//       g_CustomizingGraphXAxis = NULL;
//     } else {
//       g_CustomizingGraphXAxis = g_curCustomizingField->customizable.choices[*g_curCustomizingField->customizable.selector];
//       g_CustomizingGraphXAxis->rw->dirty = true;
//       g_curCustomizingFieldBackup = g_curCustomizingField;
//       g_curCustomizingField = NULL;
//     }

//     return true;
//   }

// // click power button to exit out of menus
// 	if (events & SCREENCLICK_STOP_CUSTOMIZING) {
//     Field *oldSelected = g_curCustomizingField->customizable.choices[*g_curCustomizingField->customizable.selector];
//     oldSelected->rw->dirty = true; // force a redraw (to remove any turds)

//     g_curCustomizingField = NULL;
//     g_CustomizingGraphXAxis->rw->dirty = true;
//     g_CustomizingGraphXAxis = NULL;

//     if(curScreen->onCustomized)
//       (*curScreen->onCustomized)();

//     return true;
// 	}

// 	return false;
// }

// bool screenOnPress(buttons_events_t events) {
// 	bool handled = false;

// 	if (curActiveEditable)
// 		handled |= onPressEditable(events);

// 	if (!handled)
// 		handled |= onPressScrollable(events);

// 	if (!handled)
// 		handled |= onPressCustomizing(events);

// 	if (!handled && curScreen && curScreen->onPress)
// 		handled |= curScreen->onPress(events);

// 	return handled;
// }

// // A low level screen render that doesn't use soft device or call exit handlers (useful for the critical fault handler ONLY)
// void panicScreenShow(Screen *screen) {
// 	setActiveEditable(NULL);
// 	g_curCustomizingField = NULL;
// 	scrollableStackPtr = 0; // new screen might not have one, we will find out when we render
// 	curScreen = screen;
// 	screenDirty = true;

// 	if (curScreen->onEnter)
// 		(*curScreen->onEnter)();

// 	screenUpdate(); // Force a draw immediately
// }

// void screenShow(Screen *screen) {
// 	if (curScreen && curScreen->onExit)
// 		curScreen->onExit();

//   // clean the full screen
//   screenDirty = true;

// 	panicScreenShow(screen);
// }

// Screen* getCurrentScreen() {
// 	return curScreen;
// }

// void screenUpdate() {
// 	if (!curScreen)
// 		return;

// 	if (curScreen->onPreUpdate)
// 		(*curScreen->onPreUpdate)();

// 	bool didDraw = false; // we only render to hardware if something changed

// 	// Every 300ms toggle any blinking animations
// 	screenUpdateCounter++;
// 	blinkChanged = (screenUpdateCounter
// 			% (BLINK_INTERVAL_MS / UPDATE_INTERVAL_MS) == 0);
// 	if (blinkChanged) {
// 		blinkOn = !blinkOn;
// 	}

// 	if (screenDirty) {
// 		// clear screen (to prevent turds from old screen staying around)
// 		UG_FillScreen(C_BLACK);
// 		didDraw = true;

// 		if (curScreen->onDirtyClean)
// 			(*curScreen->onDirtyClean)();
// 	}

// // For each field if that field is dirty (or the screen is) redraw it
// 	didDraw |= renderLayouts(curScreen->fields, screenDirty);

// 	if (didDraw) {
// 		if (curScreen->onPostUpdate)
// 			(*curScreen->onPostUpdate)();
// 	}

// #ifdef SW102
// // flush the screen to the hardware
//   if (didDraw)
//   {
//     lcd_refresh();
//   }
// #endif

// 	screenDirty = false;
// }

// void fieldPrintf(Field *field, const char *fmt, ...) {
// 	va_list argp;
// 	va_start(argp, fmt);
// 	char buf[MAX_FIELD_LEN];

// 	assert(field->variant == FieldDrawTextRW);
// 	vsnprintf(buf, sizeof(buf), fmt, argp);
// 	if (strncmp(buf, field->rw->drawTextPtr.msg, sizeof(buf)) != 0) {
// 		strcpy(field->rw->drawTextPtr.msg, buf); // because our field is DrawText we are guaranteed the dest array is in RAM
// 		field->rw->dirty = true;
// 	}

// 	va_end(argp);
// }

// void updateGraphData(uint8_t index, uint16_t sumDivisor) {
//   int32_t filtered;

//   // for now, reference the graphs global to find all possible data sources
//   extern Field *activeGraphs;

//   for (int i = 0; activeGraphs->customizable.choices[i]; i++) {
//     Field *f = activeGraphs->customizable.choices[i];
//     GraphData *graphData = f->rw->graph.data[index];
//     assert(graphData); // better be !NULL by now or we screwed up

//     // filter
//     switch (f->graph.filter) {
//       case FilterSquare:
//         filtered = (graphData->sum * 2) / sumDivisor;
//         break;

//       case FilterDefault:
//       default:
//         filtered = graphData->sum / sumDivisor;
//         break;
//     }
//     graphData->sum = 0;

//     // Now add the point to the graph point array
//     // add the point
//     graphData->points[graphData->end_valid] = filtered;
//     graphData->end_valid = (graphData->end_valid + 1) % GRAPH_MAX_POINTS; // inc ptr with wrap

//     // discard old point if needed and find new max/mins
//     bool overfull = graphData->start_valid == graphData->end_valid;
//     if (overfull) {
//       // find new max, if the value we will remove is equal to current max
//       if (graphData->points[graphData->start_valid] == graphData->max_val_bck) {
//         // find the max
//         int32_t max = 0;
//         for (uint16_t i = 0; i < GRAPH_MAX_POINTS; i++) {
//            if (graphData->points[i] > max)
//              max = graphData->points[i];
//         }
//         graphData->max_val_bck = max;

//         // find new min, if the value we will remove is equal to current min
//       } else if (graphData->points[graphData->start_valid] == graphData->min_val_bck) {
//         // find the min
//         int32_t min = graphData->points[0];
//         for (uint16_t i = 0; i < GRAPH_MAX_POINTS; i++) {
//            if (graphData->points[i] < min)
//              min = graphData->points[i];
//         }
//         graphData->min_val_bck = min;
//       }

//       graphData->start_valid = (graphData->start_valid + 1) % GRAPH_MAX_POINTS;

//       // store reference x axis scale
//       switch (index) {
//         case GRAPH_X_AXIS_SCALE_15M:
//           g_xAxisReferenceScale |= 1;
//           break;

//         case GRAPH_X_AXIS_SCALE_1H:
//           g_xAxisReferenceScale |= 2;
//           break;
//       }

//       // increase X axis scale when graph is full
//       if (f->rw->graph.x_axis_scale_config == GRAPH_X_AXIS_SCALE_AUTO) {
//         // use reference scale for the auto mode
//         // bits of g_xAxisReferenceScale are set dependind on reference the scale
//         if (g_xAxisReferenceScale & 2) {
//           if (f->rw->graph.x_axis_scale != GRAPH_X_AXIS_SCALE_4H) {
//             f->rw->graph.x_axis_scale = GRAPH_X_AXIS_SCALE_4H;
//             g_changeXAxisTrigger = true;
//           }
//         }
//         else if (g_xAxisReferenceScale & 1) {
//           if (f->rw->graph.x_axis_scale != GRAPH_X_AXIS_SCALE_1H) {
//             f->rw->graph.x_axis_scale = GRAPH_X_AXIS_SCALE_1H;
//             g_changeXAxisTrigger = true;
//           }
//         }
//       }
//     }

//     // set one max and min values at very first time
//     if (graphData->first_time_set_default_values_maxmin == 0) {
//       graphData->first_time_set_default_values_maxmin = true;
//       graphData->max_val_bck = filtered;
//       graphData->min_val_bck = filtered;
//     }

//     // update invariants
//     if (filtered > graphData->max_val_bck)
//       graphData->max_val_bck = filtered;

//     if (filtered < graphData->min_val_bck)
//       graphData->min_val_bck = filtered;

//     if (f->graph.graph_vars->auto_max_min == GRAPH_AUTO_MAX_MIN_AUTO) {
//       graphData->max_val = graphData->max_val_bck;
//       graphData->min_val = graphData->min_val_bck;
//     } else {
//       // see if real max and mins are over predefined values and if so, override
//       if (graphData->max_val_bck > f->graph.graph_vars->max)
//         graphData->max_val = graphData->max_val_bck;
//       else
//         graphData->max_val = f->graph.graph_vars->max;

//       if (graphData->min_val_bck < f->graph.graph_vars->min)
//         graphData->min_val = graphData->min_val_bck;
//       else
//         graphData->min_val = f->graph.graph_vars->min;
//     }
//   }
// }
// void rt_graph_process(void) {
// #ifndef SW102
//   static int numGraphs = 0;
//   static uint32_t counter_1 = 0;
//   static uint16_t counter_2[3] = {0, 0 , 0};

//   // for now, reference the graphs global to find all possible data sources
//   extern Field *activeGraphs;

//   // start update graphs only after a startup delay to avoid wrong values of the variables
//   if (activeGraphs) {
//     // track the number of data process cycles
//     counter_1++;
//     counter_2[0]++;
//     counter_2[1]++;
//     counter_2[2]++;

//     // keep summing every 100ms
//     for (int i = 0; activeGraphs->customizable.choices[i]; i++) {
//     	Field *f = activeGraphs->customizable.choices[i];
//     	assert(f->variant == FieldGraph);

//     	// Select a data pool from our cache
//     	if(!f->rw->graph.data[0]) {
//         assert(numGraphs < VARS_SIZE);
//         f->rw->graph.data[0] = &g_graphData[numGraphs][0];
//         f->rw->graph.data[1] = &g_graphData[numGraphs][1];
//         f->rw->graph.data[2] = &g_graphData[numGraphs][2];
//         numGraphs++;
//     	}

//     	Field *fieldGraphEditable = f->graph.source; // get the backing data source for this graph

//     	int32_t target = getEditableNumber(fieldGraphEditable, true);
//     	f->rw->graph.data[0]->sum += target;
//     	f->rw->graph.data[1]->sum += target;
//     	f->rw->graph.data[2]->sum += target;
//     }

//     // @casainho if you define something like NUM_TIMESCALES 3 (see my comment in screen.h), you don't need to do this copypasta and can instead just have one bit of code
//     // inside of a loop.  Which also has the nice property of letting you at compile time change the number of possible timescales and everything will just work.

//     // now calculate the filtered value for each new point and add to graph data array
//     uint32_t update_graph_data;
//     update_graph_data = (counter_1 % (GRAPH_DATA_0_INTERVAL_MS / REALTIME_INTERVAL_MS) == 0);
//     if (update_graph_data) {
//       updateGraphData(0, counter_2[0]);
//       counter_2[0] = 0;

//       // signal that UI can now update the graph and so access his data (should have plenty of time to access the data)
//       g_graphs_ui_update[0] = true;
//     }

//     update_graph_data = (counter_1 % (GRAPH_DATA_1_INTERVAL_MS / REALTIME_INTERVAL_MS) == 0);
//     if (update_graph_data) {
//       updateGraphData(1, counter_2[1]);
//       counter_2[1] = 0;

//       // signal that UI can now update the graph and so access his data (should have plenty of time to access the data)
//       g_graphs_ui_update[1] = true;
//     }

//     update_graph_data = (counter_1 % (GRAPH_DATA_2_INTERVAL_MS / REALTIME_INTERVAL_MS) == 0);
//     if (update_graph_data) {
//       updateGraphData(2, counter_2[2]);
//       counter_2[2] = 0;

//       // signal that UI can now update the graph and so access his data (should have plenty of time to access the data)
//       g_graphs_ui_update[2] = true;
//     }
//   }
// #endif
// }

// void graph_init(void) {
// #ifndef SW102
//   // Init graphs to empty
//   for (int i = 0; i < VARS_SIZE; i++) {
//     for (int j = 0; j < 3; j++) {
//       g_graphData[i][j].max_val = INT32_MIN;
//       g_graphData[i][j].min_val = INT32_MAX;
//       g_graphData[i][j].start_valid = 0;
//       g_graphData[i][j].end_valid = 0;
//     }
//   }
// #endif
// }

// void update_battery_power_usage_label(void) {
//   static const char str_km[] = "Wh/km";
//   static const char str_mi[] = "Wh/mi";

//   if(ui_vars.ui8_units_type == 0) {
//     updateReadOnlyLabelStr(&batteryPowerUsageField, str_km);
// #ifndef SW102
//     updateReadOnlyLabelStr(&batteryPowerUsageFieldGraph, str_km);
// #endif
//   }
//   else {
//     updateReadOnlyLabelStr(&batteryPowerUsageField, str_mi);
// #ifndef SW102
//     updateReadOnlyLabelStr(&batteryPowerUsageFieldGraph, str_mi);
// #endif
//   }
// }

// void screen_init(void) {
//   graph_init();

//   update_battery_power_usage_label();

//   ui_vars.ui8_throttle_virtual = 0;

// #ifndef SW102
//   assistLevelField.rw->visibility = FieldVisible;
// #else
//   wheelSpeedIntegerField.rw->visibility = FieldVisible;
// #endif
//   fieldAlternate.rw->visibility = FieldNotVisible;

//   ui_vars.ui16_street_mode_power_limit = ui_vars.ui8_street_mode_power_limit_div25 * 25;

//   if (ui_vars.ui8_street_mode_function_enabled)
//   {
//     // check to see if should be enable at startup
//     if (ui_vars.ui8_street_mode_enabled_on_startup)
//       ui_vars.ui8_street_mode_enabled = 1;

//     ui_vars.ui8_street_mode_power_limit_div25 = (ui_vars.ui16_street_mode_power_limit / 25);
//   }

//   // init the pointers
//   wheelSpeedField.rw->editable.number.auto_thresholds = &g_vars[VarsWheelSpeed].auto_thresholds;
//   wheelSpeedField.rw->editable.number.config_warn_threshold = &g_vars[VarsWheelSpeed].config_warn_threshold;
//   wheelSpeedField.rw->editable.number.config_error_threshold = &g_vars[VarsWheelSpeed].config_error_threshold;

//   tripADistanceField.rw->editable.number.auto_thresholds = &g_vars[VarsTripDistance].auto_thresholds;
//   tripADistanceField.rw->editable.number.config_warn_threshold = &g_vars[VarsTripDistance].config_warn_threshold;
//   tripADistanceField.rw->editable.number.config_error_threshold = &g_vars[VarsTripDistance].config_error_threshold;

//   odoField.rw->editable.number.auto_thresholds = &g_vars[VarsOdometer].auto_thresholds;
//   odoField.rw->editable.number.config_warn_threshold = &g_vars[VarsOdometer].config_warn_threshold;
//   odoField.rw->editable.number.config_error_threshold = &g_vars[VarsOdometer].config_error_threshold;

//   cadenceField.rw->editable.number.auto_thresholds = &g_vars[VarsCadence].auto_thresholds;
//   cadenceField.rw->editable.number.config_warn_threshold = &g_vars[VarsCadence].config_warn_threshold;
//   cadenceField.rw->editable.number.config_error_threshold = &g_vars[VarsCadence].config_error_threshold;

//   humanPowerField.rw->editable.number.auto_thresholds = &g_vars[VarsHumanPower].auto_thresholds;
//   humanPowerField.rw->editable.number.config_warn_threshold = &g_vars[VarsHumanPower].config_warn_threshold;
//   humanPowerField.rw->editable.number.config_error_threshold = &g_vars[VarsHumanPower].config_error_threshold;

//   batteryPowerField.rw->editable.number.auto_thresholds = &g_vars[VarsBatteryPower].auto_thresholds;
//   batteryPowerField.rw->editable.number.config_warn_threshold = &g_vars[VarsBatteryPower].config_warn_threshold;
//   batteryPowerField.rw->editable.number.config_error_threshold = &g_vars[VarsBatteryPower].config_error_threshold;

//   batteryPowerField.rw->editable.number.auto_thresholds = &g_vars[VarsBatteryPowerUsage].auto_thresholds;
//   batteryPowerField.rw->editable.number.config_warn_threshold = &g_vars[VarsBatteryPowerUsage].config_warn_threshold;
//   batteryPowerField.rw->editable.number.config_error_threshold = &g_vars[VarsBatteryPowerUsage].config_error_threshold;

//   batteryVoltageField.rw->editable.number.auto_thresholds = &g_vars[VarsBatteryVoltage].auto_thresholds;
//   batteryVoltageField.rw->editable.number.config_warn_threshold = &g_vars[VarsBatteryVoltage].config_warn_threshold;
//   batteryVoltageField.rw->editable.number.config_error_threshold = &g_vars[VarsBatteryVoltage].config_error_threshold;

//   batteryCurrentField.rw->editable.number.auto_thresholds = &g_vars[VarsBatteryCurrent].auto_thresholds;
//   batteryCurrentField.rw->editable.number.config_warn_threshold = &g_vars[VarsBatteryCurrent].config_warn_threshold;
//   batteryCurrentField.rw->editable.number.config_error_threshold = &g_vars[VarsBatteryCurrent].config_error_threshold;

//   motorCurrentField.rw->editable.number.auto_thresholds = &g_vars[VarsMotorCurrent].auto_thresholds;
//   motorCurrentField.rw->editable.number.config_warn_threshold = &g_vars[VarsMotorCurrent].config_warn_threshold;
//   motorCurrentField.rw->editable.number.config_error_threshold = &g_vars[VarsMotorCurrent].config_error_threshold;

//   batterySOCField.rw->editable.number.auto_thresholds = &g_vars[VarsBatterySOC].auto_thresholds;
//   batterySOCField.rw->editable.number.config_warn_threshold = &g_vars[VarsBatterySOC].config_warn_threshold;
//   batterySOCField.rw->editable.number.config_error_threshold = &g_vars[VarsBatterySOC].config_error_threshold;

//   motorTempField.rw->editable.number.auto_thresholds = &g_vars[VarsMotorTemp].auto_thresholds;
//   motorTempField.rw->editable.number.config_warn_threshold = &g_vars[VarsMotorTemp].config_warn_threshold;
//   motorTempField.rw->editable.number.config_error_threshold = &g_vars[VarsMotorTemp].config_error_threshold;

//   motorErpsField.rw->editable.number.auto_thresholds = &g_vars[VarsMotorERPS].auto_thresholds;
//   motorTempField.rw->editable.number.config_warn_threshold = &g_vars[VarsMotorERPS].config_warn_threshold;
//   motorErpsField.rw->editable.number.config_error_threshold = &g_vars[VarsMotorERPS].config_error_threshold;

//   pwmDutyField.rw->editable.number.auto_thresholds = &g_vars[VarsMotorPWM].auto_thresholds;
//   pwmDutyField.rw->editable.number.config_warn_threshold = &g_vars[VarsMotorPWM].config_warn_threshold;
//   pwmDutyField.rw->editable.number.config_error_threshold = &g_vars[VarsMotorPWM].config_error_threshold;

//   motorFOCField.rw->editable.number.auto_thresholds = &g_vars[VarsMotorFOC].auto_thresholds;
//   motorFOCField.rw->editable.number.config_warn_threshold = &g_vars[VarsMotorFOC].config_warn_threshold;
//   motorFOCField.rw->editable.number.config_error_threshold = &g_vars[VarsMotorFOC].config_error_threshold;
// }
