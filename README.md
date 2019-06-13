# Missing Encryption of Sensitive Data on U.are.U 4500 Fingerprint Reader
It is a PoC for de-obfuscating encrypted fingerprint image after MITMing the reader device.

# Basic Operation
When a user try to use fingerprint authentication, user might touch a finger on the fingerprint reader device.
Just after the capturing fingerprint, the driver generates a salt and sends it to the reader device.
The device makes a key for image encryption/obfuscation using provided salt and do obfuscate the fingerprint image with the key.
After then, the encrypted/obfuscated fingerprint image and the generated key are transferred to the driver.
Finally, the driver do de-obfuscate the image using provided key from device and salt that was held before.

# Vulnerability 
The key and salt should remain encrypted during transfer operation.
Unfortunately, it does not comply this criteria.
Key and salt used for obfuscating fingerprint image exhibit cleartext when fingerprint scanner device transfers a fingerprint image, that is just scanned, to the driver. 
(CWE-311: Missing Encryption of Sensitive Data)

Upon this, we could perform a PoC for de-obfuscating fingerprint image after MITMing using above vulnerabilities. (CWE-200: Information Disclosure)

# Attack Vector 
During enrolling or verifying fingerprint, we retrieved three items immediately after capturing on the USB device; encrypted/obfuscated fingerprint image, cleartext formed key and salt.
After then, we can de-obfuscate the encrypted/obfuscated fingerprint image using retrieved cleartext formed key and reproducible salt value.

# How to build
You require the following to build this project:
* glib-2.0

# Demo video
* On Windows 10:   
[![Video Label](https://img.youtube.com/vi/wEXJDyEOatM/0.jpg)](https://youtu.be/wEXJDyEOatM=0s)
* On Linux:   
[![Video Label](https://img.youtube.com/vi/Grirez2xeas/0.jpg)](https://youtu.be/Grirez2xeast=0s)

