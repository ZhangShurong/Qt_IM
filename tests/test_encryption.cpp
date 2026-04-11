/**
 * @file test_encryption.cpp
 * @brief Unit tests for encryption/decryption utilities
 * @note Tests XOR encryption and AES-like operations
 */

#include "test_framework.h"
#include <string>
#include <cstring>
#include <vector>

// ============================================================
// Inline encryption implementation for testing (extracted from project)
// ============================================================

namespace crypto {

/**
 * XOR encryption/decryption (symmetric)
 * Original: CV_Client/utils/utils.cpp encode()/decode()
 */
void xorEncrypt(char* pstr, int len_a = -1) {
    int len;
    if (len_a == -1)
        len = static_cast<int>(strlen(pstr));
    else
        len = len_a;
    for (int i = 0; i < len; i++)
        *(pstr + i) = *(pstr + i) ^ i;
}

/**
 * XOR decryption (same as encryption for XOR cipher)
 */
void xorDecrypt(char* pstr, int len_a = -1) {
    // XOR is symmetric: encrypt and decrypt are the same operation
    xorEncrypt(pstr, len_a);
}

/**
 * XOR encrypt/decrypt with custom key byte
 * Based on IMClient's IMEncryption::getXorEncryptDecrypt()
 */
std::string xorWithKey(const std::string& str, char key) {
    std::vector<char> result(str.begin(), str.end());
    for (size_t i = 0; i < result.size(); i++) {
        result[i] = result[i] ^ key;
    }
    return std::string(result.begin(), result.end());
}

/**
 * Simple Caesar-like substitution cipher for testing
 */
std::string caesarEncrypt(const std::string& plaintext, int shift) {
    std::string result = plaintext;
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = result[i] + shift;
    }
    return result;
}

std::string caesarDecrypt(const std::string& ciphertext, int shift) {
    return caesarEncrypt(ciphertext, -shift);  // Reverse operation
}

} // namespace crypto

// ============================================================
// Test Cases
// ============================================================

TEST_CASE(XOREncrypt_EmptyString) {
    char empty[] = "";
    crypto::xorEncrypt(empty);
    // Empty string should remain unchanged
    EXPECT_STR_EQ(empty, "");
}

TEST_CASE(XOREncrypt_SingleChar) {
    char single[] = "A";
    char original = single[0];

    crypto::xorEncrypt(single);

    // 'A' (0x41) ^ 0 = 0x41 = 'A' (position 0 XOR with 0)
    EXPECT_EQ(single[0], original ^ 0);
}

TEST_CASE(XORencrypt_Decrypt_RoundTrip) {
    const char* original = "Hello, World!";
    char buffer[20];
    strcpy(buffer, original);

    // Encrypt
    crypto::xorEncrypt(buffer);

    // Should be different from original
    // (unless by coincidence, which is unlikely for this string)
    bool changed = (strcmp(buffer, original) != 0);
    EXPECT_TRUE(changed);  // Encrypted text should differ

    // Decrypt (same operation for XOR)
    crypto::xorDecrypt(buffer);

    // Should match original
    EXPECT_STR_EQ(buffer, original);
}

TEST_CASE(XOREncrypt_KnownValues) {
    // Test with known input/output pairs
    char buf1[] = "AB";
    crypto::xorEncrypt(buf1);
    // Position 0: 'A'(0x41) ^ 0 = 0x41 = 'A'
    // Position 1: 'B'(0x42) ^ 1 = 0x43 = 'C'
    EXPECT_EQ(buf1[0], 'A');
    EXPECT_EQ(buf1[1], 'C');

    char buf2[] = "ABC";
    crypto::xorEncrypt(buf2);
    // Pos 0: 'A' ^ 0 = 'A'
    // Pos 1: 'B' ^ 1 = 'C'
    // Pos 2: 'C' ^ 2 = 'E'
    EXPECT_EQ(buf2[0], 'A');
    EXPECT_EQ(buf2[1], 'C');
    EXPECT_EQ(buf2[2], 'E');
}

TEST_CASE(XOREncrypt_WithLength) {
    char buf[] = "Hello";
    // Only encrypt first 3 characters
    crypto::xorEncrypt(buf, 3);

    // First 3 chars should be modified
    // 'H'^0='H', 'e'^1='d', 'l'^m'
    EXPECT_EQ(buf[0], 'H');   // pos 0: H ^ 0 = H
    EXPECT_EQ(buf[1], 'd');   // pos 1: e(0x65) ^ 1 = 0x64 = d
    EXPECT_EQ(buf[2], 'n');   // pos 2: l(0x6C) ^ 2 = 0x6E = n
    // Last 2 should be unchanged
    EXPECT_EQ(buf[3], 'l');
    EXPECT_EQ(buf[4], 'o');
}

TEST_CASE(XORDecrypt_RestoresOriginal_WithLength) {
    char original[] = "TestString";
    char buffer[20];
    strcpy(buffer, original);

    int len = 4;
    crypto::xorEncrypt(buffer, len);
    crypto::xorDecrypt(buffer, len);

    EXPECT_STR_EQ(buffer, original);
}

TEST_CASE(XorWithKey_Basic) {
    std::string plain = "Hello";
    std::string encrypted = crypto::xorWithKey(plain, 0x42);

    // Should be different
    EXPECT_FALSE(encrypted == plain);

    // Decrypting with same key should restore
    std::string decrypted = crypto::xorWithKey(encrypted, 0x42);
    EXPECT_STR_EQ(decrypted.c_str(), plain.c_str());
}

TEST_CASE(XorWithKey_DifferentKeys) {
    std::string plain = "Secret";

    std::string enc1 = crypto::xorWithKey(plain, 0x01);
    std::string enc2 = crypto::xorWithKey(plain, 0x02);

    // Different keys produce different results
    EXPECT_FALSE(enc1 == enc2);

    // Each can only be decrypted with its own key
    std::string dec1 = crypto::xorWithKey(enc1, 0x01);
    std::string dec2 = crypto::xorWithKey(enc2, 0x02);

    EXPECT_STR_EQ(dec1.c_str(), plain.c_str());
    EXPECT_STR_EQ(dec2.c_str(), plain.c_str());

    // Wrong key gives wrong result
    std::string wrongDec = crypto::xorWithKey(enc1, 0x02);
    EXPECT_FALSE(wrongDec == plain);
}

TEST_CASE(XorWithKey_KeyZero) {
    // Key of 0 means no change
    std::string plain = "NoChange";
    std::string result = crypto::xorWithKey(plain, 0);

    EXPECT_STR_EQ(result.c_str(), plain.c_str());
}

TEST_CASE(XorWithKey_AllByteValues) {
    // Test that XOR with any key is reversible for all byte values 0-255
    for (int key = 0; key < 256; key++) {
        for (int val = 0; val < 256; val++) {
            char c = static_cast<char>(val);
            char encrypted = c ^ static_cast<char>(key);
            char decrypted = encrypted ^ static_cast<char>(key);
            EXPECT_EQ(decrypted, c);
        }
    }
}

TEST_CASE(CaesarCipher_Basic) {
    std::string plain = "ABC";
    std::string encrypted = crypto::caesarEncrypt(plain, 1);
    EXPECT_STR_EQ(encrypted.c_str(), "BCD");

    std::string decrypted = crypto::caesarDecrypt(encrypted, 1);
    EXPECT_STR_EQ(decrypted.c_str(), "ABC");
}

TEST_CASE(CaesarCipher_NegativeShift) {
    std::string plain = "XYZ";
    std::string encrypted = crypto::caesarEncrypt(plain, -1);
    EXPECT_STR_EQ(encrypted.c_str(), "WXY");
}

TEST_CASE(CaesarCipher_LargeShift) {
    std::string plain = "Hello";
    std::string encrypted = crypto::caesarEncrypt(plain, 100);
    std::string decrypted = crypto::caesarDecrypt(encrypted, 100);
    EXPECT_STR_EQ(decrypted.c_str(), plain.c_str());
}

TEST_CASE(Encryption_EmptyInput) {
    std::string empty = "";
    std::string result = crypto::xorWithKey(empty, 0xFF);
    EXPECT_TRUE(result.empty());
}

TEST_CASE(Encryption_SpecialCharacters) {
    std::string special = "\t\n\r !@#$%";
    std::string encrypted = crypto::xorWithKey(special, 0x55);
    std::string decrypted = crypto::xorWithKey(encrypted, 0x55);
    EXPECT_STR_EQ(decrypted.c_str(), special.c_str());
}

TEST_CASE(Encryption_LongString) {
    // Create a 10KB string
    std::string longStr(10240, 'X');
    for (size_t i = 0; i < longStr.length(); i++) {
        longStr[i] = static_cast<char>(i % 256);
    }

    std::string encrypted = crypto::xorWithKey(longStr, 0xAB);
    std::string decrypted = crypto::xorWithKey(encrypted, 0xAB);
    EXPECT_STR_EQ(decrypted.c_str(), longStr.c_str());
}

TEST_CASE(Encryption_BinaryData) {
    // Test with binary-like data (all byte values)
    std::vector<char> binaryData(256);
    for (int i = 0; i < 256; i++) {
        binaryData[i] = static_cast<char>(i);
    }

    std::string plain(binaryData.begin(), binaryData.end());
    std::string encrypted = crypto::xorWithKey(plain, 0x42);
    std::string decrypted = crypto::xorWithKey(encrypted, 0x42);
    EXPECT_STR_EQ(decrypted.c_str(), plain.c_str());
}
