/*  
    RedFlag c99 lib
    2023 Maciej 'rxn7' Niziołek
    https://github.com/rxn7/redflag
*/

#ifndef __REDFLAG_H__
#define __REDFLAG_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
	RF_BOOL,
	RF_INT,
	RF_FLOAT,
	RF_STR
} rf_flag_type_t;

typedef union {
	bool as_bool;
	int as_int;
	float as_float;
	char *as_str;
} rf_flag_value_t;

typedef struct rf_flag_t {
	rf_flag_type_t type;
	const char *name;
	const char *description;
	rf_flag_value_t value;
	rf_flag_value_t default_value;
	struct rf_flag_t *next;
} rf_flag_t;

typedef struct rf_context_t {
	rf_flag_t *head_flag;
	rf_flag_t *last_flag;
    size_t flag_count;
} rf_context_t;

void rf_init_context(rf_context_t *ctx);
void rf_free_context(rf_context_t *ctx);

void rf_parse_flags(rf_context_t *ctx, int argc, char **argv);

void rf_print_flag(rf_flag_t *flag);
void rf_print_all_flags(rf_context_t *ctx);
void rf_print_options(rf_context_t *ctx);

rf_flag_t *rf_flag_bool(rf_context_t *ctx, const char *name, const char *description, bool default_value);
rf_flag_t *rf_flag_int(rf_context_t *ctx, const char *name, const char *description, int default_value);
rf_flag_t *rf_flag_float(rf_context_t *ctx, const char *name, const char *description, float default_value);
rf_flag_t *rf_flag_str(rf_context_t *ctx, const char *name, const char *description, char *default_value);

#ifdef REDFLAG_IMPLEMENTATION

void rf_init_context(rf_context_t *ctx) {
    ctx->head_flag = NULL;
    ctx->last_flag = NULL;
}

void rf_free_context(rf_context_t *ctx) {
	rf_flag_t *flag = ctx->head_flag;
	while (flag != NULL) {
		rf_flag_t *next = flag->next;
		free(flag);
		flag = next;
	}
}

static char *_rf_shift_args(int *argc, char ***argv) {
    char *result = **argv;
    (*argv)++;
    (*argc)--;
    return result;
}

void rf_parse_flags(rf_context_t *ctx, int argc, char **argv) {
    _rf_shift_args(&argc, &argv);

    while(argc > 0) {
        char *arg = _rf_shift_args(&argc, &argv);
        bool found = false;
        rf_flag_t *flag;

        if(*arg != '-')
            continue;

        arg++; /* Remove the dash */
        flag = ctx->head_flag;

        while(!found && flag != NULL) {
            if(strcmp(flag->name, arg) == 0) {
                found = true;

                switch(flag->type) {
                    case RF_BOOL:
                        flag->value.as_bool = true;
                        break;

                    case RF_INT:
                        flag->value.as_int = strtol(_rf_shift_args(&argc, &argv), NULL, 10);
                        break;

                    case RF_FLOAT:
                        flag->value.as_float = strtof(_rf_shift_args(&argc, &argv), NULL);
                        break;

                    case RF_STR:
                        flag->value.as_str = _rf_shift_args(&argc, &argv);
                        break;
                }
            }

            flag = flag->next;
        }

        if(!found)
            fprintf(stderr, "Failed to find option/flag of name \"%s\"", arg);
    }
}

static rf_flag_t *_rf_create_flag(rf_context_t *ctx, const char *name, const char *description) {
    rf_flag_t *flag = (rf_flag_t*)malloc(sizeof(rf_flag_t));
    flag->name = name;
    flag->description = description;
    flag->next = NULL;

    if(ctx->head_flag == NULL)
        ctx->head_flag = flag;
    else
        ctx->last_flag->next = flag;

    ctx->last_flag = flag;
    ctx->flag_count++;

    return flag;
}

void rf_print_flag(rf_flag_t *flag) {
    printf("%s - %s = ", flag->name, flag->description);
    switch(flag->type) {
        case RF_BOOL:
            printf("%s\n", flag->value.as_bool ? "true" : "false");
            break;

        case RF_INT:
            printf("%d\n", flag->value.as_int);
            break;

        case RF_FLOAT:
            printf("%f\n", flag->value.as_float);
            break;

        case RF_STR:
            printf("%s\n", flag->value.as_str);
            break;

        default:
            printf("THIS OPTIONS HAS A INVALID TYPE!\n");
            break;
    }
}

void rf_print_all_flags(rf_context_t *ctx) {
    rf_flag_t *flag = ctx->head_flag;
    while(flag != NULL) {
        rf_print_flag(flag);
        flag = flag->next;
    }
}

rf_flag_t *rf_flag_bool(rf_context_t *ctx, const char *name, const char *description, bool default_value) {
    rf_flag_t *flag = _rf_create_flag(ctx, name, description);
    flag->type = RF_BOOL;
    flag->value.as_bool = flag->default_value.as_bool = default_value;
    return flag;
}

rf_flag_t *rf_flag_int(rf_context_t *ctx, const char *name, const char *description, int default_value) {
    rf_flag_t *flag = _rf_create_flag(ctx, name, description);
    flag->type = RF_INT;
    flag->value.as_int = flag->default_value.as_int = default_value;
    return flag;
}

rf_flag_t *rf_flag_float(rf_context_t *ctx, const char *name, const char *description, float default_value) {
    rf_flag_t *flag = _rf_create_flag(ctx, name, description);
    flag->type = RF_FLOAT;
    flag->value.as_float = flag->default_value.as_float = default_value;
    return flag;
}

rf_flag_t *rf_flag_str(rf_context_t *ctx, const char *name, const char *description, char *default_value) {
    rf_flag_t *flag = _rf_create_flag(ctx, name, description);
    flag->type = RF_STR;
    flag->value.as_str = flag->default_value.as_str = default_value;
    return flag;
}

void rf_print_options(rf_context_t *ctx) {
    rf_flag_t *flag = ctx->head_flag;
    while(flag != NULL) {
        printf("    -%s\n", flag->name);
        printf("        %s\n", flag->description);

        switch(flag->type) {
            case RF_BOOL:
                printf("        %s by default\n", flag->default_value.as_bool ? "Enabled" : "Disabled");
                break;

            case RF_INT:
                printf("        %d by default\n", flag->default_value.as_int);
                break;

            case RF_FLOAT:
                printf("        %f by default\n", flag->default_value.as_float);
                break;

            case RF_STR:
                printf("        %s by default\n", flag->default_value.as_str);
                break;

            default:
                printf("        THIS OPTIONS HAS A INVALID TYPE!\n");
                break;
        }

        printf("\n");

        flag = flag->next;
    }
}
#endif

#endif /*__REDFLAG_H__ */
