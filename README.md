# De-obfuscating fingerprint image from Digital Persona U.are.U 4500 Fingerprint Reader
* It is a PoC code for de-obfuscating fingerprint image from the driver that is widely used in Linux/windows environment for fingerprint authentication.

# Basic Operation
* When users try to use fingerprint authentication, they might touch his/her own finger on the fingerprint reader device.
* Here, just before sending captured fingeprint image to the host, the driver firstly generates a random seed and transfers the seed to the reader device.
* The reader makes an encryption key using provided random seed and do obfuscate the fingerprint image with the key.
* After then, the encrypted/obfuscated fingerprint image and the generated key are transferred to the driver on a host side.
* Finally, the driver can de-obfuscate the image using the key provided from device and random seed that were held before.

# Found Issues
* The key should remain encrypted during transfer operation and random seed should also be in no predictable pattern.
* Unfortunately, it does not comply the above criteria.
  1. Key and random seed used for obfuscating fingerprint image exhibit cleartext when fingerprint scanner device transfers a fingerprint image, that is just scanned, to the driver. (CWE-319: Cleartext Transmission of Sensitive Information)
  2. Moreover, random seed for obfuscating fingerprint image is always generated the same sequence of numbers each time since it is generated from rand() in libc by default. (CWE-337: Predictable Seed in Pseudo-Random Number Generator (PRNG))
* Upon this, we performed a PoC for de-obfuscating fingerprint image after MITMing using above vulnerabilities. (CWE-200: Information Disclosure)

# De-obfuscating Procedure
* During enrolling or verifying fingerprint, we retrieved two items immediately after capturing on the USB device; encrypted/obfuscated fingerprint image, cleartext formed key and random seed.
* We can also obtain a predictable seed value via simple table of `rand()` in libc
* Finally, we can de-obfuscate the encrypted/obfuscated fingerprint image using retrieved cleartext formed key and reproducable random seed value.

# How to build
You require the following to build de-obfuscator:
* glib-2.0

# Demo video
* On Linux.
[![Video Label](https://img.youtube.com/vi/Grirez2xeas/0.jpg)](https://youtu.be/Grirez2xeast=0s) 

* On Windows 10
[![Video Label](https://img.youtube.com/vi/wEXJDyEOatM/0.jpg)](https://youtu.be/wEXJDyEOatM=0s) 

# FYI
The obfuscated fingerprint image also can be retrieved easily through brute force attack.  
The feature is easily broken due to the key for obfuscation is short for just 4-byte.

