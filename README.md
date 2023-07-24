# RedFlag
stb-style **c99** library for handling flags in CLI applications.

# Example
See ```example.c```

# Usage
- Include the library in your header files
```C++
#include "redflag.h"
```
- In **only one** source file include the library but declare **REDFLAG_IMPLEMENTATION** first.
```C++
#define REDFLAG_IMPLEMENTATION
#include "redflag.h"
```

- Create and init a context instance
```C++
rf_context_t ctx;
rf_init_context(&ctx);
```

- Add options/flags
```C++
rf_flag_bool(ctx*, name, desc, default_value);
rf_flag_int(ctx*, name, desc, default_value);
rf_flag_float(ctx*, name, desc, default_value);
rf_flag_str(ctx*, name, desc, default_value);
// All of these functions return a rf_flag_t pointer that is owned by the context
```

- After you have declared all your options/flags you can parse them
```C++
rf_parse_flags(&ctx, argc, argv);
```

- After you have parsed the options/flags you can check their values
```C++
rf_flag_t* flag = rf_flag_str(&ctx, "test", "this is a test value", "default value");
rf_parse_flags(&ctx, argc, argv);
const char *value = flag->value.as_str;
printf("Value: %s\n", value);
```

- Remember to free the allocated memory
```C++
rf_free_context(&ctx);
```