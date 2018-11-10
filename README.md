# Kendryte K210 (Sipeed K210 Dock) I2S Example with FM synth music player (FreeRTOS SDK)

This Kendryte K210 processor is located on the Sipeed K210 Dock board. General specs of the processor is as follows:

- Kendryte K210 dual core 64-bit RISC-V processor
- KPU Convolutional Neural Network (CNN) hardware accelerator
- APU audio hardware accelerator
- 6MiB of on-chip general-purpose SRAM memory and
2MiB of on-chip AI SRAM memory
- AXI ROM to load user program from SPI flash

Source: [https://www.cnx-software.com/2018/10/19/kendryte-kd233-risc-v-board-k210-soc-machinve-vision/](https://www.cnx-software.com/2018/10/19/kendryte-kd233-risc-v-board-k210-soc-machinve-vision/)

For instructions on how to deploy the project: [https://hackaday.io/project/162174-kendryte-k210-development-tutorial-for-windows-10](https://hackaday.io/project/162174-kendryte-k210-development-tutorial-for-windows-10)

After following the instructions, to build the code, you need to change "hello_world" to the "k210-i2s_fmSynth":

    cmake .. -DPROJ=k210-i2s_fmSynth -DTOOLCHAIN=X:/kendryte-toolchain/bin -G "Unix Makefiles" && make

(where X is your location of your Kendryte's toolchain)

The short I2S code is based on this example:
[https://github.com/kendryte/kendryte-standalone-demo/tree/master/play_pcm](https://github.com/kendryte/kendryte-standalone-demo/tree/master/play_pcm)

# The example program

This program plays a tune in Miditones form. You can modify the Attack, Release and Decay (Sustain not supported yet) in the respective channel's **setADSR** function.

Also, you can also modify the properties of the channel too, example:
- ch[i].setFreqMult_c(1.000f) - carrier to 1.000
- ch[i].setFreqMult_m(2.000f) - modulator to 2.000
- ch[i].setModMultiplier(2048) - multiplier for modulator (must set more than 1000 to hear the changes)

## Issues and Limitations
Unfortunately, each note which is finished playing must be damped by the 0x80 code, which are removed in later versions of Miditones. Included here is the older version of the Miditones where the software can damp each of the note before striking the next note.

Besides that, the FreeRTOS's config has the configTICK_RATE_HZ defaulted to 100. The 1ms resolution is required for the note update function (Miditones' delay are in miliseconds), therefore it is set to 1000. 

The "Release" here in the ADSR function serves as the note damping mechanism before hitting the next note.

Len Shustek's Miditones: [https://github.com/LenShustek/miditones](https://github.com/LenShustek/miditones)
