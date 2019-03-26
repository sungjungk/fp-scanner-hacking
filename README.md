# MITM Attack on XXX's Fingerprint Reader
It is a PoC source code for de-obfuscating fingerprint image after MITMing the reader device.

# Basic Operation
When a user try to use fingerprint authentication, he or she might touch a finger on the fingerprint reader device.
Just after the capturing, the driver generates a random seed and sends it to the reader device.
The device makes a key for image encryption/obfuscation using provided random seed and do obfuscate the fingerprint image with the key.
After then, the encrypted/obfuscated fingerprint image and the generated key are transferred to the host.
Finally, the driver do de-obfuscate the image using provided key from device and random seed that was held before.

# Issues
The key should remain encrypted during transfer operation and random seed should also be in no predictable pattern.
Unfortunately, it does not comply the below criteria.

1. Key and random seed used for obfuscating fingerprint image exhibit cleartext when fingerprint scanner device transfers a fingerprint image, that is just scanned, to the driver. (CWE-319: Cleartext Transmission of Sensitive Information)  
2. Moreover, random seed for obfuscating fingerprint image is always generated the same sequence of numbers each time since it is generated from rand() in libc by default. (CWE-337: Predictable Seed in Pseudo-Random Number Generator (PRNG))

Upon this, we could perform a PoC for de-obfuscating fingerprint image after MITMing using above vulnerabilities. (CWE-200: Information Disclosure)

# Procedure
1. During enrolling or verifying fingerprint, we retrieved two items immediately after capturing on the USB device; encrypted/obfuscated fingerprint image, cleartext formed key and random seed.
2. We can also obtain a predictable seed value via simple table of `rand()` in libc
3. Finally, we can de-obfuscate the encrypted/obfuscated fingerprint image using retrieved cleartext formed key and reproducible random seed value.

# How to build
You require the following to build this project:
* glib-2.0

# Demo video
* On Debian 9:  
[![Video Label](https://img.youtube.com/vi/Grirez2xeas/0.jpg)](https://youtu.be/Grirez2xeast=0s)

* On Windows 10:   
[![Video Label](https://img.youtube.com/vi/wEXJDyEOatM/0.jpg)](https://youtu.be/wEXJDyEOatM=0s)

# FYI
The obfuscated fingerprint image also can be retrieved easily through brute force attack.
This feature is easily broken due to the key for obfuscation is short for just 4-byte.

