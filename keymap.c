/* Copyright 2025 Wojciech Jabłoński
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include QMK_KEYBOARD_H
#define LAYOUT LAYOUT_ortho_5x15
#define DN KC_DOWN
#define ____ KC_TRNS
#define EMPTY_LAYER LAYOUT( \
  ____,____,____,____,____,____,____,____,____,____,____,____,____,____,____, \
  ____,____,____,____,____,____,____,____,____,____,____,____,____,____,____, \
  ____,____,____,____,____,____,____,____,____,____,____,____,____,____,____, \
  ____,____,____,____,____,____,____,____,____,____,____,____,____,____,____, \
  ____,____,____,____,____,____,____,____,____,____,____,____,____,____,____ \
)

static uint16_t shift_timer;
static bool called           = false;
static bool shifted          = false;
static bool is_caps_lock     = false;
static bool numpad_toggled   = false;

typedef struct {
  bool held;
  bool called;
  bool shifted;
  int state;
} key;

static key num     = {0};
static key lctl    = {0};
static key rctl    = {0};
static key lalt    = {0};
static key ralt    = {0};
static key lsft    = {0};
static key rsft    = {0};
static key lgui    = {0};
static key rgui    = {0};
static key rparen  = {0};
static key plus    = {0};
static key space   = {0};

enum tap_dances {
  NUM,
  LCTL,
  RCTL,
  LALT,
  RALT,
  LSFT,
  RSFT,
  LGUI,
  RGUI,
  RPAREN,
  COLN,
  DQUO,
  ASTR,
  SLSH,
  LBRACE,
  RBRACE,
  GREATER,
  F11,
  DOT,
  C_F,
  PLUS,
};

enum layers {
  BASE,
  RALT_SINGLE,
  SHIFT_LAYER,
  NUMPAD_TOGGLED,
  NAV,
  NAV_ALT,
  LGUI_LAYER,
  RGUI_LAYER,
  RPAREN_LAYER,
  NUMPAD_MOMENTARY,
  FUN,
  BSPC_LAYER,
  PAGE,
  CTRL_LAYER, // dummy
  LALT_LAYER, // dummy
  SPC_LAYER, // dummy
  RALT_DOUBLE, // dummy
  FIRMWARE,
  COMBO_REFERENCE,
};

enum my_keycodes {
  MY_SPC = SAFE_RANGE,
  MY_QUES,
  DEL_WORD,
  DEL_LINE,
  TAP_C_V,
  MY_BSPC,
              SKY2, SKY3, SKY4,                SKY7, SKY8, SKY9,
        TOP1, TOP2, TOP3, TOP4, TOP5,    TOP6, TOP7, TOP8, TOP9, TOP10,
        MID1, MID2, MID3, MID4, MID5,    MID6, MID7, MID8, MID9, MID10,
LB, LE, BOT1, BOT2, BOT3, BOT4, BOT5,    BOT6, BOT7, BOT8, BOT9, BOT10,
                    NU,   LS,   LX,      RX,   RS,
};

#include "g/keymap_combo.h"

/*
TODO
suppress alt taps (nomacs)
maybe a mouse layer or mouse scrolling in NAV
improve deleting and selecting chunks of text (eg BSPC_LAYER)
maybe remove rctl. lalt activates similar layer. so maybe change layers
maybe swap lgui and lalt combos
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [BASE] = LAYOUT(
    QK_BOOT, _______,_______,C(KC_HOME),KC_EQL, C(KC_END),_______, ____, _______, KC_PSCR, KC_LPRN, KC_RPRN, _______, _______, _______,
    _______, KC_ESC,  KC_V,    KC_D,    KC_C,    KC_Z,    KC_J,    ____, KC_Q,    KC_G,    KC_U,    KC_Y,    KC_X,    KC_BSPC, _______,
    _______, _______, KC_N,    KC_S,    KC_R,    KC_T,    KC_L,   KC_DEL,KC_H,    KC_A,    KC_E,    KC_I,    KC_O,    KC_ENT,  KC_SPC,
    MY_BSPC, KC_ENT,  TD(LCTL),KC_W,    KC_M,    KC_K,    KC_B,    ____, KC_F,    KC_P,    KC_COMM, KC_DOT,  TD(LALT),_______, _______,
    _______, _______, _______, _______, TD(NUM), TD(LSFT),MY_SPC,  ____, TD(RCTL),TD(RSFT),TD(RALT),_______, _______, _______, _______
  ),

  [RALT_SINGLE] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, KC_APP,  _______, _______, _______, _______,RALT(KC_X),____,_______, _______, KC_SLSH, KC_ASTR,RALT(KC_J),KC_ESC, _______,
    _______, _______, _______, _______, _______, KC_T,    _______, ____, KC_QUOT, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, KC_DQUO, KC_UNDS, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [SHIFT_LAYER] = LAYOUT(
    _______, _______, _______, ____, S(RALT(KC_6)), ____, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, KC_DEL,  _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [NUMPAD_TOGGLED] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, KC_X,    KC_7,    KC_8,    KC_9,    KC_DLR,  ____, XXXXXXX, KC_7,    KC_8,    KC_9,    KC_X,    _______, _______,
    _______, _______, TD(DOT), KC_4,    KC_5,    KC_6,    KC_0,    ____, TD(DOT), KC_4,    KC_5,    KC_6,    KC_0,    _______, _______,
    _______, _______, _______, KC_1,    KC_2,    KC_3,    TD(DOT), ____, TD(DOT), KC_1,    KC_2,    KC_3,    _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [NAV] = LAYOUT( // hold LCTL
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, KC_PGUP, _______, _______, _______, _______,
    _______, _______, KC_Q,  C(KC_LEFT),KC_UP, C(KC_RGHT),KC_T,    ____, _______, KC_Z,    C(KC_UP),S(KC_Z), _______, _______, _______,
    _______, _______, KC_Z,    KC_LEFT, KC_DOWN, KC_RGHT, TD(C_F), ____, KC_A,(S(C(KC_UP))),C(DN),(S(C(DN))),_______, _______, _______,
    _______, _______, _______, KC_X,    KC_C,    TAP_C_V, _______, ____, _______, _______, KC_PGDN, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [NAV_ALT] = LAYOUT( // hold LALT
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______,C(KC_Q),C(KC_LEFT),KC_UP, C(KC_RGHT),C(KC_T), ____, C(KC_T), _______, KC_UP,   _______, _______, _______, _______,
    _______, _______, C(KC_Z), KC_LEFT, KC_DOWN, KC_RGHT, C(KC_F), ____, C(KC_F), KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______,
    _______, _______, _______, C(KC_X), C(KC_C), C(KC_V), _______, ____, _______, C(KC_V), C(KC_C), C(KC_X), _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [LGUI_LAYER] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, KC_TAB,  _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [RGUI_LAYER] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, KC_TAB,  _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [RPAREN_LAYER] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, KC_TAB,  _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [NUMPAD_MOMENTARY] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, KC_X,    KC_7,    KC_8,    KC_9,    KC_DLR,  ____, XXXXXXX, KC_7,    KC_8,    KC_9,    KC_X,    _______, _______,
    _______, _______, TD(DOT), KC_4,    KC_5,    KC_6,    KC_0,    ____, TD(DOT), KC_4,    KC_5,    KC_6,    KC_0,    _______, _______,
    _______, _______, _______, KC_1,    KC_2,    KC_3,    TD(DOT), ____, TD(DOT), KC_1,    KC_2,    KC_3,    _______, _______, _______,
    _______, _______, _______, _______, _______, _______, KC_SPC,  ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [FUN] = LAYOUT( // double hold RALT
    _______, _______, _______, KC_F10,  KC_F11,  KC_F12,  _______, ____, _______, KC_F10,  KC_F11,  KC_F12,  _______, _______, _______,
    _______, KC_ESC,  XXXXXXX, KC_F7,   KC_F8,   KC_F9,   XXXXXXX, ____, XXXXXXX, KC_F7,   KC_F8,   KC_F9,   XXXXXXX, KC_BSPC, _______,
    _______, _______, XXXXXXX, KC_F4,   KC_F5,   KC_F6,   XXXXXXX, ____, XXXXXXX, KC_F4,   KC_F5,   KC_F6,   XXXXXXX, _______, _______,
    _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   XXXXXXX, ____, XXXXXXX, KC_F1,   KC_F2,   KC_F3,   _______, _______, _______,
    _______, _______, _______, _______, _______, _______, KC_SPC,  ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [BSPC_LAYER] = LAYOUT( // hold Backspace. Pressing simultaneously (as combos) has the same effect
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______,_______,C(KC_BSPC),DEL_WORD,C(KC_DEL),_______,_______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, DEL_LINE,DEL_LINE,DEL_LINE,____, _______, _______, _______, _______, _______, _______, _______
  ),

  [PAGE] = LAYOUT( // hold PLUS combo
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, KC_PGUP, _______, KC_PGDN, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, ____, _______, _______, _______, _______, _______, _______, _______
  ),

  [CTRL_LAYER] = EMPTY_LAYER,
  [LALT_LAYER] = EMPTY_LAYER,
  [SPC_LAYER] = EMPTY_LAYER,
  [RALT_DOUBLE] = EMPTY_LAYER,

  [FIRMWARE] = LAYOUT(
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RM_VALU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RM_SATU, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, RM_PREV, RM_VALD, RM_NEXT, RM_TOGG, XXXXXXX, XXXXXXX, RM_HUED, RM_SATD, RM_HUEU, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,QK_REBOOT,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [COMBO_REFERENCE] = LAYOUT(
    _______, _______, _______, SKY2,    SKY3,    SKY4,    _______, _______, _______, SKY7,    SKY8,    SKY9,    _______, _______, _______,
    _______, _______, TOP1,    TOP2,    TOP3,    TOP4,    TOP5,    _______, TOP6,    TOP7,    TOP8,    TOP9,    TOP10,   _______, _______,
    _______, _______, MID1,    MID2,    MID3,    MID4,    MID5,    _______, MID6,    MID7,    MID8,    MID9,    MID10,   _______, _______,
    LB,      LE,      BOT1,    BOT2,    BOT3,    BOT4,    BOT5,    _______, BOT6,    BOT7,    BOT8,    BOT9,    BOT10,   _______, _______,
    _______, _______, _______, _______, NU,      LS,      LX,      _______, RX,      RS,      _______, _______, _______, _______, _______
  ),

};

// handle activation of some hacky layers. Some empty (dummy) key-bound
// layers are around to ensure this function gets called
layer_state_t layer_state_set_user(layer_state_t state) {

  // LCTL or RCTL activates NAV layer; Space disactivates it
  if (IS_LAYER_ON_STATE(state, CTRL_LAYER) &&
      IS_LAYER_OFF_STATE(state, SPC_LAYER)) {
    state |= (1UL << NAV); // turn on
  } else {
    state &= ~(1UL << NAV); // turn off
  }

  // Space or any modifier but LALT disactivates NAV_ALT layer
  if (IS_LAYER_ON_STATE(state, LALT_LAYER) &&
      IS_LAYER_OFF_STATE(state, SPC_LAYER) &&
      IS_LAYER_OFF_STATE(state, CTRL_LAYER) &&
      IS_LAYER_OFF_STATE(state, SHIFT_LAYER) &&
      IS_LAYER_OFF_STATE(state, LGUI_LAYER) &&
      IS_LAYER_OFF_STATE(state, RGUI_LAYER)) {
    state |= (1UL << NAV_ALT);
  } else {
    state &= ~(1UL << NAV_ALT);
  }

  // double-holding RALT activates FUN layer;
  // RALT plus any modifier (except Shift) activates FUN layer
  if (IS_LAYER_ON_STATE(state, RALT_DOUBLE) ||
      (IS_LAYER_ON_STATE(state, RALT_SINGLE) &&
       (IS_LAYER_ON_STATE(state, CTRL_LAYER) ||
        IS_LAYER_ON_STATE(state, LALT_LAYER) ||
        IS_LAYER_ON_STATE(state, LGUI_LAYER) ||
        IS_LAYER_ON_STATE(state, RGUI_LAYER) ||
        IS_LAYER_ON_STATE(state, RPAREN_LAYER)))) {
    state |= (1UL << FUN);
  } else {
    state &= ~(1UL << FUN);
  }
  return state;
}

/////////////////////////////////////////////////////////////////////////
// Key Overrides
/////////////////////////////////////////////////////////////////////////

// some are just offhand without too much thought
const key_override_t ko_full_block = ko_make_with_layers_and_negmods(
  MOD_MASK_SHIFT, KC_HASH, S(RALT(KC_7)), ~0, MOD_BIT(KC_RALT)); // █
const key_override_t ko_tild = ko_make_with_layers_and_negmods(
  MOD_MASK_SHIFT, KC_EQL, KC_TILD, ~0, MOD_BIT(KC_RALT));
const key_override_t ko_pipe = ko_make_with_layers_and_negmods(
  MOD_MASK_SHIFT, KC_PLUS, KC_PIPE, ~0, MOD_BIT(KC_RALT));
const key_override_t ko_tab = ko_make_basic(MOD_MASK_CTRL, KC_B, C(KC_TAB));
const key_override_t ko_lbr = ko_make_basic(MOD_BIT(KC_RALT), KC_LPRN, KC_LBRC);
const key_override_t ko_rbr = ko_make_basic(MOD_BIT(KC_RALT), KC_RPRN, KC_RBRC);

const key_override_t *key_overrides[] = {
  &ko_full_block,
  &ko_tild,
  &ko_pipe,
  &ko_tab,
  &ko_lbr,
  &ko_rbr,
  NULL
};

/////////////////////////////////////////////////////////////////////////
// process_record_user
/////////////////////////////////////////////////////////////////////////

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // get key's layer
  uint8_t source_layer = read_source_layers_cache(record->event.key);

  // just like with many tap dances, we allow early Shift release on combos.
  // Useful especially when Space follows them, pressed with the same thumb
  // as Shift
  if (IS_COMBOEVENT(record->event) &&
      timer_elapsed(shift_timer) < MY_SHIFT_TERM &&
      record->event.pressed &&
      (// if we really wanna type uppercase using mirror combos
       (KC_A <= keycode && keycode <= KC_Z) ||
       // see overrides above, these work together
       keycode == KC_HASH ||
       keycode == KC_EQL ||
       keycode == KC_PLUS)) {
    add_oneshot_mods(MOD_BIT_LSHIFT);
    return true;
  }

  // due to mixing the roles of modifier and layer activator on some keys,
  // some keys on these layers must ignore the modifier. This is done in
  // the below blocks
  if (source_layer == NAV_ALT) {
    if (record->event.pressed) {
      unregister_code(KC_LALT);
    } else {
      if (lalt.held || rparen.held) {
        register_code(KC_LALT);
      }
    }
    return true;
  }

  if (source_layer == SHIFT_LAYER) {
    if (record->event.pressed) {
      unregister_code(KC_LSFT);
      unregister_code(KC_RSFT);
    } else {
      if (lsft.held) {
        register_code(KC_LSFT);
      }
      if (rsft.held) {
        register_code(KC_RSFT);
      }
    }
    return true;
  }

  if (source_layer == RALT_SINGLE || source_layer == FUN) {
    if (record->event.pressed) {
      unregister_code(KC_RALT);
    } else {
      if (IS_LAYER_ON(RALT_SINGLE)) {
        register_code(KC_RALT);
      }
    }
    return true;
  }

/////////////////////////////////////////////////////////////////////////
// Keycodes
/////////////////////////////////////////////////////////////////////////

  switch (keycode) {
    case MY_SPC:
      if (record->event.pressed) {
        space.held = true;
        // layer_state_set_user activates NAV and NAV_ALT layers based on
        // state of SPC_LAYER, while NUMPAD_TOGGLED is managed here
        // as its logic is simple
        layer_on(SPC_LAYER);
        if (lctl.held || rctl.held || lalt.held) {
          layer_off(NUMPAD_TOGGLED);
        } else {
          register_code(KC_SPC);
        }
      } else {
        space.held = false;
        layer_off(SPC_LAYER);
        unregister_code(KC_SPC);
        if (numpad_toggled) {
          layer_on(NUMPAD_TOGGLED);
        }
      }
      return true;

    case MY_QUES:
      if (record->event.pressed) {
        if (lsft.held || rsft.held ||
            timer_elapsed(shift_timer) < MY_SHIFT_TERM) {
          register_code16(KC_EXLM);
        } else {
          register_code16(KC_QUES);
        }
      } else {
        unregister_code16(KC_EXLM);
        unregister_code16(KC_QUES);
      }
      return true;

    case KC_RGHT ... KC_UP:
      if (record->event.pressed) {
        unregister_code(KC_LCTL);
        unregister_code(KC_RCTL);
      } else {
        if (lctl.held) {
          register_code(KC_LCTL);
        }
        if (rctl.held) {
          register_code(KC_RCTL);
        }
      }
      return true;

    case KC_PGUP:
    case KC_PGDN:
      if (source_layer == NAV) {
        if (record->event.pressed) {
          unregister_code(KC_LCTL);
          unregister_code(KC_RCTL);
        } else {
          if (lctl.held) {
            register_code(KC_LCTL);
          }
          if (rctl.held) {
            register_code(KC_RCTL);
          }
        }
      }
      return true;

    // I might smash the key less if I am in no rush to release it
    case TAP_C_V:
      if (record->event.pressed) {
        tap_code16(C(KC_V));
      }
      return true;

    case MY_BSPC:
      if (record->event.pressed) {
        register_code(KC_BSPC);
        layer_on(BSPC_LAYER);
      } else {
        unregister_code(KC_BSPC);
        layer_off(BSPC_LAYER);
      }
      return true;

    case DEL_WORD:
      if (record->event.pressed) {
        tap_code16(C(KC_H)); // select word. Configured in text editor
        tap_code(KC_BSPC);
      }
      return true;

    case DEL_LINE:
      if (record->event.pressed) {
        tap_code(KC_END);
        tap_code(KC_LEFT);
        tap_code(KC_END);
        tap_code16(S(KC_HOME));
        tap_code(KC_DEL);
        tap_code(KC_END);
      }
      return true;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////
// Tap Dances
/////////////////////////////////////////////////////////////////////////

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5,
  TRIPLE_TAP = 6,
  TRIPLE_HOLD = 7
};

int tap_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->pressed) return SINGLE_HOLD;
    else return SINGLE_TAP;
  }
  else if (state->count == 2) {
    if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else if (state->count == 3) {
    if (state->pressed) return TRIPLE_HOLD;
    else return TRIPLE_TAP;
  }
  else return 8;
}

/////////////////////////////////////////////////////////////////////////
// Num
/////////////////////////////////////////////////////////////////////////

void num_each(tap_dance_state_t *state, void *user_data) {
  num.held = true;
  layer_on(NUMPAD_MOMENTARY);
  if (state->count == 1) {
    num.called = false;
  }
}

void num_finished(tap_dance_state_t *state, void *user_data) {
  if (num.called) {
    return;
  }
  num.called = true;
  num.state = tap_dance(state);
  switch (num.state) {
    case SINGLE_TAP:
      if (numpad_toggled) {
        numpad_toggled = false;
        layer_off(NUMPAD_TOGGLED);
        rgb_matrix_increase_hue_noeeprom();
        rgb_matrix_increase_hue_noeeprom();
      }
      break;
    case DOUBLE_TAP:
      // limit unintentional layer toggle and maybe avoid corner cases
      if (!numpad_toggled && get_mods() == 0 && !ralt.held) {
        numpad_toggled = true;
        layer_on(NUMPAD_TOGGLED);
        rgb_matrix_decrease_hue_noeeprom();
        rgb_matrix_decrease_hue_noeeprom();
      }
      break;
    default: break;
  }
  num.state = 0;
}

void num_release(tap_dance_state_t *state, void *user_data) {
  num.held = false;
  layer_off(NUMPAD_MOMENTARY);
  if (numpad_toggled || state->count == 2) {
    num_finished(state, user_data);
    reset_tap_dance(state);
  }
}

/////////////////////////////////////////////////////////////////////////
// Control, Alt
/////////////////////////////////////////////////////////////////////////

#define MODS_EACH_FINISHED(NAME, KEYCODE, LAYER)                   \
void NAME##_each(tap_dance_state_t *state, void *user_data) {      \
  NAME.held = true;                                                \
  register_code(KEYCODE);                                          \
  layer_on(LAYER);                                                 \
  if (state->count == 2) {                                         \
    /* (S+C+A) this fires app launcher */                          \
    /* in fact don't use it; frequent double-tapping feels */      \
    /* unhealthy for the hand */                                   \
    /* tap_code16(MEH(KC_PAUSE)); */                               \
  }                                                                \
}                                                                  \
void NAME##_finished(tap_dance_state_t *state, void *user_data) {  \
  NAME.state = tap_dance(state);                                   \
  switch (NAME.state) {                                            \
    case SINGLE_TAP:                                               \
      /* (G+C+A) this hides app launcher */                        \
      tap_code16(LCAG(KC_PAUSE));                                  \
      break;                                                       \
    default: break;                                                \
  }                                                                \
  NAME.state = 0;                                                  \
}

MODS_EACH_FINISHED(lctl, KC_LCTL, CTRL_LAYER)
MODS_EACH_FINISHED(lalt, KC_LALT, LALT_LAYER)

void lctl_release(tap_dance_state_t *state, void *user_data) {
  lctl.held = false;
  unregister_code(KC_LCTL);
  if (!rctl.held) {
    layer_off(CTRL_LAYER);
  }
}

void lalt_release(tap_dance_state_t *state, void *user_data) {
  lalt.held = false;
  if (!rparen.held) {
    unregister_code(KC_LALT);
  }
  layer_off(LALT_LAYER);
}

/////////////////////////////////////////////////////////////////////////

void rctl_each(tap_dance_state_t *state, void *user_data) {
  rctl.held = true;
  register_code(KC_RCTL);
  layer_on(CTRL_LAYER);
}

void rctl_release(tap_dance_state_t *state, void *user_data) {
  rctl.held = false;
  unregister_code(KC_RCTL);
  if (!lctl.held) {
    layer_off(CTRL_LAYER);
  }
}

/////////////////////////////////////////////////////////////////////////

void ralt_each(tap_dance_state_t *state, void *user_data) {
  ralt.held = true;
  switch (state->count) {
    case 1:
      register_code(KC_RALT);
      layer_on(RALT_SINGLE);
      break;
    case 2:
      layer_on(RALT_DOUBLE);
      break;
    default: break;
  }
}

void ralt_release(tap_dance_state_t *state, void *user_data) {
  ralt.held = false;
  unregister_code(KC_RALT);
  layer_off(RALT_SINGLE);
  layer_off(RALT_DOUBLE);
}

/////////////////////////////////////////////////////////////////////////
// Shift
/////////////////////////////////////////////////////////////////////////

void lsft_finished(tap_dance_state_t *state, void *user_data) {
  lsft.state = tap_dance(state);
  switch (lsft.state) {
    case SINGLE_TAP:
      shift_timer = timer_read();
      break;
    case DOUBLE_TAP:
      caps_word_on();
      break;
    default: break;
  }
  lsft.state = 0;
}

void rsft_finished(tap_dance_state_t *state, void *user_data) {
  rsft.state = tap_dance(state);
  switch (rsft.state) {
    case SINGLE_TAP:
      shift_timer = timer_read();
      if (is_caps_lock) {
        is_caps_lock = false;
        tap_code(KC_CAPS);
        rgb_matrix_increase_hue_noeeprom();
        rgb_matrix_increase_hue_noeeprom();
      }
      break;
    case DOUBLE_TAP:
      if (!is_caps_lock) {
        is_caps_lock = true;
        tap_code(KC_CAPS);
        rgb_matrix_decrease_hue_noeeprom();
        rgb_matrix_decrease_hue_noeeprom();
      }
      break;
    default: break;
  }
  rsft.state = 0;
}

/////////////////////////////////////////////////////////////////////////

#define SHIFT_EACH_RELEASE(NAME, KEYCODE, OTHER_NAME)             \
void NAME##_each(tap_dance_state_t *state, void *user_data) {     \
  NAME.held = true;                                               \
  layer_on(SHIFT_LAYER);                                          \
  caps_word_off();                                                \
  register_code(KEYCODE);                                         \
}                                                                 \
void NAME##_release(tap_dance_state_t *state, void *user_data) {  \
  NAME.held = false;                                              \
  unregister_code(KEYCODE);                                       \
  if (!OTHER_NAME.held) {                                         \
    layer_off(SHIFT_LAYER);                                       \
  }                                                               \
}

SHIFT_EACH_RELEASE(lsft, KC_LSFT, rsft)
SHIFT_EACH_RELEASE(rsft, KC_RSFT, lsft)

/////////////////////////////////////////////////////////////////////////
// Mods on combos: GUI, Left Alt
/////////////////////////////////////////////////////////////////////////

void rparen_finished(tap_dance_state_t *state, void *user_data) {
  if (rparen.called) {
    return;
  }
  const uint8_t mod_state = get_mods();
  rparen.called = true;
  if ((lctl.held || rctl.held) && !space.held) {
    if (!state->pressed) {
      del_mods(MOD_MASK_CTRL);
      tap_code(KC_DEL);
    } else {
      // a case that doesn't need handling in the other modifier combos:
      // Ctrl + this combo + interrupting key. We do nothing here, thus act
      // as modifier
    }
  // if interrupted then send both characters: ), ). etc
  } else if (state->interrupted || !state->pressed) {
    if (!lalt.held) {
      del_mods(MOD_BIT(KC_LALT));
    }
    if (rparen.shifted) {
      del_mods(MOD_MASK_SHIFT);
      clear_weak_mods(); // needed only if interrupted, so not in CTRL above
      tap_code(KC_RBRC);
    } else {
      tap_code16(KC_RPRN);
    }
  } else if (lalt.held && state->pressed) {
    //
  }
  set_mods(mod_state);
}

void rparen_release(tap_dance_state_t *state, void *user_data) {
  rparen.held = false;
  if (!lalt.held) {
    unregister_code(KC_LALT);
  }
  layer_off(RPAREN_LAYER);
  rparen_finished(state, user_data);
  rparen.called = false;
  rparen.shifted = false;
  reset_tap_dance(state);
}

/////////////////////////////////////////////////////////////////////////

#define MODS_EACH(NAME, KEYCODE, LAYER)                        \
void NAME##_each(tap_dance_state_t *state, void *user_data) {  \
  NAME.held = true;                                            \
  layer_on(LAYER);                                             \
  register_code(KEYCODE);                                      \
  if (lsft.held || rsft.held ||                                \
      timer_elapsed(shift_timer) < MY_SHIFT_TERM) {            \
    NAME.shifted = true;                                       \
  }                                                            \
}

MODS_EACH(rgui,   KC_RGUI, RGUI_LAYER)
MODS_EACH(lgui,   KC_LGUI, LGUI_LAYER)
MODS_EACH(rparen, KC_LALT, RPAREN_LAYER)

#define MODS_FINISHED(NAME, KEYCODE, TAP, SHIFT_TAP, CTRL_TAP, INTERRUPT) \
void NAME##_finished(tap_dance_state_t *state, void *user_data) {         \
  if (NAME.called) {                                                      \
    return;                                                               \
  }                                                                       \
  const uint8_t mod_state = get_mods();                                   \
  NAME.called = true;                                                     \
  if ((lctl.held || rctl.held) && !space.held && !state->pressed) {       \
    del_mods(MOD_MASK_CTRL);                                              \
    tap_code(CTRL_TAP);                                                   \
  /* if interrupted then send mod+keycode, except the following keys */   \
  } else if (state->interrupting_keycode == KC_SPC ||                     \
             state->interrupting_keycode == MY_SPC ||                     \
             state->interrupting_keycode == INTERRUPT ||                  \
             !state->pressed) {                                           \
    unregister_code(KEYCODE);                                             \
    if (NAME.shifted) {                                                   \
      del_mods(MOD_MASK_SHIFT);                                           \
      clear_weak_mods();                                                  \
      tap_code(SHIFT_TAP);                                                \
    } else {                                                              \
      tap_code16(TAP);                                                    \
    }                                                                     \
  }                                                                       \
  set_mods(mod_state);                                                    \
}

MODS_FINISHED(rgui, KC_RGUI, KC_COLN, KC_SCLN, KC_HOME, KC_ENT)
MODS_FINISHED(lgui, KC_LGUI, KC_LPRN, KC_LBRC, KC_END,  KC_SPC)

#define MODS_RELEASE(NAME, KEYCODE, LAYER)                        \
void NAME##_release(tap_dance_state_t *state, void *user_data) {  \
  NAME.held = false;                                              \
  unregister_code(KEYCODE);                                       \
  layer_off(LAYER);                                               \
  NAME##_finished(state, user_data);                              \
  NAME.called = false;                                            \
  NAME.shifted = false;                                           \
  reset_tap_dance(state);                                         \
}

MODS_RELEASE(rgui, KC_RGUI, RGUI_LAYER)
MODS_RELEASE(lgui, KC_LGUI, LGUI_LAYER)

/////////////////////////////////////////////////////////////////////////
// Symbols
/////////////////////////////////////////////////////////////////////////

void shift_check(tap_dance_state_t *state, void *user_data) {
  if (lsft.held || rsft.held ||
      timer_elapsed(shift_timer) < MY_SHIFT_TERM) {
    shifted = true;
  }
}

void simple_shift_check(tap_dance_state_t *state, void *user_data) {
  if (lsft.held || rsft.held) {
    shifted = true;
  }
}

/////////////////////////////////////////////////////////////////////////

void coln_each(tap_dance_state_t *state, void *user_data) {
  shift_check(state, user_data);
  if ((lctl.held || rctl.held) && !space.held) {
    called = true;
    const uint8_t mod_state = get_mods();
    del_mods(MOD_MASK_CTRL);
    tap_code(KC_HOME);
    set_mods(mod_state);
  }
}

void lbrace_each(tap_dance_state_t *state, void *user_data) {
  shift_check(state, user_data);
  if ((lctl.held || rctl.held) && !space.held) {
    called = true;
    tap_code(KC_TAB);
  }
}

void rbrace_each(tap_dance_state_t *state, void *user_data) {
  shift_check(state, user_data);
  if ((lctl.held || rctl.held) && !space.held) {
    called = true;
    tap_code16(S(KC_TAB));
  }
}

void greater_finished(tap_dance_state_t *state, void *user_data) {
  if (called) {
    return;
  }
  called = true;
  if (shifted) {
    const uint8_t mod_state = get_mods();
    del_mods(MOD_MASK_SHIFT);
    clear_weak_mods(); // necessary
    tap_code(KC_SLSH);
    set_mods(mod_state);
  } else if (state->interrupted || !state->pressed) {
    tap_code16(KC_GT);
  } else {
    tap_code(KC_SLSH);
  }
}

/////////////////////////////////////////////////////////////////////////

void plus_each(tap_dance_state_t *state, void *user_data) {
  layer_on(PAGE);
  if (lsft.held || rsft.held ||
      timer_elapsed(shift_timer) < MY_SHIFT_TERM) {
    plus.shifted = true;
  }
}

void plus_finished(tap_dance_state_t *state, void *user_data) {
  if (plus.called) {
    return;
  }
  plus.called = true;
  if (!state->pressed ||
      (state->interrupted &&
       state->interrupting_keycode != KC_PGUP &&
       state->interrupting_keycode != KC_PGDN)) {
    if (plus.shifted) {
      tap_code16(KC_PIPE);
    } else {
      tap_code16(KC_PLUS);
    }
  }
}

void plus_release(tap_dance_state_t *state, void *user_data) {
  layer_off(PAGE);
  plus_finished(state, user_data);
  plus.called = false;
  plus.shifted = false;
  reset_tap_dance(state);
}

/////////////////////////////////////////////////////////////////////////

#define SYMBOL_FINISHED(NAME, TAP, HOLD, SHIFT_TAP, SHIFT_HOLD)    \
void NAME##_finished(tap_dance_state_t *state, void *user_data) {  \
  if (called) {                                                    \
    return;                                                        \
  }                                                                \
  called = true;                                                   \
  if (!shifted) {                                                  \
    if (state->interrupted || !state->pressed) {                   \
      tap_code16(TAP);                                             \
    } else {                                                       \
      tap_code16(HOLD);                                            \
    }                                                              \
  } else {                                                         \
    if (state->interrupted || !state->pressed) {                   \
      tap_code16(SHIFT_TAP);                                       \
    } else {                                                       \
      tap_code16(SHIFT_HOLD);                                      \
    }                                                              \
  }                                                                \
}

SYMBOL_FINISHED(coln,   KC_COLN, KC_SCLN, KC_AMPR, KC_PERC)
SYMBOL_FINISHED(dquo,   KC_DQUO, KC_QUOT, S(RALT(KC_GRV)), S(RALT(KC_BSLS)))
SYMBOL_FINISHED(astr,   KC_ASTR, KC_GRV,  KC_CIRC, KC_TILD)
SYMBOL_FINISHED(slsh,   KC_SLSH, KC_BSLS, KC_QUES, KC_EXLM)
SYMBOL_FINISHED(lbrace, KC_LCBR, KC_LT,   KC_LCBR, KC_LT)
SYMBOL_FINISHED(rbrace, KC_RCBR, KC_GT,   KC_RCBR, KC_GT)
SYMBOL_FINISHED(f11,    KC_F11,  KC_F14,  S(KC_F11), S(KC_F14))
SYMBOL_FINISHED(dot,    KC_DOT,  KC_COMM, KC_NO,   KC_NO)
// hold Ctrl+F to run app launcher (S+C+A)
SYMBOL_FINISHED(c_f,    C(KC_F), MEH(KC_PAUSE), KC_NO, KC_NO)

#define SYMBOL_RELEASE(NAME)                                      \
void NAME##_release(tap_dance_state_t *state, void *user_data) {  \
    NAME##_finished(state, user_data);                            \
    called = false;                                               \
    shifted = false;                                              \
    reset_tap_dance(state);                                       \
}

SYMBOL_RELEASE(coln)
SYMBOL_RELEASE(dquo)
SYMBOL_RELEASE(astr)
SYMBOL_RELEASE(slsh)
SYMBOL_RELEASE(lbrace)
SYMBOL_RELEASE(rbrace)
SYMBOL_RELEASE(greater)
SYMBOL_RELEASE(f11)
SYMBOL_RELEASE(dot)
SYMBOL_RELEASE(c_f)

#define TDR ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE
tap_dance_action_t tap_dance_actions[] = {
  [NUM]     = TDR (num_each, num_release, num_finished, 0),
  [LCTL]    = TDR (lctl_each, lctl_release, lctl_finished, 0),
  [RCTL]    = TDR (rctl_each, rctl_release, 0, 0),
  [LALT]    = TDR (lalt_each, lalt_release, lalt_finished, 0),
  [RALT]    = TDR (ralt_each, ralt_release, 0, 0),
  [LSFT]    = TDR (lsft_each, lsft_release, lsft_finished, 0),
  [RSFT]    = TDR (rsft_each, rsft_release, rsft_finished, 0),
  [LGUI]    = TDR (lgui_each, lgui_release, lgui_finished, 0),
  [RGUI]    = TDR (rgui_each, rgui_release, rgui_finished, 0),
  [RPAREN]  = TDR (rparen_each, rparen_release, rparen_finished, 0),
  [COLN]    = TDR (coln_each, coln_release, coln_finished, 0),
  [DQUO]    = TDR (shift_check, dquo_release, dquo_finished, 0),
  [ASTR]    = TDR (shift_check, astr_release, astr_finished, 0),
  [SLSH]    = TDR (shift_check, slsh_release, slsh_finished, 0),
  [LBRACE]  = TDR (lbrace_each, lbrace_release, lbrace_finished, 0),
  [RBRACE]  = TDR (rbrace_each, rbrace_release, rbrace_finished, 0),
  [GREATER] = TDR (shift_check, greater_release, greater_finished, 0),
  [F11]     = TDR (simple_shift_check, f11_release, f11_finished, 0),
  [DOT]     = TDR (0, dot_release, dot_finished, 0),
  [C_F]     = TDR (0, c_f_release, c_f_finished, 0),
  [PLUS]    = TDR (plus_each, plus_release, plus_finished, 0),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  if (keycode == TD(RALT) ||
      // keycode == TD(LCTL) ||
      // keycode == TD(LALT) ||
      keycode == TD(NUM) ||
      keycode == TD(LSFT) ||
      keycode == TD(RSFT)) {
    return 350;
  } else if (keycode >= QK_TAP_DANCE && keycode <= QK_TAP_DANCE_MAX) {
    return 250;
  } else return TAPPING_TERM;
}

/////////////////////////////////////////////////////////////////////////
// Combos
/////////////////////////////////////////////////////////////////////////

bool process_combo_key_release(uint16_t combo_index, combo_t *combo,
                               uint8_t key_index, uint16_t keycode) {
  switch (combo_index) {
    case bksp_top1 ... bksp_space:
      switch(keycode) {
        case LB:
          layer_off(BSPC_LAYER);
          return true;
        default:
          layer_on(BSPC_LAYER);
          unregister_code16(C(KC_BSPC));
          unregister_code16(C(KC_DEL));
          return false;
      }
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////
// LEDs
/////////////////////////////////////////////////////////////////////////

//      74 73    72 71 70 69 68    67    66 65 64 63 62    61 60
//      59 58    57 56 55 54 53    52    51 50 49 48 47    46 45
//      44 43    42 41 40 39 38    37    36 35 34 33 32    31 30
//      29 28    27 26 25 24 23    22    21 20 19 18 17    16 15
//      14 13    12 11 10 9  8     7     6  5  4  3  2     1  0
//
//            82     83     84     85     86     87     88
//            81     80     79     78     77     76     75

// only enable LEDs under letters
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  const uint8_t leds_to_disable[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 22, 27, 28, 29,
    30, 31, 37, 43, 44,
    45, 46, 52, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
    75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88
  };
  // set specified LEDs to off (black)
  for (uint8_t i = 0; i < sizeof(leds_to_disable); i++) {
    RGB_MATRIX_INDICATOR_SET_COLOR(leds_to_disable[i], 0, 0, 0);
  }
  return false;
}
