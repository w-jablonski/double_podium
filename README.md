# double_podium WIP

Mouse-friendly symmetrical ortholinear keyboard layout implemented in QMK

## Goals

* Mouse friendly, i.e. most actions, characters and symbols are available under left hand. If needed, they are then also provided in a more convenient way elsewhere. 
Additionally, navigation, common shortcuts and others are also available for right hand-only operation, often by mirroring the left half.<br><br>
In general, using both hands to balance the effort is necessary and recommended for any heavy keyboard user — but there are times that mouse must be used and
switching the hand between the two can be a nuisance. This design aims to provide for both scenarios in a viable way.

* No Tap-Hold on character keys as I believe it's not without issues. I use Tap-Hold on combos, though, as these are pressed slower and separately anyway.

* AltGr as a first-class citizen to type accented characters in Polish.

## Concepts

### Double podium

<img width="700" alt="keyboard-layout(4)" src="https://github.com/user-attachments/assets/7ceb44f3-df00-4d44-bf63-ede2db1af478" />

Putting a layer activator under lowest pinkie allows a comfortable podium-style navigation cluster at home row when in one-hand operation. 
And when using both hands, the two clusters can have complementary functions dependant on the layer and modifiers.<br>
As the number of keys available for modifiers is limited, the two pinkie-keys also serve as bare Ctrl and Alt — holding Space key in addition achieves that, besides some other logic.
I use slightly different (lower) keycaps on these two keys so they can be easily found.

### Combos

Combos are used to send symbols, modifiers and others using just one hand. Many of them are mirrored on both sides. Using these is a pleasure: the mind freely chooses side 
depending on which hand's turn it is or just general convenience.

Many combos are implemented using Tap-Dance to achieve tap-or-hold operation. The resultig experience is similar to a smartphone keyboard — you hold it down 
until the alternative character is produced. Other combos simply repeat the character — these are used for symbols you might actually want to repeat, like drawing a line of dashes etc.
Holding Shift also modifies some combos (it sends *any* character, not just the previous one shifted). It can be released early, too (One Shot Mod).

Only some combos are shown; their features may also be missing.

<img width="415" alt="combos0" src="https://github.com/user-attachments/assets/6ab69933-a40e-4b91-9b14-b5e25f4708ed" />
<img width="415" alt="combos1" src="https://github.com/user-attachments/assets/35f0b280-f895-46d1-9584-c58b381c7a43" />

<br><br>

<img width="410" alt="combos2" src="https://github.com/user-attachments/assets/6880e836-6376-4488-bf38-abbacb4c0b05" />
<img width="410" alt="combos3" src="https://github.com/user-attachments/assets/43f4a3f2-b35a-4d26-b28a-9a7afc66edd8" />
<br><br>

When a NAV layer is on, the below combos accompany it. This means pressing ```Ctrl+(``` outputs ```End```. Press ```Ctrl+Space+(``` to get ```Ctrl+(```.

<img width="450" alt="combo_ctrl" src="https://github.com/user-attachments/assets/5d69c984-af37-4185-883c-fc282ec01387" />
<br><br>

When held, some combos produce modifiers. These also activate their mini-layers with just the Tab key adjacent to the triggering combo. 

<img width="450" alt="combo_mods" src="https://github.com/user-attachments/assets/a86ae41d-b73e-4b48-b82c-961f0342b776" />

<br>

## Features

### NAV layer

<img width="700" alt="nav" src="https://github.com/user-attachments/assets/0f149942-9b77-4bf9-a0ff-ec92b1470a47" />

Activator keys register Ctrl for the mouse.

<br>

### Numpad layer

<img width="700" alt="num" src="https://github.com/user-attachments/assets/3efb2683-fc81-4630-9103-57ca79eacb44" />

Only major features are shown. Double-tapping the activator key locks the layer, tapping it once releases it. The Dot produces Comma if held. 
Stating the obvious, the left part is for left hand-only operation and the right part is for more convenient two-hands typing.

<br>

### Function layer

<img width="700" alt="fn" src="https://github.com/user-attachments/assets/c8bfce10-44b2-4336-bae0-61c4ee3e078d" />

It is activated by double-holding the AltGr key. As the AltGr has no meaning when combined with any other modifier (except Shift), doing so also activates this layer (e.g. Ctrl+AltGr+S produces Ctrl+F4).



### General layout

<img width="700" alt="non-letters" src="https://github.com/user-attachments/assets/30749dfc-4e48-4b97-a4a1-eba5a1695ecc" />
<!--
<img width="700" alt="non-letters-AAA" src="https://github.com/user-attachments/assets/508c4c5d-39d5-4250-aa77-7ca9b69c9f70" />
<img width="700" alt="non-letters-BBB" src="https://github.com/user-attachments/assets/1a76be14-28a9-4265-910c-cef13e32b2d0" />
-->

Ctrl+Home and Ctrl+End scroll the view to top/bottom in every browser and text editor I tried.<br>Character layout in use can be found [here](https://github.com/w-jablonski/files/blob/main/Polish_English_characters_layout/README.md).




### Other features

The ```layer_state_set_user()``` function, which gets called anytime a layer changes, is used to handle activation of conditional layers. 
Some empty key-bound layers are used for this.

AltGr with some keys produces frequently used symbols. Like a symbol layer wrapped around accented characters.

Double-tapping left Shift activates Caps Word, double-tapping right Shift activates Caps Lock. Tapping them once disactivates these.





##

Graphics on this page were created using the excellent [Keyboard Layout Editor NG](https://editor.keyboard-tools.xyz/)


<br><br><br><br><br><br><br><br>

<!-- 



Put together, this makes the maximum of four characters shown per a combo on the below renders.

These are a simplified representation and only show some combos. Included are features which are less likely to change.

At least to me, holding a layer with a pinkie, especially for longer time, feels more natural (read: healthier) than with a thumb.<br>

Depending on the layer and modifiers applied the two clusters will optimally have complementary functions when using both hands. 
The two pinkie-keys are also serving as modifiers (CTRL and ALT) .
which inevitably complicates things
As tlimited room and by principle hand never moves, these

for the hand than thumb + podium (of the same hand) would be and even in . 
Read below about the inevitable complications of mixing a modifier and a layer activator into one key.
Modifiers under pinkies (they're in fact CTRL and ALT) . 

LATER: it allows Ctrl+XCV without finger-breaking

designate employing

navigation cluster
navigation block
navigation section 

symmetrical

Combos are embraced where there is no better way to provide some functionality (quite often). 

Tap hold is absent from regular character keys as it causes an annoying delay which is visible and annoying. No hold on tapped characters. No tap on mods except when in quick succession OR to go back to normal state.

Many combos for symbols and other buttons are placed symetrically (or so) under both hands. I recommend this highly - even for typing with both hands. 
You memorize it easily (and once) and your mind freely chooses depending on which hand's "turn" it is or which hand is less tired. Or the hand opposite to the modifier in use. 
It also allows heavy mouse users to exercise the less-used/less-typing hand a bit more (without memorizing it twice).


The keys under pinkies are in fact ctrl, alt. But when they modify / when used as modifiers for the keys lying on their side, they trigger some common shortcuts instead, as shown on the picture. 
This is to provide (navigation) when other hand is on mouse or holding tea. This must be the hackiest thing on the planet... 
To cancel the overrides you additionally hold Space (so ctrl+Space+s gives ctrl+s; alt+Space+a gives alt+a). 
Additionally, for the right hand only, holding any other modifier also/already cancels the override (ie alt+ctrl+a gives alt+ctrl+a). This took me many years to understand.

-->






<!--  

# Polish+English Keyboard Character Layouts <br><br>AAA

Conservative Symmetrical Mouse-friendly Carved Ortho Layout With Polish+English Characters Layout And Two Sets of Arrow Keys And No Mod-Taps




<p align="center"> </p>

However, with the UU now being at index I imagine this comes across as the two vowels are now at index finger, the hand feels more mobile.

opposes
is aligned to the keyboard

-->


















