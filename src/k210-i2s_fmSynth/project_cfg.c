#include <devices.h>
#include <pin_cfg.h>

const fpioa_cfg_t g_fpioa_cfg =
    {
        // Kendryte K210 Dock:
        .version = PIN_CFG_VERSION,
        .functions_count = 4,
        .functions[0] = {34, FUNC_I2S2_OUT_D1},
        .functions[1] = {33, FUNC_I2S2_WS},
        .functions[2] = {35, FUNC_I2S2_SCLK},

        .functions[3] = {39, FUNC_GPIOHS23}};