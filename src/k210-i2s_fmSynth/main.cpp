/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <devices.h>
#include <string.h>
#include <stdio.h>
#include <pin_cfg.h>
#include <FreeRTOS.h>
#include <task.h>

#include <math.h>

#include "playtune.h"
#include "wavetable.h"
#include "songdata.h"
#include "tuningwords.h"
#include "envelope.h"

// Kendryte K210 Dock (Sipeed) I2S FM Synth for FreeRTOS SDK.
// Ng YH (www.github.com/uncle-yong)
// Acknowledgement to YangFangFei for the assistance!
// Also thanks to Kendryte and Sipeed team for the help too!
// Note: Please change this from 100 to 1000 in FreeRTOSCOnfig.h:
//#define configTICK_RATE_HZ					( ( TickType_t ) 1000 ) // <- For 1ms resolution!

const audio_format_t audio = { AUDIO_FMT_PCM, 16, 44100, 2 };

uint8_t *buffer = NULL;
size_t frames;
handle_t i2s0;
handle_t gio;

extern FMchannel ch[NUM_OF_CHANNELS];

void init_i2s(void)
{
    i2s_stop(i2s0);
    // 12 ------------------------------|
    // 12 * 44.1 (sample rate) = 529.2 -> 529 frames.
    // Therefore, 529 frames must be declared/allocated for the buffer.
    i2s_config_as_render(i2s0, &audio, 12, I2S_AM_RIGHT, 0xc);
    i2s_start(i2s0);
}

extern "C" int main()
{

    gio = io_open("/dev/gpio0");
    configASSERT(gio);
    
    i2s0 = io_open("/dev/i2s2");
    configASSERT(i2s0);
    init_i2s();

    gpio_set_drive_mode(gio, 23, GPIO_DM_OUTPUT);
    gpio_set_pin_value(gio, 23, GPIO_PV_HIGH);

    for (unsigned int i = 0; i < NUM_OF_CHANNELS; i++)
    {
        // Sets the ratio of the C:M :
        ch[i].setFreqMult_c(1.000f);
        ch[i].setFreqMult_m(2.000f);
        ch[i].setModMultiplier(2048);
        // The setADSR's parameters are in seconds:
        ch[i].setADSR(0.05f, 0.25f, 0.00f, 0.010f);
    }

    xTaskCreate(&generateFModTask, "generateFModTask", 2048, NULL, 3, NULL);
    xTaskCreate(&updateNoteTask, "updateNoteTask", 2048, NULL, 2, NULL);

    while (1)
        ;
}