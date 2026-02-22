# double_podium (v1)
# (THIS IS THE OLD VERSION)

Mouse-friendly ortholinear keyboard layout implemented in QMK

## Goals

* Mouse-friendly, i.e. most actions and characters are available under left hand. If needed, they 
are then also provided in a more convenient way elsewhere. Additionally, navigation, common shortcuts and 
others are also available for right hand-only operation, often by mirroring the left half.

* No Tap-Hold on regular character keys as I believe it's not without issues. Tap-Hold is used on combos, 
though, as these are pressed slower and separately anyway.

* AltGr as a first-class citizen.

## Concept
<img width="700" alt="podiums" src="https://github.com/user-attachments/assets/7ceb44f3-df00-4d44-bf63-ede2db1af478" />

Putting a layer activator on the lowest pinkie allows a comfortable podium-style navigation cluster at home 
row even in one-hand operation. When using both hands, the two clusters can have complementary functions 
dependent on the layer and modifiers. As the number of keys available for modifiers is limited, the two 
pinkie-keys also serve as plain Ctrl and Alt — holding Space key in addition achieves that, besides some other 
logic. I use slightly different (lower) keycaps on these two keys so they can be easily found.





# Features

## Combos

Combos are used to send symbols, modifiers and others using just one hand. Many of them are mirrored on both 
sides. Using these is a pleasure: the mind freely chooses side depending on which hand's turn it is or just 
general convenience.

Many combos are implemented using Tap-Dance to achieve tap-or-hold operation. The resulting experience is 
similar to a smartphone keyboard — you hold it down until the alternative character is produced. Other combos 
simply repeat the character — these are used for symbols you might actually want to repeat, like drawing
a line of dashes etc. Holding Shift also modifies some combos (it sends *any* character, not just the previous
one shifted). It can be released early, too (One Shot Mod).

Only some combos are shown; their features may also be missing.

<img width="410" alt="combos0" src="https://github.com/user-attachments/assets/6ab69933-a40e-4b91-9b14-b5e25f4708ed" />
<img width="410" alt="combos1" src="https://github.com/user-attachments/assets/35f0b280-f895-46d1-9584-c58b381c7a43" />
<br>
<img width="410" alt="combos2" src="https://github.com/user-attachments/assets/6880e836-6376-4488-bf38-abbacb4c0b05" />
<img width="410" alt="combos3" src="https://github.com/user-attachments/assets/43f4a3f2-b35a-4d26-b28a-9a7afc66edd8" />
<br><br>

When a NAV layer is on, the below combos accompany it. This means that pressing ```Ctrl+(``` outputs 
```End```, for example. Press ```Ctrl+Space+(``` instead to send ```Ctrl+(```.

<img width="430" alt="combo_ctrl" src="https://github.com/user-attachments/assets/5d69c984-af37-4185-883c-fc282ec01387" />
<br><br>

When held, some combos produce modifiers. These also activate their mini-layers with just the Tab key adjacent 
to the triggering combo. 

<img width="430" alt="combo_mods" src="https://github.com/user-attachments/assets/a86ae41d-b73e-4b48-b82c-961f0342b776" />
<br><br>

"Mirror" combos are also used: a character on the left half plus the Space key send a character on 
the right half.





## Layers

### NAV layer
<img width="700" alt="nav" src="https://github.com/user-attachments/assets/0f149942-9b77-4bf9-a0ff-ec92b1470a47" />

Activated by holding any of the two keys. These also register Ctrl to modify mouse events and any keys outside
of this layer. Holding Space key in addition to these *drops* the NAV layer, making them act as plain 
Ctrl. I am actually not using the right Ctrl a lot — it might get replaced by a Space key. 

The left part features some common shortcuts placed (close to) where they are in the QWERTY layout. 
```Ctrl+Z``` is pressed by stacking the ring finger on top of the pinkie. For this to be comfortable the two 
keycaps differ in height. Pressing and holding ```Ctrl+F``` opens my app launcher.

The right part is less firmly set. It aims to offer directional scrolling in a text editor — but some editors 
don't seem to allow horizontal scrolling via keyboard at all. Comfortable "Undo" and "Redo" are also provided.

### NAV_ALT layer
<img width="700" alt="nav_alt" src="https://github.com/user-attachments/assets/eff55695-0dec-4501-b5e9-7fb479f5a9fd" />

Activated by holding the Alt key. Just like with the Ctrl, holding Space makes the key act as plain Alt. 
Additionally, any other modifier also deactivates NAV_ALT (e.g. holding both pinkie-keys results in an Alt-ed 
NAV layer).

The layer currently serves a dual role which makes it suboptimal. First, it provides basic navigation and 
shortcuts under right hand. Second, it serves the left hand the NAV layer, letting the left pinkie rest. 
This second role would imply activating the *whole* NAV layer (like the right Ctrl does), though, which 
conflicts with the first goal.

### Numpad layer
<img width="700" alt="num" src="https://github.com/user-attachments/assets/3efb2683-fc81-4630-9103-57ca79eacb44" />

Only major features are shown. Double-tapping the activator key locks the layer, tapping it once releases it. 
The Dot produces Comma if held. The left part is for left hand-only operation and the right part is for more 
comfortable typing with the free hand.

### Function layer
<img width="700" alt="fn" src="https://github.com/user-attachments/assets/c8bfce10-44b2-4336-bae0-61c4ee3e078d" />

It is activated by double-holding the AltGr key. As the AltGr has no meaning when combined with any other 
modifier (except Shift), doing so also activates this layer (e.g. ```Ctrl+AltGr+S``` produces ```Ctrl+F4```). 
To keep the code maintainable, all conditional layers are managed in the ```layer_state_set_user()``` function 
that gets called whenever a layer changes.

### Base layer
<img width="700" alt="non-letters" src="https://github.com/user-attachments/assets/30749dfc-4e48-4b97-a4a1-eba5a1695ecc" />
<!--
<img width="700" alt="non-letters-AAA" src="https://github.com/user-attachments/assets/508c4c5d-39d5-4250-aa77-7ca9b69c9f70" />
<img width="700" alt="non-letters-BBB" src="https://github.com/user-attachments/assets/1a76be14-28a9-4265-910c-cef13e32b2d0" />
-->

```Ctrl+Home``` and ```Ctrl+End``` scroll the view to top/bottom in every browser and text editor I tried.

Character layout in use can be found 
[here](https://github.com/w-jablonski/files/blob/main/Polish_English_characters_layout/README.md).





## Other features

AltGr with some keys produces frequently used symbols. Like a symbol layer wrapped around accented characters.

Double-tapping left Shift activates Caps Word, double-tapping right Shift sends Caps Lock. Tapping them once 
deactivates these.

Some thought has been given to the order of the layers. For example, momentary numpad layer sits above the NAV 
layer to allow sending ```Ctrl+1,2,3…```, since the Ctrl key also activates the NAV. However, the toggled 
NUMPAD_TOGGLED layer is placed lower, allowing any momentary layers to take precedence. 

To send ```Ctrl+Space``` or ```Alt+Space```, hold AltGr or Num in addition.

## Implementation

I use this design on a 15x5 Idobao ID75 keyboard and have implemented it in QMK. See my 
[keymap.c](https://github.com/w-jablonski/double_podium/blob/main/keymap.c) and other files in the repo.

Graphics on this page were created using the excellent 
[Keyboard Layout Editor NG](https://editor.keyboard-tools.xyz/)

<br><br><br><br><br>












