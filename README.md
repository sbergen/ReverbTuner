ReverbTuner
===========

ReverbTuner is a program that uses AI to tune LV2 reverb parameters to match a convolution reverb.
It’s a school project made for an AI course, and is a rather quick hack.
It’s probably the lousiest ever LV2 host, and will crash with some plugins.
However, it works, and the basic architecture should be rather good :)

I’ve designed it to be as modular and scalable as possible,
so using other plugin standards or distributing the computational load over a network should not be too hard.
Also, the basic concept should work for any LTI effect, not only reverbs.
