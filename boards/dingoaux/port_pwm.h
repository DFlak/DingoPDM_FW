#pragma once

#include "hal.h"

//===============================================
// TIM3: OUT1(PA0), OUT2(PA1), OUT3(PA2), OUT4(PA10)
//===============================================
static void pwmTim3pcb(PWMDriver *pwmp)
{
    if ((pwmp->enabled & (1 << 0)) && (pwmp->tim->CCR[0] > 0)) palSetLine(LINE_OUT1);
    if ((pwmp->enabled & (1 << 1)) && (pwmp->tim->CCR[1] > 0)) palSetLine(LINE_OUT2);
    if ((pwmp->enabled & (1 << 2)) && (pwmp->tim->CCR[2] > 0)) palSetLine(LINE_OUT3);
    if ((pwmp->enabled & (1 << 3)) && (pwmp->tim->CCR[3] > 0)) palSetLine(LINE_OUT4);
}
static void pwmOut1cb(PWMDriver *pwmp)  { (void)pwmp; if (pwmp->enabled & (1 << 0)) palClearLine(LINE_OUT1); }
static void pwmOut2cb(PWMDriver *pwmp)  { (void)pwmp; if (pwmp->enabled & (1 << 1)) palClearLine(LINE_OUT2); }
static void pwmOut3cb(PWMDriver *pwmp)  { (void)pwmp; if (pwmp->enabled & (1 << 2)) palClearLine(LINE_OUT3); }
static void pwmOut4cb(PWMDriver *pwmp)  { (void)pwmp; if (pwmp->enabled & (1 << 3)) palClearLine(LINE_OUT4); }

static const PWMConfig pwm3Cfg = {
    .frequency = 1000000,
    .period = 10000,
    .callback = pwmTim3pcb,
    .channels = {
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut1cb},
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut2cb},
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut3cb},
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut4cb}
    },
    .cr2 = 0,
    .bdtr = 0,
    .dier = 0
};

//===============================================
// TIM4: OUT5(PA15), OUT6(PB0), OUT7(PB1), OUT8(PB2)
//===============================================
static void pwmTim4pcb(PWMDriver *pwmp)
{
    if ((pwmp->enabled & (1 << 0)) && (pwmp->tim->CCR[0] > 0)) palSetLine(LINE_OUT5);
    if ((pwmp->enabled & (1 << 1)) && (pwmp->tim->CCR[1] > 0)) palSetLine(LINE_OUT6);
    if ((pwmp->enabled & (1 << 2)) && (pwmp->tim->CCR[2] > 0)) palSetLine(LINE_OUT7);
    if ((pwmp->enabled & (1 << 3)) && (pwmp->tim->CCR[3] > 0)) palSetLine(LINE_OUT8);
}
static void pwmOut5cb(PWMDriver *pwmp)  { (void)pwmp; if (pwmp->enabled & (1 << 0)) palClearLine(LINE_OUT5); }
static void pwmOut6cb(PWMDriver *pwmp)  { (void)pwmp; if (pwmp->enabled & (1 << 1)) palClearLine(LINE_OUT6); }
static void pwmOut7cb(PWMDriver *pwmp)  { (void)pwmp; if (pwmp->enabled & (1 << 2)) palClearLine(LINE_OUT7); }
static void pwmOut8cb(PWMDriver *pwmp)  { (void)pwmp; if (pwmp->enabled & (1 << 3)) palClearLine(LINE_OUT8); }

static const PWMConfig pwm4Cfg = {
    .frequency = 1000000,
    .period = 10000,
    .callback = pwmTim4pcb,
    .channels = {
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut5cb},
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut6cb},
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut7cb},
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut8cb}
    },
    .cr2 = 0,
    .bdtr = 0,
    .dier = 0
};

//===============================================
// TIM5: OUT9(PB12), OUT10(PB13), OUT11(PB14), OUT12(PB15)
//===============================================
static void pwmTim5pcb(PWMDriver *pwmp)
{
    if ((pwmp->enabled & (1 << 0)) && (pwmp->tim->CCR[0] > 0)) palSetLine(LINE_OUT9);
    if ((pwmp->enabled & (1 << 1)) && (pwmp->tim->CCR[1] > 0)) palSetLine(LINE_OUT10);
    if ((pwmp->enabled & (1 << 2)) && (pwmp->tim->CCR[2] > 0)) palSetLine(LINE_OUT11);
    if ((pwmp->enabled & (1 << 3)) && (pwmp->tim->CCR[3] > 0)) palSetLine(LINE_OUT12);
}
static void pwmOut9cb(PWMDriver *pwmp)  { (void)pwmp; if (pwmp->enabled & (1 << 0)) palClearLine(LINE_OUT9);  }
static void pwmOut10cb(PWMDriver *pwmp) { (void)pwmp; if (pwmp->enabled & (1 << 1)) palClearLine(LINE_OUT10); }
static void pwmOut11cb(PWMDriver *pwmp) { (void)pwmp; if (pwmp->enabled & (1 << 2)) palClearLine(LINE_OUT11); }
static void pwmOut12cb(PWMDriver *pwmp) { (void)pwmp; if (pwmp->enabled & (1 << 3)) palClearLine(LINE_OUT12); }

static const PWMConfig pwm5Cfg = {
    .frequency = 1000000,
    .period = 10000,
    .callback = pwmTim5pcb,
    .channels = {
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut9cb},
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut10cb},
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut11cb},
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut12cb}
    },
    .cr2 = 0,
    .bdtr = 0,
    .dier = 0
};

//===============================================
// TIM9: OUT13(PC6), OUT14(PC7) — 2-channel timer
//===============================================
static void pwmTim9pcb(PWMDriver *pwmp)
{
    if ((pwmp->enabled & (1 << 0)) && (pwmp->tim->CCR[0] > 0)) palSetLine(LINE_OUT13);
    if ((pwmp->enabled & (1 << 1)) && (pwmp->tim->CCR[1] > 0)) palSetLine(LINE_OUT14);
}
static void pwmOut13cb(PWMDriver *pwmp) { (void)pwmp; if (pwmp->enabled & (1 << 0)) palClearLine(LINE_OUT13); }
static void pwmOut14cb(PWMDriver *pwmp) { (void)pwmp; if (pwmp->enabled & (1 << 1)) palClearLine(LINE_OUT14); }

static const PWMConfig pwm9Cfg = {
    .frequency = 1000000,
    .period = 10000,
    .callback = pwmTim9pcb,
    .channels = {
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut13cb},
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut14cb},
        {PWM_OUTPUT_DISABLED,    NULL},
        {PWM_OUTPUT_DISABLED,    NULL}
    },
    .cr2 = 0,
    .bdtr = 0,
    .dier = 0
};

//===============================================
// TIM10: OUT15(PC8) — 1-channel timer
//===============================================
static void pwmTim10pcb(PWMDriver *pwmp)
{
    if ((pwmp->enabled & (1 << 0)) && (pwmp->tim->CCR[0] > 0)) palSetLine(LINE_OUT15);
}
static void pwmOut15cb(PWMDriver *pwmp) { (void)pwmp; if (pwmp->enabled & (1 << 0)) palClearLine(LINE_OUT15); }

static const PWMConfig pwm10Cfg = {
    .frequency = 1000000,
    .period = 10000,
    .callback = pwmTim10pcb,
    .channels = {
        {PWM_OUTPUT_ACTIVE_HIGH, pwmOut15cb},
        {PWM_OUTPUT_DISABLED,    NULL},
        {PWM_OUTPUT_DISABLED,    NULL},
        {PWM_OUTPUT_DISABLED,    NULL}
    },
    .cr2 = 0,
    .bdtr = 0,
    .dier = 0
};
