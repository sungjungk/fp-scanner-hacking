# De-obfuscating fingerprint image from libfprint/uru4000
It is a PoC code for de-obfuscating fingerprint image from libfprint/uru4000 that is widely used in Linux environment for fingerprint authentication.

# Found Issues
* In libfprint/uru4000 driver, a fingerprint image and a key from fingerprint scanner device are transferred to the driver with obsfucation feature immediately after capturing fingerprint on scanner device.
* The key is generated from the scanner device and used for obfuscation, and it is originally derived from random seed value that is generated and provided by the driver.
* After then, the driver tries to de-obfuscate the image using the key provided from the deivce and random seed value that were held before.

* Here, the key should remain encrypted during transfer operation and random seed should also be in no predictable pattern.
* Unfortunately, it does not comply the above criteria.
  1. Keys used for obfuscating fingerprint image exhibit cleartext when fingerpint scanner device transfers a fingerprint image, that is just scanned, to the libfprint/uru4000 driver. (CWE-319: Cleartext Transmission of Sensitive Information)
  2. Random seeds for obfuscating fingerprint image are always generated the same sequence of numbers each time since it is generated from rand() in libc by default. (CWE-337: Predictable Seed in Pseudo-Random Number Generator (PRNG))
* Upon this, we performed a PoC for de-obfuscating finerprint image after MITMing using above vulnerabilities. (CWE-200: Information Disclosure)

# De-obfuscating Procedure
* During enrolling or verifying fingerprint via libfprint, we retrieved two items immediately after capturing on the USB device; encrypted/obfuscated fingerprint image and cleartext formed key.
* We could easily obtain a predictable seed value via simple table of `rand()` in libc. 
* In case of physical attack, it has been reproduced on device re-initialization (e.g., unplug and replug the scanner device).
* Finally, we could de-obfuscate the encrypted/obfuscated fingerprint image from libfprint/uru4000 using retrieved cleartext formed key and reproducable random seed value.

# How to build
You require the following to build de-obfuscator:
* glib-2.0

# Demo video
* Let's do de-obfuscating fingerpint image from libfprint/uru4000.
[![Video Label](https://img.youtube.com/vi/Grirez2xeas/0.jpg)](https://youtu.be/Grirez2xeast=0s) 

# FYI
The obfuscated fingerprint image also can be retrieved easily through brute force attack.  
The feature is easily broken due to the key for obfuscation is short for just 4-byte.

