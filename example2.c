#define REDFLAG_IMPLEMENTATION
#include "redflag.h"

int main(int argc, char *argv[]) {
  rf_context_t ctx;
  rf_init_context(&ctx);

  rf_flag_t *age_flag = rf_flag_int(&ctx, "age", "Your age", 16);
  rf_flag_t *name_flag = rf_flag_str(&ctx, "name", "Your name", "Maciej");

  rf_parse_flags(&ctx, argc, argv);
  rf_print_all_flags(&ctx);

  rf_free_context(&ctx);

  return 0;
}
