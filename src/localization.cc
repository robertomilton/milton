// Copyright (c) 2015-2017 Sergio Gonzalez. All rights reserved.
// License: https://github.com/serge-rgb/milton#license

#include "localization.h"

#include "common.h"
#include "memory.h"
#include "utils.h"

enum Languages
{
    Language_FOOBAR,
    Language_ENGLISH,
    Language_SPANISH,

    Language_COUNT,
};
static int g_chosen_language = Language_ENGLISH;

static char* g_localized_strings[Language_COUNT][TXT_Count];
static char* g_command_abbreviations[TXT_Count];  // Exclusively NULL pointers except for translated strings which represent a command.
static char* g_baked_strings_with_commands[TXT_Count];  // These get malloc'd once in case that the corresponding text includes a command shortcut (see g_command_abbreviations)

#define EN(N,S) mlt_assert(!g_localized_strings[Language_ENGLISH][N]); g_localized_strings[Language_ENGLISH][N] = S
#define ES(N,S) g_localized_strings[Language_SPANISH][N] = S

void
init_localization()
{
    { // English
        EN(TXT_file, "File");
        EN(TXT_open_milton_canvas, "Open Milton Canvas");
        EN(TXT_export_to_image_DOTS, "Export to Image...");
        EN(TXT_quit, "Quit");
        EN(TXT_canvas, "Canvas");
        EN(TXT_set_background_color, "Set Background Color");
        EN(TXT_help, "Help");
        EN(TXT_help_me, "Help me!");
        EN(TXT_brushes, "Brushes");
        EN(TXT_opacity, "Opacity");
        EN(TXT_brush_size, "Brush size");
        EN(TXT_switch_to_brush, "Switch to brush");
        EN(TXT_switch_to_eraser, "Switch to eraser");
        EN(TXT_choose_background_color, "Choose background color");
        EN(TXT_color, "Color");
        EN(TXT_export_DOTS, "Export...");
        EN(TXT_MSG_click_and_drag_instruction, "Click and drag to select the area to export.");
        EN(TXT_current_selection, "Current selection");
        EN(TXT_scale_up, "Scale up");
        EN(TXT_final_image_resolution, "Final image resolution");
        EN(TXT_export_selection_to_image_DOTS, "Export selection to image...");
        EN(TXT_MSG_memerr_did_not_write, "Did not write file. Not enough memory available for operation.");
        EN(TXT_error, "Error");
        EN(TXT_cancel, "Cancel");
        EN(TXT_view, "View");
        EN(TXT_toggle_gui_visibility, "Toggle GUI Visibility");
        EN(TXT_layers, "Layers");
        EN(TXT_new_layer, "New Layer");
        EN(TXT_rename, "Rename");
        EN(TXT_move, "Move");
        EN(TXT_move_canvas, "Move canvas");
        EN(TXT_stop_moving_canvas, "Stop moving");
        EN(TXT_up, "Up");
        EN(TXT_down, "Down");
        EN(TXT_are_you_sure, "Are you sure?");
        EN(TXT_cant_be_undone, "Can't be undone");
        EN(TXT_yes, "Yes");
        EN(TXT_no, "No");
        EN(TXT_ok, "OK");
        EN(TXT_delete, "Delete");
        EN(TXT_edit, "Edit");
        EN(TXT_undo, "Undo");
        EN(TXT_redo, "Redo");
        EN(TXT_tools, "Tools");
        EN(TXT_brush, "Brush");
        EN(TXT_eraser, "Eraser");
        EN(TXT_zoom_in, "Zoom In");
        EN(TXT_zoom_out, "Zoom Out");
        EN(TXT_brush_options, "Brush Options");
        EN(TXT_set_opacity_to, "Set brush opacity to");
        EN(TXT_save_milton_canvas_as_DOTS, "Save Milton Canvas As...");
        EN(TXT_new_milton_canvas, "New Milton Canvas");
        EN(TXT_decrease_brush_size, "Decrease Brush Size");
        EN(TXT_increase_brush_size, "Increase Brush Size");
        EN(TXT_eye_dropper, "Eye Dropper");
        EN(TXT_milton_version, "Milton Version");
        EN(TXT_website, "Website");
        EN(TXT_disable_stroke_smoothing, "Disable Stroke Smoothing");
        EN(TXT_enable_stroke_smoothing, "Enable Stroke Smoothing");
        EN(TXT_transparent_background, "Transparent background");
    }

    {  // Spanish
        ES(TXT_file, "Archivo");
        ES(TXT_open_milton_canvas, "Abrir Lienzo");
        ES(TXT_export_to_image_DOTS, "Exportar a Imagen...");
        ES(TXT_quit, "Salir");
        ES(TXT_canvas, "Lienzo");
        ES(TXT_set_background_color, "Cambiar Color de Fondo");
        ES(TXT_help, "Ayuda");
        ES(TXT_brushes, "Brochas");
        ES(TXT_opacity, "Opacidad");
        ES(TXT_brush_size, "Tamaño");
        ES(TXT_switch_to_brush, "Usar brocha");
        ES(TXT_switch_to_eraser, "Usar goma");
        ES(TXT_choose_background_color, "Escoger color de fondo");
        ES(TXT_color, "Color");
        ES(TXT_export_DOTS, "Exportar...");
        ES(TXT_MSG_click_and_drag_instruction, "Haz click y Arrastra");
        ES(TXT_current_selection, "Selección actual");
        ES(TXT_scale_up, "Escalar");
        ES(TXT_final_image_resolution, "Resolución final");
        ES(TXT_export_selection_to_image_DOTS, "Exportar Selección a Imagen...");
        ES(TXT_MSG_memerr_did_not_write, "No se escribió archivo. No hay suficiente memoria.");
        ES(TXT_error, "Error");
        ES(TXT_cancel, "Cancelar");
        ES(TXT_view, "Vista");
        ES(TXT_toggle_gui_visibility, "Mostrar/Ocultar Interfaz");
        ES(TXT_layers, "Capas");
    }
    // Non-Mac:
    //  C(x) => [Ctrl+x]
#if !defined(__MACH__)
#define C(s) "Ctrl+" s
#else
#define C(s) "CMD+" s
#endif

    // TODO: Translate keys...
    {
        g_command_abbreviations [TXT_export_to_image_DOTS]  = C("E");
        g_command_abbreviations [TXT_quit]                  = C("Q");
        g_command_abbreviations [TXT_toggle_gui_visibility] = "TAB";
        g_command_abbreviations [TXT_brush]                 = "B";
        g_command_abbreviations [TXT_eraser]                = "E";
        g_command_abbreviations [TXT_undo]                  = C("Z");
        g_command_abbreviations [TXT_redo]                  = C("Shift+Z");
        g_command_abbreviations [TXT_zoom_in]               = C(" +");
        g_command_abbreviations [TXT_zoom_out]              = C(" -");
        g_command_abbreviations [TXT_move_canvas]           = "SPACE";
        g_command_abbreviations [TXT_stop_moving_canvas]    = "SPACE";
        g_command_abbreviations [TXT_decrease_brush_size]   = " [ ";
        g_command_abbreviations [TXT_increase_brush_size]   = " ] ";
        g_command_abbreviations [TXT_eye_dropper]           = "i";
        g_command_abbreviations [TXT_switch_to_brush]       = "B";
        g_command_abbreviations [TXT_switch_to_eraser]      = "E";
    }
#undef C
#undef EN
#undef ES
}


// str -- A string, translated and present in the tables within localization.c
char*
get_localized_string(int id)
{
    // TODO: Grab this from system
    i32 loc = Language_ENGLISH;

    char* result = g_localized_strings[loc][id];
    if ( result ) {
        char* cmd = g_command_abbreviations[id];

        // Include keyboard shortcut in string
        if ( cmd ) {
            if ( !g_baked_strings_with_commands[id] ) {
                char* name = g_localized_strings[loc][id];
                char* spacer = " - ";

                size_t len = strlen(name) + strlen(spacer) + strlen(cmd) + 2 /*[]*/+ 1/*\n*/;
                char* with_cmd = (char*)mlt_calloc(len, 1, "Strings");

                strncat(with_cmd, name, strlen(name));
                strncat(with_cmd, spacer, strlen(spacer));
                strncat(with_cmd, "[", 1);
                strncat(with_cmd, cmd, strlen(cmd));
                strncat(with_cmd, "]", 1);

                g_baked_strings_with_commands[id] = with_cmd;
            }
            result = g_baked_strings_with_commands[id];
        }
    } else {
        result = "STRING NEEDS LOCALIZATION";
    }

    return result;
}
