# Folder-Specific Ransomware Simulator

> **Disclaimer**  
> This project is intended for educational and ethical testing purposes only.  
> Do not use this software on any system without the explicit consent of the owner.  
> The author is not responsible for any misuse or damage resulting from this tool.

C# console app that simulates ransomware by encrypting and decrypting files in a chosen folder using AES-256.

<p align="center">
  <img src="image1.png" width="450" alt="# Folder-Specific Ransomware Simulator"/>
</p>

---

## Features

- Encrypts `.txt`, `.docx`, `.jpg` files recursively in specified folder  
- Saves encrypted files with `.enc` extension, deletes originals  
- Stores AES key locally (`aes.key`)  
- Decrypts files with `--decrypt` flag  
- Cross-platform: Windows, macOS, Linux

---

## Usage

1. Build: `dotnet build`  
2. Encrypt: `dotnet run` and enter folder path  
3. Decrypt: `dotnet run -- --decrypt` and enter folder path  

---

## Notes

- For educational use only — test on safe folders  
- Backup files before use 
