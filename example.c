#define REDFLAG_IMPLEMENTATION
#include "redflag.h"

int main(int argc, char *argv[]) {
    rf_context_t ctx;
    rf_init_context(&ctx);

    rf_flag_t *help_flag = rf_flag_bool(&ctx, "help", "Print the usage description", false);
    rf_flag_t *int_flag = rf_flag_int(&ctx, "int", "Example int value", 69);
    rf_flag_t *float_flag = rf_flag_float(&ctx, "float", "Example float value", 420.0f);
    rf_flag_t *str_flag = rf_flag_str(&ctx, "str", "Example string value", "lol");

    rf_parse_flags(&ctx, argc, argv);

    if(help_flag->value.as_bool)
        rf_print_options(&ctx);
    else
        rf_print_all_flags(&ctx);

    rf_free_context(&ctx);

    return 0;
}