# De-obfuscating fingerprint image from libfprint/uru4000
It is a PoC code for de-obfuscating fingerprint image from libfprint/uru4000 that is widely used in Linux desktop for fingerprint authentication.

# Found Issues
* In libfprint/uru4000 driver, key and random seed used for obfuscating fingerprint image should remain encrypted and no predictable pattern during transfer between the device and the driver respectively.
* Unfortunately, it does not comply the above criteria.
  1. Keys used for obfuscating fingerprint image exhibit cleartext when fingerpint scanner device transfers a fingerprint image, that is just scanned, to the libfprint/uru4000 driver.
  2. Random seeds for obfuscating fingerprint image are always generated the same sequence of numbers each time since it is generated from rand() in libc by default.
* Upon this, we perform a PoC for de-obfuscating finerprint image after MITMing using above vulnearabilities.

# De-obfuscating Procedure
* During enrolling or verifying fingerprint via libfprint, we retrieved two items immediately after capturing on the USB device; encrypted/obfuscated fingerprint image and cleartext formed key.
* We obtain a reproducable seed value from rand() in libc by default.
* Then, We can de-obfuscate the encrypted/obfuscated fingerprint image from libfprint/uru4000 using retrieved cleartext formed key and reproducable random seed value.

# How to build
You require the following to build de-obfuscator:
* glib-2.0

# Demo video
* Let's do de-obfuscating fingerpint image from libfprint/uru4000.
[![Video Label](https://img.youtube.com/vi/Grirez2xeas/0.jpg)](https://youtu.be/Grirez2xeast=0s) 

# FYI
The obfuscated fingerprint image also can be retrieved easily through brute force.  
The feature is easily broken due to the key for obfuscation is short for just 4-byte.

