#include "clsEncrypt.h"

clsEncrypt::clsEncrypt()
{

}

clsEncrypt::~clsEncrypt()
{

}

/*!
 * \brief clsEncrypt::encrypt 加密
 * \param value 要解密的字符串
 * \return 返回加密字符串
 */
QString clsEncrypt::encrypt(QString value)
{
    TAesClass *aes = new TAesClass;

    char mingwen[strLength] = {0};
    strcpy(mingwen,value.toLatin1().data());
    DWORD size = strlen(mingwen);
    char miwen[strLength] = {0};

    UCHAR key[50] = "Wayne Kerr Electronics";
    UCHAR *p = key;
    aes->InitializePrivateKey(16, p); //进行初始化
    aes->OnAesEncrypt((LPVOID)mingwen, size, (LPVOID)miwen); //进行加密

    free(aes);
    aes = 0;
    return QString(mingwen);
}

/*!
 * \brief clsEncrypt::deEncrypt 解密
 * \param value 要解密的字符串
 * \return 返回解密字符串
 */

QString clsEncrypt::deEncrypt(QString value)
{
    TAesClass *aes = new TAesClass;
    char jiemi[strLength] = {0};

    UCHAR key[50] = "Wayne Kerr Electronics";
    UCHAR *p = key;
    aes->InitializePrivateKey(16, p); //进行初始化
    QString strMiWen =value;
    char MiWen[strLength] = {0};
    strcpy(MiWen, strMiWen.toLatin1().data());
    aes->OnAesUncrypt((LPVOID)MiWen, (DWORD)sizeof(MiWen),(LPVOID)jiemi); //进行解密
    free(aes);
    aes = 0;
    return QString(jiemi);
}

