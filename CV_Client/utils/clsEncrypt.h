#ifndef CLSENCRYPT_H
#define CLSENCRYPT_H

#include <QString>
#include <QDebug>
#include "TAesClass.h"
const int strLength=1000;
/*!
 * \brief The clsEncrypt class 要加密 解密类
 */
class clsEncrypt
{
public:
    clsEncrypt();
    ~clsEncrypt();

    static QString encrypt(QString value);
    static QString deEncrypt(QString value);
private:

};

#endif // CLSENCRYPT_H
