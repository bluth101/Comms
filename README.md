# Comms
Simple program that plays custom PTT (push-to-talk) sounds.

This can be used for games like Arma 3 to enhance realism, or for apps like Discord that don't allow for custom PTT sounds.

Currently only supports Windows.

## config.ini
You can create custom loadouts by adding a new section, for example:

```
[settings]
loadout=test

[test]
ptt_open=sounds/beep.wav
key=T
```

You don't have to define open & close, if you only want 1 of the sounds to play.

A list of keys can be found [here](https://wiki.libsdl.org/SDL_Keycode) under the "Key Name" tab.
